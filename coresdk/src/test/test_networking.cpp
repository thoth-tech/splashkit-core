#include "window_manager.h"
#include "networking.h"

#include "assert.h"

#include <iostream>

#define TEST_IP "127.0.0.1"
#define TEST_IP_HEX "0x7F000001"
#define TEST_MAC "00:00:00:00:00:00"
#define TEST_MAC_HEX "0x000000000000"

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
    assert(mac_to_hex("AB:CD:EF:12:34:56") == "0xABCDEF123456");
    assert(mac_to_hex(TEST_MAC) == TEST_MAC_HEX);

    std::string result = mac_to_hex("AB:CD:EF:12:34:56");
    assert(result == "0xABCDEF123456");

    // Additional positive tests
    assert(mac_to_hex("01:23:45:67:89:AB") == "0x0123456789AB");
    assert(mac_to_hex("DE:AD:BE:EF:00:01") == "0xDEADBEEF0001");

    // Negative tests
    assert(mac_to_hex("00:00:00:00:00:00") != "0xFFFFFFFFFFFF");
    assert(mac_to_hex("FF:FF:FF:FF:FF:FF") != "0x000000000000");
    assert(mac_to_hex("12:34:56:78:9A:BC") != "0xABCDEF123456");
    assert(mac_to_hex("AB:CD:EF:12:34:56") != "0x123456789ABC");

    // Additional negative tests
    assert(mac_to_hex("01:23:45:67:89:AB") != "0xDEADBEEF0001");
    assert(mac_to_hex("DE:AD:BE:EF:00:01") != "0x0123456789AB");

    // Tests for invalid types of MAC addresses
    assert(mac_to_hex("01:23:45:67:89") != "0x0123456789ABC");
    assert(mac_to_hex("01:23:45:AB") != "0x0123456789");
    assert(mac_to_hex("01:23:45:67:89:AB:CD") != "0x0123456789AB");
    assert(mac_to_hex("01:23:67:89:AB") != "0x0123456789ABCD");
    assert(mac_to_hex("0000") != "0x0000");


    std::cout << "All MAC to Hexadecimal tests passed!\n"
              << "AB:CD:EF:12:34:56" << " in hex: " << result << std::endl;
}

void hex_to_mac_test()
{
    assert(hex_to_mac("0x000000000000") == "00:00:00:00:00:00");
    assert(hex_to_mac("0xFFFFFFFFFFFF") == "FF:FF:FF:FF:FF:FF");
    assert(hex_to_mac("0x123456789ABC") == "12:34:56:78:9A:BC");
    assert(hex_to_mac("0xABCDEF123456") != "AB:CD:GF:12:34:56");
    assert(hex_to_mac("0xABCDEF123456") == "AB:CD:EF:12:34:56");

    std::string result = hex_to_mac(TEST_MAC_HEX);
    assert(result == TEST_MAC);

    // Additional positive tests
    assert(hex_to_mac("0x0123456789AB") == "01:23:45:67:89:AB");
    assert(hex_to_mac("0xDEADBEEF0001") == "DE:AD:BE:EF:00:01");

    // Negative tests
    assert(hex_to_mac("0x000000000000") != "FF:FF:FF:FF:FF:FF");
    assert(hex_to_mac("0xFFFFFFFFFFFF") != "00:00:00:00:00:00");
    assert(hex_to_mac("0x123456789ABC") != "AB:CD:EF:12:34:56");

    // Additional negative tests
    assert(hex_to_mac("0x0123456789AB") != "DE:AD:BE:EF:00:01");
    assert(hex_to_mac("0xDEADBEEF0001") != "01:23:45:67:89:AB");

    // Tests for invalid types of hex values
    assert(hex_to_mac("0x123456789AB") != "01:23:45:67:89:AB");
    assert(hex_to_mac("0x123456789ABCD") != "01:23:45:67:89:AB");
    assert(hex_to_mac("000000000000") != "00:00:00:00:00:00");

    std::cout << "All Hexadecimal to MAC tests passed!\n"
              << TEST_MAC_HEX << " in MAC format: " << result << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

void is_valid_mac()
{
    // Valid MAC addresses
    assert(is_valid_mac("00:00:00:00:00:00"));
    assert(is_valid_mac("FF:FF:FF:FF:FF:FF"));
    assert(is_valid_mac("12:34:56:78:9A:BC"));
    assert(is_valid_mac("AB:CD:EF:12:34:56"));
    assert(is_valid_mac("01:23:45:67:89:AB"));
    assert(is_valid_mac("DE:AD:BE:EF:00:01"));

    // Invalid MAC addresses - wrong length
    assert(!is_valid_mac("00:00:00:00:00:0"));
    assert(!is_valid_mac("FF:FF:FF:FF:FF:F"));
    assert(!is_valid_mac("12:34:56:78:9A:B"));
    assert(!is_valid_mac("AB:CD:EF:12:34:5"));
    assert(!is_valid_mac("AB:CD:EF:12:34:567"));
    assert(!is_valid_mac("AB:CD:EF:12:34"));

    // Invalid MAC addresses - invalid characters
    assert(!is_valid_mac("GG:00:00:00:00:00"));
    assert(!is_valid_mac("00:00:00:00:00:GG"));
    assert(!is_valid_mac("ZZ:ZZ:ZZ:ZZ:ZZ:ZZ"));
    assert(!is_valid_mac("12:34:56:78:9A:BG"));

    // Invalid MAC addresses - wrong format
    assert(!is_valid_mac("00-00-00-00-00-00"));
    assert(!is_valid_mac("0000.0000.0000"));
    assert(!is_valid_mac("000000000000"));
    assert(!is_valid_mac("00:00:00:00:00"));
    assert(!is_valid_mac("00:00:00:00:00:00:00"));

    std::cout << "All MAC address validation tests passed!\n";
}

void run_networking_test()
{
    run_encoding_decoding_tests();
    mac_to_hex_test();
    hex_to_mac_test();
}