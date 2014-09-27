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

#include "libmoxie.h"
#include "libmoxie-access.h"
#include "libmoxie-compiler.h"
#include "libmoxie-opcode.h"

void c_test_2010_09(moxie_p moxie)
{
	ORG(0);
LABEL(start);
	LDI_L(rSP, 0x00004000);
	LDI_L(rFP, 0);
	DEC(rSP, 0x0c);
	NOP;
	NOP;
}

void c_test_2011_09(moxie_p moxie)
{
	ORG(0);
LABEL(start);
	XOR(r0, r0);
	MOV(r1, r0);
	MOV(r2, r0);
	MOV(r3, r0);
	MOV(r4, r0);
	MOV(r5, r0);
LABEL(loop);
	INC(r0, 1);
	INC(r1, 1);
	INC(r2, 1);
	INC(r3, 1);
	INC(r4, 1);
	JMPA(loop);
	NOP
	NOP
}

void c_test_2014_24_jsra_ret(moxie_p moxie)
{
	ORG(0x1000);
LABEL(start);
	LDI_L(rSP, 0x00000100);
	MOV(rFP, rSP);
	XOR(r0, r0);
	MOV(r1, r0);
LABEL(back_jmp);
	CMP(r0, r1);
//	BEQ(skip1);
	BEQ(PC + 2);
	RET
LABEL(skip1);
	INC(r0, 1);
	JSRA(back_jmp);
//	BRK;
	JSRA(start);
	BAD;
}

void c_test_load_binner(moxie_p moxie)
{
	FILE *fd = fopen("rom2.bin", "r");
	
	ORG(0x1000);
	uint16_t *src, *dst = (uint16_t *)&moxie->data[moxie->start_pc];
	
	int len = fread((uint8_t *)dst, 1, 65536, fd);
	fclose(fd);
	
	printf("\n\nStart PC: 0x%08x, LEN: 0x%04x\n\n", moxie->start_pc, len);

}

void moxie_compiler(moxie_p moxie)
{
	printf(">> %s\n", __FUNCTION__);
	
//	c_test_2010_09(moxie);
//	c_test_2011_09(moxie);

	c_test_2014_24_jsra_ret(moxie);

//	c_test_load_binner(moxie);
	
	PC = moxie->start_pc;

	printf("<< %s\n", __FUNCTION__);
}

