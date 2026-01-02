using static SplashKitSDK.SplashKit;
using System.Diagnostics;

const string TestIp = "127.0.0.1";
const string TestIpHex = "0x7F000001";
const string TestMac = "00:00:00:00:00:00";
const string TestMacHex = "0x000000000000";

void RunEncodingDecodingTests()
{
    // Incomplete ip address tests
    Trace.Assert(Ipv4ToHex("127.0.0") == "0x7F000000");
    Trace.Assert(DecToIpv4(Ipv4ToDec("127.0.0")) == "127.0.0.0");
    Trace.Assert(DecToIpv4(Ipv4ToDec("127,0.0")) == "127.0.0.0");
    Trace.Assert(DecToIpv4(Ipv4ToDec("")) == "0.0.0.0");

    uint tooSmallInt = 4000;
    Trace.Assert(DecToIpv4(tooSmallInt) == "0.0.15.160");

    // Complete ip address tests
    Trace.Assert(DecToIpv4(Ipv4ToDec(TestIp)) == TestIp);
    Trace.Assert(DecToHex(Ipv4ToDec(TestIp)) == TestIpHex);
    Trace.Assert(Ipv4ToHex(TestIp) == TestIpHex);
    Trace.Assert(HexStrToIpv4(TestIpHex) == TestIp);
    Trace.Assert(MyIP() == "127.0.0.1");

    Console.WriteLine("All encoding/decoding tests passed!\n");
}

void MacToHexTest()
{
    Trace.Assert(MacToHex("00:00:00:00:00:00") == "0x000000000000");
    Trace.Assert(MacToHex("FF:FF:FF:FF:FF:FF") == "0xFFFFFFFFFFFF");
    Trace.Assert(MacToHex("12:34:56:78:9A:BC") == "0x123456789ABC");
    Trace.Assert(MacToHex("AB:CD:EF:12:34:56") == "0xABCDEF123456");
    Trace.Assert(MacToHex(TestMac) == TestMacHex);

    string result = MacToHex("AB:CD:EF:12:34:56");
    Trace.Assert(result == "0xABCDEF123456");

    // Additional positive tests
    Trace.Assert(MacToHex("01:23:45:67:89:AB") == "0x0123456789AB");
    Trace.Assert(MacToHex("DE:AD:BE:EF:00:01") == "0xDEADBEEF0001");

    // Negative tests
    Trace.Assert(MacToHex("00:00:00:00:00:00") != "0xFFFFFFFFFFFF");
    Trace.Assert(MacToHex("FF:FF:FF:FF:FF:FF") != "0x000000000000");
    Trace.Assert(MacToHex("12:34:56:78:9A:BC") != "0xABCDEF123456");
    Trace.Assert(MacToHex("AB:CD:EF:12:34:56") != "0x123456789ABC");

    // Additional negative tests
    Trace.Assert(MacToHex("01:23:45:67:89:AB") != "0xDEADBEEF0001");
    Trace.Assert(MacToHex("DE:AD:BE:EF:00:01") != "0x0123456789AB");

    // Tests for invalid types of MAC addresses
    Console.WriteLine("Testing invalid MAC addresses (Expect ERROR):");
    Trace.Assert(MacToHex("01:23:45:67:89") != "0x0123456789ABC");
    Trace.Assert(MacToHex("01:23:45:AB") != "0x0123456789");
    Trace.Assert(MacToHex("01:23:45:67:89:AB:CD") != "0x0123456789AB");
    Trace.Assert(MacToHex("01:23:67:89:AB") != "0x0123456789ABCD");
    Trace.Assert(MacToHex("0000") == "");

    Console.WriteLine($"All MAC to Hexadecimal tests passed!\nAB:CD:EF:12:34:56 in hex: {result}\n");
}

void HexToMacTest()
{
    Trace.Assert(HexToMac("0x000000000000") == "00:00:00:00:00:00");
    Trace.Assert(HexToMac("0xFFFFFFFFFFFF") == "FF:FF:FF:FF:FF:FF");
    Trace.Assert(HexToMac("0x123456789ABC") == "12:34:56:78:9A:BC");
    Trace.Assert(HexToMac("0xABCDEF123456") != "AB:CD:GF:12:34:56");
    Trace.Assert(HexToMac("0xABCDEF123456") == "AB:CD:EF:12:34:56");

    string result = HexToMac(TestMacHex);
    Trace.Assert(result == TestMac);

    // Additional positive tests
    Trace.Assert(HexToMac("0x0123456789AB") == "01:23:45:67:89:AB");
    Trace.Assert(HexToMac("0xDEADBEEF0001") == "DE:AD:BE:EF:00:01");

    // Negative tests
    Trace.Assert(HexToMac("0x000000000000") != "FF:FF:FF:FF:FF:FF");
    Trace.Assert(HexToMac("0xFFFFFFFFFFFF") != "00:00:00:00:00:00");
    Trace.Assert(HexToMac("0x123456789ABC") != "AB:CD:EF:12:34:56");

    // Additional negative tests
    Trace.Assert(HexToMac("0x0123456789AB") != "DE:AD:BE:EF:00:01");
    Trace.Assert(HexToMac("0xDEADBEEF0001") != "01:23:45:67:89:AB");

    // Tests for invalid types of hex values
    Console.WriteLine("Testing invalid hex values (Expect ERROR):");
    Trace.Assert(HexToMac("0x123456789AB") != "01:23:45:67:89:AB");
    Trace.Assert(HexToMac("0x123456789ABCD") != "01:23:45:67:89:AB");
    Trace.Assert(HexToMac("000000000000") != "00:00:00:00:00:00");

    Console.WriteLine($"All Hexadecimal to MAC tests passed!\n{TestMacHex} in MAC format: {result}");
    Console.WriteLine("-------------------------------------\n");
}

void IsValidMacTest()
{
    // Valid MAC addresses
    Trace.Assert(IsValidMac("00:00:00:00:00:00"));
    Trace.Assert(IsValidMac("FF:FF:FF:FF:FF:FF"));
    Trace.Assert(IsValidMac("12:34:56:78:9A:BC"));
    Trace.Assert(IsValidMac("AB:CD:EF:12:34:56"));
    Trace.Assert(IsValidMac("01:23:45:67:89:AB"));
    Trace.Assert(IsValidMac("DE:AD:BE:EF:00:01"));

    // Invalid MAC addresses - wrong length
    Trace.Assert(!IsValidMac("00:00:00:00:00:0"));
    Trace.Assert(!IsValidMac("FF:FF:FF:FF:FF:F"));
    Trace.Assert(!IsValidMac("12:34:56:78:9A:B"));
    Trace.Assert(!IsValidMac("AB:CD:EF:12:34:5"));
    Trace.Assert(!IsValidMac("AB:CD:EF:12:34:567"));
    Trace.Assert(!IsValidMac("AB:CD:EF:12:34"));

    // Invalid MAC addresses - invalid characters
    Trace.Assert(!IsValidMac("GG:00:00:00:00:00"));
    Trace.Assert(!IsValidMac("00:00:00:00:00:GG"));
    Trace.Assert(!IsValidMac("ZZ:ZZ:ZZ:ZZ:ZZ:ZZ"));
    Trace.Assert(!IsValidMac("12:34:56:78:9A:BG"));

    // Invalid MAC addresses - wrong format
    Trace.Assert(!IsValidMac("00-00-00-00-00-00"));
    Trace.Assert(!IsValidMac("0000.0000.0000"));
    Trace.Assert(!IsValidMac("000000000000"));
    Trace.Assert(!IsValidMac("00:00:00:00:00"));
    Trace.Assert(!IsValidMac("00:00:00:00:00:00:00"));

    Console.WriteLine("All MAC address validation tests passed!\n");
}

RunEncodingDecodingTests();
MacToHexTest();
HexToMacTest();
IsValidMacTest();
