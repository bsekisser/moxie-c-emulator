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

#ifndef __LIBMOXIE_OPCODE_H__
#define __LIBMOXIE_OPCODE_H__

#define INST_ESAC_0(esac, name, traceop) opcode_##esac = 0x00,
#define INST_ESAC(esac, name, traceop) opcode_##esac,

/*  moxie form 1 instructions */
#define FORM1_INST_ESAC_0(esac, name, traceop) INST_ESAC_0(esac, name, traceop)
#define FORM1_INST_ESAC(esac, name, traceop) INST_ESAC(esac, name, traceop)

#define FORM1_INST_LIST \
	FORM1_INST_ESAC_0(bad_0, "bad_0", Z) \
	FORM1_INST_ESAC(ldi_l, "ldi.l", Ra_I32) \
	FORM1_INST_ESAC(mov, "mov", Ra_vRb) \
	FORM1_INST_ESAC(jsra, "jsra", I32) \
	FORM1_INST_ESAC(ret, "ret", Z) \
	FORM1_INST_ESAC(add_l, "add.l", vRa_vRb) \
	FORM1_INST_ESAC(push, "push", vRa_vRb) \
	FORM1_INST_ESAC(pop, "pop", Rb_vRa) \
	FORM1_INST_ESAC(lda_l, "lda.l", Ra_I32) \
	FORM1_INST_ESAC(sta_l, "sta.l", I32_vRa) \
	FORM1_INST_ESAC(ld_l, "ld.l", Ra_vRb) \
	FORM1_INST_ESAC(st_l, "st.l", Ra_vRb) \
	FORM1_INST_ESAC(ldo_l, "ldo.l", Ra_vI32ORb) \
	FORM1_INST_ESAC(sto_l, "sto.l", I32OvRa_vRb) \
	FORM1_INST_ESAC(cmp, "cmp", vRa_vRb) \
	FORM1_INST_ESAC(nop, "nop", Z) \
	FORM1_INST_ESAC(bad_10, "bad_10", Z) \
	FORM1_INST_ESAC(bad_11, "bad_11", Z) \
	FORM1_INST_ESAC(bad_12, "bad_12", Z) \
	FORM1_INST_ESAC(bad_13, "bad_13", Z) \
	FORM1_INST_ESAC(bad_14, "bad_14", Z) \
	FORM1_INST_ESAC(bad_15, "bad_15", Z) \
	FORM1_INST_ESAC(bad_16, "bad_16", Z) \
	FORM1_INST_ESAC(bad_17, "bad_17", Z) \
	FORM1_INST_ESAC(bad_18, "bad_18", Z) \
	FORM1_INST_ESAC(jsr, "jsr", vRa) \
	FORM1_INST_ESAC(jmpa, "jmpa", I32) \
	FORM1_INST_ESAC(ldi_b, "ldi.b", I32) \
	FORM1_INST_ESAC(ld_b, "ld.b", Ra_I32) \
	FORM1_INST_ESAC(lda_b, "lda.b", Ra_I32) \
	FORM1_INST_ESAC(st_b, "st.b", Ra_vRb) \
	FORM1_INST_ESAC(sta_b, "sta.b", Ra_I32) \
	FORM1_INST_ESAC(ldi_s, "ldi.s", Ra_I32) \
	FORM1_INST_ESAC(ld_s, "ld.s", Ra_vRb) \
	FORM1_INST_ESAC(lda_s, "lda.s", Ra_I32) \
	FORM1_INST_ESAC(st_s, "st.s", vRa_vRb) \
	FORM1_INST_ESAC(sta_s, "sta.s", I32_vRa) \
	FORM1_INST_ESAC(jmp, "jmp", vRa) \
	FORM1_INST_ESAC(and, "and", vRa_vRb) \
	FORM1_INST_ESAC(lshr, "lshr", vRa_vRb) \
	FORM1_INST_ESAC(ashl, "ashl", vRa_vRb) \
	FORM1_INST_ESAC(sub_l, "sub.l", vRa_vRb) \
	FORM1_INST_ESAC(neg, "neg", vRa_vRb) \
	FORM1_INST_ESAC(or, "or", vRa_vRb) \
	FORM1_INST_ESAC(not, "not", vRa_vRb) \
	FORM1_INST_ESAC(ashr, "ashr", vRa_vRb) \
	FORM1_INST_ESAC(xor, "xor", vRa_vRb) \
	FORM1_INST_ESAC(mul_l, "mul.l", vRa_vRb) \
	FORM1_INST_ESAC(swi, "swi", I32) \
	FORM1_INST_ESAC(div_l, "div.l", vRa_vRb) \
	FORM1_INST_ESAC(udiv_l, "udiv.l", vRa_vRb) \
	FORM1_INST_ESAC(mod_l, "mod.l", vRa_vRb) \
	FORM1_INST_ESAC(umod_l, "umod.l", vRa_vRb) \
	FORM1_INST_ESAC(brk, "brk", Z) \
	FORM1_INST_ESAC(ldo_b, "ldo.b", Ra_I32OvRb) \
	FORM1_INST_ESAC(sto_b, "sto.b", I32OvRa_vRb) \
	FORM1_INST_ESAC(ldo_s, "ldo.s", Ra_I32OvRb) \
	FORM1_INST_ESAC(sto_s, "sto.s", I32OvRa_vRb)

enum {
	FORM1_INST_LIST
};

/* moxie form 2 instructions */
#define FORM2_INST_ESAC_0(esac, name, traceop) INST_ESAC_0(esac, name, traceop)
#define FORM2_INST_ESAC(esac, name, traceop) INST_ESAC(esac, name, traceop)

#define FORM2_INST_LIST \
	FORM2_INST_ESAC_0(inc, "inc", vRa_I) \
	FORM2_INST_ESAC(dec, "dec", vRa_I) \
	FORM2_INST_ESAC(gsr, "gsr", Z) \
	FORM2_INST_ESAC(ssr, "ssr", Z)

enum {
	FORM2_INST_LIST
};

/* moxie form 3 instructions */
#define FORM3_INST_ESAC_0(esac, name, cc) INST_ESAC_0(esac, name, O10)
#define FORM3_INST_ESAC(esac, name, cc) INST_ESAC(esac, name, O10)

#define FORM3_INST_LIST \
	FORM3_INST_ESAC_0(beq, "beq", CC_EQ) \
	FORM3_INST_ESAC(bne, "bne", CC_EQ) \
	FORM3_INST_ESAC(blt, "blt", CC_LT) \
	FORM3_INST_ESAC(bgt, "bgt", CC_GT) \
	FORM3_INST_ESAC(bltu, "bltu", CC_LTU) \
	FORM3_INST_ESAC(bgtu, "bgtu", CC_GTU) \
	FORM3_INST_ESAC(bge, "bge", (CC_GT | CC_EQ)) \
	FORM3_INST_ESAC(ble, "ble", (CC_LT | CC_EQ)) \
	FORM3_INST_ESAC(bgeu, "bgeu", (CC_GTU | CC_EQ)) \
	FORM3_INST_ESAC(bleu, "bleu", (CC_LTU | CC_EQ))

enum {
	FORM3_INST_LIST
};

#define get_form3_opcode(inst) (((inst) >> 10) & 0x0f)
#define get_form2_opcode(inst) (((inst) >> 12) & 3)
#define get_form1_opcode(inst) ((inst) >> 8)

/* (((signed short)((inst & ((1 << 10) -1)) << 6)) >> 6) << 1)
	(1 << 10)
	( ^ - 1)
	(inst & ^)
 */
#define INST2OFFSET(inst) (((((int32_t)(inst)) << (32 - 10)) >> (32 - 10)) << 1)

#define get_form1_a4(inst) \
	const int a = (inst >> 4) & 0xf; \

#define get_form1_a4v(inst) \
	get_form1_a4(inst); \
	const uint32_t av = R(a);

#define get_form1_a4_b4(inst) \
	get_form1_a4(inst); \
	const int b = inst & 0xf;

#define get_form1_a4_b4v(inst) \
	get_form1_a4_b4(inst); \
	const uint32_t bv = R(b);

#define get_form1_a4_b4v_i32(inst) \
	get_form1_a4_b4v(inst) \
	get_i32(inst);

#define get_form1_a4_i32(inst) \
	get_form1_a4(inst) \
	get_i32(inst);

#define get_form1_a4v_b4v(inst) \
	get_form1_a4_b4(inst); \
	const uint32_t av = R(a); \
	const uint32_t bv = R(b);

#define get_form1_a4v_b4v_i32(inst) \
	get_form1_a4v_b4v(inst) \
	get_i32(inst);

#define get_form1_a4v_i32(inst) \
	get_form1_a4v(inst); \
	get_i32(inst);

#define get_i32(inst) \
	const uint32_t ival = moxie_fetch_post_increment(moxie, &PC, 4); \

#endif /* __LIBMOXIE_OPCODE_H__ */

