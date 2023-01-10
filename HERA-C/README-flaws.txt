Known shortcomings of the HERA-C simulator:

*** The debugger typically cannot single-step past a RETURN, so a
*** breakpoint must be set right after each call so that a CONTINUE
*** can be used to get out of a called function;
*** Setting a breakpoint on the doing_RETURN function can help this
*** a bit, as it stops at the next return statement.

Branches, calls, and returns don't actually use registers, so computed
targets are not possible, and if the stack is corrupted the return
will not fail in the usual way.

HERA-C cannot figure out if a relative branch target would actually be
too far away, and thus may accept programs with relative branches that
couldn't really work (and execute them as if these branches could work).

