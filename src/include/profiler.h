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

#ifndef __PROFILER_H__
#define __PROFILER_H__

static inline uint64_t profiler_get_dtime(void) {
   	uint32_t hi, lo;
   	
	__asm__ __volatile__ ("xorl %%eax,%%edx\n" : : : "%eax", "%edx");
	__asm__ __volatile__ ("xorl %%edx,%%eax\n" : : : "%edx", "%eax");
	__asm__ __volatile__ ("xorl %%eax,%%edx\n" : : : "%eax", "%edx");
	__asm__ __volatile__ ("xorl %%edx,%%eax\n" : : : "%edx", "%eax");
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	
	return(((uint64_t)hi << 32) | (uint64_t)lo);
}

static inline uint64_t profiler_get_dtime_overhead(uint64_t overhead)
{
	return(profiler_get_dtime() - overhead);
}

static inline uint64_t profiler_get_elapsed_dtime(uint64_t start_time)
{
	return(profiler_get_dtime() - start_time);
}

static inline uint64_t profiler_get_elapsed_dtime_overhead(uint64_t start_time, uint64_t overhead)
{
	return(profiler_get_dtime_overhead(overhead) - start_time);
}

#define KHz(hz) ((hz)*1000ULL)
#define MHz(hz) KHz(KHz(hz))

uint64_t profiler_calibrate(uint64_t *overhead);

#endif /* __PROFILER_H__ */

