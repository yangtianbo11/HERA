#include <HERA.h>
#include <HERA-print.h>  // This is needed to use the hera 'print' function

// FIRST, some definitions that tell us how much of this project we want to attempt
#define PRELAB      0
#define LIST_IMPL   1
#define LIST_TESTS  LIST_IMPL  /* If they're there, test them (for now) */

/*
 *  TEAM MEMBERS for Lab 1H (record them here as well as the README; also record each author in each file)
 * Sam S, Xavier D, Femi O, Tianbo Y
 */


void HERA_main()
{
  // Put any #defines or, strings, or other data declarations here,
  //  or get them with #includes, e.g.
#include <Tiger-stdlib-reg-data.hera>

#if PRELAB
#include "prelab-data.hera"
#endif
#if LIST_IMPL
#include "List-data.hera"
#include "ListHelpers-data.hera"
#endif
#if LIST_TESTS
#include "test-List-data.hera"
#endif

	// Set the carry block before doing any other operations:
    CBON()

#if PRELAB	    
#include "prelab.hera"
#endif
	    
#if LIST_TESTS	    
#include "test-List.hera"
#endif

    HALT()	// end of main HERA program
	

#if LIST_IMPL
#include "List.hera"
#include "ListHelpers.hera"
#endif


  // Put a "HALT" before the library, i.e., do some cautious programming,
  // to make sure the program doesn't run straight into the library functions:
	  HALT()
//The following include is needed to use the hera 'print' function
#include <Tiger-stdlib-reg.hera>
}
