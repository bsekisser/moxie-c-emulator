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

#ifndef __LIBMOXIE_ACCESS_H__
#define __LIBMOXIE_ACCESS_H__

uint32_t moxie_fetch(moxie_p moxie, uint32_t addr, int len);

uint8_t moxie_fetch8(moxie_p moxie, uint32_t addr);
uint16_t moxie_fetch16(moxie_p moxie, uint32_t addr);
uint32_t moxie_fetch32(moxie_p moxie, uint32_t addr);

uint32_t moxie_fetch_post_increment(moxie_p moxie, uint32_t *addr, int len);

void moxie_store(moxie_p moxie, uint32_t addr, uint32_t dval, int len);

void moxie_store8(moxie_p moxie, uint32_t addr, uint8_t dval);
void moxie_store16(moxie_p moxie, uint32_t addr, uint16_t dval);
void moxie_store32(moxie_p moxie, uint32_t addr, uint32_t dval);

void moxie_store_post_increment(moxie_p moxie, uint32_t *addr, uint32_t dval, int len);
void moxie_store_pre_decrement(moxie_p moxie, uint32_t *addr, uint32_t dval, int len);

void moxie_push32(moxie_p moxie, uint32_t data);
uint32_t moxie_pop32(moxie_p moxie);

#endif /* __LIBMOXIE_ACCESS_H__ */
