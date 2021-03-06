//------------------------------------------------------------------------------
// GB_mex_msort_1: sort using GB_msort_1
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

#include "GB_mex.h"

#define USAGE "I = GB_mex_msort_1 (I,nthreads)"

void mexFunction
(
    int nargout,
    mxArray *pargout [ ],
    int nargin,
    const mxArray *pargin [ ]
)
{
    bool malloc_debug = GB_mx_get_global (true) ;

    // check inputs
    if (nargin != 2 || nargout != 1)
    {
        mexErrMsgTxt ("Usage: " USAGE) ;
    }
    if (!mxIsClass (pargin [0], "int64"))
    {
        mexErrMsgTxt ("I must be a int64 array") ;
    }

    int64_t *I = mxGetData (pargin [0]) ;
    int64_t n = (uint64_t) mxGetNumberOfElements (pargin [0]) ;

    // get # of threads to use
    int GET_SCALAR (1, int, nthreads, 1) ;
    nthreads = GB_MSORT_NTHREADS (nthreads) ;

    // make a copy of the input array
    pargout [0] = GB_mx_create_full (n, 1, GrB_INT64) ;
    int64_t *Iout = mxGetData (pargout [0]) ;
    memcpy (Iout, I, n * sizeof (int64_t)) ;

    // get workspace
    int64_t *Work_0 = NULL ;
    if (nthreads > 1)
    {
        Work_0 = mxMalloc ((n+1) * sizeof (int64_t)) ;
    }

    GB_MEX_TIC ;

    GB_msort_1 (Iout, Work_0, n, nthreads) ;

    GB_MEX_TOC ;

    // free workspace
    if (nthreads > 1)
    {
        mxFree (Work_0) ;
    }

    GB_mx_put_global (true, 0) ;
}

