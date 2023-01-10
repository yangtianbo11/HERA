/*
 * This is a sample of what could go after the HERA data and code statements
 *  to make it work with HERA-C, analogous to the preamble.
 */

// To help debugging simple steps of compilers in CS350, print R1 and R2 if nothing was output (like in CMSC 240)
// This was subsumed by HERA_C_VERBOSE=2
/// #if defined HERA_C_DETECT_CALLS_TO_PRINT
/// if (!HERA_C_DETECTED_A_CALL_TO_PRINT) { print_reg(R1); print_reg(R2); }
/// #endif

HALT();

#if defined HERA_ADD_TO_TYPICALY_POSTAMBLE
HERA_ADD_TO_TYPICAL_POSTAMBLE
#endif

#if ! defined HERA_C_NO_LIB_NEEDED
#if defined HERA_C_STACK_LIB && HERA_C_STACK_LIB==1
#include "Tiger-stdlib-stack.hera"
#else
#include "Tiger-stdlib-reg.hera"
#endif
#endif
}
