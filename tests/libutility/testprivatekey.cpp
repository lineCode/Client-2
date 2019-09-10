// testprivatekey.cpp
//

///// Includes /////

#include <gtest/gtest.h>

#include <utility/privatekey.hpp>
#include <utility/publickey.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace utility
{

namespace tests
{

///// Classes /////

TEST(PrivateKey, Sign)
{
  utility::PrivateKey privatekey;
  ASSERT_FALSE(privatekey.Init());
  for (int i = 0; i < 50; ++i)
  {
    const std::string message = utility::GenerateRandomString(20);
    const boost::optional< std::array<unsigned char, 256> > signature = privatekey.Sign(reinterpret_cast<const unsigned char*>(message.data()), message.size());
    ASSERT_TRUE(signature.is_initialized());
    utility::PublicKey publickey;
    ASSERT_FALSE(publickey.Init(privatekey));
    ASSERT_TRUE(publickey.Verify(reinterpret_cast<const unsigned char*>(message.data()), message.size(), signature->data()));
  }
}

TEST(PrivateKey, GetString)
{
  utility::PrivateKey privatekey;
  ASSERT_FALSE(privatekey.Init());
  ASSERT_FALSE(privatekey.GetString().empty());
}

TEST(PrivateKey, Save)
{
  utility::PrivateKey privatekey;
  ASSERT_FALSE(privatekey.Init());
  const std::string filename("privatekey.pem");
  ASSERT_FALSE(privatekey.Save(filename));
  ASSERT_FALSE(privatekey.Init(filename, std::string()));
}

TEST(PrivateKey, Size)
{
  utility::PrivateKey privatekey;
  ASSERT_FALSE(privatekey.Init());
  ASSERT_EQ(256, privatekey.GetSize());
}

TEST(PublicKey, Binary)
{
  utility::PrivateKey privatekey;
  ASSERT_FALSE(privatekey.Init());
  utility::PublicKey publickey;
  ASSERT_FALSE(publickey.Init(privatekey));
  const std::string text = publickey.GetString();
  ASSERT_FALSE(text.empty());

  utility::PublicKey publickey2;
  ASSERT_FALSE(publickey2.Init(text));

  const std::string message;
  ASSERT_TRUE(publickey.Verify(reinterpret_cast<const unsigned char*>(message.data()), message.size(), privatekey.Sign(reinterpret_cast<const unsigned char*>(message.data()), message.size())->data()));

}

}

}
