/*
    FlexCOS - Copyright (C) 2013 AGSI, Department of Computer Science, FU-Berlin

    FOR MORE INFORMATION AND INSTRUCTION PLEASE VISIT
    http://www.inf.fu-berlin.de/groups/ag-si/smart.html


    This file is part of the FlexCOS project.

    FlexCOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 3) as published by the
    Free Software Foundation.

    Some parts of this software are from different projects. These files carry
    a different license in their header.

    FlexCOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
    details. You should have received a copy of the GNU General Public License
    along with FlexCOS; if not it can be viewed here:
    http://www.gnu.org/licenses/gpl-3.0.txt and also obtained by writing to
    AGSI, contact details for whom are available on the FlexCOS WEB site.

*/

#include <flxlib.h>
#include <array.h>

#include <miracl.h>

PUBLIC err_t
kdf_nist2(const array *input, const array *other, array *key)
{
	char   h[32];
	const unsigned char   *c;
	sha256 sh;
	int    i;
	u32    cnt;

	for (cnt = 0x01; array_bytes_left(key); cnt++) {
		shs256_init(&sh);
		for (i = 3; (int)i >= 0; i--)
			shs256_process(&sh, (cnt >> i));

		array_each(input, c)
			shs256_process(&sh, *c);

		if (other) array_each(other, c)
			shs256_process(&sh, *c);

		shs256_hash(&sh, h);
		array_append(key, h, 32);
	}
	return E_GOOD;
}

PUBLIC u16
array_put_big(array *arr, const big b)
{
	u16 bytes;
	// TODO check parameter

	// TODO use correct byte length
	bytes = b->len * sizeof(mr_small);
	if (array_bytes_left(arr) < bytes)
		return 0;

	arr->len += big_to_bytes(bytes, b, array_end(arr), TRUE);
	return bytes;
}
