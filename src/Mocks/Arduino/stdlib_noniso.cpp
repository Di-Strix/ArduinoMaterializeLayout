#if UNIT_TEST && UNIT_TEST_NATIVE

/*
  stdlib_noniso.h - nonstandard (but useful) conversion functions

  Copyright (c) 2021 David Gauchard. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "stdlib_noniso.h"

// ulltoa() is slower than std::to_char() (1.6 times)
// but is smaller by ~800B/flash and ~250B/rodata

// ulltoa fills str backwards and can return a pointer different from str
char* ulltoa(unsigned long long val, char* str, int slen, unsigned int radix)
{
    str += --slen;
    *str = 0;
    do
    {
        auto mod = val % radix;
        val /= radix;
        *--str = mod + ((mod > 9) ? ('a' - 10) : '0');
    } while (--slen && val);
    return val? nullptr: str;
}

char* itoa (int val, char *str, int radix) {
    const char * format = nullptr;

	switch(radix)
	{
		case 8:
			format = "%o";
			break;
		case 10:
			format = "%u";
			break;
		case 16:
			format = "%x";
			break;
	}

	if(format == nullptr)
		return str;

	int size = std::sprintf(str, format, val);
	return &str[size];
}

char * utoa(unsigned int value, char * str, int radix)
{
	const char * format = nullptr;

	switch(radix)
	{
		case 8:
			format = "%o";
			break;
		case 10:
			format = "%u";
			break;
		case 16:
			format = "%x";
			break;
	}

	if(format == nullptr)
		return str;

	int size = std::sprintf(str, format, value);
	return &str[size];
}

// lltoa fills str backwards and can return a pointer different from str
char* lltoa (long long val, char* str, int slen, unsigned int radix)
{
    bool neg;
    if (val < 0)
    {
        val = -val;
        neg = true;
    }
    else
    {
        neg = false;
    }
    char* ret = ulltoa(val, str, slen, radix);
    if (neg)
    {
        if (ret == str || ret == nullptr)
            return nullptr;
        *--ret = '-';
    }
    return ret;
}

#endif