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

#ifndef __LIBMOXIE_CONFIG_H__
#define __LIBMOXIE_CONFIG_H__

//#define CONFIG_MOXIE_USE_SWTICH_LOOP
#undef CONFIG_MOXIE_USE_SWTICH_LOOP

#define CONFIG_MOXIE_BUILD_TRACE
//#undef CONFIG_MOXIE_BUILD_TRACE

#define CONFIG_MOXIE_BUILD_COMPILER
//#undef CONFIG_MOXIE_BUILD_COMPILER

#define CONFIG_MOXIE_LE
//#undef CONFIG_MOXIE_LE

#define CONFIG_MOXIE_TRAP_PC_NON_WORD_BOUNDRY
//#undef CONFIG_MOXIE_TRAP_PC_NON_WORD_BOUNDRY

#endif /* __LIBMOXIE_CONFIG_H__ */

