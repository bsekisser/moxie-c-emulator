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
#include <stdio.h> // printf
#include <string.h> //strlen

#include "config.h"

#include "libmoxie.h"
#include "libmoxie-access.h"
#include "libmoxie-opcode.h"

#undef REG_ESAC_0
#define REG_ESAC_0(x) #x,
#undef REG_ESAC
#define REG_ESAC(x) #x,

const char *regname[NUM_MOXIE_REGS] = {
	REG_LIST
};

static const char spaces[41] = "                                        \0";
#define SPACES(x) &spaces[40 - (x)]

typedef struct trace_info_t {
	char		*name;
	uint16_t	inst;
	int		form;
	uint16_t	opcode;
	int		trace_arg;
}trace_info_t, *trace_info_p;

enum {
	TRACEOP_Z = 0x00,
	TRACEOP_I32_vRa,
	TRACEOP_I32,
	TRACEOP_I32OvRa_vRb,
	TRACEOP_O10,
	TRACEOP_Ra_Rb,
	TRACEOP_Ra_I32,
	TRACEOP_Ra_I32OvRb,
	TRACEOP_Ra_v32,
	TRACEOP_Ra_vI32,
	TRACEOP_Ra_vI32ORb,
	TRACEOP_Ra_vRb,
	TRACEOP_Rb_vRa,
	TRACEOP_vRa,
	TRACEOP_vRa_I,
	TRACEOP_vRa_I32,
	TRACEOP_vRa_Rb,
	TRACEOP_vRa_vRb
};

#define TRACE_INST_ESAC(esac, trace_name, traceop) \
	case opcode_##esac: \
		trace_info->name = trace_name; \
		trace_info->trace_arg = TRACEOP_##traceop; \
		break;

#undef INST_ESAC_0
#define INST_ESAC_0(esac, trace_name, traceop) \
	TRACE_INST_ESAC(esac, trace_name, traceop)
	
#undef INST_ESAC
#define INST_ESAC(esac, trace_name, traceop) \
	TRACE_INST_ESAC(esac, trace_name, traceop)


static uint16_t trace_get_inst_info(moxie_p moxie, trace_info_p trace_info, uint16_t inst)
{
	trace_info->inst = inst;
	
	uint16_t len;
	const char *name;
	
	if(inst & (1 << 15)) {
		if(inst & (1 << 14)) {
			trace_info->form = 3;
			trace_info->opcode = get_form3_opcode(inst);
			switch(trace_info->opcode) {
				FORM3_INST_LIST
				default:
					name = "FORM3_DEFAULT";
				break;
			}
		} else {
			trace_info->form = 2;
			trace_info->opcode = get_form2_opcode(inst);
			switch(trace_info->opcode) {
				FORM2_INST_LIST
				default:
					name = "FORM2_DEFAULT";
				break;
			}
		}
	} else {
		trace_info->form = 1;
		trace_info->opcode = get_form1_opcode(inst);
		switch(trace_info->opcode) {
			FORM1_INST_LIST
			default:
				name = "FORM1_DEFAULT";
			break;
		}
	}
	
	return(inst);
}

static void moxie_trace_hook(moxie_p moxie, uint32_t trace_pc, int compiling) {
	uint32_t saved_pc = PC;
	
	PC = trace_pc;
	
	trace_info_t trace_info;

	printf("%08x: ", PC);
	
	uint16_t inst = trace_get_inst_info(moxie, &trace_info, moxie_fetch_post_increment(moxie, &PC, 2));

	int inst_len = 0;
	switch(trace_info.trace_arg) {
		case	TRACEOP_I32:
		case	TRACEOP_I32OvRa_vRb:
		case	TRACEOP_Ra_I32:
		case	TRACEOP_Ra_I32OvRb:
		case	TRACEOP_Ra_vI32:
		case	TRACEOP_Ra_vI32ORb:
		case	TRACEOP_vRa_I32:
			inst_len = 6;
			break;
		default:
			inst_len = 2;
			break;
	}

	int i;
#ifdef CONFIG_MOXIE_LE
	printf("%02x ", (inst >> 8) & 0xff);
	printf("%02x ", inst & 0xff);
	for(i = 2; i < 7; i++)
#else
	for(i = 0; i < 7; i++)
#endif
	{
		if(i < inst_len)
#ifdef CONFIG_MOXIE_LE
			printf("%02x ", moxie_fetch8(moxie, trace_pc + inst_len - i + 1));
#else
			printf("%02x ", moxie_fetch8(moxie, trace_pc + i));
#endif
		else
			printf("   ");
	}

	printf("%s", trace_info.name);
	
	printf("%s", SPACES(8 - strlen(trace_info.name)));

	switch(trace_info.trace_arg) {
		case	TRACEOP_I32: {
			get_i32(trace_info.inst);
			printf("0x%08lx", ival);
		} break;
		case	TRACEOP_I32_vRa: {
			if(compiling) {
				get_form1_a4_i32(trace_info.inst);
				printf("0x%08lx, %s", ival, regname[a]);
			} else {
				get_form1_a4v_i32(trace_info.inst);
				printf("0x%08lx, %s(0x%08x)", ival, regname[a], av);
			}
		} break;
		case	TRACEOP_I32OvRa_vRb: { /* FIXME */
			if(compiling) {
				get_form1_a4_b4(trace_info.inst);
				get_i32(trace_info.inst);
				printf("0x%08lx[%s], %s", ival, regname[b], regname[a]);
			} else {
				get_form1_a4v_b4v_i32(trace_info.inst);
				printf("0x%08lx[%s(0x%08x)], %s(%08x)", 
					ival, regname[b], bv, regname[a], av);
			}
		} break;
		case	TRACEOP_O10: {
			int32_t branch_pc = PC + (int32_t)INST2OFFSET(trace_info.inst);
			printf("0x%08lx", branch_pc);
		} break;
		case	TRACEOP_Ra_I32: {
			get_form1_a4_i32(trace_info.inst);
			printf("%s, 0x%08lx", regname[a], ival);
		} break;
		case	TRACEOP_Ra_I32OvRb:
		case	TRACEOP_Ra_vI32ORb: {
			if(compiling) {
				get_form1_a4_b4(trace_info.inst);
				get_i32(trace_info.inst);
				printf("%s, 0x%08lx[%s]", regname[a], ival, regname[b]);
			} else {
				get_form1_a4_b4v_i32(trace_info.inst);
				printf("%s, 0x%08lx[%s(0x%08x)]", regname[a], ival, regname[b], bv);
			}
		} break;
		case	TRACEOP_Ra_Rb: {
			get_form1_a4_b4(trace_info.inst);
			printf("%s, %s", regname[a], regname[b]);
		} break;
		case	TRACEOP_Ra_vRb: {
			if(compiling) {
				get_form1_a4_b4(trace_info.inst);
				printf("%s, %s", regname[a], regname[b]);
			} else {
				get_form1_a4_b4v(trace_info.inst);
				printf("%s, %s(0x%08x)", regname[a], regname[b], bv);
			}
		} break;
		case	TRACEOP_Rb_vRa: {
			if(compiling) {
				get_form1_a4_b4(trace_info.inst);
				printf("%s, %s", regname[b], regname[a]);
			} else {
				get_form1_a4_b4(trace_info.inst);
				printf("%s, %s(0x%08x)", regname[b], regname[a], R(a));
			}
		} break;
		case	TRACEOP_vRa: {
			if(compiling) {
				get_form1_a4(trace_info.inst);
				printf("%s", regname[a]);
			} else {
				get_form1_a4v(trace_info.inst);
				printf("%s(0x%08x)", regname[a], av);
			}
		}	break;
		case	TRACEOP_vRa_I: {
			int a = (trace_info.inst >> 8) & 0x0f;
			uint32_t v = trace_info.inst & 0xff;
			if(compiling) {
				printf("%s, 0x%02x", regname[a], v);
			} else {
				printf("%s(0x%08x), 0x%02x", regname[a], R(a), v);
			}
			break;
		}
		case	TRACEOP_vRa_I32: {
			if(compiling) {
				get_form1_a4_i32(trace_info.inst);
				printf("%s, 0x%08lx", regname[a], ival);
			} else {
				get_form1_a4v_i32(trace_info.inst);
				printf("%s(0x%08x), 0x%08lx", regname[a], av, ival);
			}
		} break;
		case	TRACEOP_vRa_Rb: {
			if(compiling) {
				get_form1_a4_b4(trace_info.inst);
				printf("%s, %s", regname[a], regname[b]);
			} else {
				get_form1_a4v_b4v(trace_info.inst);
				printf("%s(0x%08x), %s(0x%08x)", regname[a], av, regname[b], bv);
			}
		} break;
		case	TRACEOP_vRa_vRb: {
			if(compiling) {
				get_form1_a4_b4(trace_info.inst);
				printf("%s, %s", regname[a], regname[b]);
			} else {
				get_form1_a4v_b4v(trace_info.inst);
				printf("%s(0x%08x), %s(0x%08x)", regname[a], av, regname[b], bv);
			}
		} break;
	}
	
	printf("\n");
	
	PC = saved_pc;
}

void moxie_trace_init(moxie_p moxie)
{
	moxie->trace_hook = moxie_trace_hook;
}
