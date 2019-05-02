//------------------------------------------------------------------------------
// GB_unaryop:  hard-coded functions for each built-in unary operator
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// If this file is in the Generated/ folder, do not edit it (auto-generated)

#include "GB.h"
#ifndef GBCOMPACT
#include "GB_unaryop__include.h"

// C=unop(A) is defined by the following types and operators:

// op(A)  function:  GB_unop__minv_uint64_int8
// op(A') function:  GB_tran__minv_uint64_int8

// C type:   uint64_t
// A type:   int8_t
// cast:     uint64_t cij = aij
// unaryop:  cij = GB_IMINV_UNSIGNED (aij, 64)

#define GB_ATYPE \
    int8_t

#define GB_CTYPE \
    uint64_t

// aij = Ax [pA]
#define GB_GETA(aij,Ax,pA)  \
    int8_t aij = Ax [pA]

#define GB_CX(p) Cx [p]

// unary operator
#define GB_OP(z, x)   \
    z = GB_IMINV_UNSIGNED (x, 64) ;

// casting
#define GB_CASTING(z, x)   \
    uint64_t z = x ;

// cij = op (cast (aij))
#define GB_CAST_OP(pC,pA)           \
{                                   \
    /* aij = Ax [pA] */             \
    GB_GETA (aij, Ax, pA) ;         \
    /* Cx [pC] = op (cast (aij)) */ \
    GB_CASTING (x, aij) ;           \
    GB_OP (GB_CX (pC), x) ;         \
}

//------------------------------------------------------------------------------
// Cx = op (cast (Ax)): apply a unary operator
//------------------------------------------------------------------------------

void GB_unop__minv_uint64_int8
(
    uint64_t *restrict Cx,
    int8_t *restrict Ax,
    int64_t anz,
    int nthreads
)
{ 
    #pragma omp parallel for num_threads(nthreads)
    for (int64_t p = 0 ; p < anz ; p++)
    {
        GB_CAST_OP (p, p) ;
    }
}

//------------------------------------------------------------------------------
// C = op (cast (A')): transpose, typecast, and apply a unary operator
//------------------------------------------------------------------------------

void GB_tran__minv_uint64_int8
(
    GrB_Matrix C,
    const GrB_Matrix A,
    int64_t **Rowcounts,
    GBI_single_iterator Iter,
    const int64_t *restrict A_slice,
    int naslice
)
{ 
    #define GB_PHASE_2_OF_2
    #include "GB_unaryop_transpose.c"
}

#endif
