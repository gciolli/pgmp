/* trf -- tail-recursive factorial
 *
 * Copyright (C) 2011 Gianni Ciolli
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


/* 

   Requires: PostgreSQL 8.4+ with mpz contrib module installed.

   Description. In this file we compare the speed of mpz with that of
   NUMERIC. As I needed large numbers, I have implemented
   multiplication and factorial.

   Note: PostgreSQL provides also a "native" numeric_fac function,
   which however is too slow to be useful.

   Note: the algorithm is "almost" tail-recursive, as WITH queries
   cannot recurse in constant space.

   The speed on both datatypes is measured on the computation of
   32177! -- the largest factorial that doesn't give overflow
   on NUMERIC. On that value my cheap laptop (eeePC 900A) gives
   approximately

     speed(mpz) = 1.5 * speed(NUMERIC) .

   Numbers whose factorial has more than 2^17 decimal digits cannot be
   compared as they are supported only by mpz.

*/

\set my_number 32177
-- repeat with 32178 to see NUMERIC's overflow

\timing

\echo -- with NUMERIC
\echo

WITH RECURSIVE x(n,fact) AS (
	SELECT :my_number, 1::numeric
UNION ALL
	SELECT n - 1, n::numeric * fact
	FROM x
	WHERE n > 1
)
SELECT length(fact::text) FROM x WHERE n = 1;

\echo -- with mpz
\echo

WITH RECURSIVE x(n,fact) AS (
	SELECT :my_number, 1::mpz
UNION ALL
	SELECT n - 1, n::mpz * fact
	FROM x
	WHERE n > 1
)
SELECT length(fact::text) FROM x WHERE n = 1;
