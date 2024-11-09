#include "window_manager.h"
#include "networking.h"

#include "assert.h"

#include <iostream>

#define TEST_IP "127.0.0.1"
#define TEST_IP_HEX "0x7F000001"

using namespace splashkit_lib;

void run_encoding_decoding_tests()
{
    // Incomplete ip address tests
    assert(ipv4_to_hex("127.0.0") == "0x7F000000");
    assert(ipv4_to_str(ipv4_to_dec("127.0.0")) == "127.0.0.0");
    assert(ipv4_to_str(ipv4_to_dec("127,0.0")) == "127.0.0.0");
    assert(ipv4_to_str(ipv4_to_dec("")) == "0.0.0.0");

    int too_small_int = 4000;
    assert(ipv4_to_str(too_small_int) == "0.0.15.160");

    // Complete ip address tests
    assert(ipv4_to_str(ipv4_to_dec(TEST_IP)) == TEST_IP);
    assert(dec_to_hex(ipv4_to_dec(TEST_IP)) == TEST_IP_HEX);
    assert(ipv4_to_hex(TEST_IP) == TEST_IP_HEX);
    assert(hex_str_to_ipv4(TEST_IP_HEX) == TEST_IP);
    assert(my_ip() == "127.0.0.1");
}

void mac_to_hex_test()
{
    assert(mac_to_hex("00:00:00:00:00:00") == "0x000000000000");
    assert(mac_to_hex("FF:FF:FF:FF:FF:FF") == "0xFFFFFFFFFFFF");
    assert(mac_to_hex("12:34:56:78:9A:BC") == "0x123456789ABC");
    assert(mac_to_hex(TEST_MAC) == TEST_MAC_HEX);

    std::string result = mac_to_hex("AB:CD:EF:12:34:56");
    assert(result == "0xABCDEF123456");

    std::cout << "All MAC to Hexadecimal tests passed!\n"
              << "AB:CD:EF:12:34:56" << " in hex: " << result << std::endl;
}

void hex_to_mac_test()
{
    assert(hex_to_mac("0x000000000000") == "00:00:00:00:00:00");
    assert(hex_to_mac("0xFFFFFFFFFFFF") == "FF:FF:FF:FF:FF:FF");
    assert(hex_to_mac("0x123456789ABC") == "12:34:56:78:9A:BC");

    std::string result = hex_to_mac(TEST_MAC_HEX);
    assert(result == TEST_MAC);

    std::cout << "All Hexadecimal to MAC tests passed!\n"
              << TEST_MAC_HEX << " in MAC format: " << result << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

void run_networking_test()
{
    run_encoding_decoding_tests();
}