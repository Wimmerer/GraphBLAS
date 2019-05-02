//------------------------------------------------------------------------------
// GB_red:  hard-coded functions for reductions
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

#include "GB.h"
#ifndef GBCOMPACT
#include "GB_red__include.h"

// The reduction is defined by the following types and operators:

// Reduce to scalar:  GB_red_scalar__(none)
// Assemble tuples:   GB_bild__first_int16

// A type:   int16_t

// Reduce:   ;
// Identity: (none)
// Terminal: ;

#define GB_ATYPE \
    int16_t

// monoid identity value
#define GB_IDENTITY \
    (none)

// scalar workspace for each thread
#define GB_REDUCE_WORKSPACE(w,nthreads) \
    int16_t w [nthreads] ;

// t = identity
#define GB_REDUCE_INIT(t) \
    int16_t t = (none) ;

// t += Ax [p]
#define GB_REDUCE(t,Ax,p)   \
    ;

// w [tid] = t
#define GB_REDUCE_WRAPUP(w,tid,t) \
    w [tid] = t ;

// s += w [tid], sum up results of each thread
#define GB_REDUCE_W(s,w,tid)  \
    ;

// break if terminal value of the monoid is reached
#define GB_REDUCE_TERMINAL(t) \
    ;

// Tx [p] += S [k]
#define GB_BUILD_OP(Tx, p, S, k) \
    ;

// Tx [p] = S [k]
#define GB_BUILD_COPY(Tx, p, S, k) \
    Tx [p] = S [k] ;

//------------------------------------------------------------------------------
// reduce to a scalar, for monoids only
//------------------------------------------------------------------------------

#if 0

void GB_red_scalar__(none)
(
    int16_t *result,
    const GrB_Matrix A,
    int nthreads
)
{ 
    GB_REDUCE_INIT (s) ;
    #include "GB_reduce_to_scalar_template.c"
    (*result) = s ;
}

#endif

//------------------------------------------------------------------------------
// build matrix
//------------------------------------------------------------------------------

void GB_bild__first_int16
(
    int16_t *restrict Tx,
    int64_t  *restrict Ti,
    const int16_t *restrict S,
    int64_t nvals,
    int64_t ndupl,
    const int64_t *restrict iwork,
    const int64_t *restrict kwork,
    const int64_t *restrict tstart_slice,
    const int64_t *restrict tnz_slice,
    int nthreads
)
{
    #include "GB_build_template.c"
}

#endif
