#include <HERA.h>
#include <HERA-print.h>


This is antique code, from HERA 2.2 or earlier.


void HERA_main()
{
    DLABEL(five);
    INTEGER(5);
    CONSTANT(MALLOC1_SIZE, 12);
    CONSTANT(MALLOC2_SIZE, 57);
    DLABEL(hello);
    TIGER_STRING("hello");
    DLABEL(hellos);
    TIGER_STRING("hellos");
    DLABEL(_world);
    TIGER_STRING(" world");
    DLABEL(hello_world);
    TIGER_STRING("hello world");
    DLABEL(goodbye);
    TIGER_STRING("goodbye");
    DLABEL(el);
    TIGER_STRING("el");
    DLABEL(endl);
    TIGER_STRING("\n");
    DLABEL(good);
    TIGER_STRING("\n  *** ALL TESTS PASSED *** \n\n  now try printing some numbers\n\n");
    DLABEL(bad);
    TIGER_STRING("\n   ***  FAILED SOME TESTS -- HOW MANY? SEE R1: ");

#include "Tiger-stdlib-stack-data.hera"

    // And now, the program starts here

    // TIGER STDLIB always uses single-precision, so...
    CBON();

    // Register R1 keeps the current number of tests failed
    SET(r1,0);

    // And now, the tests:


// Test div:  
    SETLO(r2, 0x21)
    SETHI(r2, 0x03)  // n = 0x321  i.e., 3*256 + 2*16 + 1 == 801
    STORE(r2, 3,SP)
    SETLO(r2, 0x11)  // d = 0x11 == 17
    STORE(r2, 4,SP)
    CALL(5, div)
    LOAD(r2, 3,SP)
    SET(r3, 0x2f)    // q = 2f == 47
    CMP(r2, r3)
    BZ(div_ok1)
    INC(R1, 1)
    LABEL(div_ok1)

    SETLO(r2, 0x1f)
    SETHI(r2, 0x03)  // n = 0x31f == 799
    STORE(r2, 3,SP)
    SETLO(r2, 0x11)  // d = 0x11 == 17
    STORE(r2, 4,SP)
    CALL(5, div)
    LOAD(r2, 3,SP)
    SET(r3, 0x2f)    // q = 2f == 47
    CMP(r2, r3)
    BZ(div_ok2)
    INC(R1, 1)
    LABEL(div_ok2)

    SETLO(r2, 0x1e)
    SETHI(r2, 0x03)  // n = 0x31e  == 798
    STORE(r2, 3,SP)
    SETLO(r2, 0x11)  // d = 0x11 == 17
    STORE(r2, 4,SP)
    CALL(5, div)
    LOAD(r2, 3,SP)
    SET(r3, 0x2e)    // q = 2e == 46
    CMP(r2, r3)
    BZ(div_ok3)
    INC(R1, 1)
    LABEL(div_ok3)
    
    SETLO(r2, 0x74)
    SETHI(r2, 0x09)  // n = 0x974 == 2420
    STORE(r2, 3,SP)
    SETLO(r2, 0x37)  // d = 0x37  == 55
    STORE(r2, 4,SP)
    CALL(5, div)
    LOAD(r2, 3,SP)
    SET(r3, 0x2c)    // q = 2c == 44
    CMP(r2, r3)
    BZ(div_ok4)
    INC(R1, 1)
    LABEL(div_ok4)


// Test mod
    SETLO(r2, 0x21)
    SETHI(r2, 0x03)  // n = 0x321  i.e., 3*256 + 2*16 + 1 == 801
    STORE(r2, 3,SP)
    SETLO(r2, 0x11)  // d = 0x11 == 17
    STORE(r2, 4,SP)
    CALL(5, mod)
    LOAD(r2, 3,SP)
    SET(r3, 2)    // r = 2
    CMP(r2, r3)
    BZ(mod_ok1)
    INC(R1, 1)
    LABEL(mod_ok1)

    SETLO(r2, 0x1f)
    SETHI(r2, 0x03)  // n = 0x31f == 799
    STORE(r2, 3,SP)
    SETLO(r2, 0x11)  // d = 0x11 == 17
    STORE(r2, 4,SP)
    CALL(5, mod)
    LOAD(r2, 3,SP)
    SET(r3, 0)    //  r = 0
    CMP(r2, r3)
    BZ(mod_ok2)
    INC(R1, 1)
    LABEL(mod_ok2)

    SETLO(r2, 0x1e)
    SETHI(r2, 0x03)  // n = 0x31e  == 798
    STORE(r2, 3,SP)
    SETLO(r2, 0x11)  // d = 0x11 == 17
    STORE(r2, 4,SP)
    CALL(5, mod)
    LOAD(r2, 3,SP)
    SET(r3, 0x10)    // r = 0x10  == 16
    CMP(r2, r3)
    BZ(mod_ok3)
    INC(R1, 1)
    LABEL(mod_ok3)
    
    SETLO(r2, 0x74)
    SETHI(r2, 0x09)  // n = 0x974 == 2420
    STORE(r2, 3,SP)
    SETLO(r2, 0x37)  // d = 0x37  == 55
    STORE(r2, 4,SP)
    CALL(5, mod)
    LOAD(r2, 3,SP)
    SET(r3, 0)    // r=0 again
    CMP(r2, r3)
    BZ(mod_ok4)
    INC(R1, 1)
    LABEL(mod_ok4)


// Test ORD
    SET(r2, hello);
    STORE(r2, 3, SP);
    CALL(4, ord);
    LOAD(r2, 3, SP);
    SET(r3, 104);  // lower-case h
    CMP(r2, r3);
    BZ(ORD_ok);
    INC(R1, 1);
    LABEL(ORD_ok);

// Test malloc
    SET(r2, 12);
    STORE(r2, 3, SP);
    CALL(4, malloc);
    LOAD(r2, 3, SP);
    print("first malloc (of 12 words) returned: ");
    print_reg(r2);
    SET(TMP,0x1000);
    CMP(r2,TMP);
    BC(MALLOC1_OK);	// no borrow in r2-1000 is good
    INC(R1,1);
    LABEL(MALLOC1_OK);

    SET(r3, MALLOC2_SIZE);
    STORE(r3, 3, SP);
    CALL(4, malloc);
    LOAD(r3, 3, SP);
    print("second malloc returned: ");
    print_reg(r3);
    SET(TMP,0x1000);
    CMP(r3,TMP);
    BNC(MALLOC2_BAD);	//    borrow in r3-1000 is bad
    SET(TMP, MALLOC1_SIZE);
    ADD(r2,r2,TMP);  // what could be 1st free byte after 1st allocation
    CMP(r3,r2);
    BNC(MALLOC2_BAD);	//    borrow in r3-r2 is bad
    BR(MALLOC2_OK);
    LABEL(MALLOC2_BAD);
    INC(R1,1);
    LABEL(MALLOC2_OK);



// TEST CHR
    SET(r2, 104);  // lower-case h
    STORE(r2, 3,SP);
    CALL(4, chr);
    LOAD(r2, 3,SP);
    //   print("malloc in CHR returned: ");
    //   print_reg(r2);
    SET(TMP,0x1000);
    CMP(r2,TMP);
    BNC(CHR_BAD);
    SET(TMP, MALLOC2_SIZE);
    ADD(r3,r3,TMP);  // what could be 1st free byte after 1st allocation
    CMP(r2,r3);
    BNC(CHR_BAD);
    LOAD(r3,0,r2);  // get size of string
    DEC(r3,1);
    BNZ(CHR_BAD);
    LOAD(r3,1,r2);  // get 1st char of string
    SET(TMP, 104);
    CMP(r3,TMP);
    BNZ(CHR_BAD);

    BR(CHR_OK);
    LABEL(CHR_BAD);
    INC(R1,1);
    LABEL(CHR_OK);


// Test size

    SET(r2,hello);
    STORE(r2, 3,SP);
    CALL(4,size);
    LOAD(r2, 3,SP);
    SET(r3, 5);
    CMP(r2, r3);
    BZ(SIZE_OK);
    INC(R1,1);
    LABEL(SIZE_OK);

// Test NOT

    SET(r2, 1);
    STORE(r2, 3,SP);
    CALL(4, not);
    LOAD(r2, 3,SP);
    CMP(r2,r0);
    BNZ(NOT_IS_BAD);

    SET(r2, 0);
    STORE(r2, 3,SP);
    CALL(4, not);
    LOAD(r2, 3,SP);
    CMP(r2,r0);
    BZ(NOT_IS_BAD);

    SET(r2, 17);
    STORE(r2, 3,SP);
    CALL(4, not);
    LOAD(r2, 3,SP);
    CMP(r2,r0);
    BNZ(NOT_IS_BAD);

    SET(r2, -42);
    STORE(r2, 3,SP);
    CALL(4, not);
    LOAD(r2, 3,SP);
    CMP(r2,r0);
    BNZ(NOT_IS_BAD);

    BR(NOT_IS_OK);
    LABEL(NOT_IS_BAD);
    INC(R1,1);
    LABEL(NOT_IS_OK);



// Test tstrcmp
    SET(r2,hello);
    STORE(r2, 3,SP);
    STORE(r2, 4,SP);
    CALL(5,tstrcmp);  // tstrcmp("hello", "hello") --> 0
    LOAD(r2, 3,SP);
    CMP(r2, r0);
    BNZ(TSTRCMP_BAD);

    SET(r2,hello);
    STORE(r2, 3,SP);
    SET(r2,goodbye);
    STORE(r2, 4,SP);
    CALL(5,tstrcmp);  // tstrcmp("hello", "goodbye") --> positive #
    LOAD(r2, 3,SP);
    CMP(r2, r0);
    BLE(TSTRCMP_BAD);

    SET(r2,goodbye);
    STORE(r2, 3,SP);
    SET(r2,hello);
    STORE(r2, 4,SP);
    CALL(5,tstrcmp);  // tstrcmp("goodbye", "hello") --> negative #
    LOAD(r2, 3,SP);
    CMP(r2, r0);
    BGE(TSTRCMP_BAD);

    SET(r2,hello);
    STORE(r2, 3,SP);
    SET(r2,hellos);
    STORE(r2, 4,SP);
    CALL(5,tstrcmp);  // tstrcmp("hello", "hellos") --> negative #
    LOAD(r2, 3,SP);
    CMP(r2, r0);
    BGE(TSTRCMP_BAD);

    SET(r2,hellos);
    STORE(r2, 3,SP);
    SET(r2,hello);
    STORE(r2, 4,SP);
    CALL(5,tstrcmp);  // tstrcmp("hellos", "hello") --> positive #
    LOAD(r2, 3,SP);
    CMP(r2, r0);
    BLE(TSTRCMP_BAD);

    BR(TSTRCMP_OK);
    LABEL(TSTRCMP_BAD);
    INC(R1,1);
    LABEL(TSTRCMP_OK);



// Test substring
    SET(r2,hello);
    STORE(r2, 3,SP);
    SETLO(r2, 1);
    STORE(r2, 4,SP);
    SETLO(r2, 2);
    STORE(r2, 5,SP);
    CALL(6,substring);  // substring("hello", 1, 2) --> "el"
    SET(r2, el);
    STORE(r2, 4,SP);  // result is already there from substring
    CALL(5, tstrcmp);   // tstrcmp("el", "el") should be zero
    LOAD(r2, 3,SP);
    CMP(r2,r0);
    BZ(SUBSTRING_OK);
    INC(R1,1);
    LABEL(SUBSTRING_OK);


// Test concat
    SET(r2, hello);
    STORE(r2, 3,SP);
    SET(r2, _world);
    STORE(r2, 4,SP);
    CALL(5,concat);  // concat("hello", " world")
    SET(r2, hello_world);
    STORE(r2, 4,SP);  // result is already there from concat
    CALL(5, tstrcmp);   // should be zero if we got "hello world"
    LOAD(r2, 3,SP);
    CMP(r2,r0);
    BZ(CONCAT_OK);
    INC(R1,1);
    LABEL(CONCAT_OK);

// NOW report the results

    FLAGS(R1);
    BNZ(blew_something);
    SET(R2, good);
    STORE(R2, 3,SP);
    CALL(4, print);
    BR(didnt_blow_anything);
    LABEL(blew_something);
    SET(R2, bad);
    STORE(R2, 3,SP);
    CALL(4, print);
    STORE(R1, 3,SP);
    CALL(4, printint);
    print_reg(R1);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);
    LABEL(didnt_blow_anything);

    // Finally, test I/O some more
    SET(R2, 0);
    STORE(R2, 3,SP);
    CALL(3, printint);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);

    SET(R2, 6);
    STORE(R2, 3,SP);
    CALL(3, printint);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);

    SET(R2, 12);
    STORE(R2, 3,SP);
    CALL(3, printint);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);

    SET(R2, 0x1234);
    STORE(R2, 3,SP);
    CALL(3, printint);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);

    SET(R2, 0xbeef);
    STORE(R2, 3,SP);
    CALL(3, printint);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);
    SET(R2, endl);
    STORE(R2, 3,SP);
    CALL(3, print);



    print("Malloc should fail after a few calls\n");
    LABEL(eatmemory);
    print("  calling malloc\n");
    SET(r1,0x1234);
    STORE(r1, 3,SP);
    CALL(4,malloc);
    LOAD(r1, 3,SP);
    print("  malloc gave: ");
    print_reg(r1);
    BR(eatmemory);


    HALT();  // just on general practice...

#include "Tiger-stdlib-stack.hera"
}
