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

#include "profiler.h"

static uint64_t profiler_calibrate_get_dtime_overhead(void)
{
	uint64_t elapsedTime = profiler_get_elapsed_dtime(profiler_get_dtime());

	int i;
	for(i=2; i<=1024; i++) {
		elapsedTime += profiler_get_elapsed_dtime(profiler_get_dtime());
	}
		
	return(elapsedTime / i);
	
}

static uint64_t profiler_calibrate_get_dtime_sleep_overhead(uint64_t overhead)
{
   	uint64_t start = profiler_get_dtime_overhead(overhead);
	
	sleep(1);
		
	return(profiler_get_elapsed_dtime_overhead(start, overhead));
}

uint64_t profiler_calibrate(uint64_t *overhead)
{
	*overhead = profiler_calibrate_get_dtime_overhead();
	uint64_t elapsedTime, ecdt;
	double emhz;

	printf("%s: calibrate_get_dtime_cycles (overhead: %016llu)\n", __FUNCTION__, *overhead);

	elapsedTime = 0;

	int i;
	for(i = 1; i <= 3; i++) {
		elapsedTime += profiler_calibrate_get_dtime_sleep_overhead(*overhead);

		ecdt = elapsedTime / i;
		emhz = ecdt / MHz(1);
		printf("%s: elapsed time: %016llu  ecdt: %016llu  estMHz: %010.4f\n", 
			__FUNCTION__, elapsedTime, ecdt, emhz);
	}
	return(ecdt);
}

