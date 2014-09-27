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

#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "libmoxie-config.h"

#include "profiler.h"

#include "libmoxie.h"
#include "libmoxie-access.h"
#include "libmoxie-opcode.h"

#define CC_GT	1 << 0
#define CC_LT	1 << 1
#define CC_EQ	1 << 2
#define CC_GTU	1 << 3
#define CC_LTU	1 << 4

#define CC moxie->cc

void set_initial_gprs(moxie_p moxie)
{
	int i;

	for(i = 0; i < NUM_MOXIE_REGS; i++)
		R(i) = 0;
	
	for(i = 0; i < NUM_MOXIE_SREGS; i++)
		moxie->sregs[i] = 0;

	PC = moxie->start_pc;
}

#define OPCODE_ESAC_BRCS(esac, i_cc) \
	case opcode_##esac: { \
		if(moxie->cc & (i_cc)) \
			goto do_branch; \
	} break;
		
#define OPCODE_ESAC_BRCC(esac, i_cc) \
	case opcode_##esac: { \
		if(!(moxie->cc & (i_cc))) \
			goto do_branch; \
	} break;

#define EXCEPTION(trace_msg, the_exception) \
	do { \
		moxie->exception.msg = trace_msg; \
		moxie->exception.id = the_exception; \
		PC -= 2; /* set pc back to the position which cased the exception */\
	} while(0);

static moxie_inst_form3(moxie_p moxie, uint16_t inst)
{
	int opcode = get_form3_opcode(inst);
	
	switch(opcode) {
		OPCODE_ESAC_BRCS(beq, CC_EQ);
		OPCODE_ESAC_BRCC(bne, CC_EQ);
		OPCODE_ESAC_BRCS(blt, CC_LT);
		OPCODE_ESAC_BRCS(bgt, CC_GT);
		OPCODE_ESAC_BRCS(bltu, CC_LTU);
		OPCODE_ESAC_BRCS(bgtu, CC_GTU);
		OPCODE_ESAC_BRCS(bge, CC_GT | CC_EQ);
		OPCODE_ESAC_BRCS(ble, CC_LT | CC_EQ);
		OPCODE_ESAC_BRCS(bgeu, CC_GTU | CC_EQ);
		OPCODE_ESAC_BRCS(bleu, CC_LTU | CC_EQ);
		default: {
			EXCEPTION("SIGILL3", SIGILL);
		} break;
	}
	return;

do_branch:
	PC += INST2OFFSET(inst);
}

static void moxie_inst_form2(moxie_p moxie, uint16_t inst)
{
	int opcode = get_form2_opcode(inst);

	int a = (inst >> 8) & 0x0f;
	uint32_t v = inst & 0xff;
	
	switch(opcode) {
		case opcode_inc: {
			R(a) += v;
		} break;
		case opcode_dec: {
			R(a) -= v;
		} break;
		case opcode_gsr: {
			R(a) = moxie->sregs[v];
		} break;
		case opcode_ssr: {
			moxie->sregs[v] = R(a);
		} break;
		/* getting here is theoretically impossible */
		default: {
			EXCEPTION("SIGILL2", SIGILL);
		} break;
	}
}

#define ESAC_OPCODE_A4RMW_B4V_ALU(trace_msg, esac, instop) \
	case opcode_##esac: { \
		get_form1_a4_b4v(inst); \
		R(a) instop bv; \
	} break;

#define ESAC_OPCODE_A4RMW_B4V_ALU_SIGNED(trace_msg, esac, instop) \
	case opcode_##esac: { \
		get_form1_a4_b4v(inst); \
		R(a) = ((signed)R(a)) instop ((signed)bv); \
	} break;

#define ESAC_OPCODE_LD_X(trace_msg, esac, len, regop, addrop) \
	case opcode_ld##esac: { \
		get_form1_a4_##regop(inst); \
		R(a) = moxie_fetch##len(moxie, addrop); \
	} break;

#define ESAC_OPCODE_LD(trace_msg, esac, len) \
	ESAC_OPCODE_LD_X(trace_msg, _##esac, len, b4v, bv);

#define ESAC_OPCODE_LDA(trace_msg, esac, len) \
	ESAC_OPCODE_LD_X(trace_msg, a_##esac, len, i32, ival);

#define ESAC_OPCODE_LDI(trace_msg, esac, len) \
	case opcode_ldi_##esac: { \
		get_form1_a4_i32(inst); \
		R(a) = ival & len; \
	} break;

#define ESAC_OPCODE_LDO(trace_msg, esac, len) \
	ESAC_OPCODE_LD_X(trace_msg, o_##esac, len, b4v_i32, ival + bv);

#define ESAC_OPCODE_ST_X(trace_msg, esac, len, regop, addrop, dataop) \
	case opcode_st##esac: { \
		get_form1_a4v_##regop(inst); \
		moxie_store##len(moxie, addrop, dataop); \
	} break;


#define ESAC_OPCODE_ST(trace_msg, esac, len) \
	ESAC_OPCODE_ST_X(trace_msg, _##esac, len, b4v, av, bv);

#define ESAC_OPCODE_STA(trace_msg, esac, len) \
	ESAC_OPCODE_ST_X(trace_msg, a_##esac, len, i32, ival, av);

#define ESAC_OPCODE_STO(trace_msg, esac, len) \
	ESAC_OPCODE_ST_X(trace_msg, o_##esac, len, b4v_i32, ival + av, bv);

#ifdef CONFIG_MOXIE_JUMPS_ABSOLUTE
	#define ESAC_OPCODE_JMP(trace_msg, esac, regop, new_pc) \
		case opcode_##esac: { \
			regop; \
			PC = new_pc; \
		} break;
#else
	#define ESAC_OPCODE_JMP(trace_msg, esac, regop, new_pc) \
		case opcode_##esac: { \
			regop; \
			PC = new_pc - 2; \
		} break;
#endif

#ifdef CONFIG_MOXIE_JUMPS_ABSOLUTE
	#define ESAC_OPCODE_JSR(trace_msg, esac, regop, new_pc) \
		case opcode_##esac: { \
			regop; \
			/* Save a slot for the static chain. */ \
			SP -= 4; \
			/* Push the return address */ \
			moxie_push32(moxie, PC); \
			/* Push the current frame pointer */ \
			moxie_push32(moxie, FP); \
			/* Set new frame pointer */ \
			FP = SP; \
			/* Set new pc */ \
			PC = new_pc; \
		} break;
#else
#define ESAC_OPCODE_JSR(trace_msg, esac, regop, new_pc) \
		case opcode_##esac: { \
			regop; \
			/* Save a slot for the static chain. */ \
			SP -= 4; \
			/* Push the return address */ \
			moxie_push32(moxie, PC); \
			/* Push the current frame pointer */ \
			moxie_push32(moxie, FP); \
			/* Set new frame pointer */ \
			FP = SP; \
			/* Set new pc */ \
			PC = new_pc - 2; \
		} break;
#endif

static moxie_inst_form1_swi(moxie_p moxie, uint16_t inst)
{
}

static moxie_inst_form1(moxie_p moxie, uint16_t inst)
{
	uint8_t opcode = get_form1_opcode(inst);
	switch(opcode) {
		case opcode_bad_0: {
			EXCEPTION("SIGILL0", SIGILL);
		} break;
		ESAC_OPCODE_LDI("ldi.l", l, -1L);
		ESAC_OPCODE_A4RMW_B4V_ALU("mov", mov, =);
		ESAC_OPCODE_JSR("jsra", jsra, get_i32(), ival);
		case opcode_ret: {
			/* back stack to frame */
			SP = FP;
			/* Pop the frame pointer. */
			FP = moxie_pop32(moxie);
			/* Pop the return address. */
			PC = moxie_pop32(moxie);
			/* Skip over the static chain slot. */
			(void)moxie_pop32(moxie);
		} break;
		ESAC_OPCODE_A4RMW_B4V_ALU("add.l", add_l, +=);
		case opcode_push: {
			get_form1_a4_b4v(inst);
			moxie_store_pre_decrement(moxie, &R(a), bv, 4);
		} break;
		case opcode_pop: {
			get_form1_a4_b4(inst);
			R(b) = moxie_fetch_post_increment(moxie, &R(a), 4);
		} break;
		ESAC_OPCODE_LDA("lda.l", l, 32);
		ESAC_OPCODE_STA("sta.l", l, 32);
		ESAC_OPCODE_LD("ld.l", l, 32);
		ESAC_OPCODE_ST("st.l", l, 32);
		ESAC_OPCODE_LDO("ldo.l", l, 32);
		ESAC_OPCODE_STO("sto.l", l, 32);
		case opcode_cmp: {
			get_form1_a4v_b4v(inst);
			if(av == bv)
				CC = CC_EQ;
			else {
				CC = (((signed)av) < ((signed)bv) ? CC_LT : 0);
				CC |= (((signed)av) > ((signed)bv) ? CC_GT : 0);
				CC |= (av < bv ? CC_LTU : 0);
				CC |= (av > bv ? CC_GTU : 0);
			}
		} break;
		case opcode_nop: {
		} break;
		case opcode_bad_10:
		case opcode_bad_11:
		case opcode_bad_12:
		case opcode_bad_13:
		case opcode_bad_14:
		case opcode_bad_15:
		case opcode_bad_16:
		case opcode_bad_17:
		case opcode_bad_18: {
			EXCEPTION("SIGILL0", SIGILL);
		} break;
		ESAC_OPCODE_JSR("jsr", jsr, get_form1_a4v(inst), av);
		ESAC_OPCODE_JMP("jmpa", jmpa, get_i32(inst), ival);
		ESAC_OPCODE_LDI("ldi.b", b, 0xff);
		ESAC_OPCODE_LD("ld.b", b, 8);
		ESAC_OPCODE_LDA("lda.b", b, 8);
		ESAC_OPCODE_ST("st.b", b, 8);
		ESAC_OPCODE_STA("sta.b", b, 8);
		ESAC_OPCODE_LDI("ldi.s", s, 0xffff);
		ESAC_OPCODE_LD("ld.s", s, 16);
		ESAC_OPCODE_LDA("lda.s", s, 16);
		ESAC_OPCODE_ST("st.s", s, 16);
		ESAC_OPCODE_STA("sta.s", s, 16);
		ESAC_OPCODE_JMP("jmp", jmp, get_form1_a4v(inst), av);
		ESAC_OPCODE_A4RMW_B4V_ALU("and", and, &=);
		ESAC_OPCODE_A4RMW_B4V_ALU("lshr", lshr, >>=);
		ESAC_OPCODE_A4RMW_B4V_ALU("ashl", ashl, <<=);
		ESAC_OPCODE_A4RMW_B4V_ALU("sub.l", sub_l, -=);
		ESAC_OPCODE_A4RMW_B4V_ALU("neg", neg, = -(signed));
		ESAC_OPCODE_A4RMW_B4V_ALU("or", or, |=);
		ESAC_OPCODE_A4RMW_B4V_ALU("not", not, = -1UL ^);
		ESAC_OPCODE_A4RMW_B4V_ALU_SIGNED("ashr", ashr, >>);
		ESAC_OPCODE_A4RMW_B4V_ALU("xor", xor, ^=);
		ESAC_OPCODE_A4RMW_B4V_ALU("mul.l", mul_l, *=);
		case opcode_swi: {
			moxie_inst_form1_swi(moxie, inst);
		} break;
		ESAC_OPCODE_A4RMW_B4V_ALU_SIGNED("div.l", div_l, /);
		ESAC_OPCODE_A4RMW_B4V_ALU("udiv.l", udiv_l, /=);
		ESAC_OPCODE_A4RMW_B4V_ALU_SIGNED("mod.l", mod_l, %);
		ESAC_OPCODE_A4RMW_B4V_ALU("umod.l", umod_l, %=);
		case opcode_brk: {
			EXCEPTION("brk", SIGTRAP);
		} break;

		ESAC_OPCODE_LDO("ldo.b", b, 8);
		ESAC_OPCODE_STO("sto.b", b, 8);
		ESAC_OPCODE_LDO("ldo.s", s, 16);
		ESAC_OPCODE_STO("sto.s", s, 16);
		default: {
#ifdef CONFIG_MOXIE_USE_SWTICH_LOOP
			if(inst & (1 << 15)) {
				if(inst & (1 << 14)) {
					moxie_inst_form3(moxie, inst);
				} else {
					moxie_inst_form2(moxie, inst);
				}
			} else
#endif
				EXCEPTION("SIGILL1", SIGILL);
		} break;
	}
}

void inline moxie_run_one(moxie_p moxie)
{
	if(PC > moxie->data_size) {
		printf("%s: pc address (%08x) exceeds end of data.\n", __FUNCTION__, PC);
		exit(-1);
	}

	uint16_t inst = moxie_fetch_post_increment(moxie, &PC, 2);

#ifdef CONFIG_MOXIE_USE_SWTICH_LOOP
	moxie_inst_form1(moxie, inst);
#else
	if(inst & (1 << 15)) {
		if(inst & (1 << 14)) {
			moxie_inst_form3(moxie, inst);
		} else {
			moxie_inst_form2(moxie, inst);
		}
	} else {
		moxie_inst_form1(moxie, inst);
	}
#endif
	moxie->insts++;
}

uint64_t moxie_run_count(moxie_p moxie, uint16_t count)
{
	uint64_t start_time = profiler_get_dtime();

#ifdef CONFIG_MOXIE_BUILD_TRACE
	int saved_signal = moxie->exception.id;
	moxie->exception.id = moxie->trace ? SIGTRAP : 0;	
#endif

	do {
		moxie_run_one(moxie);
		count--;
	} while(!moxie->exception.id && count);

#ifdef CONFIG_MOXIE_BUILD_TRACE
	if(moxie->trace_hook && moxie->trace) {
		moxie->trace_hook(moxie, PC, 0);
		if(moxie->exception.id == SIGTRAP)
			moxie->exception.id = saved_signal;
	}
#endif

	return(profiler_get_elapsed_dtime(start_time));
}

moxie_p moxie_init(moxie_p user_moxie, uint32_t data_size, int trace)
{
	moxie_p moxie;

	if(user_moxie) {
		moxie = user_moxie;
	} else {
		moxie = malloc(sizeof(moxie_t));
		moxie->data = 0;
	}
	
	if(!moxie)
		goto no_moxie_fail;

	if(user_moxie && moxie->data) {
		if(moxie->data_size != data_size) {
			uint8_t *moxie_data = realloc(moxie->data, data_size);
			if(!moxie_data)
				goto no_data_fail;
			moxie->data = moxie_data;
		}
	} else {
		moxie->data_size = data_size;
		moxie->data = malloc(moxie->data_size);
	}
	
	if(!moxie->data)
		goto no_data_fail;

	set_initial_gprs(moxie);

	moxie->exception.id = 0;
	moxie->insts = 0;

#ifdef CONFIG_MOXIE_BUILD_TRACE
	if(trace)
		moxie_trace_init(moxie);
	else
#endif
		moxie->trace_hook = 0;

	return(moxie);

no_data_fail:
	if(moxie && (moxie != user_moxie))
		free(moxie);

no_moxie_fail:
	return(0);
}

