/*
 * This file defines the data pseudo-ops for HERA-C:
 * CONSTANT, DLABEL, INTEGER, DSKIP, and LP_STRING
 * NOTE than LP_STRING is not guaranteed to work for
 *  control characters, but it does (should) observe the
 *  C language string codes starting with "\", e.g. "\n".
 */

// *******************************************************************************
// 
//  WARNING: HEAP range definitions should agree with Hassem definitions
//   in Hassem/HERA.c and in Hassem's Grammar.y and the Hassem script
//   if any code (e.g. library functions) might depend on them.
//
// *******************************************************************************

#if ! defined first_space_for_stack
#define first_space_for_stack 0x0000
#else
#error "changing first_space_for_stack probably won't work"
#endif
#if ! defined last_space_for_stack
#define last_space_for_stack 0x3fff     /* See warning above */
#endif

// Library now initializes heap on first call to malloc (identified, e.g., by fsheap[0]=0
#if ! defined first_space_for_fsheap
#define first_space_for_fsheap 0x4000     /* See warning above */
#endif
#define next_free_space_on_fsheap first_space_for_fsheap  /* See warning above */
#if ! defined last_space_for_fsheap
#define last_space_for_fsheap 0xbfff      /* See warning above */
#endif

#if ! defined first_space_for_globals
#define first_space_for_globals 0xc000     /* See warning above */
#endif
#define next_free_space_for_globals first_space_for_globals  /* See warning above */
#if ! defined last_space_for_globals
#define last_space_for_globals 0xffdf      /* See warning above */
#endif

// Depricate: Version <= 2.3 compatibility
#define first_space_for_heap    first_space_for_fsheap
#define last_space_for_heap     last_space_for_fsheap
#define next_free_space_on_heap first_space_for_heap

// This is used in HERA_DATA for data layout
static address &next_free_memory = (address &) memory[memory[next_free_space_for_globals]=(next_free_space_for_globals+1),next_free_space_for_globals];


// if next_free + nwords > MSIZE, there's trouble
#define INC_NEXT_FREE_MEMORY_BY(nwords) \
	(( (int) next_free_memory > (int) MSIZE - ((int) (nwords)))) ?	\
		(exit(127), 0)						\
	:								\
		(next_free_memory += (nwords))


#define CONSTANT(label,val)	const word label = word(val);
#define DLABEL(label)   	LABEL_NAME(label): const int label = next_free_memory;
#define INTEGER(I)      	(SETMEM(next_free_memory, (I)), INC_NEXT_FREE_MEMORY_BY(1));
// just leave some space we expect to fill later
#define DSKIP(N)        	(INC_NEXT_FREE_MEMORY_BY(N));


// To work with Ch. 7 of appel, under "Strings" (p. 167 in 2nd ed of C version)
// These are stored as "length-prefixed strings", as in Pascal.
// Since the escape characters are those from C, however, we call them LP_STRING
#define TIGER_STRING(S)	LP_STRING(S)
#define LP_STRING(S) { \
			  SETMEM(next_free_memory, strlen(S)); INC_NEXT_FREE_MEMORY_BY(1); \
			  for (unsigned int i=0; i<strlen(S); i++) { \
			    SETMEM(next_free_memory, S[i]); INC_NEXT_FREE_MEMORY_BY(1); \
			  } \
			}
