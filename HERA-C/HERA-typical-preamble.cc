/*
 * This is a sample of what could go before the HERA data and code statements
 *  to make it work with HERA-C. It is used by HERA-C-Run when the program
 *  does not contain "HERA.h"
 */

// This is typically used in Programming Languages or Compiler Design,
//  where it is desirable to print R1 if nothing was printed in the program
//  (typically before learning about, or implementing, calls to print functions)

#define HERA_C_DETECT_CALLS_TO_PRINT 1
bool HERA_C_DETECTED_A_CALL_TO_PRINT = false;

#include "HERA.h"
#include "HERA-print.h"

void HERA_main()
{
	; // this does nothing during the run,
	  // but when debugging "break HERA_main" will stop here
	  // rather than in the data segment of the library (included below),
	  // and thus one can have a chance to set a breakpoint a few lines below that
#if ! defined HERA_C_NO_LIB_NEEDED
#if defined HERA_C_STACK_LIB && HERA_C_STACK_LIB==1
#include "Tiger-stdlib-stack-data.hera"
#else
#include "Tiger-stdlib-reg-data.hera"
#endif
#endif

#if defined HERA_ADD_TO_TYPICAL_PREAMBLE
HERA_ADD_TO_TYPICAL_PREAMBLE
#endif
