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

#ifndef __LIBMOXIE_H__
#define __LIBMOXIE_H__

#define NUM_MOXIE_REGS 17
#define NUM_MOXIE_SREGS 256

typedef struct moxie_t *moxie_p;
typedef struct moxie_t {
	uint32_t	regs[NUM_MOXIE_REGS + 1];
	uint32_t	sregs[NUM_MOXIE_SREGS];
	int		cc;

	uint64_t	insts;

	uint8_t		*data;
	uint32_t	data_size;

	struct {
		const char	*msg;
		int		id;
	}exception;
	
	uint32_t	start_pc;
	
	int		trace;
	void		(*trace_hook)(moxie_p moxie, uint32_t trace_pc, int compiling);
}moxie_t;

#define REG_LIST \
	REG_ESAC_0(rFP) \
	REG_ESAC(rSP) \
	REG_ESAC(r0) \
	REG_ESAC(r1) \
	REG_ESAC(r2) \
	REG_ESAC(r3) \
	REG_ESAC(r4) \
	REG_ESAC(r5) \
	REG_ESAC(r6) \
	REG_ESAC(r7) \
	REG_ESAC(r8) \
	REG_ESAC(r9) \
	REG_ESAC(r10) \
	REG_ESAC(r11) \
	REG_ESAC(r12) \
	REG_ESAC(r13) \
	REG_ESAC(rPC)

#undef REG_ESAC_0
#define REG_ESAC_0(x) x = 0x00,
#undef REG_ESAC
#define REG_ESAC(x) x,

enum {
	REG_LIST
};

#define R(x) moxie->regs[x]

#define FP R(rFP)
#define PC R(rPC)
#define SP R(rSP)

void set_initial_gprs(moxie_p moxie);
void moxie_run_one(moxie_p moxie);
uint64_t moxie_run_count(moxie_p moxie, uint16_t count);
moxie_p moxie_init(moxie_p user_moxie, uint32_t data_size, int trace);

void moxie_trace_init(moxie_p moxie);

#endif /* __LIBMOXIE_H__ */
