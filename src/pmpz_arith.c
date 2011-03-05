/* pmpz_arith -- mpz arithmetic functions
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


PG_FUNCTION_INFO_V1(pmpz_uminus);
PG_FUNCTION_INFO_V1(pmpz_uplus);

PG_FUNCTION_INFO_V1(pmpz_add);
PG_FUNCTION_INFO_V1(pmpz_mul);


Datum       pmpz_uminus(PG_FUNCTION_ARGS);
Datum       pmpz_uplus(PG_FUNCTION_ARGS);

Datum       pmpz_add(PG_FUNCTION_ARGS);
Datum       pmpz_mul(PG_FUNCTION_ARGS);


/*
 * Unary minus, plus
 */

Datum
pmpz_uminus(PG_FUNCTION_ARGS)
{
    const mpz_t     z1;
    mpz_t           zf;
    pmpz            *res;

    mpz_from_pmpz(z1, PG_GETARG_PMPZ(0));

    mpz_init_set(zf, z1);
    mpz_neg(zf, zf);

    res = pmpz_from_mpz(zf);
    PG_RETURN_POINTER(res);
}

Datum
pmpz_uplus(PG_FUNCTION_ARGS)
{
    const pmpz      *pz1;
    pmpz            *res;

    pz1 = PG_GETARG_PMPZ(0);

	res = (pmpz *)palloc(VARSIZE(pz1));
	memcpy(res, pz1, VARSIZE(pz1));

    PG_RETURN_POINTER(res);
}


/*
 * Binary operators
 */

Datum
pmpz_add(PG_FUNCTION_ARGS)
{
    const mpz_t     z1;
    const mpz_t     z2;
    mpz_t           zf;
    pmpz            *res;

    mpz_from_pmpz(z1, PG_GETARG_PMPZ(0));
    mpz_from_pmpz(z2, PG_GETARG_PMPZ(1));

    mpz_init(zf);
    mpz_add(zf, z1, z2);

    res = pmpz_from_mpz(zf);
    PG_RETURN_POINTER(res);
}

Datum
pmpz_mul(PG_FUNCTION_ARGS)
{
    const mpz_t     z1;
    const mpz_t     z2;
    mpz_t           zf;
    pmpz            *res;

    mpz_from_pmpz(z1, PG_GETARG_PMPZ(0));
    mpz_from_pmpz(z2, PG_GETARG_PMPZ(1));

    mpz_init(zf);
    mpz_mul(zf, z1, z2);

    res = pmpz_from_mpz(zf);
    PG_RETURN_POINTER(res);
}
