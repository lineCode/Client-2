// testelement.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestElement, Compare)
{
  pugi::xml_document doc;
  Element element = Element(testing::UnitTest::GetInstance()->current_test_info()->name(), { { "a", "b" }, { "c", "d" } }, "text", { Element("name2", { { "e", "f" }, { "g", "h" } }, "text2", { Element("name3", { { "i", "j" }, { "k", "l" } }, "text3", {}) }) } );
  ASSERT_TRUE(doc.load(element.ToXml().c_str()));
  ASSERT_EQ(element, Element(doc.child(testing::UnitTest::GetInstance()->current_test_info()->name())));
}

}

}
