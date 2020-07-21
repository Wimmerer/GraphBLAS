//------------------------------------------------------------------------------
// GB_subassign_24: make a deep copy of a sparse matrix
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// C = A, making a deep copy into an existing non-shallow matrix C, but
// possibly reusing parts of C if C is dense.  See also GB_dup.

// Handles arbitrary typecasting.  A is either sparse or dense; the name of
// the function is a bit of a misnomer since it implies that only the dense
// case is handled.

// FULL: if C sparse and A dense/full, convert C to full

#include "GB_dense.h"
#include "GB_Pending.h"
#define GB_FREE_ALL ;

GrB_Info GB_subassign_24    // C = A, copy A into an existing matrix C
(
    GrB_Matrix C,           // output matrix to modify
    const GrB_Matrix A,     // input matrix to copy
    GB_Context Context
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    GrB_Info info ;
    ASSERT_MATRIX_OK (C, "C for C_subassign_24", GB0) ;
    ASSERT_MATRIX_OK (A, "A for A_subassign_24", GB0) ;
    ASSERT (GB_ZOMBIES_OK (A) && GB_PENDING_OK (A)) ;
    ASSERT (GB_ZOMBIES_OK (C) && GB_PENDING_OK (C)) ;

    //--------------------------------------------------------------------------
    // delete any lingering zombies and assemble any pending tuples
    //--------------------------------------------------------------------------

    GB_MATRIX_WAIT (A) ;
    if (A->nvec_nonempty < 0)
    { 
        A->nvec_nonempty = GB_nvec_nonempty (A, Context) ;
    }

    //--------------------------------------------------------------------------
    // determine the number of threads to use
    //--------------------------------------------------------------------------

    GB_GET_NTHREADS_MAX (nthreads_max, chunk, Context) ;

    //--------------------------------------------------------------------------
    // C = A
    //--------------------------------------------------------------------------

    bool copy_dense_A_to_C =            // copy from dense A to dense C if:
        (
            GB_is_dense (C)             // both A and C are dense
            && GB_is_dense (A)
            && C->vdim == A->vdim       // A and C have the same size
            && C->vlen == A->vlen
            && C->is_csc == A->is_csc   // A and C have the same format
            && C->x != NULL             // C->x exists
            && !(C->x_shallow)          // C->x is not shallow
        ) ;

    if (copy_dense_A_to_C)
    { 

        //----------------------------------------------------------------------
        // discard the pattern of C
        //----------------------------------------------------------------------

        // make C full, if not full already
        GBBURBLE ("(dense copy) ") ;
        C->nzombies = 0 ;                   // overwrite any zombies
        GB_Pending_free (&(C->Pending)) ;   // abandon all pending tuples
        if (!GB_IS_FULL (C))
        { 
            GB_sparse_to_full (C) ;
        }

    }
    else
    { 

        //----------------------------------------------------------------------
        // copy the pattern from A to C
        //----------------------------------------------------------------------

        // clear prior content of C, but keep the CSR/CSC format and its type
        GBBURBLE ("(deep copy) ") ;
        bool C_is_csc = C->is_csc ;
        GB_phix_free (C) ;
        // copy the pattern, not the values
        GB_OK (GB_dup2 (&C, A, false, C->type, Context)) ;
        C->is_csc = C_is_csc ;      // do not change the CSR/CSC format of C
    }

    //-------------------------------------------------------------------------
    // copy the values from A to C, typecasting as needed
    //-------------------------------------------------------------------------

    if (C->type != A->type)
    { 
        GBBURBLE ("(typecast) ") ;
    }

    int64_t anz = GB_NNZ (A) ;
    int nthreads = GB_nthreads (anz, chunk, nthreads_max) ;
    GB_cast_array (C->x, C->type->code, A->x, A->type->code, A->type->size,
                       anz, nthreads) ;

    //-------------------------------------------------------------------------
    // return the result
    //--------------------------------------------------------------------------

    ASSERT_MATRIX_OK (C, "C result for GB_subassign_24", GB0) ;
    return (GrB_SUCCESS) ;
}
