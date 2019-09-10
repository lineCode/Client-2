// testutility.cpp
//

///// Includes /////

#include <boost/asio/ip/address_v4.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <gtest/gtest.h>
#include <random>
#include <thread>
#include <utility/utility.hpp>

///// Functions /////

GTEST_API_ int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Functions /////

bool InsertSortedPred(const int lhs, const int rhs)
{
  return (lhs < rhs);
}

///// Tests /////

TEST(Utility, Intersects)
{
  ASSERT_FALSE(Intersects(std::vector<int>(), std::vector<int>()));
  ASSERT_FALSE(Intersects(std::vector<int>(), std::vector<uint16_t>()));
  ASSERT_FALSE(Intersects(std::vector<int>({ 0, 1, 2 }), std::vector<uint16_t>({ 3, 4, 5 })));
  ASSERT_TRUE(Intersects(std::vector<int>({ 0, 1, 2 }), std::vector<uint16_t>({ 2, 3, 4 })));
  ASSERT_TRUE(Intersects(std::vector<int>({ 0, 1, 2 }), std::vector<uint16_t>({ 2 })));
}

TEST(Utility, Unique)
{
  ASSERT_TRUE(Unique(std::vector<int>({ 1, 2 }), [](const int lhs, const int rhs){ return (lhs == rhs); }));
  ASSERT_FALSE(Unique(std::vector<int>({ 1, 2, 1 }), [](const int lhs, const int rhs) { return (lhs == rhs); }));
}

TEST(Utility, SubnetMask)
{
  ASSERT_TRUE(IsValidSubnetMask(boost::asio::ip::address_v4::from_string("255.0.0.0").to_ulong()));
  ASSERT_TRUE(IsValidSubnetMask(boost::asio::ip::address_v4::from_string("255.255.0.0").to_ulong()));
  ASSERT_TRUE(IsValidSubnetMask(boost::asio::ip::address_v4::from_string("255.255.255.0").to_ulong()));
  ASSERT_FALSE(IsValidSubnetMask(99999));
}

TEST(Utility, SubnetMaskToPrefix)
{
  ASSERT_EQ(0, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("0.0.0.0").to_ulong()));
  ASSERT_EQ(1, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("128.0.0.0").to_ulong()));
  ASSERT_EQ(2, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("192.0.0.0").to_ulong()));
  ASSERT_EQ(3, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("224.0.0.0").to_ulong()));
  ASSERT_EQ(4, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("240.0.0.0").to_ulong()));
  ASSERT_EQ(5, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("248.0.0.0").to_ulong()));
  ASSERT_EQ(6, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("252.0.0.0").to_ulong()));
  ASSERT_EQ(7, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("254.0.0.0").to_ulong()));
  ASSERT_EQ(8, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.0.0.0").to_ulong()));
  ASSERT_EQ(9, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.128.0.0").to_ulong()));
  ASSERT_EQ(10, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.192.0.0").to_ulong()));
  ASSERT_EQ(11, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.224.0.0").to_ulong()));
  ASSERT_EQ(12, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.240.0.0").to_ulong()));
  ASSERT_EQ(13, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.248.0.0").to_ulong()));
  ASSERT_EQ(14, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.252.0.0").to_ulong()));
  ASSERT_EQ(15, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.254.0.0").to_ulong()));
  ASSERT_EQ(16, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.0.0").to_ulong()));
  ASSERT_EQ(17, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.128.0").to_ulong()));
  ASSERT_EQ(18, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.192.0").to_ulong()));
  ASSERT_EQ(19, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.224.0").to_ulong()));
  ASSERT_EQ(20, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.240.0").to_ulong()));
  ASSERT_EQ(21, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.248.0").to_ulong()));
  ASSERT_EQ(22, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.252.0").to_ulong()));
  ASSERT_EQ(23, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.254.0").to_ulong()));
  ASSERT_EQ(24, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.0").to_ulong()));
  ASSERT_EQ(25, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.128").to_ulong()));
  ASSERT_EQ(26, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.192").to_ulong()));
  ASSERT_EQ(27, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.224").to_ulong()));
  ASSERT_EQ(28, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.240").to_ulong()));
  ASSERT_EQ(29, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.248").to_ulong()));
  ASSERT_EQ(30, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.252").to_ulong()));
  ASSERT_EQ(31, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.254").to_ulong()));
  ASSERT_EQ(32, SubnetMaskToPrefix(boost::asio::ip::address_v4::from_string("255.255.255.255").to_ulong()));
}

TEST(Utility, PrefixToSubnetMask)
{
  ASSERT_EQ(std::string(), PrefixToSubnetMask(-1));
  ASSERT_EQ(std::string("0.0.0.0"), PrefixToSubnetMask(0));
  ASSERT_EQ(std::string("128.0.0.0"), PrefixToSubnetMask(1));
  ASSERT_EQ(std::string("192.0.0.0"), PrefixToSubnetMask(2));
  ASSERT_EQ(std::string("224.0.0.0"), PrefixToSubnetMask(3));
  ASSERT_EQ(std::string("240.0.0.0"), PrefixToSubnetMask(4));
  ASSERT_EQ(std::string("248.0.0.0"), PrefixToSubnetMask(5));
  ASSERT_EQ(std::string("252.0.0.0"), PrefixToSubnetMask(6));
  ASSERT_EQ(std::string("254.0.0.0"), PrefixToSubnetMask(7));
  ASSERT_EQ(std::string("255.0.0.0"), PrefixToSubnetMask(8));
  ASSERT_EQ(std::string("255.128.0.0"), PrefixToSubnetMask(9));
  ASSERT_EQ(std::string("255.192.0.0"), PrefixToSubnetMask(10));
  ASSERT_EQ(std::string("255.224.0.0"), PrefixToSubnetMask(11));
  ASSERT_EQ(std::string("255.240.0.0"), PrefixToSubnetMask(12));
  ASSERT_EQ(std::string("255.248.0.0"), PrefixToSubnetMask(13));
  ASSERT_EQ(std::string("255.252.0.0"), PrefixToSubnetMask(14));
  ASSERT_EQ(std::string("255.254.0.0"), PrefixToSubnetMask(15));
  ASSERT_EQ(std::string("255.255.0.0"), PrefixToSubnetMask(16));
  ASSERT_EQ(std::string("255.255.128.0"), PrefixToSubnetMask(17));
  ASSERT_EQ(std::string("255.255.192.0"), PrefixToSubnetMask(18));
  ASSERT_EQ(std::string("255.255.224.0"), PrefixToSubnetMask(19));
  ASSERT_EQ(std::string("255.255.240.0"), PrefixToSubnetMask(20));
  ASSERT_EQ(std::string("255.255.248.0"), PrefixToSubnetMask(21));
  ASSERT_EQ(std::string("255.255.252.0"), PrefixToSubnetMask(22));
  ASSERT_EQ(std::string("255.255.254.0"), PrefixToSubnetMask(23));
  ASSERT_EQ(std::string("255.255.255.0"), PrefixToSubnetMask(24));
  ASSERT_EQ(std::string("255.255.255.128"), PrefixToSubnetMask(25));
  ASSERT_EQ(std::string("255.255.255.192"), PrefixToSubnetMask(26));
  ASSERT_EQ(std::string("255.255.255.224"), PrefixToSubnetMask(27));
  ASSERT_EQ(std::string("255.255.255.240"), PrefixToSubnetMask(28));
  ASSERT_EQ(std::string("255.255.255.248"), PrefixToSubnetMask(29));
  ASSERT_EQ(std::string("255.255.255.252"), PrefixToSubnetMask(30));
  ASSERT_EQ(std::string("255.255.255.254"), PrefixToSubnetMask(31));
  ASSERT_EQ(std::string("255.255.255.255"), PrefixToSubnetMask(32));
  ASSERT_EQ(std::string(), PrefixToSubnetMask(33));
}

TEST(Utility, IsIPV4InRange)
{
  ASSERT_TRUE(IsIPV4InRange(boost::asio::ip::address_v4::from_string("192.168.1.1").to_ulong(), boost::asio::ip::address_v4::from_string("192.168.1.0").to_ulong(), boost::asio::ip::address_v4::from_string("255.255.255.0").to_ulong()));
  ASSERT_FALSE(IsIPV4InRange(boost::asio::ip::address_v4::from_string("192.168.1.1").to_ulong(), boost::asio::ip::address_v4::from_string("192.168.1.2").to_ulong(), boost::asio::ip::address_v4::from_string("255.255.255.255").to_ulong()));
  ASSERT_FALSE(IsIPV4InRange(boost::asio::ip::address_v4::from_string("192.168.1.1").to_ulong(), boost::asio::ip::address_v4::from_string("192.168.2.2").to_ulong(), boost::asio::ip::address_v4::from_string("255.255.255.0").to_ulong()));
  ASSERT_TRUE(IsIPV4InRange(boost::asio::ip::address_v4::from_string("169.254.4.177").to_ulong(), boost::asio::ip::address_v4::from_string("169.254.0.0").to_ulong(), boost::asio::ip::address_v4::from_string("255.255.0.0").to_ulong()));
  ASSERT_FALSE(IsIPV4InRange(boost::asio::ip::address_v4::from_string("169.253.4.177").to_ulong(), boost::asio::ip::address_v4::from_string("169.254.0.0").to_ulong(), boost::asio::ip::address_v4::from_string("255.255.0.0").to_ulong()));
}

TEST(Utility, Contains)
{
  const std::vector<std::string> strings =
  {
    "Hello123",
    "GoodbyeABC",
    "GoAway"
  };

  ASSERT_TRUE(Contains(strings, std::string("GoAway")));
  ASSERT_FALSE(Contains(strings, std::string("Nooooo")));
  ASSERT_TRUE(Contains(strings, { std::string("Hello123"), std::string("GoAway") }));
  ASSERT_FALSE(Contains(strings, { std::string("Hello123"), std::string("Nooooo") }));
}

TEST(Utility, Split)
{
  ASSERT_EQ(std::vector< std::vector<char> >({ { 'a' },{ 'a' },{ 'a' },{ 'a' } }), Split<char>({ 'a', 'b', 'c', 'a', 'b', 'c', 'a', 'b', 'c', 'a' }, { 'b', 'c' }));
  ASSERT_EQ(std::vector< std::vector<char> >({ { 'a' },{ 'c', 'a' },{ 'c', 'a' },{ 'c', 'a' } }), Split<char>({ 'a', 'b', 'c', 'a', 'b', 'c', 'a', 'b', 'c', 'a' }, { 'b' }));
  ASSERT_EQ(std::vector< std::vector<int> >({ { 1, 2, 3 },{ 2, 3 },{ 2, 3 },{ 2, 3, 4 } }), Split<int>({ 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4 }, { 4, 1 }));
  ASSERT_EQ(std::vector< std::vector<int> >({ { 1, 2, 3 },{ 2, 3 },{ 2, 3 },{ 2, 3 } }), Split<int>({ 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1 }, { 4, 1 }));
}

TEST(Utility, InsertSorted)
{
  std::vector<int> values = { 0, 1, 2, 3 };
  InsertSorted(values, 4, InsertSortedPred);
  ASSERT_EQ(std::vector<int>({ 0, 1, 2, 3, 4 }), values);
  InsertSorted(values, -1, InsertSortedPred);
  ASSERT_EQ(std::vector<int>({ -1, 0, 1, 2, 3, 4 }), values);
  InsertSorted(values, 0, InsertSortedPred);
  ASSERT_EQ(std::vector<int>({ -1, 0, 0, 1, 2, 3, 4 }), values);
}

TEST(Utility, EmplaceSorted)
{
  std::vector<int> values = { 0, 1, 2, 3 };
  EmplaceSorted(values, 4, InsertSortedPred);
  ASSERT_EQ(std::vector<int>({ 0, 1, 2, 3, 4 }), values);
  EmplaceSorted(values, -1, InsertSortedPred);
  ASSERT_EQ(std::vector<int>({ -1, 0, 1, 2, 3, 4 }), values);
  EmplaceSorted(values, 0, InsertSortedPred);
  ASSERT_EQ(std::vector<int>({ -1, 0, 0, 1, 2, 3, 4 }), values);
}

TEST(Utility, Copy)
{
  std::vector< std::unique_ptr<int> > ints;
  ints.emplace_back(std::make_unique<int>(1));
  ints.emplace_back(std::make_unique<int>(2));
  std::vector< std::unique_ptr<int> > copy = utility::Copy(ints);
  ASSERT_EQ(ints.size(), copy.size());
  for (size_t i = 0; i < ints.size(); ++i)
  {
    ASSERT_EQ(*ints[i], *copy[i]);

  }
}

TEST(Utility, ToString60)
{
  std::vector<char> buffer;
  for (int i = 0; i < 60; ++i)
  {
    buffer.clear();
    ToString60Padding(i, buffer);
    ASSERT_EQ(i, boost::lexical_cast<int>(std::string(buffer.data(), buffer.size())));
  }
}

TEST(Utility, ToString999Padding)
{
  std::vector<char> buffer;
  for (int i = 0; i < 999; ++i)
  {
    buffer.clear();
    ToString999Padding(i, buffer);
    ASSERT_EQ(i, boost::lexical_cast<int>(std::string(buffer.data(), buffer.size())));
  }
}

TEST(Utility, ToString4999)
{
  std::vector<char> buffer;
  for (int i = 0; i < 4999; ++i)
  {
    buffer.clear();
    ToString4999Padding(i, buffer);
    ASSERT_EQ(i, boost::lexical_cast<int>(std::string(buffer.data(), buffer.size())));
  }
}

TEST(Utility, IsDigit)
{
  ASSERT_TRUE(IsDigits("2387329482"));
  EXPECT_FALSE(IsDigits("2312839ieaas"));
  EXPECT_FALSE(IsDigits("sdafaaaSDadd"));
  ASSERT_TRUE(IsDigits("2100000000000000"));
  ASSERT_TRUE(IsDigits("0"));
  EXPECT_FALSE(IsDigits("a"));
}

TEST(Utility, StringToBool)
{
  EXPECT_FALSE(StringToBool(""));
  EXPECT_FALSE(StringToBool("asd"));
  EXPECT_FALSE(StringToBool("false"));
  ASSERT_TRUE(StringToBool("true"));
  ASSERT_TRUE(StringToBool("TRue"));
}

TEST(Utility, HexToData)
{
  EXPECT_TRUE(HexToData("a").empty());
  EXPECT_EQ(std::vector<uint8_t>({ 18 }), HexToData("123")); // Clips the 3
  EXPECT_EQ(std::vector<uint8_t>({ 18 }), HexToData("12"));
  EXPECT_EQ(std::vector<uint8_t>({ 18, 52, 86, 120, 144 }), HexToData("1234567890"));
  EXPECT_EQ(std::vector<uint8_t>({ 18, 52, 86, 120, 144, 171, 205, 239 }), HexToData("1234567890abcdef"));
}

TEST(Utility, GenerateRandomString)
{
  ASSERT_EQ(0, GenerateRandomString(0).length());
  ASSERT_EQ(16, GenerateRandomString(16).length());
  ASSERT_EQ(32, GenerateRandomString(32).length());
  ASSERT_EQ(64, GenerateRandomString(64).length());
  ASSERT_EQ(128, GenerateRandomString(128).length());

  ASSERT_EQ(GenerateRandomString(0), GenerateRandomString(0));
  EXPECT_NE(GenerateRandomString(16), GenerateRandomString(16));
  EXPECT_NE(GenerateRandomString(32), GenerateRandomString(32));
  EXPECT_NE(GenerateRandomString(64), GenerateRandomString(64));
  EXPECT_NE(GenerateRandomString(128), GenerateRandomString(128));
}

TEST(Utility, GenerateRandomData)
{
  ASSERT_EQ(0, GenerateRandomData<char>(0).size());
  ASSERT_EQ(16, GenerateRandomData<char>(16).size());
  ASSERT_EQ(32, GenerateRandomData<char>(32).size());
  ASSERT_EQ(64, GenerateRandomData<char>(64).size());
  ASSERT_EQ(128, GenerateRandomData<char>(128).size());
}

TEST(Utility, SplitString)
{
  ASSERT_EQ(0, SplitString("", { '\n', '\r', ' ' }).size());
  ASSERT_EQ(0, SplitString(" \n\r ", { '\n', '\r', ' ' }).size());
  ASSERT_EQ(std::vector<std::string>({ "abc", "123", "xyz", "qwr" }), SplitString("abc.123.xyz.qwr", '.'));
  ASSERT_EQ(std::vector<std::string>({ "ABC", "DEF", "XYZ", "123" }), SplitString("ABC DEF XYZ 123", ' '));
  ASSERT_EQ(std::vector<std::string>({ "abc", "123", "xyz", "qwr" }), SplitString("abc.123 xyz\tqwr", { '.', ' ', '\t' }));
  ASSERT_EQ(std::vector<std::string>({ "abc", "123", "xyz", "qwr" }), SplitString("abc..123 \t xyz \t qwr", { '.', ' ', '\t' }));
  ASSERT_EQ(std::vector<std::string>({ "ABC", "DEF", "XYZ", "123" }), SplitString("ABC\rDEF\rXYZ\n123", { '\n', '\r', ' ' }));
}

TEST(Utility, RemoveChars)
{
  ASSERT_EQ(std::string("hp"), RemoveChars("help", { 'e', 'l' }));
}

TEST(Utility, ToString)
{
  ASSERT_EQ(std::string("Hello"), utility::ToString(L"Hello"));
}

TEST(Utility, FactorialDivide)
{
  ASSERT_EQ(30240, FactorialDivide(5, 10));
  ASSERT_EQ(6, FactorialDivide(5, 6));
  ASSERT_EQ(1, FactorialDivide(5, 5));
}

TEST(Utility, Factorial)
{
  ASSERT_EQ(3628800, Factorial(10));

}

TEST(Utility, SplitDigits)
{
  ASSERT_EQ(std::vector<int>({ 1, 2, 3, 4, 5, 6 }), SplitDigits(123456));

}

TEST(Utility, Min)
{
  ASSERT_EQ(1, Min(1));
  ASSERT_EQ(1, Min(1, 2));
  ASSERT_EQ(1, Min(1, 2, 3));
  ASSERT_EQ(1, Min(1, 2, 3, 4));
  ASSERT_EQ(1, Min(1, 2, 3, 4, 5));
}

TEST(Utility, Max)
{
  ASSERT_EQ(1, Max(1));
  ASSERT_EQ(2, Max(1, 2));
  ASSERT_EQ(3, Max(1, 2, 3));
  ASSERT_EQ(4, Max(1, 2, 3, 4));
  ASSERT_EQ(5, Max(1, 2, 3, 4, 5));
}

TEST(Utility, Clip)
{
  ASSERT_FLOAT_EQ(Clip(-1.0f, 1.0f, 2.0f), 1.0f);
  ASSERT_FLOAT_EQ(Clip(1.5f, 1.0f, 2.0f), 1.5f);
  ASSERT_FLOAT_EQ(Clip(3.0f, 1.0f, 2.0f), 2.0f);
  ASSERT_EQ(Clip(1, 0, 128), 1);
}

TEST(Utility, Sha256)
{
  const std::array<unsigned char, 32> digest = Sha256("myspecialtestwhichworks");
  char output[64];
  std::memset(output, 0, sizeof(output));
  for (int i = 0; i < digest.size(); ++i)
  {
    sprintf(output + (i * 2), "%02x", digest[i]);
  }
  ASSERT_EQ(std::string("5808bdaaedd946a773e460624e335ed98be51360bd9ff748c0f4c3656886ed1d"), std::string(output, output + sizeof(output)));
}

TEST(Utility, Md5)
{
  ASSERT_EQ("bf5ec0c46ce6560b1fd3437e844a8bd0", Md5("chris:AXIS_WS_ACCC8E1813B0:pass"));
  
}

TEST(Utility, Base64)
{
  const std::string input = GenerateRandomString(20);
  ASSERT_EQ(0, std::memcmp(input.data(), Base64Decode(Base64Encode(input)).data(), input.size()));
}

TEST(Utility, Architecture)
{
  ASSERT_FALSE(GetArchitecture().empty());

}

TEST(Utility, Compiler)
{
  ASSERT_FALSE(GetCompiler().empty());

}

TEST(Utility, GetComputerName)
{
  ASSERT_FALSE(GetComputerName().empty());

}

TEST(Utility, GetUsername)
{
  ASSERT_FALSE(GetUsername().empty());

}

TEST(Utility, SwapBits)
{
  unsigned char value = 1;
  utility::SwapBit(&value, 0, 7);
  ASSERT_EQ(128, value);

  std::array<unsigned char, 10> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  utility::SwapBit(data.data(), 0, 64); // Swap two bits which are both 0, so we don't actualy change anything
  utility::SwapBit(data.data(), 15, 68); // Swap two bits which are both 1, so we don't actualy change anything
  ASSERT_EQ(data, data);

  utility::SwapBit(data.data(), 0, 8);
  std::array<unsigned char, 10> newdata = { 1, 0, 2, 3, 4, 5, 6, 7, 8, 9 };
  ASSERT_EQ(newdata, data);

  utility::SwapBit(data.data(), 8, 0);
  newdata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  ASSERT_EQ(newdata, data);
}

TEST(Utility, RoundUp)
{
  ASSERT_EQ(0, RoundUp(0, 5));
  ASSERT_EQ(5, RoundUp(1, 5));
  ASSERT_EQ(10, RoundUp(6, 5));
}

TEST(Utility, AES)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint16_t> datadist(0, std::numeric_limits<unsigned char>::max()); // can't use unsigned char until C++17 I think
  std::uniform_int_distribution<size_t> sizedist(1, 1024);

  for (int i = 0; i < 1000; ++i)
  {
    std::array<unsigned char, 32> key;
    for (auto& byte : key)
    {
      byte = static_cast<unsigned char>(datadist(gen));

    }

    std::array<unsigned char, 16> iv;
    for (auto& byte : iv)
    {
      byte = static_cast<unsigned char>(datadist(gen));

    }

    std::vector<unsigned char> data;
    data.resize(sizedist(gen));
    for (auto& byte : data)
    {
      byte = static_cast<unsigned char>(datadist(gen));

    }

    std::vector<unsigned char> cipher;
    ASSERT_EQ(0, utility::AESEncrypt(key, iv, data.data(), static_cast<int>(data.size()), nullptr, 0, nullptr, 0, cipher));
    ASSERT_GT(cipher.size(), 0);

    std::vector<unsigned char> decrypted;
    ASSERT_EQ(0, utility::AESDecrypt(key, iv.data(), cipher.data(), static_cast<int>(cipher.size()), decrypted));
    ASSERT_EQ(data, decrypted);
  }
}

TEST(Utility, GetMountPoint)
{
  const boost::filesystem::path currentpath = boost::filesystem::current_path();
  const boost::filesystem::path mountpoint = GetMountPoint(currentpath);
  ASSERT_FALSE(mountpoint.empty());
  const boost::filesystem::path newpath = currentpath / "a" / "b" / "c";
  boost::filesystem::create_directories(newpath);
  ASSERT_EQ(mountpoint, GetMountPoint(newpath));
}

TEST(Utility, IsParent)
{
#ifdef _WIN32
  ASSERT_TRUE(IsParent("C:/", "C:/test.txt"));
  ASSERT_FALSE(IsParent("C:/", "D:/test.txt"));
  ASSERT_FALSE(IsParent("C:/test/", "D:/test.txt"));
  ASSERT_TRUE(IsParent("C:\\test", "C:\\test\\test.txt"));
  ASSERT_TRUE(IsParent("C:\\test", "C:/test/test.txt"));
  ASSERT_FALSE(IsParent("C:/test/", "D:/test/test.txt"));
#else
  ASSERT_TRUE(IsParent("/", "/test.txt"));
  ASSERT_FALSE(IsParent("/test/", "/test.txt"));
  ASSERT_TRUE(IsParent("/test", "/test/test.txt"));
#endif
}

TEST(Utility, AlignedAlloc)
{
  std::array< std::unique_ptr< int, utility::DeleteAligned<int> >, 100> allocations;
  for (auto& allocation : allocations)
  {
    const int value = 10;
    allocation = std::move(utility::AlignedAlloc<int>(PAGE_SIZE, value));
    ASSERT_EQ(reinterpret_cast<size_t>(allocation.get()), PAGE_ROUND_DOWN(reinterpret_cast<size_t>(allocation.get())));
    ASSERT_EQ(value, *allocation);
  }
}

TEST(Utility, AlignedAllocArray)
{
  std::array< std::unique_ptr< int[], utility::DeleteAligned<int> >, 100> allocations;
  for (auto& allocation : allocations)
  {
    allocation = std::move(utility::AlignedAllocArray<int>(PAGE_SIZE, 10));
    ASSERT_EQ(reinterpret_cast<size_t>(allocation.get()), PAGE_ROUND_DOWN(reinterpret_cast<size_t>(allocation.get())));
  }
}

TEST(Utility, CompressDecompress)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned int> dist(1, 60000);
  for (int i = 0; i < 100; ++i)
  {
    z_stream zstreamdeflate;
    zstreamdeflate.zalloc = Z_NULL;
    zstreamdeflate.zfree = Z_NULL;
    zstreamdeflate.opaque = Z_NULL;
    ASSERT_EQ(Z_OK, deflateInit(&zstreamdeflate, Z_DEFAULT_COMPRESSION));
    const std::vector<char> randomdata = utility::GenerateRandomData<char>(dist(gen));
    std::vector<char> buffer;
    ASSERT_EQ(0, utility::Compress(zstreamdeflate, randomdata.data(), randomdata.size(), buffer));
    ASSERT_GT(buffer.size(), 0); // Nobody can compress this well
    deflateEnd(&zstreamdeflate);

    z_stream zstreaminflate;
    zstreaminflate.zalloc = Z_NULL;
    zstreaminflate.zfree = Z_NULL;
    zstreaminflate.opaque = Z_NULL;
    zstreaminflate.avail_in = 0;
    zstreaminflate.next_in = Z_NULL;
    ASSERT_EQ(Z_OK, inflateInit(&zstreaminflate));
    std::vector<char> decompressbuffer;
    ASSERT_EQ(0, utility::Decompress(zstreaminflate, buffer.data(), buffer.size(), decompressbuffer));
    inflateEnd(&zstreaminflate);

    ASSERT_EQ(randomdata, decompressbuffer);
  }
}

}

}
