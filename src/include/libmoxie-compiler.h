/*
    This file is part of the moxie-c-emulator project.

    Copyright (C) 2014  Michael G Hughes <squirmyworms@embarqmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __LIBMOXIE_COMPILER_H__
#define __LIBMOXIE_COMPILER_H__

#define c(dval) moxie_store_post_increment(moxie, &PC, dval, 1);
#define c16(dval) moxie_store_post_increment(moxie, &PC, dval, 2);
#define c32(dval) moxie_store_post_increment(moxie, &PC, dval, 4);

void c_form1_opcode(moxie_p moxie, uint8_t opcode, char *opname);

#define C_FORM1_OPCODE(opname) c_form1_opcode(moxie, opcode_##opname, #opname)

void c_form1abi_opcode(moxie_p moxie, uint8_t opcode, char *opname,
	int ra_dst, int rb_src,	uint32_t ival);

#define C_FORM1ABI_OPCODE(opname, ra_dst, rb_src, ival) \
	c_form1abi_opcode(moxie, opcode_##opname, #opname, ra_dst, rb_src, ival)

void c_form1ai_opcode(moxie_p moxie, uint8_t opcode, char *opname, int ra_dst, uint32_t ival);

#define C_FORM1AI_OPCODE(opname, ra_dst, ival) \
	c_form1ai_opcode(moxie, opcode_##opname, #opname, ra_dst, ival)

void c_form1i_opcode(moxie_p moxie, uint8_t opcode, char *opname, uint32_t ival);

#define C_FORM1I_OPCODE(opname, ival) \
	c_form1i_opcode(moxie, opcode_##opname, #opname, ival)

#define C_FORM1_A_OPCODE(opname, ival) \
	c_form1i_opcode(moxie, opcode_##opname, #opname, ival)

void c_form1ab_opcode(moxie_p moxie, uint8_t opcode, char *opname, int ra_dst, int rb_src);

#define C_FORM1AB_OPCODE(opname, ra_dst, rb_src) \
	c_form1ab_opcode(moxie, opcode_##opname, #opname, ra_dst, rb_src)

void c_form2av_opcode(moxie_p moxie, uint8_t opcode, char *opname, int ra_dst, int ival);

#define C_FORM2AV_OPCODE(opname, ra_dst, ival) \
	c_form2av_opcode(moxie, opcode_##opname, #opname, ra_dst, ival)

void c_form3_opcode(moxie_p moxie, uint8_t opcode, char *opname, int32_t ival);

#define C_FORM3_O_OPCODE(opname, ival) \
	c_form3_opcode(moxie, opcode_##opname, #opname, ival)

#define BAD		C_FORM1_OPCODE(bad_0);
#define BEQ(dst)	C_FORM3_O_OPCODE(beq, dst);
#define BGE(dst)	C_FORM3_O_OPCODE(bge, dst);
#define BLEU(dst)	C_FORM3_O_OPCODE(bleu, dst);
#define BNE(dst)	C_FORM3_O_OPCODE(bne, dst);
#define BRK		C_FORM1_OPCODE(brk);
#define CMP(arg1, arg2)	C_FORM1AB_OPCODE(cmp, arg1, arg2);
#define DEC(dst, val)	C_FORM2AV_OPCODE(dec, dst, val);
#define INC(dst, val)	C_FORM2AV_OPCODE(inc, dst, val);
#define LDI_L(dst, val)	C_FORM1AI_OPCODE(ldi_l, dst, val);
#define JMPA(dst)	C_FORM1_A_OPCODE(jmpa, dst);
#define JSRA(dst)	C_FORM1_A_OPCODE(jsra, dst);
#define MOV(dst, src)	C_FORM1AB_OPCODE(mov, dst, src);
#define NOP 		C_FORM1_OPCODE(nop);
#define RET		C_FORM1_OPCODE(ret);
#define XOR(dst, src)	C_FORM1AB_OPCODE(xor, dst, src);

#define ORG(x)		moxie->start_pc = x; PC = x;

#define LABEL(x)	uint32_t x = PC;

#endif /* __LIBMOXIE_COMPILER_H__ */

