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
#include <stdlib.h> // malloc
#include <stdio.h> // printf

#include "libmoxie.h"
#include "profiler.h"

#define KiB(x) ((x) * 1024UL)
#define MiB(x) (KiB(x) * 1024UL)

#define SINGLE_LINE_STATS

void main(int arc, char *argv[])
{
	int trace = 1;
	uint32_t data_size = KiB(64);

	moxie_t run_moxie;
	run_moxie.data = 0;
	run_moxie.start_pc = 0x1000;
	
	moxie_p moxie = moxie_init(&run_moxie, data_size, trace);
	if(!moxie) {
		printf("%s: Could not intialize moxie\n");
		exit(-1);
	}

	moxie_compiler(moxie);
	
	printf("%s: data @(%0p) size: 0x%08x, pc(%08x)\n\n", 
		__FUNCTION__, moxie->data, moxie->data_size, PC);

	uint64_t overhead;
	uint64_t cyclesPerSecond = profiler_calibrate(&overhead);
	uint64_t run_start_time = profiler_get_dtime_overhead(overhead);
	uint64_t one_elapsed_dtime = 0;
	double target_ipsec = MHz(10.0);
	uint16_t run_cycles = 65535;
	uint64_t run_dtime = 0;

	printf("\n\n");
	
	while(!moxie->exception.id) {
		run_cycles = (run_cycles > 0) ? run_cycles : 1;
		
		run_dtime += moxie_run_count(moxie, run_cycles);

		uint64_t elapsed_dtime = profiler_get_elapsed_dtime_overhead(run_start_time, overhead);

		double eacdt = (double)elapsed_dtime / (double)moxie->insts;
		double oeacdt = (double)run_dtime / (double)moxie->insts;
		
		double run_seconds = (double)elapsed_dtime / (double)cyclesPerSecond;
		double ipsec = (double)moxie->insts / run_seconds;
		double mipsec = ipsec / MHz(1);

		if(!moxie->trace) {
#ifdef SINGLE_LINE_STATS
			printf("count: %016llu elapsed: %016llu eacdt: %012.4f oeacdt: %012.4f ", 
				moxie->insts, elapsed_dtime, eacdt, oeacdt);
			if(mipsec > 1) {
				printf("mipsec: %08.4f ", mipsec);
			} else {
				double kipsec = ipsec / KHz(1);
				printf("kipsec: %012.6f ", kipsec);
			}

			printf("run_cycles: %08d    \r", run_cycles);
#else
			printf("\n");
			printf("     count: %016llu    \n", moxie->insts);
			printf("   elapsed: %016llu    \n", elapsed_dtime);
			printf("     eacdt: %012.4f    \n", eacdt);
			printf("    oeacdt: %012.4f    \n", oeacdt); 

			if(mipsec > 1) {
				printf("    mipsec: %08.4f    \n", mipsec);
			} else {
				double kipsec = ipsec / KHz(1);
				printf("    kipsec: %012.6f    \n", kipsec);
			}

			printf("run_cycles: %08d    \n", run_cycles);
		
			int i;
			for(i = 0; i < 7; i++) {
				printf("%c%c", 0x1B, 'M');
			}
#endif
		}
	}

	printf("\n\n");

	if(moxie->exception.id)
		printf("%s: Stopped at PC: 0x%08x with exception (%d) %s\n", 
			__FUNCTION__, PC, moxie->exception.id, moxie->exception.msg);

	printf("\n\n");
}

