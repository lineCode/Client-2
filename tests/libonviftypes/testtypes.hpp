// testtypes.hpp
//

///// Includes /////

#include <boost/optional.hpp>
#include <gtest/gtest.h>

#include "../../pugixml/src/pugixml.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Functions /////

template <typename T>
void Compare(const T& type)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(type.ToXml(testing::UnitTest::GetInstance()->current_test_info()->name()).c_str()));
  ASSERT_EQ(type, T(doc.child(testing::UnitTest::GetInstance()->current_test_info()->name())));
}

template <typename T>
void CompareElement(const T& type)
{
  pugi::xml_document doc;
  ASSERT_TRUE(doc.load(type.ToElement(testing::UnitTest::GetInstance()->current_test_info()->name()).ToXml().c_str()));
  ASSERT_EQ(type, T(doc.child(testing::UnitTest::GetInstance()->current_test_info()->name())));
}
}

}
