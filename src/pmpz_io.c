/* pmpz_io -- mpz Input/Output functions
 *
 * Copyright (C) 2011 Daniele Varrazzo, Gianni Ciolli
 *
 * This file is part of the PostgreSQL GMP Module
 *
 * The PostgreSQL GMP Module is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the License,
 * or (at your option) any later version.
 *
 * The PostgreSQL GMP Module is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the PostgreSQL GMP Module.  If not, see
 * http://www.gnu.org/licenses/.
 */

#include "pmpz.h"

#include "fmgr.h"


PG_FUNCTION_INFO_V1(pmpz_in);
PG_FUNCTION_INFO_V1(pmpz_out);

PG_FUNCTION_INFO_V1(pmpz_from_smallint);
PG_FUNCTION_INFO_V1(pmpz_from_integer);
PG_FUNCTION_INFO_V1(pmpz_from_bigint);

Datum       pmpz_in(PG_FUNCTION_ARGS);
Datum       pmpz_out(PG_FUNCTION_ARGS);

Datum       pmpz_from_smallint(PG_FUNCTION_ARGS);
Datum       pmpz_from_integer(PG_FUNCTION_ARGS);
Datum       pmpz_from_bigint(PG_FUNCTION_ARGS);


/*
 * Input/Output functions
 */

Datum
pmpz_in(PG_FUNCTION_ARGS)
{
    char    *str;
    mpz_t   z;
    pmpz    *res;

    str = PG_GETARG_CSTRING(0);

    /* TODO: make base variable */
    if (0 != mpz_init_set_str(z, str, 10))
    {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for mpz: \"%s\"",
                        str)));
    }

    res = pmpz_from_mpz(z);
    PG_RETURN_POINTER(res);
}

Datum
pmpz_out(PG_FUNCTION_ARGS)
{
    const pmpz      *pz;
    const mpz_t     z;
    char            *res;

    pz = PG_GETARG_PMPZ(0);
    mpz_from_pmpz(z, pz);

    /* TODO: make base variable */
    res = (char *)palloc(mpz_sizeinbase(z, 10) + 2);    /* add sign and 0 */
    mpz_get_str(res, 10, z);

    PG_RETURN_CSTRING(res);
}


/*
 * Cast functions
 */

static Datum _pmpz_from_long(int64 in);

Datum
pmpz_from_smallint(PG_FUNCTION_ARGS)
{
    int16 in = PG_GETARG_INT16(0);
    return _pmpz_from_long(in);
}

Datum
pmpz_from_integer(PG_FUNCTION_ARGS)
{
    int32 in = PG_GETARG_INT32(0);
    return _pmpz_from_long(in);
}

Datum
pmpz_from_bigint(PG_FUNCTION_ARGS)
{
    /* TODO: this function doesn't work - see regression tests. */
    int64 in = PG_GETARG_INT64(0);
    elog(INFO, "%ld", in);
    return _pmpz_from_long(in);
}

static Datum
_pmpz_from_long(int64 in)
{
    mpz_t   z;
    pmpz    *res;

    mpz_init_set_si(z, in);

    res = pmpz_from_mpz(z);
    PG_RETURN_POINTER(res);
}

