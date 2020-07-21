//------------------------------------------------------------------------------
// GB_Scalar_extractElement_template: x = S
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// Extract the value of single scalar, x = S, typecasting from the
// type of S to the type of x, as needed.

// Returns GrB_SUCCESS if the GxB_Scalar entry is present, and sets x to its
// value.  Returns GrB_NO_VALUE if the GxB_Scalar is not present, and x is
// unmodified.

// This template constructs GxB_Scalar_extractElement_[TYPE] for each of the
// 13 built-in types, and the _UDT method for all user-defined types.

GrB_Info GB_EXTRACT_ELEMENT     // extract a single entry from S
(
    GB_XTYPE *x,                // scalar to extract, not modified if not found
    const GxB_Scalar S          // GxB_Scalar to extract a scalar from
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    GB_RETURN_IF_NULL_OR_FAULTY (S) ;
    GB_RETURN_IF_NULL (x) ;

    // delete any lingering zombies and assemble any pending tuples
    if (GB_PENDING_OR_ZOMBIES (S))
    {   GB_cov[477]++ ;
// NOT COVERED (477):
        GrB_Info info ;
        GB_WHERE1 (GB_WHERE_STRING) ;
        GB_BURBLE_START ("GxB_Scalar_extractElement") ;
        GB_OK (GB_Matrix_wait ((GrB_Matrix) S, Context)) ;
        ASSERT (!GB_ZOMBIES (S)) ;
        ASSERT (!GB_PENDING (S)) ;
        GB_BURBLE_END ;
    }

    // GB_XCODE and S must be compatible
    GB_Type_code scode = S->type->code ;
    if (!GB_code_compatible (GB_XCODE, scode))
    {   GB_cov[478]++ ;
// covered (478): 2
        return (GrB_DOMAIN_MISMATCH) ;
    }

    if (S->nzmax == 0 || (S->p != NULL && S->p [1] == 0))
    {   GB_cov[479]++ ;
// covered (479): 2
        // quick return
        return (GrB_NO_VALUE) ;
    }

    //--------------------------------------------------------------------------
    // extract the scalar
    //--------------------------------------------------------------------------

    #if !defined ( GB_UDT_EXTRACT )
    if (GB_XCODE == scode)
    {   GB_cov[480]++ ;
// covered (480): 4032
        // copy the value from S into x, no typecasting, for built-in
        // types only.
        GB_XTYPE *GB_RESTRICT Sx = ((GB_XTYPE *) (S->x)) ;
        (*x) = Sx [0] ;
    }
    else
    #endif
    {   GB_cov[481]++ ;
// covered (481): 4334
        // typecast the value from S into x
        GB_cast_array ((GB_void *) x, GB_XCODE,
            ((GB_void *) S->x), scode, S->type->size, 1, 1) ;
    }
    return (GrB_SUCCESS) ;
}

#undef GB_UDT_EXTRACT
#undef GB_EXTRACT_ELEMENT
#undef GB_XTYPE
#undef GB_XCODE
