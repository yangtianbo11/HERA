Important Design Decisions:

   *IF and LET_STAR are separately parsed in matchEInParens()
   *EQUAL, LESS_OR_EQUAL, GREATER_OR_EQUAL are implemented as operators


Project Status (T = code completed and test suite exists, C = code completed, P = in-progress, N = not started):

   T  Parse + and * operations, constants and identifiers
   T  Build an AST from the above ... this _should_ work once Tree.h, etc., are added to the project
   T  Evaluate +, *, and constants
   T  Scan, Parse, and Evaluate subtraction ... scanning and parsing should work already, need to add to eval.cc
   T  Scan, Parse, and Evaluate boolean literals for correct expressions
   T  Update evaluator to give errors for type mismatches e.g. (+ 3 #t)
   T  Scan, Parse, and Evaluate comparisons
   T  Scan, Parse, and Evaluate conditionals
   T  Scan, Parse, and Evaluate "let" expressions


Complexity:

   * Assuming constant-time integer arithmetic, most expressions are evaluated in O(n) time, where n is the total number of subexpressions (i.e., size(expression tree)).
   * A let expression with m variables and containing a body of size n will be evaluated in O(m*n) time. For every body of size of n, the expression will have to go through the symbol table of size m. This means going though the symbol table n times.

