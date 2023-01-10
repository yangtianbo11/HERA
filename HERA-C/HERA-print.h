#include <iostream>
#include <iomanip>

#if ! defined HERA_C
#error "HERA-print.h is only available (for debugging) in HERA_C simulator"
#endif

#if ! defined HERA_PRINT
#define HERA_PRINT

using namespace std;

// this gets "__STRING" macro ...
#include <sys/cdefs.h>
// ... except, reportedly, on MS Windows. So, let's do this too:
#ifndef __STRING
#define __STRING(x) #x
#endif

#define print(s)	HERA_print(s);
#define print_reg(r)	HERA_print_reg(r, __STRING(r));
#define print_regs(nz,sys)	HERA_print_regs(nz,sys);
#define print_flag(f)	HERA_print_flag(f);
#define print_flags()	HERA_print_flags();
#define print_lots(oct)	HERA_print_lots("", oct);
#define print_stack()	HERA_print_lots("", 00010); // 00010 = include 0...SP-1
#define print_globals()	HERA_print("Ask Dave to finish print_globals"); // HERA_print_lots("", 00700); // not done yet
#define print_fsheap()	HERA_print("Ask Dave to finish print_fsheap"); // HERA_print_lots("", 07000); // not done yet
#define dump()		HERA_print_lots("", 001|004);
#define dump_w_preheap()(HERA_print_lots("", 001|004), HERA_print_mem(0, first_space_for_fsheap-1));
#define dump_w_mem()	(HERA_print_lots("", 001|004), HERA_print_mem(0, MSIZE-1,true,true));
#define print_mem(skip, print) HERA_print_mem(skip, print, false, false);
#define print_mem_with_addr(skip, print) HERA_print_mem(skip, print, true, true);


static void HERA_print(const char *s)
{
    cout << s << endl;
}

static void HERA_print_reg(register_accessor x, const char *name)
{
    cout << " " << setfill(' ') << setw(12) << name << " = ";
    
    cout <<   "0x" << setfill('0') << setw(4)
	 << setbase(16) << uword(x.get_register());
    cout << " = 0" <<  setbase(8) << uword(x.get_register());
    //  cout << " = 0b"<<  setbase(2) << uword(x.get_register());
    cout << " = "  << setbase(10) << x.get_register();
    if (x.get_register() < 0)
	cout << " = " << uword(x.get_register());
    if (x.get_register() < 128 && x.get_register() >= 0 && isprint((x.get_register())))
	cout << " = '"  << char(x.get_register()) << "'";

    cout << endl;
}
static void HERA_print_regs(bool nonZeroToo, bool alsoSys)
{
	cout << (alsoSys?"All ":"") << (nonZeroToo?"":"Non-Zero ") << "Registers:" << endl;
	if (alsoSys && (nonZeroToo || reg(r0)!=0))  print_reg(r0);
	if (nonZeroToo || reg(r1)!=0)  print_reg(r1);
	if (nonZeroToo || reg(r2)!=0)  print_reg(r2);
	if (nonZeroToo || reg(r3)!=0)  print_reg(r3);
	if (nonZeroToo || reg(r4)!=0)  print_reg(r4);
	if (nonZeroToo || reg(r5)!=0)  print_reg(r5);
	if (nonZeroToo || reg(r6)!=0)  print_reg(r6);
	if (nonZeroToo || reg(r7)!=0)  print_reg(r7);
	if (nonZeroToo || reg(r8)!=0)  print_reg(r8);
	if (nonZeroToo || reg(r9)!=0)  print_reg(r9);
	if (nonZeroToo || reg(r10)!=0) print_reg(r10);
	if (alsoSys && (nonZeroToo || reg(r11)!=0)) print_reg((RT,r11));
	if (alsoSys && (nonZeroToo || reg(r12)!=0)) print_reg((FP_alt,r12));
	if (alsoSys && (nonZeroToo || reg(r13)!=0)) print_reg((PC_ret,r13));
	if (alsoSys && (nonZeroToo || reg(r14)!=0)) print_reg((FP,r14));
	if (alsoSys && (nonZeroToo || reg(r15)!=0)) print_reg((SP,r15));
}

static void HERA_print_flag(int f)
{
    static const char *bool_name[2] = { "FALSE", "TRUE" };

    if (f == 0)
	cout << " S: " << bool_name[flags[flag_s]];
    else if (f == 1)
	cout << " Z: " << bool_name[flags[flag_z]];
    else if (f == 2)
	cout << " V: " << bool_name[flags[flag_v]];
    else if (f == 3)
	cout << " C: " << bool_name[flags[flag_c]];
    else if (f == 4)
	cout << " carry-block: " << bool_name[flags[flag_carry_block]];
    else
	cout << " warning: illegal flag print request\n";
    cout << endl;
}

static void HERA_print_flags()
{
    static const char *bool_name[2] = { "FALSE", "TRUE " };

    cout << "Flags: ";
    cout << "carry-block=" << bool_name[flags[flag_carry_block]];
    cout << " C=" << bool_name[flags[flag_c]];
    cout << " V=" << bool_name[flags[flag_v]];
    cout << " Z=" << bool_name[flags[flag_z]];
    cout << " S=" << bool_name[flags[flag_s]];
    cout << endl;
}


#if ! defined HERA_print_mem_addr
#define HERA_print_mem_addr(m)	" " << m << ":"
#endif

static void HERA_print_mem(int skip, int print, bool withAddr, bool skipZero)
{
    int lo=skip, hi=skip+print;
    bool printAddrNextTime = false;
    cout << "Memory[" << lo << "..." << hi << "]:";
    for (int m=lo; m<hi;m++) {
	if (mem(m) == 0 && m<MSIZE-2 && mem(m+1) == 0 && mem(m+2) == 0) {
		cout << endl << " 0x" << setw(4) << setfill('0') << setbase(16) << m << " ... ";
	    do {
		m++;
	    } while(mem(m) == 0 && m<MSIZE-1 && mem(m+1) == 0);
	    // assert mem(m) is the last zero we'll see (or next-to-top?) */
	    cout << setw(4) << setfill('0') << setbase(16) << m << ": 0" << setw(0) << setfill(' ') << setbase(10) << endl;
	    printAddrNextTime=true;
	} else /* regular case, don't see a pair of 0's */ {
	    if (mem(m) || !skipZero) {
		if (withAddr || printAddrNextTime)
			cout << " 0x" << setw(4) << setfill('0') << setbase(16) << m << ": " << mem(m);
		else
			cout << " " << mem(m);
		cout << setfill(' ') << setw(0) << setbase(10);
		printAddrNextTime=false;
	    }
	}
    }
    cout << endl;
}

static void HERA_print_lots(const char *message, int verbosityFromOctalCode)
{
	if (verbosityFromOctalCode>0 && message)
		Hprint(message);
	if (verbosityFromOctalCode & 00003) { // 2=zero too, 1=system too, 3=all
		HERA_print_regs(verbosityFromOctalCode&1, verbosityFromOctalCode&2);
	}
	if (verbosityFromOctalCode & 00004) {
		Hprint("\n");
		HERA_print_flags();
	}
	if (verbosityFromOctalCode & 00070) { // stack
		// 00010, include 0...SP-1, maybe including 0's
		// 00020, include all non-zero stack elements
		Hprint("\nStack is ");
		int overTop = reg(SP);
		if (verbosityFromOctalCode & 00020) {
			Hprint("(part of) ");
			overTop=last_space_for_stack+1;
		}
		if (overTop > 0)
			HERA_print_mem(0, overTop-1, verbosityFromOctalCode&010000, verbosityFromOctalCode&020000);
		else
			Hprint("(empty)\n");
	}
	if (verbosityFromOctalCode & 00700) { // globals, C000...(*C000-1)
		Herror("Not ready to print globals yet");		
		// HERA_print_mem(???, ???, verbosityFromOctalCode&010000, verbosityFromOctalCode&020000);
	}
	if (verbosityFromOctalCode & 07000) { // heap, non-zeros in 4000-BFFF
		Herror("Not ready to print heap yet");
		// HERA_print_mem(???, ???, verbosityFromOctalCode&010000, verbosityFromOctalCode&020000);
	}
}

#endif /* defined HERA_PRINT */
