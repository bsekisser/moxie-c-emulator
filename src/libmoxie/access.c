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
#include <stdio.h>
#include <stdlib.h>
#include <endian.h>

#include "libmoxie-config.h"

#include "libmoxie.h"
#include "libmoxie-access.h"

#ifdef CONFIG_MOXIE_LE
	#define HOST32(x) le32toh(x)
	#define HOST16(x) le16toh(x)
	#define MOXIE32(x) htole32(x)
	#define MOXIE16(x) htole16(x)
#else
	#define HOST32(x) be32toh(x)
	#define HOST16(x) be16toh(x)
	#define MOXIE32(x) htobe32(x)
	#define MOXIE16(x) htobe16(x)
#endif

uint32_t moxie_fetch(moxie_p moxie, uint32_t addr, int len)
{
	if(addr > moxie->data_size) {
		printf("%s: address (%08x) exceeds end of data.\n", __FUNCTION__, addr);
		exit(-1);
	}

	void		*src = (void *)&moxie->data[addr];
	uint32_t	dval;

	switch(len) {
		case	4:
			dval = HOST32(*(uint32_t *)src);
			break;
		case	2:
			dval = HOST16(*(uint16_t *)src);
			break;
		case	1:
			dval = *(uint8_t *)src;
			break;
		default:
			printf("%s: unhandle length %d\n", __FUNCTION__, len);
			exit(-1);
			break;
	}
	return(dval);
}

uint8_t moxie_fetch8(moxie_p moxie, uint32_t addr)
{
	if(addr > moxie->data_size) {
		printf("%s: address (%08x) exceeds end of data.\n", __FUNCTION__, addr);
		exit(-1);
	}

	return(moxie->data[addr]);
}

uint16_t moxie_fetch16(moxie_p moxie, uint32_t addr)
{
	return(moxie_fetch(moxie, addr, 2));
}

uint32_t moxie_fetch32(moxie_p moxie, uint32_t addr)
{
	return(moxie_fetch(moxie, addr, 4));
}

uint32_t moxie_fetch_post_increment(moxie_p moxie, uint32_t *addr, int len)
{
	uint32_t	dval = moxie_fetch(moxie, *addr, len);

	(*addr) += len;

	return(dval);
}

void moxie_store(moxie_p moxie, uint32_t addr, uint32_t dval, int len)
{
	if(addr > moxie->data_size) {
		printf("%s: address (%08x) exceeds end of data.\n", __FUNCTION__, addr);
		exit(-1);
	}

	void		*dst = (void *)&moxie->data[addr];
	
	switch(len) {
		case	4:
			*(uint32_t *)dst = MOXIE32(dval);
			break;
		case	2:
			*(uint16_t *)dst = MOXIE16(dval);
			break;
		case	1:
			*(uint8_t *)dst = dval;
			break;
		default:
			printf("%s: unhandle length %d\n", __FUNCTION__, len);
			exit(-1);
			break;
	}
}

void moxie_store8(moxie_p moxie, uint32_t addr, uint8_t dval)
{
	moxie->data[addr] = dval;
}

void moxie_store16(moxie_p moxie, uint32_t addr, uint16_t dval)
{
	moxie_store(moxie, addr, dval, 2);
}

void moxie_store32(moxie_p moxie, uint32_t addr, uint32_t dval)
{
	moxie_store(moxie, addr, dval, 4);
}

void moxie_store_post_increment(moxie_p moxie, uint32_t *addr, uint32_t dval, int len)
{
	moxie_store(moxie, *addr, dval, len);
	(*addr) += len;
}

void moxie_store_pre_decrement(moxie_p moxie, uint32_t *addr, uint32_t dval, int len)
{
	(*addr) -= len;
	moxie_store(moxie, *addr, dval, len);
}

void moxie_push32(moxie_p moxie, uint32_t data)
{
	moxie_store_pre_decrement(moxie, &moxie->regs[rSP], data, 4);
}

uint32_t moxie_pop32(moxie_p moxie)
{
	return(moxie_fetch_post_increment(moxie, &moxie->regs[rSP], 4));
}

