#!/usr/bin/env python3

import sys
import argparse
from pathlib import Path

KERNEL_FILE_TEMPLATE = """
%(headers)s
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "%(descr)s";
unsigned long block_inst = %(block_inst)s;
unsigned long lanes = %(lanes)s;
unsigned long block_ops = %(block_ops)s;
unsigned long unroll = %(unroll)s;
static %(bits_type)s *mem = nullptr;
static int avl = 1;

void init_mem()
{
    %(init)s
    %(init_mem)s
}

void kernel(unsigned long iters)
{
    %(init)s

    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
%(body)s
        : /* no input */
        : "r"(mem)
        : "memory"
        );
    }
%(check_reg)s
free(mem);
}

"""
     
def modrange(start, stop, mod, step=1):
    return [((i) % mod) for i in range(start, stop, step)]


def lmuls_to_int(lmul: str) -> int:
    if lmul == "mf8":
        return 1/8
    if lmul == "mf4":
        return 1/4
    if lmul == "mf2":
        return 1/2
    if lmul == "m1":
        return 1
    if lmul == "m2":
        return 2
    if lmul == "m4":
        return 4
    if lmul == "m8":
        return 8

def usage_error(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    sys.exit(100)

def bits_load(databits: int, isa: str, type: str)  -> str:
    if isa == "rv64gc":
        if type == "int":
            if databits == 64: return "ld"
            if databits == 32: return "lw"
            if databits == 16: return "lh"
            if databits == 8: return "lb"
        if type == "float":
            if databits == 64: return "fld"
            if databits == 32: return "flw"
            if databits == 16: return "flh"
            if databits == 8: return "flb"
    if isa == "rv64gcv":
        if databits == 64: return "vle64.v"
        if databits == 32: return "vle32.v"
        if databits == 16: return "vle16.v"
        if databits == 8: return "vle8.v"

def bits_format(databits: int, type: str) -> str:
    if type == "float":
        if databits == 64: return "double"
        if databits == 32: return "float"
        if databits == 16: return "_Float16"
    if type == "int":
        if databits == 64: return "int64_t"
        if databits == 32: return "int32_t"
        if databits == 16: return "int16_t"
        if databits == 8: return "int8_t"

def generate_block(lines, clobber, block_ops, blk, lanes, init, init_mem, check_reg):
    def ignore_register(x):
        return x[0] not in ("x", "w", "v", "v0.t", "a")

    indent = 8*" "
    count = blk.count
    opcode = blk.opcode
    formats = blk.operand[0::2]
    values = []
    load = bits_load(blk.typebits, blk.isa, blk.type)

    for val in blk.operand[1::2]:
        try:
            evaluated = eval(val)
        except SyntaxError as err:
            usage_error("Syntax error in operand range value: %s" % err.text)
        if (evaluated is not None) and (len(evaluated) != count):
            usage_error("Invalid length %d of operand range value '%s' (expected %d)" % (len(evaluated), val, count))
        values.append(evaluated)
    for i in range(count):
        operands = [fmt % val[i] if val is not None else fmt for (fmt, val) in zip(formats, values)]
        clobber |= set([x.split(".")[0] for x in operands if not ignore_register(x)])
        lines.append('%s"%s %s, (%%0) \\n\\t"' % (indent, opcode, operands[0]))
    if blk.isa == "rv64gcv":
        lanes.append("((int)8*__riscv_vlenb()*%s/%s)" % (lmuls_to_int(blk.lmul), blk.typebits))
        init.append(f"""
    asm volatile ("vsetvli %0, %1, e{blk.typebits}, {blk.lmul}, {blk.tail}, {blk.mask}": "=r"(avl): "r"(lanes): "vl", "vtype");
    //printf("The active vector length of RVV is %d\\n", avl);
        """)
    elif blk.isa == "rv64gc":
        lanes.append("1")
    block_ops.append("(%s*(%s*lanes))" % (blk.count, blk.laneops))
    
    bits_type=bits_format(blk.typebits, blk.type)

    if blk.type == "float":
        mem = f"""mem[i] = ( {bits_type} )(({blk.type})rand() / RAND_MAX * 10.0);"""
    if blk.type == "int":
        mem = f"""mem[i] = ( {bits_type} )(({blk.type})rand());"""

    init_mem.append(f"""

    mem = ({bits_type} *)malloc(avl * sizeof({bits_type}));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){{
	    {mem}
    printf("init value%ld=%f\\t",i, (double)mem[i]);
   }}
   printf("\\n");
    """)
    if blk.isa == "rv64gcv":
        check_reg.append(f"""
        {bits_type} buf[32];
        asm volatile(
            "vse{blk.typebits}.v v0, (%0)\\n\\t"
            :
            : "r"(buf)
            : "t0", "t1", "memory"
        );

        printf("Check if the v0 register has been written to\\n");

        for (size_t i = 0; i < avl; i++)
            printf("element%zu=%-10f\\t", i, (double)buf[i]);
        printf("\\n");
                         """)


def describe(unroll, blocks):
    parts = ["%d(" % unroll]
    for blk in blocks:
        parts.append("%d(%s_%s_%db)" % (blk.count, blk.isa, blk.opcode.upper(), blk.typebits))
    parts.append(")")
    return " ".join(parts)


def generate(unroll, blocks, output):
    lines = []
    clobber = set()
    headers = set()
    block_ops = []
    lanes = []
    init = []
    init_mem = []
    check_reg = []
    for blk in blocks:
        generate_block(lines, clobber, block_ops, blk, lanes, init, init_mem, check_reg)
        if blk.isa == "rv64gcv":
            headers.add("riscv_vector.h")
            headers.add("stdio.h")
        
    block_ops = "+".join(block_ops)
    lanes = "+".join(lanes)
    init = "+".join(init)
    init_mem = "+".join(init_mem)
    check_reg = "+".join(check_reg)
    block_inst = str(len(lines))
    lines *= unroll
    headers = "\n".join(["#include <%s>" % x for x in headers])
    body = "\n".join(lines)
    bits_type=bits_format(blk.typebits, blk.type)
    descr = describe(unroll, blocks)
    clobber = ", ".join(sorted(['"%s"' % x for x in clobber]))
    code = KERNEL_FILE_TEMPLATE % {
            "headers": headers,
            "lanes" : lanes,
            "init"  : init,
            "descr": descr, 
            "block_inst": block_inst, 
            "block_ops": block_ops, 
            "unroll": unroll, 
            "body": body,
            "clobber": clobber,
            "init_mem": init_mem,
            "check_reg": check_reg,
            "bits_type": bits_type}

    out = Path(output)
    out.parent.mkdir(parents=True, exist_ok=True)  # creates "kernel/" if needed
    out.write_text(code)


def parse_args(args):
    block_parser = argparse.ArgumentParser(prog="", add_help=False)
    block_parser.add_argument("isa", choices=["rv64gc", "rv64gcv"], help="Instruction ISA [rv64gc|rv64gcv]")
    block_parser.add_argument("typebits", type=int, help="Size of the operation datatype in bits")
    block_parser.add_argument("type", type=str, help="float|int")
    block_parser.add_argument("lmul", choices=["mf8", "mf4", "mf2", "m1", "m2", "m4", "m8"], help="Length multiplier [mf8|mf4|mf2|m1|m2|m4|m8]")
    block_parser.add_argument("tail", choices=["ta", "tu"], help="Tail agnostic (ta) or tail undisturbed (tu) [ta|tu]", default="ta")
    block_parser.add_argument("mask", choices=["ma", "mu"], help="Mask agnostic (ma) or mask undisturbed (mu) [ma|mu]", default="ma")
    block_parser.add_argument("count", type=int, help="Instructions in the block")
    block_parser.add_argument("laneops", type=int, help="Operations performed per lane")
    block_parser.add_argument("opcode", help="Instruction opcode")
    block_parser.add_argument("operand", nargs="+", help="Instruction operands")
    block_help = block_parser.format_usage().replace("usage: ", "")

    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", type=str, help="file path generated", default="kernel/kernel.cpp")
    parser.add_argument("-u", "--unroll", type=int, help="Number of times to unroll the loop", default=4)
    parser.add_argument("-b", required=True, nargs="+", metavar="block_template", dest="blocks", action="append", help=block_help)

    parsed = parser.parse_args(args)
    parsed.blocks = [block_parser.parse_args(blk) for blk in parsed.blocks]
    return parsed


def main(*args, **kwargs):
    parsed = parse_args(*args)
    generate(parsed.unroll, parsed.blocks, parsed.file)


if __name__ == "__main__":
    main(sys.argv[1:])
