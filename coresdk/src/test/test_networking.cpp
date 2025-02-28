#include "window_manager.h"
#include "networking.h"

#include "assert.h"

#include <iostream>

#define TEST_IP "127.0.0.1"
#define TEST_IP_HEX "0x7F000001"
#define TEST_IP_DEC 2130706433  // Decimal representation of 127.0.0.1

using namespace splashkit_lib;

void run_encoding_decoding_tests()
{
    // Incomplete ip address tests
    assert(ipv4_to_hex("127.0.0") == "0x7F000000");
    assert(dec_to_ipv4(ipv4_to_dec("127.0.0")) == "127.0.0.0");
    assert(dec_to_ipv4(ipv4_to_dec("127,0.0")) == "127.0.0.0");
    assert(dec_to_ipv4(ipv4_to_dec("")) == "0.0.0.0");


    // Complete ip address tests
    assert(dec_to_ipv4(ipv4_to_dec(TEST_IP)) == TEST_IP);
    assert(dec_to_hex(ipv4_to_dec(TEST_IP)) == TEST_IP_HEX);
    assert(ipv4_to_hex(TEST_IP) == TEST_IP_HEX);
    assert(hex_str_to_ipv4(TEST_IP_HEX) == TEST_IP);
    assert(my_ip() == TEST_IP);
}

void is_valid_ip_test()
{
    // Valid IP addresses - should return true
    assert(is_valid_ipv4("192.168.1.1") == true);     // Common private IP
    assert(is_valid_ipv4("127.0.0.1") == true);       // Localhost
    assert(is_valid_ipv4("8.8.8.8") == true);         // Google DNS
    assert(is_valid_ipv4("255.255.255.255") == true); // Maximum values
    assert(is_valid_ipv4("0.0.0.0") == true);         // Minimum values
    assert(is_valid_ipv4("172.16.254.1") == true);    // Valid class B private IP
    assert(is_valid_ipv4("10.0.0.1") == true);        // Valid class A private IP

    // Invalid IP addresses - should return false
    assert(is_valid_ipv4("256.1.2.3") == false);       // First octet > 255
    assert(is_valid_ipv4("1.256.2.3") == false);       // Second octet > 255
    assert(is_valid_ipv4("1.2.256.3") == false);       // Third octet > 255
    assert(is_valid_ipv4("1.2.3.256") == false);       // Fourth octet > 255
    assert(is_valid_ipv4("192.168.1") == false);       // Missing octet
    assert(is_valid_ipv4("192.168.1.1.1") == false);   // Too many octets
    assert(is_valid_ipv4("192.168.1.") == false);      // Trailing dot
    assert(is_valid_ipv4(".192.168.1") == false);      // Leading dot
    assert(is_valid_ipv4("192.168..1") == false);      // Empty octet
    assert(is_valid_ipv4("192.168.1.1a") == false);    // Invalid character
    assert(is_valid_ipv4("") == false);                // Empty string
    assert(is_valid_ipv4("abc.def.ghi.jkl") == false); // Letters
    assert(is_valid_ipv4("192,168,1,1") == false);     // Wrong separator

    std::cout << "All IP validation tests passed!" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

void run_networking_test()
{
    run_encoding_decoding_tests();
    is_valid_ip_test();
    std::cout << "Starting decimal to IPv4 and IPv4 to decimal tests" << std::endl;

    // Testing decimal to IPv4 conversion
    int decimal_ip = TEST_IP_DEC;
    std::string converted_ip = dec_to_ipv4(decimal_ip);
    std::cout << "Decimal " << decimal_ip << " converted to IPv4: " << converted_ip << std::endl;
    assert(converted_ip == TEST_IP);

    // Testing IPv4 to decimal conversion
    std::string ip_address = TEST_IP;
    int converted_decimal = ipv4_to_dec(ip_address);
    std::cout << "IPv4 " << ip_address << " converted to decimal: " << converted_decimal << std::endl;
    assert(converted_decimal == decimal_ip);

    std::string test_ip_2 = "10.0.0.1";
    int dec_ip_2 = ipv4_to_dec(test_ip_2);
    std::cout << "IPv4 " << test_ip_2 << " converted to decimal: " << dec_ip_2 << std::endl;

    std::string result_ip_2 = dec_to_ipv4(dec_ip_2);
    std::cout << "Decimal " << dec_ip_2 << " converted back to IPv4: " << result_ip_2 << std::endl;
    assert(result_ip_2 == test_ip_2);
}
