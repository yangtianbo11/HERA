// iterative factorial calculation just for 6! and 9! in HERA
// by Dave Wonnacott, Summer 2005 (and later revised), as an example for CMSC 245/240
//Names of Lab Members: Alex Reichard<areichard@haverford.edu>, Femi Obiwumi<oobiwumi@hc>, Seamus Flannery<smflannery@haverford.edu>, Tianbo Yang<tyang3@haverford.edu>;

#include <HERA.h>		/* This is required for HERA projects */
#include <HERA-print.h>	/*  This is needed to use print_reg and print for debugging in HERA-C */

void HERA_main()
{
  // We use "single-precision" initially,
  //  so we set the carry-block flag here.

/*    CBON()

  // COMPUTE AND PRINT 6! AND 9!

  print("\n***  Let's 6! and 9! as easily as we can, i.e., by straight-line sequences of steps \n")

	SET(r1, 1)
	SET(r2, 2)
	MUL(r1, r1, r2)   // r1 = 1*2
	SET(r2, 3)
	MUL(r1, r1, r2)   // r1 = 1*2*3
	SET(r2, 4)
	MUL(r1, r1, r2)   // r1 = 1*2*3*4
	SET(r2, 5)	     // note: since r2 was 4, this could be INC(r2, 1)
	MUL(r1, r1, r2)   // r1 = 1*2*3*4*5
	SET(r2, 6)	     // note: since r2 was 5, this could be INC(r2, 1)
	MUL(r1, r1, r2)   // r1 = 1*2*3*4*5*6
	
  print("6! is ")
  print_reg(r1)

	// now, try to find 9! in single precision (starting with 6! in r2)

	INC(r2, 1)	// taking advantage of the observation above
	MUL(r1, r1, r2)
	
	INC(r2, 1)
	MUL(r1, r1, r2)
	
	INC(r2, 1)
	MUL(r1, r1, r2)
	
  print("9! computed in single precision is ")
  print_reg(r1)
	
  print("Oops, 9! should be 362880, which doesn't fit in single precision\n")
  print("\n\n***  Let's explore multi-word precision\n")
	
	// double-precision multiplication is beyond the scope of this discussion,
	// but we can try a double-precision add -- first, turn on multi-prec.
	
	CCBOFF()
	
	// use r1/r2 as one register pair meaning one number (285234),
	//     r3/r4 as the other (702702)
	// put result in r1/r2
	
	SET(r1, 4)
	SET(r2, 23090)
	
	SET(r3, 10)
	SET(r4, 47342)
	
	COFF()
	ADD(r2, r2, r4)
	ADD(r1, r1, r3) /// uses "carry" from above
	
  print("The double-precision add of 285234+702702 should give us 987936\n")
  print(" That's also 15 * 2**16 + 4896\n")
  print_reg(r1)
  print_reg(r2)



	  // Now demonstrate a loop --- once again, find 6!

  print("\n\n***  Let's find 6! again, using a loop, but not fully follow-through by writing double-precision loop \n")
  //  Below is the HERA equivalent of this Python program:
  // j = 1
  // i = 1
  // while i <= 6:
  //   j = j * i
  //   i = i + 1

  // BETTER USE SINGLE-PRECISION OR REGRET IT!
  CBON()

  // j = 1
  SET(R2, 1)	// R2 will be j
  // i = 1
  SET(R1, 1)   // R1 will be i
  // while i <= 6:
  LABEL(Start_of_Fact_while)
  //  note --- now we want to get out of the loop if i<=6 is false
  //			in other words, if i-6 <= 0 is false, i.e. 6-i < 0
  SET(R3,6)
  SUB(R0, R3,R1)
  //             now get out of the loop if we got a negative
  BS(Out_of_Fact_while)
  //   j = j * i
  MUL(R2, R2,R1)
  //   i = i + 1
  INC(R1, 1)
  BR(Start_of_Fact_while)
  LABEL(Out_of_Fact_while)

  print ("with any luck, R2 now has 6!")
  print_reg(R2) */

//loop that computes fibonacci sequence

CBON()

SET(R1,12)         //R1 holds the number of fibonacci term we are calculating
SET(R2,1)          //value of previous term
SET(R3,1)          //value of current term
SET(R9,2)          //R9 is a constant

LABEL(while)
ADD(R4,R3,R2)
ADD(R2,R3,R0)
ADD(R3,R4,R0)
DEC(R1,1)
SUB(R0,R1,R9)
BG(while)

print_reg(R3)

  HALT()
}
