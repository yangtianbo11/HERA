#include <HERA.h>

void HERA_main()
{
#define ADD_DP(d_hi,d_lo, a_hi,a_lo, b_hi,b_lo) \
  COFF() ADD(d_lo,a_lo,b_lo) ADD(d_hi,a_hi,b_hi)

#define SUB_DP(d_hi,d_lo, a_hi,a_lo, b_hi,b_lo) \
  CON()  SUB(d_lo,a_lo,b_lo) SUB(d_hi,a_hi,b_hi)

#include "Tiger-stdlib-stack-data.hera"

DLABEL(HERES_SUM)
LP_STRING("The double-precision sum is ")
DLABEL(HERES_DIFF)
LP_STRING("\nThe double-precision difference is ")
DLABEL(HERES_SEP)
LP_STRING(" ")

// Make [R1 R2] the (double-precision) sum of [R3 R4] + [R5 R6] + 1000000;
// finally, update [R7 R8] by subtracting the above total from it

CCBOFF()       // Enable use of carry flag for multiple-precision arithmetic
  // ...          (possibly other instructions that may set or clear the carry)

// start by finding [R3 R4] + [R5 R6]
ADD_DP(R1,R2, R3,R4, R5,R6)

// now add 1000000 to the above
SET(Rt, 16960) //  16960 is the lower 16 bits of 1000000, i.e., 1000000 %65536
SETLO(R10, 15) //  15    is the higer 16 bits of 1000000, i.e., 1000000//65536
ADD_DP(R1,R2, R1,R2, R10,Rt)
#if defined HERA_C
print("Just did two double-precision adds into R1/R2:"); print_reg(R1); print_reg(R2);
#endif

// next calculation: subtract the above from [R7 R8]
SUB_DP(R7,R8, R7,R8, R1,R2)
#if defined HERA_C
print("Just did double-precision subtract into R7/R8:"); print_reg(R7); print_reg(R8);
#endif

// The following assumes that Tiger-stdlib-reg conforms to the "Hybrid Conventions"
//  section of the HERA 2.4 booklet, with R4..R7 (and R12..R15) preserved through calls,
//  which (as of this writing) was true for the library except tstrcmp, see output of
//  egrep 'R[4567]' lib/*

MOVE(FP_alt, SP)
INC(SP, 4)

SET(Rt, HERES_SUM)
STORE(Rt, 3,FP_alt)
CALL(FP_alt, print)
STORE(R1, 3,FP_alt)
CALL(FP_alt, printint)
SET(Rt, HERES_SEP)
STORE(Rt, 3,FP_alt)
CALL(FP_alt, print)
STORE(R2, 3,FP_alt)
CALL(FP_alt, printint)

SET(Rt, HERES_DIFF)
STORE(Rt, 3,FP_alt)
CALL(FP_alt, print)
STORE(R7, 3,FP_alt)
CALL(FP_alt, printint)
SET(Rt, HERES_SEP)
STORE(Rt, 3,FP_alt)
CALL(FP_alt, print)
STORE(R8, 3,FP_alt)
CALL(FP_alt, printint)

DEC(SP, 4)

HALT()
#include "Tiger-stdlib-stack.hera"
}

