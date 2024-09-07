#include "window_manager.h"
#include "networking.h"

#include "assert.h"

#include <iostream>

#define TEST_IP "127.0.0.1"
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
    assert(dec_to_hex(ipv4_to_dec(TEST_IP)) == "0x7F000001");
    assert(ipv4_to_hex(TEST_IP) == "0x7F000001");
    assert(hex_str_to_ipv4("0x7F000001") == TEST_IP);
    assert(my_ip() == TEST_IP);
}

void run_networking_test()
{
    run_encoding_decoding_tests();
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
