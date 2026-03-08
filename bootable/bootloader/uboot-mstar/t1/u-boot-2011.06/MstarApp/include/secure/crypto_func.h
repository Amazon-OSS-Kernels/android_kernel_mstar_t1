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
#ifndef _FUNC_H_
#define _FUNC_H_

typedef int (*FP_MP_MOD_EXP)(DIGIT_T yout[], const DIGIT_T x[], const DIGIT_T e[], const DIGIT_T m[], int ndigits);

typedef int (*FP_MP_CONV_FROM_OCTETS)(DIGIT_T a[], int ndigits, const unsigned char *c, int nbytes);

typedef int (*FP_MP_EQUAL)(const DIGIT_T a[], const DIGIT_T b[], int ndigits);

typedef void (*FP_MP_SETZERO)(const DIGIT_T a[], int ndigits);

typedef void (*FP_NULL)(void);

//int mpSquare(DIGIT_T w[], const DIGIT_T x[], int ndigits)
typedef int (*FP_MP_SQUARE)(DIGIT_T w[], const DIGIT_T x[], int ndigits);

//int mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], int ndigits)
typedef int (*FP_MP_MULTIPLY)(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], int ndigits);

//int mpDivide(DIGIT_T q[], DIGIT_T r[], const DIGIT_T u[], int udigits, DIGIT_T v[], int vdigits)
typedef int (*FP_MP_DIVIDE)(DIGIT_T q[], DIGIT_T r[], const DIGIT_T u[], int udigits, DIGIT_T v[], int vdigits);

//DIGIT_T mpShortDiv(DIGIT_T q[], const DIGIT_T u[], DIGIT_T v, int ndigits)
typedef DIGIT_T (*FP_MP_SHORT_DIV)(DIGIT_T q[], const DIGIT_T u[], DIGIT_T v, int ndigits);

//DIGIT_T mpShiftLeft(DIGIT_T a[], const DIGIT_T *b, int shift, int ndigits)
typedef DIGIT_T (*FP_MP_SHIFT_LEFT)(DIGIT_T a[], const DIGIT_T *b, int shift, int ndigits);

//DIGIT_T mpShiftRight(DIGIT_T a[], const DIGIT_T b[], int shift, int ndigits)
typedef DIGIT_T (*FP_MP_SHIFT_RIGHT)(DIGIT_T a[], const DIGIT_T b[], int shift, int ndigits);

//int spMultiply(DIGIT_T p[2], DIGIT_T x, DIGIT_T y)
typedef int (*FP_SP_MULTIPLY)(DIGIT_T p[2], DIGIT_T x, DIGIT_T y);

//DIGIT_T spDivide(DIGIT_T *q, DIGIT_T *r, const DIGIT_T u[2], DIGIT_T v)
typedef DIGIT_T (*FP_SP_DIVIDE)(DIGIT_T *q, DIGIT_T *r, const DIGIT_T u[2], DIGIT_T v);

typedef struct
{
    FP_MP_CONV_FROM_OCTETS ConvFromOctets;
    FP_MP_MOD_EXP       ModExp;
    FP_MP_EQUAL         Equal;

    FP_MP_SQUARE        Square;
    FP_MP_MULTIPLY      Multiply;
    FP_MP_DIVIDE        Divide;
    FP_MP_SHORT_DIV     ShortDiv;
    FP_MP_SHIFT_LEFT    ShiftLeft;
    FP_MP_SHIFT_RIGHT   ShiftRight;

    FP_SP_MULTIPLY      SpMultiply;
    FP_SP_DIVIDE        SpDivide;

    FP_NULL             reserve_1[13];
} MpFuncTbl;

extern MpFuncTbl        *mpFunc;
extern MpFuncTbl        _mpFunc;

#endif

