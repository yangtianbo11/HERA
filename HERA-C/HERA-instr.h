// The ## pre-processor operation fuses the two symbols,
//  so that LABEL_NAME(fred) becomes label_fred.
// This then lets us use label_fred as a jump target,
//  and fred as an integer address (for the data pseudo-ops)

// NOTE that labels are done here along with the CALL and branch pseudo-ops,
//  because real branch instructions are not implemented in the simulator


static int TEMPORARY_VALUE;
static int TEMPORARY_MULT;   // also used in shifts as a temporary
static unsigned int TEMPORARY_UVALUE;


#define LABEL_NAME(label)	label_ ## label

#define DO_SETREG(d, value)    ((d).set_register(value))


/* set a register to a value but don't touch flags */
#define SETREG_none(d, value)  (TEMPORARY_VALUE = (value),          	\
			        DO_SETREG((d),TEMPORARY_VALUE)		\
    			       )

/* set a register to a value, adjust s and z but nothing else */
#define SETREG_sz(d, value)  (TEMPORARY_VALUE = (value),          	\
			      DO_SETREG((d),TEMPORARY_VALUE),		\
			      flags[flag_s]= ((TEMPORARY_VALUE)&(s_maxval+1)),\
			      flags[flag_z]= (word(TEMPORARY_VALUE) == 0) \
    			     )

/* set a register to a value, adjust all flags */
#define SETREG_f(d, value,uval) (TEMPORARY_UVALUE = (uval),           \
				 SETREG_sz((d), (value)),  /* sets T_V */ \
			         flags[flag_v]= (((TEMPORARY_VALUE) > s_maxval) | ((TEMPORARY_VALUE) < s_minval)),\
			         flags[flag_c]= ((TEMPORARY_UVALUE) > (u_maxval)) \
    			     )

#define BOOL(X)  ((X) ? 1 : 0)


#if HERA_C_WARN_ABOVE_STACK > 0
#define REFMEM(addr)	     ( \
			      (((unsigned((address)addr))>=unsigned(reg(SP)) && (unsigned((address)addr))<(unsigned(reg(SP))+HERA_C_WARN_ABOVE_STACK))?Herror("WARNING: REFERENCE TO MEMORY AT/JUST-ABOVE SP", false):42), \
			      mem((address) addr) \
			     )
#else
#define REFMEM(addr)	     (mem((address) addr))
#endif
#define SETMEM(addr, value)  (REFMEM((address) addr) = (value))

#define PACK_THE_FLAGS       ((flags[flag_carry_block])*16 + (flags[flag_c])*8 + (flags[flag_v])*4 + (flags[flag_z])*2 + (flags[flag_s]))
#define CARRY_FOR_ADD	     (int(flags[flag_c] && (!flags[flag_carry_block])))
#define CARRY_FOR_SUB	     (int(flags[flag_c] ||  (flags[flag_carry_block])))

// #define UNIMPLEMENTED(OP)    ((cerr << OP " not implemented in HERA-C yet" << endl),exit(3));
// #define DEPRECATED(OP)       ((void) Herror(OP), (void) Herror(" no longer provided in HERA-C 2.0\n"),exit(4));

// ************************************************
//
// 2.1  Arithmetic, Shift, and Logical Instructions
//
// ************************************************


#define SETLO(d,v)	SETREG_none((d), ((void)(((v)<-128||(v)>255)?Herror("Illegal constant for SETLO"):0),((signed char) (v))));
#define SETHI(d,v)	SETREG_none((d), ((void)(((v)<-128||(v)>255)?Herror("Illegal constant for SETHI"):0),(((reg(d)) & 0xff) | ((v) << 8))));


#define AND(d,a,b)	SETREG_sz((d), (reg(a)) & (reg(b)));
#define  OR(d,a,b)	SETREG_sz((d), (reg(a)) | (reg(b)));

#define ADD(d,a,b)	SETREG_f((d), reg(a)+reg(b)+CARRY_FOR_ADD,     ureg(a)+ureg(b)+CARRY_FOR_ADD);
#define SUB(d,a,b)	SETREG_f((d), reg(a)-reg(b)-(1-CARRY_FOR_SUB), (u_maxval+1)+ureg(a)-ureg(b)-(1-CARRY_FOR_SUB));


/*
   HERA 2.4.0 doc (reformatted):

	When carry- block is false, MUL can produce a variety of different results,
	depending on the values of all four other flags.
	* When all are zero, MUL produces produces the low word (bits 15-0) of
	  the product of R a and R b, exactly as it does when carry-block is true.
	* When only s is true, MUL produces the high word (bits 31-16) of
	  the signed interpretations of R a and R b.
	The behavior for the other fourteen possible sets of flag values are currently undefined.

   Below, letters indicate flags are on (capital) or off (lower case); underscore = don't care,
   e.g. MUL_bcvzS means the sign flag must be on and all others off to trigger this behaviour.
*/

#define MUL_B____(d,a,b)	(SETREG_f((d), reg(a)*reg(b), ureg(a)*ureg(b))) /* last arg used for setting c flag */
#define MUL_bcvzs(d,a,b)	MUL_B____(d,a,b)
#define MUL_bcvzS(d,a,b)	(SETREG_f((d), (reg(a)*reg(b))>>16, reg(a)*reg(b)))

#define MUL(d,a,b)	( flags[flag_carry_block]	\
			? MUL_B____(d,a,b)		\
			: (!(flags[flag_c] || flags[flag_v] || flags[flag_z])		\
			  ? (flags[flag_s] ? MUL_bcvzS(d,a,b) : MUL_bcvzs(d,a,b))	\
			  : Herror("Sorry MUL only defined in HERA-C 2.4.0 for CB=true, or C=V=Z=false")));

// old def that assumed CB was on, before I got around to handling other cases...
#define MUL_CBON(d,a,b)	(SETREG_f((d), \
			 (flags[flag_carry_block] ? reg(a)*reg(b) : (Herror("Sorry MUL only implemented in HERA-C 2.4.0 for CB=true"),0xdead)), \
			 ureg(a)*ureg(b)) /* this last arg used for setting c flag */ \
			);
// old 2.3 definition:
// #define MUL(d,a,b)	(TEMPORARY_MULT = reg(a)*reg(b), DO_SETREG(TMP,unsigned(TEMPORARY_MULT)>>16), DO_SETREG((d),unsigned(TEMPORARY_MULT)&0xffff), flags[flag_s] = TEMPORARY_MULT<0, flags[flag_z] = (TEMPORARY_MULT == 0), flags[flag_v] = TEMPORARY_MULT != (int) reg(d));


# define XOR(d,a,b)	SETREG_sz((d), reg(a) ^ reg(b));


// NEW -- flags for shifts adjusted November 2018; should have checked them better in 2014 :-P
# define LSL(d,b) 	(TEMPORARY_MULT = reg(b), SETREG_sz((d), (uword(reg(b))<<(1)) | (CARRY_FOR_ADD       )),  \
			 flags[flag_c] = BOOL(TEMPORARY_MULT & 0x8000));
# define LSR(d,b) 	(TEMPORARY_MULT = reg(b), SETREG_sz((d), (uword(reg(b))>>(1)) | (CARRY_FOR_ADD*0x8000)),  \
			 flags[flag_c] = BOOL(TEMPORARY_MULT & 0x0001));
# define ASL(d,b)	(TEMPORARY_MULT = reg(b), SETREG_sz((d), (reg(b)*2 + CARRY_FOR_ADD)),  \
			 flags[flag_c] = BOOL(TEMPORARY_MULT & 0x8000),  \
			 flags[flag_v] = (((TEMPORARY_MULT*2+CARRY_FOR_ADD) > s_maxval) | ((TEMPORARY_MULT*2+CARRY_FOR_ADD) < s_minval)));
# define ASR(d,b)	(TEMPORARY_MULT = reg(b), SETREG_sz((d), ((reg(b))>>1)), \
			 flags[flag_c] = BOOL(TEMPORARY_MULT & 0x0001));
# define LSL8(d,b)	 SETREG_sz((d), (uword(reg(b))<<(8)));
# define LSR8(d,b)	 SETREG_sz((d), (uword(reg(b))>>(8)));


#define DEF_FLAGS(val)	((flags[flag_carry_block] = ((val) & 16)), \
                         (flags[flag_c]           = ((val) & 8)),  \
                         (flags[flag_v]           = ((val) & 4)),  \
                         (flags[flag_z]           = ((val) & 2)),  \
                         (flags[flag_s] = ((val) & 1)))
#define CHECK_FLAGS(u5)  ((((u5)<0)|(((unsigned) u5)>0x1f))?((void)Herror("Illegal flag specification"),((u5)&0x1f)):(u5))
#define CHECK_FLAGS4(u4) (((((u4)<0)|(((unsigned) u4)>0x0f))?((void)Herror("Illegal flag specification"),((u4)&0x0f)):(u4)) | flags[flag_carry_block]*0x10 )
#define FON(u5) 	DEF_FLAGS( ((PACK_THE_FLAGS) |   (CHECK_FLAGS(u5)))  );
#define FOFF(u5)	DEF_FLAGS( ((PACK_THE_FLAGS) & (~(CHECK_FLAGS(u5)))) );
#define FSET4(u4)	DEF_FLAGS( (CHECK_FLAGS4(u4)) );
#define FSET5(u5)	DEF_FLAGS( (CHECK_FLAGS (u5)) );
#define SAVEF(d)	DO_SETREG((d), PACK_THE_FLAGS);
#define RSTRF(a)	DEF_FLAGS(reg(a));


# define INC(d,u)	SETREG_f((d),           \
				 reg(d)+((u)>32?((void)Herror("Illegal increment ... must be <= 32"),32):((u)<1?((void)Herror("Illegal increment ... must be >= 1"),1):(u))), \
				 ureg(d)+((u)>32?32:((u)<1?1:(u))));
# define DEC(d,u)	SETREG_f((d),           \
				 reg(d)-((u)>32?((void)Herror("Illegal decrement ... must be <= 32"),32):((u)<1?((void)Herror("Illegal decrement ... must be >= 1"),1):(u))), \
				 (u_maxval+1)+ureg(d)-((u)>32?32:((u)<1?1:(u)))); /* stole this last part from SUB */



// ***********************
//
// 2.2 Memory Instructions
//
// ***********************

#define LOAD(d,o,a)	SETREG_sz((d), REFMEM(reg(a)+((unsigned(o) > 31 ? ((void)Herror("Illegal offset for LOAD, must be 0..31"),1):1),unsigned(o)&0x1f)));
#define STORE(d,o,a)	SETMEM(reg(a)+((unsigned(o) > 31 ? ((void)Herror("Illegal offset for STORE, must be 0..31"),1):1),unsigned(o)&0x1f), reg(d));


// ***************************************
//
// 2.3 Control-Flow and Other Instructions
//
// ***************************************



// Branches and Call are done in a strange way in the simulator,
//  since the program isn't really stored in RAM.
// Basically, only the label-based pseudo-ops are done.
//
// SWI and RTI don't work, but don't produce errors until run-time.
//
// Thanks to Todd Miller HC'01 for dealing with the setjmp/longjmp code
//  in the original (32-bit) version of this system.


#define LABEL(label)    LABEL_NAME(label): ;
// tried to do this in code too, but got jumps crossing initializations

#define BRR(label)	{                    goto LABEL_NAME(label) ; }

#define BLR(label)	{ if( ((flags[flag_s]+flags[flag_v]) == 1) )                  goto LABEL_NAME(label); }
#define BGER(label)	{ if(!((flags[flag_s]+flags[flag_v]) == 1) )                  goto LABEL_NAME(label); }
#define BLER(label)	{ if( ((flags[flag_s]+flags[flag_v]) == 1 || flags[flag_z]) ) goto LABEL_NAME(label); }
#define BGR(label)	{ if(!((flags[flag_s]+flags[flag_v]) == 1 || flags[flag_z]) ) goto LABEL_NAME(label); }
#define BULER(label)	{ if( (!flags[flag_c] || flags[flag_z]) )                      goto LABEL_NAME(label); }
#define BUGR(label)	{ if(!(!flags[flag_c] || flags[flag_z]) )                      goto LABEL_NAME(label); }

#define BZR(label)	{ if( flags[flag_z]) goto LABEL_NAME(label); }
#define BNZR(label)	{ if(!flags[flag_z]) goto LABEL_NAME(label); }
#define BCR(label)	{ if( flags[flag_c]) goto LABEL_NAME(label); }
#define BNCR(label)	{ if(!flags[flag_c]) goto LABEL_NAME(label); }
#define BSR(label)	{ if( flags[flag_s]) goto LABEL_NAME(label); }
#define BNSR(label)	{ if(!flags[flag_s]) goto LABEL_NAME(label); }
#define BVR(label)	{ if( flags[flag_v]) goto LABEL_NAME(label); }
#define BNVR(label)	{ if(!flags[flag_v]) goto LABEL_NAME(label); }


#define BR(label)	{ BRR(label); SETREG_none((Rt),__LINE__); }

#define BL(label)	{ BLR(label); SETREG_none((Rt),__LINE__); }
#define BGE(label)	{ BGER(label); SETREG_none((Rt),__LINE__); }
#define BLE(label)	{ BLER(label); SETREG_none((Rt),__LINE__); }
#define BG(label)	{ BGR(label); SETREG_none((Rt),__LINE__); }
#define BULE(label)	{ BULER(label); SETREG_none((Rt),__LINE__); }
#define BUG(label)	{ BUGR(label); SETREG_none((Rt),__LINE__); }

#define BZ(label)	{ BZR(label); SETREG_none((Rt),__LINE__); }
#define BNZ(label)	{ BNZR(label); SETREG_none((Rt),__LINE__); }
#define BC(label)	{ BCR(label); SETREG_none((Rt),__LINE__); }
#define BNC(label)	{ BNCR(label); SETREG_none((Rt),__LINE__); }
#define BS(label)	{ BSR(label); SETREG_none((Rt),__LINE__); }
#define BNS(label)	{ BNSR(label); SETREG_none((Rt),__LINE__); }
#define BV(label)	{ BVR(label); SETREG_none((Rt),__LINE__); }
#define BNV(label)	{ BNVR(label); SETREG_none((Rt),__LINE__); }


/*
 Added the following to make the stack look more interesting in the debugger.
 It also may help find bugs is the CALL/RETURN instructions
 */


#if ! defined RANDOM_HERA_OFFSET
#define RANDOM_HERA_OFFSET  17    /* makes sure that common number such as 0 doesn't look like the right answer */
#endif
#if ! defined TYPICAL_FRAME_MAX_SIZE
#define TYPICAL_FRAME_MAX_SIZE 68	/* HELP noticing errors */
#endif

register_accessor *RETURN_old_FP;

#if defined DONT_USE_HERA_C_VERBOSE
#define DUMP_AT_BAD_CALL(message)	Herror(message)
#define DUMP_AT_BAD_RETURN(message)	Herror(message)
#define DUMP_AT_ANY_CALL(message)
#define DUMP_AT_ANY_RETURN(message)
#else
#define DUMP_AT_BAD_CALL(message)   HERA_print_lots(message, HERA_C_CALL_ERROR_VERBOSITY)
#define DUMP_AT_BAD_RETURN(message) HERA_print_lots(message, HERA_C_RETURN_ERROR_VERBOSITY)
#define DUMP_AT_ANY_CALL(message)   HERA_print_lots(message, HERA_C_CALL_VERBOSITY)
#define DUMP_AT_ANY_RETURN(message) HERA_print_lots(message, HERA_C_RETURN_VERBOSITY)
#endif

#define CALL(r_new_FP, l_for_PC)					\
	{	DUMP_AT_ANY_CALL("Calling function ");			\
		doing_CALL();	/* help trigger debugger */		\
		word temp, temp_RET;  /* two vars just in case */	\
		if (	/* note that "l" must be a label to compile */	\
		    (r_new_FP.get_register() != FP_alt.get_register())) { \
			Herror("NOTE --- unconventional CALL parameters: for simulation in HERA-C, use FP_alt (R12) for new FP (R13 is implicitly used for target address)", true);			\
		}							\
  /* Rb=PC+1 */	DO_SETREG(r_RA, __LINE__ + 1);				\
		if (    reg(r_new_FP)+TYPICAL_FRAME_MAX_SIZE > last_space_for_stack || \
			reg(SP)                              > last_space_for_stack) { \
			DUMP_AT_BAD_CALL("WARNING --- STACK OVERFLOWING INTO STANDARD HEAP SPACE");			\
		}							\
		temp=reg(FP);						\
  /* FP=Ra */	DO_SETREG(FP, reg(r_new_FP));				\
  /* Ra=FP */	DO_SETREG(r_new_FP, temp);				\
		next_free_return++;					\
		if (!setjmp((returns[next_free_return-1]))) {		\
  /* PC=label*/ 	goto LABEL_NAME(l_for_PC);			\
		} else	{						\
			DUMP_AT_ANY_RETURN("returned from function");	\
  /* PC= Rb */		/****** RETURNED TO HERE ******/		\
			if (reg(PC_ret) != __LINE__ + 1) {		\
				DUMP_AT_BAD_RETURN("WARNING --- HERA-C simulator detected incorrect return address in PC_ret; did you restore it correctly?");		\
			}						\
			temp_RET = reg(FP);				\
  /* FP= return Ra */	DO_SETREG(FP, reg(*RETURN_old_FP));	/**/	\
  /* Ra= return FP */	DO_SETREG(*RETURN_old_FP, temp_RET);	/**/	\
			if (reg(FP) > reg(SP) || reg(FP) < reg(SP)-TYPICAL_FRAME_MAX_SIZE) { \
				DUMP_AT_BAD_RETURN("WARNING --- suspicious FP/SP values during RETURN --- Did you\n\t\ta) save and restore return address and old FP properly and\n\t\tb) make sure each _execution_ of an SP increment is matched by an execution of an SP decrement of the same size?\n\tIf not, check those things! If so, it may be a problem with a pointer in your HERA code :-(\n");		\
			}						\
			doing_RETURN();  /* help trigger debugger */	\
		}							\
	}

#define RETURN(R_old_FP, home)						\
	{	DUMP_AT_ANY_RETURN("About to return from function");	\
		if (	   (home.get_register() != PC_ret.get_register()) ||  \
			   (R_old_FP.get_register() != FP_alt.get_register())) \
			DUMP_AT_BAD_RETURN("NOTE --- unconventional RETURN parameters; for simulation in HERA-C, use FP_alt (R12) for old FP and PC_ret (R13) for return address\n"); \
		RETURN_old_FP = &(FP_alt);			       	\
		next_free_return--;					\
		if (next_free_return >= 0)                              \
		    longjmp((returns[next_free_return]),1);		\
		else                                                    \
		    DUMP_AT_BAD_RETURN("WARNING --- RETURNS outnumber CALLS, HERA-C will not be accurate on after this problem");			\
	}



#define SWI(interrupt_number)	\
	Herror("Sorry, SWI can't execute in HERA-C; You may want to test your code by temporarily commenting it out (possibly using #if) and putting in a BR to your interrupt routine");


#define RTI(interrupt_number)	\
	Herror("Sorry, RTI can't execute in HERA-C; You may want to test your code by temporarily commenting it out (possibly using #if) and putting in a BR instruction back to the instruction after the SWI you're using in testing");
