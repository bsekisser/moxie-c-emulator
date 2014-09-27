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
#include <stdlib.h> //exit

#include "libmoxie-config.h"

#include "libmoxie.h"
#include "libmoxie-access.h"
#include "libmoxie-compiler.h"
#include "libmoxie-opcode.h"

#define TRACE() \
	do { \
		if(moxie->trace_hook && moxie->trace) \
			moxie->trace_hook(moxie, trace_pc, 1); \
	} while(0);

void c_form1_opcode(moxie_p moxie, uint8_t opcode, char *opname)
{
	uint32_t trace_pc = PC;

	c16((opcode & 0x3f) << 8);

	TRACE();
}

void c_form1abi_opcode(moxie_p moxie, uint8_t opcode, char *opname,
	int ra_dst, int rb_src,	uint32_t ival)
{
	uint32_t trace_pc = PC;

	uint16_t dval_out = (opcode & 0x3f) << 8;
	dval_out |= ((ra_dst & 0xf) << 4) | (rb_src & 0xf);

	c16(dval_out);
	c32(ival);

	TRACE();
}

void c_form1ai_opcode(moxie_p moxie, uint8_t opcode, char *opname,
	int ra_dst, uint32_t ival)
{
	uint32_t trace_pc = PC;

	uint16_t dval_out = (opcode & 0x3f) << 8;
	dval_out |= ((ra_dst & 0xf) << 4);
	
	c16(dval_out);
	c32(ival);

	TRACE();
}

void c_form1i_opcode(moxie_p moxie, uint8_t opcode, char *opname, uint32_t ival)
{
	uint32_t trace_pc = PC;
	
	uint16_t dval_out = (opcode & 0x3f) << 8;
	
	c16(dval_out);
	c32(ival);
	
	TRACE();
}

void c_form1ab_opcode(moxie_p moxie, uint8_t opcode, char *opname, int ra_dst, int rb_src)
{
	uint32_t trace_pc = PC;

	uint16_t dval_out = (opcode & 0x3f) << 8;
	dval_out |= ((ra_dst & 0xf) << 4) | (rb_src & 0xf);

	c16(dval_out);

	TRACE();
}

void c_form2av_opcode(moxie_p moxie, uint8_t opcode, char *opname, int ra_dst, int ival)
{
	uint32_t trace_pc = PC;

	uint16_t dval_out = (0x80 | ((opcode & 0x3) << 4) | (ra_dst & 0xf)) << 8;
	dval_out |= ival;
	
	c16(dval_out);
	
	TRACE();
}

#define OFFSET_MASK ((1 << 10) - 1)
#define FORM3_OPCODE_MASK ((1 << 4) - 1)

void c_form3_opcode(moxie_p moxie, uint8_t opcode, char *opname, int32_t ival)
{
	uint32_t trace_pc = PC;

	int32_t offset = ival - PC; /* get PC relative offset */
	offset >>= 1; /* branches as word offsets */

	if(offset > OFFSET_MASK) {
		printf("offset overflow");
		exit(-1);
	}

	uint16_t opcode_masked = opcode & FORM3_OPCODE_MASK;
	uint16_t offset_masked = offset & OFFSET_MASK;
	
	uint16_t dval_out = 0xC000 | (opcode_masked << (16 - 6)) | offset_masked;

	c16(dval_out);

	TRACE();
}

