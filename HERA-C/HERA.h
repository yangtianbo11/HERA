// IF YOU SEE THIS FILE IN THE DEBUGGER, SEE THE NOTES IN main BELOW!

// useful HERA #define options: HERA_C_WARN_ABOVE_STACK (define as, e.g. 5, to warn about LOAD/STORE to FP+0..4)

#ifndef HERA_C
#define HERA_C

#define HERA_VERSION_STRING "2.4.0"
#define HERAC_DATE_STRING   "October 2018"

#if ! defined HERA_C_WARN_ABOVE_STACK
#define HERA_C_WARN_ABOVE_STACK 80
#endif


#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>  /* exit */
#include <string.h>

static int Hprint(const char *str)
{
    write(1, str, strlen(str));
    return 0; // needed to let us use Hprint in a comma-expression with registers
}

static int Hprintchar(char c)
{
    char s[2];
    s[0] = c;
    s[1] = 0; // probably not needed at all
    write(1, s, 1);
    return 0; // needed to let us use it in a comma-expression with registers
}

// This is a good place for a breakpoint when debugging
static int Herror(const char *str, bool fatal=false)
{
    write(2, str, strlen(str));
    write(2, "\n", 1);
    if (fatal)
	    exit(1);
    return 0; // needed to let us use it in a comma-expression with registers
}

static void doing_RETURN()	// just useful to set breakpoints on all RETURN statements
{
  ;  // Do nothing ... just do a breakpoint here to help with single-stepping "RETURN"s
     // If you have set such a breakpoint and arrive here in the debugger,
     //  it means the HERA processor is just starting to do the RETURN instruction,
     //  partially done updating the "position in the program" ("program counter)
     //  but not yet having updated any registers for the return ...
     //  doing a "next line" should get you to the target of the return
}

static void doing_CALL()	// just for symmetry with doing_RETURN ... not really needed, but maybe useful
{
  ;
}

static void HERA_breakpoint()	// just useful if the programmer wants to put in a call, and then break there
{
  ;
}

#include <HERA-arch.h>
// relevant part of architecture for debugging:
// view the arrays memory, registers,
// and flags (size 4, order s z v c).

#include <HERA-instr.h>
#include <HERA-pseudo.h>
#include <HERA-data.h>
#include <HERA-opcode.h>
#if defined DONT_USE_HERA_C_VERBOSITY
const  int HERA_C_VERBOSITY=001 | 004 | 010;	// non-zero user reg, flags, stack
const  int HERA_C_CALL_VERBOSITY=0;
const  int HERA_C_RETURN_VERBOSITY=0;
#else
static int HERA_C_VERBOSITY=001 | 004 | 010;	// non-zero user reg, flags, stack
static int HERA_C_CALL_VERBOSITY=0;
static int HERA_C_RETURN_VERBOSITY=0;
const  int HERA_C_CALL_ERROR_VERBOSITY=002|004|010;
const  int HERA_C_RETURN_ERROR_VERBOSITY=002|004|010;
#include <HERA-print.h>
static int getenvAsOctal(const char *name, int defaultValue)
{
	char *textVal=getenv(name);
	if (textVal) {
		if (textVal[0] && textVal[0] == '0')
			return strtol(textVal,0,8);
		else {
			Herror("Warning: nonsense value for "); Herror(name); Herror(" environment variable ignored. Use e.g.\n\t");
			Herror(name); Herror("=00002 HERA-C-Run myfile.hera\n");
		}
	}
	return defaultValue; // if not returning something better
}

#endif


#include <ctype.h>
#include <assert.h>
#include <string.h>

int main( int argc, char ** argv )
{
	; // The debugger should not show this file, but if it does, this ; might start it here
	
// IF YOU SEE THIS FILE IN THE DEBUGGER,
// EITHER (a) configure the debugger to stop at HERA_main rather than main,
//   OR   (b) switch to one of your HERA source files and set a breakpoint
//            and then continue the program to get there (in Eclipse, click
//            on the green "Resume" triangle or hit F8; in gdb, type "cont").
//
// TO set the debugger to stop at HERA_main, do the following,
// but remember that this will stop at the first thing done by HERA_main,
// which may well be a data declaration you've #include'd.
//
//    If you're using Eclipse,
//		choose "Debug Configurations..." from the "Run" menu,
//		click on the "Debugger" tab, and
//		then change "main" to "HERA_main"
//		in the "Stop on startup at:" box
//		and apply your changes, close the dialog, stop the program (via the red square) and re-enter Debug
//    or if you're using GDB,
//		TYPE "break HERA_main"
//		AT THE "gdb" PROMPT BEFORE USING "run"
//    (you may also want to set a breakpoint at 'Herror' to see errors,
//	or at "doing_RETURN" to help debug programs with function calls,
//	as the debugger "next line" operation doesn't do RETURNs right)
//
	
	if ( !(  // If you see this line in the debugger, read instructions above
		 (&SP.get_register()   == &registers[RCOUNT-1]) &&
		 (&FP.get_register()   == &registers[RCOUNT-2]) &&
		 (&r_RA.get_register() == &registers[RCOUNT-3]) &&
		 (&r_DL.get_register() == &registers[RCOUNT-4]) &&
		 (&Rt.get_register()   == &registers[RCOUNT-5])
	      ))
	{
	    Herror("Internal inconsistency in HERA-C architecture.\n");
	    Herror("See the start of main in HERA.h for details.\n");
	    exit(1);
	}

	if ( !(
	       (8*sizeof(word) == BITS_PER_WORD) &&
	       (sizeof(word) == sizeof(uword) && sizeof(word) < sizeof(int)) &&
	       (sizeof(signed char) == 1)  // for SETLO sign extension
	      ))
	{
	    Herror("Architecture mismatch between HERA-C and underlying hardware.\n");
	    Herror("See the start of main in HERA.h for details.\n");
	    exit(2);
	}

	#if ! defined DONT_USE_HERA_C_VERBOSITY
	HERA_C_VERBOSITY        = getenvAsOctal("HERA_C_VERBOSITY",        HERA_C_VERBOSITY);
	HERA_C_CALL_VERBOSITY   = getenvAsOctal("HERA_C_CALL_VERBOSITY",   HERA_C_CALL_VERBOSITY);
	HERA_C_RETURN_VERBOSITY = getenvAsOctal("HERA_C_RETURN_VERBOSITY", HERA_C_RETURN_VERBOSITY);
	#endif

	void HERA_main();

#if ! defined DONT_USE_HERA_C_VERBOSITY
	if (HERA_C_VERBOSITY > 0)
#endif		
		Hprint("\nWelcome to the HERA-C HERA " HERA_VERSION_STRING  " simulator, " HERAC_DATE_STRING " edition.\n"),
		Hprint("  Please report bugs to davew@cs.haverford.edu\n"),
#if defined WARN_HERA_2_3_DIFFS && WARN_HERA_2_3_DIFFS
		Hprint("  *********************************** NOTE FOR USERS OF HERA 2.3 ******************************************\n"),
		Hprint("	  If you are used to HERA 2.3, there have been some changes for 2.4. ****\n"),
		Hprint("	  You may want to #include <HERA_2_3_compatibility.hera> to run old code.\n"),
		Hprint("	  For details on changes for 2.4, see the first two pages of the errata/history document\n"),
		Hprint("	  https://docs.google.com/document/d/1fhgaAMjyWz0MAoe1fssMR97LQgQUDCQFsnaQhA5r_Io\n"),
		Hprint("  *********************************************************************************************************\n\n"),
#endif
		Hprint("\nStarting HERA program at HERA_main() ...\n\n");

	HERA_main();

#if ! defined DONT_USE_HERA_C_VERBOSITY
	if (HERA_C_VERBOSITY > 0)
		HERA_print_lots("\n\nHERA program is done.\n", HERA_C_VERBOSITY);
#else
	Hprint("\n\nHERA program is done.\n");
#endif

	return 0;
} /* end main() */

#endif
