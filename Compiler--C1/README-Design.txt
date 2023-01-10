PLEASE DO NOT EDIT THIS PRELIMINARY VERISON OF THE FILE, TO AVOID GIT CONFLICTS LATER
 (if you're working before the update version is available, make a copy and edit that)

Important Design Decisions:

   * a global symbol table is used to store the mapping between the let* variable index to registers so store and load work on the correct registers
   * a limited registers determines the limited let* variable amounts allowed for the hera code
   * all final answers end up in R10


Project Status (T = code completed and test suite exists, C = code completed, P = in-progress, N = not started):

   T  Parse + and* operations, constants and identifiers
   T  Build an Object-Oriented AST from the above
   T  Appropriately allocate registers for arithmetic operations
   T  Generate HERA code for +, *, and constants

 ---- more will be added later ----
