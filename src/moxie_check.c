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

#include "config.h"

#include "libmoxie.h"
#include "libmoxie-compiler.h"
#include "libmoxie-opcode.h"

/*
	Adaption of test code pulled from:

		github repo:
		owner: toptensoftware
		file: rom.s
*/

#define BCX_TEST(lc_opname, uc_opname, x0, x1) \
	LABEL(lc_opname##_test); \
		CMP(x0, x1); \
		uc_opname(lc_opname##_pass); \
		BAD; \
	LABEL(lc_opname##_pass);

#define BCX_J_TEST(lc_opname, uc_opname, x0, x1) \
	LABEL(lc_opname##_test); \
		CMP(x0, x1); \
		uc_opname(lc_opname##_fail); \
		JMPA(lc_opname##_pass); \
	LABEL(lc_opname##_fail); \
		BAD; \
	LABEL(lc_opname##_pass);

static void moxie_check_tests_bcx_equality(moxie_p moxie)
{
	/* Equality Tests */
	LDI_L(r0, 1);
	LDI_L(r1, 2);
	BCX_TEST(beq_1, BEQ, r0, r0);
	BCX_J_TEST(beq_2, BEQ, r0, r1);
	BCX_TEST(bne_1, BNE, r0, r1);
	BCX_J_TEST(bne_2, BNE, r0, r0);
}

static void moxie_check_tests_bcx_unsigned(moxie_p moxie)
{
	/* Unsigned GT/LT */
	LDI_L(r0, 0xA0000100)
	LDI_L(r1, 0xA0000200)
	/* BGTU */
	BCX_TEST(bgtu_1, BGTU, r1, r0);
	BCX_J_TEST(bgtu_2, BGTU, r0, r1);
	BCX_TEST(bgtu_3, BGTU, r0, r0);
	/* BLTU */
	BCX_TEST(bltu_1, BLTU, r0, r1);
	BCX_J_TEST(bltu_2, BLTU, r1, r0);
	BCX_J_TEST(bltu_3, BLTU, r0, r0);
	/* BGEU */
	BCX_TEST(bgeu_1, BGEU, r1, r0);
	BCX_J_TEST(bgeu_2, BGEU, r0, r1);
	BCX_TEST(bgeu_3, BGEU, r0, r0);
	/* BLEU */	
	BCX_TEST(bleu_1, BLEU, r0, r1);
	BCX_J_TEST(bleu_2, BLEU, r1, r0);
	BCX_TEST(bleu_3, BLEU, r0, r0);
}

static void moxie_check_tests_bcx_signed(moxie_p moxie)
{
	/* Signed GT/LT */	
	LDI_L(r0, -100)
	LDI_L(r1, 100)
	/* BGT */
	BCX_TEST(bgt_1, BGT, r1, r0);
	BCX_J_TEST(bgt_2, BGT, r0, r1);
	BCX_TEST(bgt_3, BGT, r0, r0);
	/* BLT */
	BCX_TEST(blt_1, BLT, r0, r1);
	BCX_J_TEST(blt_2, BLT, r1, r0);
	BCX_J_TEST(blt_3, BLT, r0, r0);
	/* BGE */
	BCX_TEST(bge_1, BGE, r1, r0);
	BCX_J_TEST(bge_2, BGE, r0, r1);
	BCX_TEST(bge_3, BGE, r0, r0);
	/* BLE */	
	BCX_TEST(ble_1, BLE, r0, r1);
	BCX_J_TEST(ble_2, BLE, r1, r0);
	BCX_TEST(ble_3, BLE, r0, r0);
}

#define ALU_TEST(lc_op, uc_op, vr0, vr1, vr2) \
	LABEL(lc_op##_test); \
		LDI_L(r0, vr0); \
		LDI_L(r1, vr1); \
		LDI_L(r2, vr2); \
		uc_op(r0, r1); \
		CMP(r0, r2); \
		BEQ(lc_op##_test_pass); \
		BAD; \
	LABEL(lc_op##_test_pass);

static void moxie_check_tests_primary_alu(moxie_p moxie)
{
	ALU_TEST(mov, MOV, 0, 200, 200);
	ALU_TEST(add, ADD, 100, 200, 300);
	ALU_TEST(sub, SUB, 300, 100, 200);
	ALU_TEST(and, AND, 0x12345678, 0xF0F0F0F0, 0x10305070);
	ALU_TEST(or, OR, 0x12340101, 0xFFF01010, 0xFFF41111);
	ALU_TEST(xor, XOR, 0x12340101, 0xFFF01010, 0xEDC41111);
	ALU_TEST(not, NOT, 0, 0x12345678, 0xEDCBA987);
	ALU_TEST(neg_1, NEG, 0, 100, -100);
	ALU_TEST(neg_2, NEG, 0, -100, 100);
	ALU_2_TEST(inc, INC, 100, 350, 250);
	ALU_2_TEST(dec, DEC, 400, 150, 250);
	ALU_TEST(lshr, LSHR, 0x00AAAA00, 5, 0x00055550);
	ALU_TEST(ashl, ASHL, 0x00555500, 5, 0x0AAAA000);
	ALU_TEST(ashr_1, ASHR, 0x00AAAA00, 5, 0x00055550);
	ALU_TEST(ashr_2, ASHR, 0x80AAAA00, 5, 0xFC055550);
}

static void moxie_check_tests_secondary_alu(moxie_p moxie)
{
	ALU_TEST(mul_1, MUL, 100, 200, 20000);
	ALU_TEST(mul_2, MUL, 100, -200, -20000);
	ALU_TEST(mul_3, MUL, -100, 200, -20000);
	ALU_TEST(mul_4, MUL, -100, -200, 20000);
	ALU_TEST(div_1, DIV, 20000, 200, 100);
	ALU_TEST(div_2, DIV, 20000, -200, -100);
	ALU_TEST(div_3, DIV, -20000, 200, -100);
	ALU_TEST(div_4, DIV, -20000, -200, 100);
	ALU_TEST(udiv, UDIV, 3000000000, 150000000, 20);
	/* FIXME */
}

void moxie_check_tests(moxie_p moxie)
{
	/* Misc Tests */
LABEL(nop_test);
	NOP;
	NOP;
	NOP;
LABEL(nop_test_pass);
	moxie_check_tests_bcx_equality(moxie);
	moxie_check_tests_bcx_unsigned(moxie);
	moxie_check_tests_bcx_signed(moxie);
	moxie_check_tests_primary_alu(moxie);
}

