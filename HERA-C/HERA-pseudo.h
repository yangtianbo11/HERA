/*
 *  Most Pseudo-ops for HERA-C defined here;
 *  Watch out for *SLIGHTLY* different defs of same terms in Hassem too
 */

static void breakpoint_here_for_CBON() { }  /* Set a breakpoint on this function to stop during SETCB */

#define SET(d,v)	SETLO(d, (v)&0xff) SETHI(d, (v)>>8)
#define SETRF(d,v)	SET(d,v) FLAGS(d)
#define MOVE(a,b)	OR(a, b,R0)

#define CMP(a,b)	CON() SUB(r0,a,b)
#define NEG(d,b)	CON() SUB(d,r0,b)
#define NOT(d,b)	SET(R12, 0xffff) XOR((d), R12, (b))

#define CON()		FON(0x08)
#define COFF()		FOFF(0x08)
#define CBON()		breakpoint_here_for_CBON(); FON(0x10)
#define CCBOFF()	FOFF(0x18)

#if defined HERA_2_3_COMPATIBILITY && HERA_2_3_COMPATIBILITY
#define SETF(f) 	FON(f)
#define CLRF(f) 	FOFF(f)
#define SETC()		CON()
#define CLRC()		COFF()
#define SETCB()		CBON()
#define CLCCB()		CCBOFF()
#define MULT(d, a,b)	MUL(d, a,b)
#endif

#define FLAGS(a)	FOFF(0x08) ADD(r0,r0,(a))

#define HALT()		return;
#define NOP()		;

/* data pseudo-ops are in HERA-data.h */
/* label-based branches and calls are in HERA-instr.h */
