//------------------------------------------------------------------------------
// GB_unop:  hard-coded functions for each built-in unary operator
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// If this file is in the Generated/ folder, do not edit it (auto-generated).

#include "GB.h"
#ifndef GBCOMPACT
#include "GB_control.h"
#include "GB_unop__include.h"

// C=unop(A) is defined by the following types and operators:

// op(A)  function:  GB_unop_apply__identity_int64_bool
// op(A') function:  GB_unop_tran__identity_int64_bool

// C type:   int64_t
// A type:   bool
// cast:     int64_t cij = (int64_t) aij
// unaryop:  cij = aij

#define GB_ATYPE \
    bool

#define GB_CTYPE \
    int64_t

// aij = Ax [pA]
#define GB_GETA(aij,Ax,pA) \
    bool aij = Ax [pA]

#define GB_CX(p) Cx [p]

// unary operator
#define GB_OP(z, x) \
    z = x ;

// casting
#define GB_CAST(z, aij) \
    int64_t z = (int64_t) aij ;

// cij = op (aij)
#define GB_CAST_OP(pC,pA)           \
{                                   \
    /* aij = Ax [pA] */             \
    bool aij = Ax [pA] ;          \
    /* Cx [pC] = op (cast (aij)) */ \
    int64_t z = (int64_t) aij ;               \
    Cx [pC] = z ;        \
}

// disable this operator and use the generic case if these conditions hold
#define GB_DISABLE \
    (GxB_NO_IDENTITY || GxB_NO_INT64 || GxB_NO_BOOL)

//------------------------------------------------------------------------------
// Cx = op (cast (Ax)): apply a unary operator
//------------------------------------------------------------------------------

GrB_Info GB_unop_apply__identity_int64_bool
(
    int64_t *Cx,       // Cx and Ax may be aliased
    const bool *Ax,
    const int8_t *GB_RESTRICT Ab,   // A->b if A is bitmap
    int64_t anz,
    int nthreads
)
{

    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    int64_t p ;
    if (Ab == NULL)
    { 
        // FIXME: not needed for the identity operator, when Ab is NULL
        // A and C are hypersparse, sparse, or full
        #pragma omp parallel for num_threads(nthreads) schedule(static)
        for (p = 0 ; p < anz ; p++)
        {
            bool aij = Ax [p] ;
            int64_t z = (int64_t) aij ;
            Cx [p] = z ;
        }
    }
    else
    { 
        // bitmap case, no transpose
        // A->b has already been memcpy'd into C->b
        #pragma omp parallel for num_threads(nthreads) schedule(static)
        for (p = 0 ; p < anz ; p++)
        {
            if (!Ab [p]) continue ;
            bool aij = Ax [p] ;
            int64_t z = (int64_t) aij ;
            Cx [p] = z ;
        }
    }
    return (GrB_SUCCESS) ;
    #endif
}

//------------------------------------------------------------------------------
// C = op (cast (A')): transpose, typecast, and apply a unary operator
//------------------------------------------------------------------------------

GrB_Info GB_unop_tran__identity_int64_bool
(
    GrB_Matrix C,
    const GrB_Matrix A,
    int64_t *GB_RESTRICT *Rowcounts,
    const int64_t *GB_RESTRICT A_slice,
    int naslice,
    int nthreads
)
{ 
    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    #define GB_PHASE_2_OF_2
    #include "GB_unop_transpose.c"
    return (GrB_SUCCESS) ;
    #endif
}

#endif

