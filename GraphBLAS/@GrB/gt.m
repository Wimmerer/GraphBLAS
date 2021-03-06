function C = gt (A, B)
%A > B greater than.
% C = (A > B) is an element-by-element comparison of A and B.  One or
% both may be scalars.  Otherwise, A and B must have the same size.
%
% See also GrB/lt, GrB/le, GrB/ge, GrB/ne, GrB/eq.

% FUTURE: gt(A,B) for two matrices A and B is slower than it could be.
% See comments in gb_union_op.

% SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights
% Reserved. http://suitesparse.com.  See GraphBLAS/Doc/License.txt.

C = lt (B, A) ;

