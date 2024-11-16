//
//  test_terminal.cpp
//  splashkit
//
//  Created by Andrew Cain on 8/10/16.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include "terminal.h"
#include "utils.h"
#include <iostream>
#include "basics.h"
#include "assert.h"

using namespace std;
using namespace splashkit_lib;

void test_simple_terminal()
{
    write_line("Hello World!" + to_string(10));
    write("Hello");
    write(" ");
    write("World!");
    write_line();
    write_line("Test character output");
    write('a');
    write_line();
    write_line('c');
    delay(1000);
}

void dec_to_bin()
{
    write_line("Testing decimal to binary conversion");
    assert(dec_to_bin(0) == "0");              // Zero
    assert(dec_to_bin(1) == "1");              // Single bit
    assert(dec_to_bin(2) == "10");             // Simple two-bit binary
    assert(dec_to_bin(10) == "1010");          // Example given in TEST_BIN
    assert(dec_to_bin(123) == "1111011");      // Arbitrary number
    assert(dec_to_bin(1025) == "10000000001"); // Mixed power of 2 with additional bit

    // Larger numbers
    assert(dec_to_bin(255) == "11111111");       // Maximum 8-bit value
    assert(dec_to_bin(256) == "100000000");      // Edge of 9 bits
    assert(dec_to_bin(1023) == "1111111111");    // Maximum 10-bit value
    assert(dec_to_bin(4096) == "1000000000000"); // Larger power of 2

    // Powers of 2
    assert(dec_to_bin(4) == "100");            // 2^2
    assert(dec_to_bin(8) == "1000");           // 2^3
    assert(dec_to_bin(16) == "10000");         // 2^4
    assert(dec_to_bin(1024) == "10000000000"); // 2^10

    // High values (32-bit boundaries)
    assert(dec_to_bin(4294967295) == "11111111111111111111111111111111"); // Max 32-bit unsigned int
    assert(dec_to_bin(2147483648) == "10000000000000000000000000000000"); // 2^31

    // Mixed bits
    assert(dec_to_bin(123) == "1111011");      // Arbitrary number
    assert(dec_to_bin(1025) == "10000000001"); // Mixed power of 2 with additional bit

    // Tests for inequality
    assert(dec_to_bin(0) != "1");
    assert(dec_to_bin(1) != "0");
    assert(dec_to_bin(2) != "11");
    assert(dec_to_bin(10) != "1011");
    assert(dec_to_bin(123) != "1111100");
    assert(dec_to_bin(1025) != "10000000010");
    assert(dec_to_bin(255) != "100000000");
    assert(dec_to_bin(256) != "100000001");
    assert(dec_to_bin(1023) != "10000000000");
    assert(dec_to_bin(4096) != "1000000000001");
    assert(dec_to_bin(4294967295) != "1000");
    assert(dec_to_bin(2147483648) != "1000001");
    assert(dec_to_bin(123456789) != "1110101");
    assert(dec_to_bin(1100) != "11110");

    int dec_input1 = 1330;
    string result1 = dec_to_bin(dec_input1);
    write_line("1023 in binary is " + result1);

    int dec_input2 = 293842;
    string result2 = dec_to_bin(dec_input2);
    write_line("293842 in binary is " + result2);

    write_line("All decimal to binary tests passed!");
    write_line("-------------------------------------");
}

void bin_to_dec()
{
    write_line("Testing binary to decimal conversion");
    assert(bin_to_dec("0") == 0);              // Single '0'
    assert(bin_to_dec("1") == 1);              // Single '1'
    assert(bin_to_dec("10") == 2);             // Simple binary for decimal 2
    assert(bin_to_dec("1010") == 10);          // Example given in TEST_BIN
    assert(bin_to_dec("1111011") == 123);      // Arbitrary number
    assert(bin_to_dec("10000000001") == 1025); // Mixed power of 2 with additional bit

    // Larger numbers
    assert(bin_to_dec("11111111") == 255);       // Maximum 8-bit binary
    assert(bin_to_dec("100000000") == 256);      // Edge of 9 bits
    assert(bin_to_dec("1111111111") == 1023);    // Maximum 10-bit binary
    assert(bin_to_dec("1000000000000") == 4096); // Larger power of 2

    // Powers of 2
    assert(bin_to_dec("100") == 4);            // 2^2
    assert(bin_to_dec("1000") == 8);           // 2^3
    assert(bin_to_dec("10000") == 16);         // 2^4
    assert(bin_to_dec("10000000000") == 1024); // 2^10

    // High values (32-bit boundaries)
    assert(bin_to_dec("11111111111111111111111111111111") == 4294967295); // Max 32-bit unsigned int
    assert(bin_to_dec("10000000000000000000000000000000") == 2147483648); // 2^31

    // Mixed bits
    assert(bin_to_dec("1111011") == 123);      // Arbitrary number
    assert(bin_to_dec("10000000001") == 1025); // Mixed power of 2 with additional bit

    // Tests for inequality
    assert(bin_to_dec("0") != 1);
    assert(bin_to_dec("1") != 0);
    assert(bin_to_dec("10") != 3);
    assert(bin_to_dec("1010") != 11);
    assert(bin_to_dec("1111011") != 124);
    assert(bin_to_dec("10000000001") != 1026);
    assert(bin_to_dec("11111111") != 256);
    assert(bin_to_dec("100000000") != 257);
    assert(bin_to_dec("1111111111") != 1024);
    assert(bin_to_dec("1000000000000") != 4097);
    assert(bin_to_dec("11111111111111111111111111111111") != 4294967296);
    assert(bin_to_dec("10000000000000000000000000000000") != 2147483649);
    assert(bin_to_dec("abcde") != 2147483647);
    assert(bin_to_dec("a1b2b3i4f02") != 1234);

    string bin_input1 = "1111011";
    int result1 = bin_to_dec(bin_input1);
    write_line("1111011 in decimal is " + to_string(result1));

    string bin_input2 = "10000000001";
    int result2 = bin_to_dec(bin_input2);
    write_line("10000000001 in decimal is " + to_string(result2));

    write_line("All binary to decimal tests passed!");
    write_line("-------------------------------------");
}

void hex_to_bin()
{
    write_line("Testing hexadecimal to binary conversion");
    assert(hex_to_bin("0") == "0");    // Single digit "0" in full 4-bit form
    assert(hex_to_bin("1") == "1");    // Single hex "1" in 4-bit form
    assert(hex_to_bin("A") == "1010"); // Single hex letter "A"
    assert(hex_to_bin("F") == "1111"); // Maximum single hex digit "F"

    // Multi-digit hexadecimal conversions
    assert(hex_to_bin("10") == "00010000");           // "10" in hex to full 8-bit binary
    assert(hex_to_bin("FF") == "11111111");           // Max 8-bit binary from hex "FF"
    assert(hex_to_bin("100") == "000100000000");      // "100" in hex to full 12-bit binary
    assert(hex_to_bin("ABC") == "101010111100");      // Arbitrary 3-digit hex value "ABC"
    assert(hex_to_bin("1234") == "0001001000110100"); // Mixed 16-bit hex "1234"

    // Larger hexadecimal values
    assert(hex_to_bin("FFFF") == "1111111111111111");                     // Max 16-bit binary from "FFFF"
    assert(hex_to_bin("FFFFFFFF") == "11111111111111111111111111111111"); // Max 32-bit from "FFFFFFFF"
    assert(hex_to_bin("1FFFFFF") == "0001111111111111111111111111");      // Full width for "1FFFFFF"
    assert(hex_to_bin("ABCDEF") == "101010111100110111101111");           // Arbitrary large hex "ABCDEF"

    // Tests for inequality
    assert(hex_to_bin("0") != "1");
    assert(hex_to_bin("1") != "0");
    assert(hex_to_bin("A") != "1011");
    assert(hex_to_bin("F") != "10000");
    assert(hex_to_bin("10") != "00010001");
    assert(hex_to_bin("FF") != "100000000");
    assert(hex_to_bin("100") != "000100000001");
    assert(hex_to_bin("ABC") != "101010111101");
    assert(hex_to_bin("1234") != "0001001000110101");
    assert(hex_to_bin("FFFF") != "11111111111111110");
    assert(hex_to_bin("FFFFFFFF") != "11111");
    assert(hex_to_bin("1FFFFFF") != "0111110");
    assert(hex_to_bin("GGGGGG") != "10101011110010");

    string hex_input1 = "ABCDEF";
    string result1 = hex_to_bin(hex_input1);
    write_line("ABCDEF in binary is " + result1);

    string hex_input2 = "1234";
    string result2 = hex_to_bin(hex_input2);
    write_line("1234 in binary is " + result2);

    write_line("All hexadecimal to binary tests passed!");
    write_line("-------------------------------------");
}

void bin_to_hex()
{
    write_line("Testing binary to hexadecimal conversion");
    assert(bin_to_hex("0") == "0");    // Single digit zero
    assert(bin_to_hex("1") == "1");    // Single digit one
    assert(bin_to_hex("1010") == "A"); // Four-bit binary to single hex
    assert(bin_to_hex("1111") == "F"); // Max four-bit binary

    // Multi-bit binary conversions
    assert(bin_to_hex("00010000") == "10");           // 8-bit binary with leading zeros for "10"
    assert(bin_to_hex("11111111") == "FF");           // 8-bit maximum value
    assert(bin_to_hex("000100000000") == "100");      // 12-bit binary with leading zeros for "100"
    assert(bin_to_hex("101010111100") == "ABC");      // Arbitrary binary sequence "ABC"
    assert(bin_to_hex("0001001000110100") == "1234"); // Full width for "1234"

    // Larger binary values
    assert(bin_to_hex("1111111111111111") == "FFFF");                     // 16-bit maximum value
    assert(bin_to_hex("11111111111111111111111111111111") == "FFFFFFFF"); // 32-bit max value
    assert(bin_to_hex("0001111111111111111111111111") == "1FFFFFF");      // Full width for "1FFFFFF"
    assert(bin_to_hex("101010111100110111101111") == "ABCDEF");           // Arbitrary large binary "ABCDEF"

    // Tests for inequality
    assert(bin_to_hex("0") != "1");
    assert(bin_to_hex("1") != "0");
    assert(bin_to_hex("1010") != "B");
    assert(bin_to_hex("1111") != "10");
    assert(bin_to_hex("00010000") != "11");
    assert(bin_to_hex("11111111") != "100");
    assert(bin_to_hex("000100000000") != "101");
    assert(bin_to_hex("101010111100") != "ABD");
    assert(bin_to_hex("0001001000110100") != "1235");
    assert(bin_to_hex("1111111111111111") != "FFFFF");
    assert(bin_to_hex("11111111111111111111111111111111") != "FFFFFFFFF");
    assert(bin_to_hex("00445566") != "1FFFFF");
    assert(bin_to_hex("1abc332") != "ABCDEG");

    string bin_input1 = "1010101111001111";
    string result1 = bin_to_hex(bin_input1);
    write_line("1010101111001111 in hexadecimal is " + result1);

    string bin_input2 = "0001001000110100";
    string result2 = bin_to_hex(bin_input2);
    write_line("0001001000110100 in hexadecimal is " + result2);

    write_line("All binary to hexadecimal tests passed!");
}

void test_dec_to_oct()
{
    write_line("Testing decimal to octal conversion");
    assert(dec_to_oct(0) == "0");     // Zero case
    assert(dec_to_oct(8) == "10");    // Single-digit octal result
    assert(dec_to_oct(64) == "100");  // Powers of 8
    assert(dec_to_oct(123) == "173"); // Arbitrary decimal to octal
    assert(dec_to_oct(255) == "377"); // Max 8-bit value
    // Larger numbers
    assert(dec_to_oct(1023) == "1777"); // Larger decimal to octal
    assert(dec_to_oct(4095) == "7777"); // Edge of 12-bit value

    // Powers of 8
    assert(dec_to_oct(8) == "10");     // 8 = 8^1
    assert(dec_to_oct(64) == "100");   // 64 = 8^2
    assert(dec_to_oct(512) == "1000"); // 512 = 8^3

    // High values (32-bit boundaries)
    assert(dec_to_oct(4294967295) == "37777777777"); // Max 32-bit unsigned int
    assert(dec_to_oct(2147483648) == "20000000000"); // 2^31

    // Mixed bits
    assert(dec_to_oct(123) == "173");   // Arbitrary decimal to octal
    assert(dec_to_oct(1025) == "2001"); // Mixed power of 8 with additional bit

    // Tests for inequality
    assert(dec_to_oct(0) != "1");
    assert(dec_to_oct(1) != "0");
    assert(dec_to_oct(8) != "11");
    assert(dec_to_oct(64) != "101");
    assert(dec_to_oct(123) != "174");
    assert(dec_to_oct(255) != "400");
    assert(dec_to_oct(1023) != "2000");
    assert(dec_to_oct(4095) != "8000");
    assert(dec_to_oct(4294967295) != "40000000000");
    assert(dec_to_oct(2147483648) != "20000000001");
    assert(dec_to_oct(123456789) != "1234567");
    assert(dec_to_oct(987654321) != "7654321");

    int dec_input1 = 1330;
    string result1 = dec_to_oct(dec_input1);
    write_line("1330 in octal is " + result1);

    int dec_input2 = 293842;
    string result2 = dec_to_oct(dec_input2);
    write_line("293842 in octal is " + result2);

    write_line("All decimal to octal tests passed!");
    write_line("-------------------------------------");
}
void test_oct_to_dec()
{
    write_line("Testing octal to decimal conversion");
    assert(oct_to_dec("0") == 0);     // Zero case
    assert(oct_to_dec("10") == 8);    // Single-digit decimal result
    assert(oct_to_dec("100") == 64);  // Powers of 8
    assert(oct_to_dec("173") == 123); // Arbitrary octal to decimal
    assert(oct_to_dec("377") == 255); // Max 8-bit value

    assert(oct_to_dec("1777") == 1023); // Larger octal to decimal
    assert(oct_to_dec("7777") == 4095); // Edge of 12-bit value

    // Powers of 8
    assert(oct_to_dec("10") == 8);     // 8 = 8^1
    assert(oct_to_dec("100") == 64);   // 64 = 8^2
    assert(oct_to_dec("1000") == 512); // 512 = 8^3

    // High values (32-bit boundaries)
    assert(oct_to_dec("37777777777") == 4294967295); // Max 32-bit unsigned int
    assert(oct_to_dec("20000000000") == 2147483648); // 2^31

    // Mixed bits
    assert(oct_to_dec("173") == 123);   // Arbitrary octal to decimal
    assert(oct_to_dec("2001") == 1025); // Mixed power of 8 with additional bit

    // Tests for inequality
    assert(oct_to_dec("0") != 1);
    assert(oct_to_dec("1") != 0);
    assert(oct_to_dec("10") != 9);
    assert(oct_to_dec("100") != 65);
    assert(oct_to_dec("173") != 124);
    assert(oct_to_dec("377") != 256);
    assert(oct_to_dec("1777") != 1024);
    assert(oct_to_dec("7777") != 4096);
    assert(oct_to_dec("37777777777") != 4294967296);
    assert(oct_to_dec("20000000000") != 2147483649);
    assert(oct_to_dec("abcde") != 2147483647);
    assert(oct_to_dec("a1b2b3i4f02") != 1234);

    string oct_input1 = "173";
    int result1 = oct_to_dec(oct_input1);
    write_line("173 in decimal is " + to_string(result1));

    string oct_input2 = "2001";
    int result2 = oct_to_dec(oct_input2);
    write_line("2001 in decimal is " + to_string(result2));

    write_line("All octal to decimal tests passed!");
    write_line("-------------------------------------");
}

void test_octal_to_bin() {
    write_line("Testing octal to binary conversion");
    assert(oct_to_bin("0") == "0");    // Zero case
    assert(oct_to_bin("1") == "1");    // Single digit
    assert(oct_to_bin("7") == "111");  // Max single octal digit
    assert(oct_to_bin("10") == "1000"); // Power of 8

    // Multi-digit octal numbers
    assert(oct_to_bin("12") == "1010");   // Simple two-digit octal
    assert(oct_to_bin("77") == "111111"); // Two max digits
    assert(oct_to_bin("100") == "1000000"); // Three digits
    assert(oct_to_bin("173") == "1111011"); // Arbitrary octal number
    
    // Larger octal values
    assert(oct_to_bin("777") == "111111111");     // Max 9-bit value
    assert(oct_to_bin("1000") == "1000000000");   // Power of 8
    assert(oct_to_bin("7777") == "111111111111"); // Max 12-bit value
    
    // Edge cases and boundary values
    assert(oct_to_bin("37777777777") == "11111111111111111111111111111111"); // Max 32-bit 
    assert(oct_to_bin("20000000000") == "10000000000000000000000000000000"); // 2^31

    // Tests for inequality
    assert(oct_to_bin("0") != "1");
    assert(oct_to_bin("1") != "0");
    assert(oct_to_bin("7") != "1000");
    assert(oct_to_bin("10") != "11");
    assert(oct_to_bin("12") != "1011");
    assert(oct_to_bin("77") != "1000000");
    assert(oct_to_bin("100") != "1000001");
    assert(oct_to_bin("173") != "1111100");
    assert(oct_to_bin("777") != "1000000000");
    assert(oct_to_bin("1000") != "1000000001");
    assert(oct_to_bin("7777") != "100000000000");
    assert(oct_to_bin("37777777777") != "10000000000000");
    assert(oct_to_bin("205550000") != "1000000001");

    string oct_input1 = "173";
    string result1 = oct_to_bin(oct_input1);
    write_line("173 in binary is " + result1);

    string oct_input2 = "7777";
    string result2 = oct_to_bin(oct_input2);
    write_line("7777 in binary is " + result2);

    write_line("All octal to binary tests passed!");
    write_line("-------------------------------------");
}

void test_bin_to_oct() {
    write_line("Testing binary to octal conversion");
    assert(bin_to_oct("0") == "0");    // Zero case
    assert(bin_to_oct("1") == "1");    // Single bit
    assert(bin_to_oct("111") == "7");  // Three bits to single octal
    assert(bin_to_oct("1000") == "10"); // Power of 2 to octal

    // Multi-bit binary numbers
    assert(bin_to_oct("1010") == "12");     // Simple 4-bit binary
    assert(bin_to_oct("111111") == "77");   // Six bits to two octals
    assert(bin_to_oct("1000000") == "100"); // Seven bits to three octals
    assert(bin_to_oct("1111011") == "173"); // Arbitrary binary number

    // Larger binary values
    assert(bin_to_oct("111111111") == "777");       // Nine bits
    assert(bin_to_oct("1000000000") == "1000");     // Ten bits
    assert(bin_to_oct("111111111111") == "7777");   // Twelve bits

    // Edge cases and boundary values
    assert(bin_to_oct("11111111111111111111111111111111") == "37777777777"); // Max 32-bit
    assert(bin_to_oct("10000000000000000000000000000000") == "20000000000"); // 2^31

    // Tests for inequality
    assert(bin_to_oct("0") != "1");
    assert(bin_to_oct("1") != "0");
    assert(bin_to_oct("111") != "8");
    assert(bin_to_oct("1000") != "11");
    assert(bin_to_oct("1010") != "13");
    assert(bin_to_oct("111111") != "100");
    assert(bin_to_oct("1000000") != "101");
    assert(bin_to_oct("1111011") != "174");
    assert(bin_to_oct("111111111") != "1000");
    assert(bin_to_oct("1000000000") != "1001");
    assert(bin_to_oct("111111111111") != "10000");
    assert(bin_to_oct("1error1") != "40000000000");
    assert(bin_to_oct("1000077770") != "400097979701");

    string bin_input1 = "1111011";
    string result1 = bin_to_oct(bin_input1);
    write_line("1111011 in octal is " + result1);

    string bin_input2 = "111111111111";
    string result2 = bin_to_oct(bin_input2);
    write_line("111111111111 in octal is " + result2);

    write_line("All binary to octal tests passed!");
    write_line("-------------------------------------");
}

void test_oct_to_hex()
{
    write_line("Testing octal to hexadecimal conversion");
    assert(oct_to_hex("0") == "0");    // Zero case
    assert(oct_to_hex("1") == "1");    // Single digit
    assert(oct_to_hex("7") == "7");    // Max single octal digit
    assert(oct_to_hex("10") == "8");   // Power of 8

    // Multi-digit octal numbers
    assert(oct_to_hex("12") == "A");     // Simple two-digit octal
    assert(oct_to_hex("77") == "3F");    // Two max digits
    assert(oct_to_hex("100") == "40");   // Three digits
    assert(oct_to_hex("173") == "7B");   // Arbitrary octal number

    // Larger octal values
    assert(oct_to_hex("777") == "1FF");       // Max 9-bit value
    assert(oct_to_hex("1000") == "200");      // Power of 8 
    assert(oct_to_hex("7777") == "FFF");      // Max 12-bit value
    assert(oct_to_hex("77777") == "7FFF");    // Max 16-bit value

    // Edge cases and boundary values  
    assert(oct_to_hex("37777777777") == "FFFFFFFF"); // Max 32-bit
    assert(oct_to_hex("20000000000") == "80000000"); // 2^31

    // Tests for inequality
    assert(oct_to_hex("0") != "1");
    assert(oct_to_hex("1") != "0");
    assert(oct_to_hex("7") != "8");
    assert(oct_to_hex("10") != "9");
    assert(oct_to_hex("12") != "B");
    assert(oct_to_hex("77") != "40");
    assert(oct_to_hex("100") != "41");
    assert(oct_to_hex("173") != "7C");
    assert(oct_to_hex("777") != "200");
    assert(oct_to_hex("1000") != "201");
    assert(oct_to_hex("7777") != "1000");
    assert(oct_to_hex("77777") != "8000");
    assert(oct_to_hex("37777777777") != "FFFFFFFFF");
    assert(oct_to_hex("20000000000") != "80000001");
    assert(oct_to_hex("abcde") != "80000000");
    assert(oct_to_hex("a1b2b3i4f02") != "80000001");

    string oct_input1 = "173";
    string result1 = oct_to_hex(oct_input1);
    write_line("173 in hexadecimal is " + result1);

    string oct_input2 = "7777"; 
    string result2 = oct_to_hex(oct_input2);
    write_line("7777 in hexadecimal is " + result2);

    write_line("All octal to hexadecimal tests passed!");
    write_line("-------------------------------------");
}

void test_hex_to_oct() 
{
    write_line("Testing hexadecimal to octal conversion");
    assert(hex_to_oct("0") == "0");    // Zero case
    assert(hex_to_oct("1") == "1");    // Single digit  
    assert(hex_to_oct("7") == "7");    // Single hex digit
    assert(hex_to_oct("8") == "10");   // Octal two digits

    // Multi-digit hex numbers
    assert(hex_to_oct("A") == "12");     // Single letter hex
    assert(hex_to_oct("3F") == "77");    // Two hex digits
    assert(hex_to_oct("40") == "100");   // Power of 16
    assert(hex_to_oct("7B") == "173");   // Arbitrary hex number

    // Larger hex values
    assert(hex_to_oct("1FF") == "777");       // Three hex digits
    assert(hex_to_oct("200") == "1000");      // Power of 16
    assert(hex_to_oct("FFF") == "7777");      // Three F's
    assert(hex_to_oct("FFFF") == "177777");   // Four F's 

    // Edge cases and boundary values
    assert(hex_to_oct("FFFFFFFF") == "37777777777"); // Max 32-bit
    assert(hex_to_oct("80000000") == "20000000000"); // 2^31

    // Tests for inequality
    assert(hex_to_oct("0") != "1");
    assert(hex_to_oct("1") != "0");
    assert(hex_to_oct("7") != "8");
    assert(hex_to_oct("8") != "11");
    assert(hex_to_oct("A") != "13");
    assert(hex_to_oct("3F") != "100");
    assert(hex_to_oct("40") != "101");
    assert(hex_to_oct("7B") != "174");
    assert(hex_to_oct("1FF") != "1000");
    assert(hex_to_oct("200") != "1001");
    assert(hex_to_oct("FFF") != "10000");
    assert(hex_to_oct("FFFF") != "200000");
    assert(hex_to_oct("FFFFFFFF") != "40000000000");
    assert(hex_to_oct("80000000") != "40000000001");
    assert(hex_to_oct("abcde") != "40000000000");
    assert(hex_to_oct("a1b2b3i4f02") != "40000000001");

    string hex_input1 = "7B";
    string result1 = hex_to_oct(hex_input1);
    write_line("7B in octal is " + result1);

    string hex_input2 = "FFF";
    string result2 = hex_to_oct(hex_input2); 
    write_line("FFF in octal is " + result2);

    write_line("All hexadecimal to octal tests passed!");
    write_line("-------------------------------------");
}

void test_base64_encode()
{
    write_line("Testing Base64 encoding");
    assert(base64_encode("Man") == "TWFu");                                          // Standard example
    assert(base64_encode("M") == "TQ==");                                            // Single character with padding
    assert(base64_encode("Ma") == "TWE=");                                           // Two characters with padding
    assert(base64_encode("any carnal pleasure.") == "YW55IGNhcm5hbCBwbGVhc3VyZS4="); // Sentence with punctuation

    // Edge cases
    assert(base64_encode("") == "");        // Empty string
    assert(base64_encode("A") == "QQ==");   // Single character
    assert(base64_encode("BC") == "QkM=");  // Two characters
    assert(base64_encode("yes") == "eWVz"); // Three characters, no padding needed

    // Larger strings
    assert(base64_encode("Hello, World!") == "SGVsbG8sIFdvcmxkIQ==");                                                                       // Common phrase
    assert(base64_encode("The quick brown fox jumps over the lazy dog") == "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw=="); // Long sentence

    string input1 = "Hello, World!";
    string result1 = base64_encode(input1);
    write_line("Hello, World! in Base64 is " + result1);

    string input2 = "The quick brown fox jumps over the lazy dog";
    string result2 = base64_encode(input2);
    write_line("The quick brown fox jumps over the lazy dog in Base64 is " + result2);

    write_line("All Base64 encoding tests passed!");
    write_line("-------------------------------------");
}

void test_base64_decode()
{
    write_line("Testing Base64 decoding");
    assert(base64_decode("TWFu") == "Man");                                          // Standard example
    assert(base64_decode("TQ==") == "M");                                            // Single character with padding
    assert(base64_decode("TWE=") == "Ma");                                           // Two characters with padding
    assert(base64_decode("YW55IGNhcm5hbCBwbGVhc3VyZS4=") == "any carnal pleasure."); // Sentence with punctuation

    // Edge cases
    assert(base64_decode("") == "");        // Empty string
    assert(base64_decode("QQ==") == "A");   // Single character
    assert(base64_decode("QkM=") == "BC");  // Two characters
    assert(base64_decode("eWVz") == "yes"); // Three characters, no padding

    // Larger strings
    assert(base64_decode("SGVsbG8sIFdvcmxkIQ==") == "Hello, World!");                                                                       // Common phrase
    assert(base64_decode("VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw==") == "The quick brown fox jumps over the lazy dog"); // Long sentence

    string input1 = "SGVsbG8sIFdvcmxkIQ==";
    string result1 = base64_decode(input1);
    write_line("SGVsbG8sIFdvcmxkIQ== in Base64 is " + result1);

    string input2 = "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw==";
    string result2 = base64_decode(input2);
    write_line("VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw== in Base64 is " + result2);

    write_line("All Base64 decoding tests passed!");
    write_line("-------------------------------------");
}

void run_terminal_test()
{
    test_simple_terminal();
    dec_to_bin();
    bin_to_dec();
    hex_to_bin();
    bin_to_hex();
    test_dec_to_oct();
    test_oct_to_dec();
    test_octal_to_bin();
    test_bin_to_oct();
    test_oct_to_hex();
    test_hex_to_oct();
    test_base64_encode();
    test_base64_decode();    
}
