/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/
#include <secure/crypto_rsa.h>
#include <secure/crypto_sha.h>
#include <secure/crypto_func.h>

// mpFunc Table
void _Null(void) { while (1) ;}

MpFuncTbl _mpFunc =
{
    _mpConvFromOctets,
    _mpModExp,
    _mpEqual,

    _mpSquare,
    _mpMultiply,
    _mpDivide,
    _mpShortDiv,
    _mpShiftLeft,
    _mpShiftRight,
    _spMultiply,
    _spDivide,

   { _Null, _Null, _Null, _Null, _Null, _Null, _Null, _Null,
    _Null, _Null, _Null, _Null, _Null},
};

MpFuncTbl *mpFunc = &_mpFunc;

