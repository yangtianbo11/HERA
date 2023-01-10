/*
 * This is the definition of the HERA architecture for the HERA-C simulator.
 */

#include <setjmp.h>
#include <limits.h>
#include <stdlib.h>

	
/* architecture */
const int MSIZE  = 65536;
const int RCOUNT = 16;
const int NFLAGS = 5;


typedef signed char byte;
const int BITS_PER_WORD = 16;
typedef short word;
typedef unsigned short uword;
typedef unsigned short address;

const int s_maxval = 32767;
const int s_minval = -32768;
const unsigned int u_maxval = 65535;
const unsigned int u_minval = 0;

word registers[RCOUNT], junk_register;

// let the C++ type checker distinguish ADD(5,3,4) from ADD(r5,r3,r4)
class register_accessor {
private:
    word &value;
public:
    explicit register_accessor(word &it): value(it) { if (!(&it >= &registers[0] && &it <= &registers[RCOUNT-1])) Herror("Internal HERA-C inconsistency when setting up registers"); }
    const word &get_register() { return value; }
    void set_register(word new_value) { if (&value != &registers[0]) value=new_value; }
};

#define reg(x)	((x).get_register())
#define ureg(x)	((uword) ((x).get_register()))

static register_accessor r0(registers[  0]), R0 = r0;
static register_accessor r1(registers[  1]), R1 = r1;
static register_accessor r2(registers[  2]), R2 = r2;
static register_accessor r3(registers[  3]), R3 = r3;
static register_accessor r4(registers[  4]), R4 = r4;
static register_accessor r5(registers[  5]), R5 = r5;
static register_accessor r6(registers[  6]), R6 = r6;
static register_accessor r7(registers[  7]), R7 = r7;
static register_accessor r8(registers[  8]), R8 = r8;
static register_accessor r9(registers[  9]), R9 = r9;
static register_accessor ra(registers[0xa]), RA = ra, rA = ra, r10 = ra, R10 = ra;
static register_accessor rb(registers[0xb]), RB = rb, rB = rb, r11 = rb, R11 = rb;
static register_accessor rc(registers[0xc]), RC = rc, rC = rc, r12 = rc, R12 = rc;
static register_accessor rd(registers[0xd]), RD = rd, rD = rd, r13 = rd, R13 = rd;
static register_accessor re(registers[0xe]), RE = re, rE = re, r14 = re, R14 = re;
static register_accessor fp = re, FP = re;  // Call hardware knows of this
static register_accessor rf(registers[0xf]), RF = rf, rF = rf, r15 = rf, R15 = rf;
static register_accessor sp = rf, SP = rf;  // Might as well define this near FP

static register_accessor rt = rb, RT = rt, Rt = rt, tmp = rt, TMP = rt;
static register_accessor FP_alt = rc, r_DL = rc;  // for use in CALL/RETURN convention
static register_accessor PC_ret = rd, r_RA = rd;  // for use in CALL/RETURN convention


word memory[MSIZE], junk_memory_cell;
#define mem(addr)   (((addr) >= 0 && (addr) < MSIZE) ? memory[(address) (addr)] : ((void)Herror("Illegal memory reference\n"), junk_memory_cell))

bool flags[NFLAGS];

const int flag_s=0;
const int flag_z=1;
const int flag_v=2;
const int flag_c=3;
const int flag_carry_block=4;

jmp_buf returns[MSIZE/2];
word next_free_return = 0;

