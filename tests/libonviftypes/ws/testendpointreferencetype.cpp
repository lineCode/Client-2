// testendpointreferencetype.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestEndpointReferenceType, Compare)
{
  Compare(ws::EndpointReferenceType("address", Element("wsa:ReferenceParameters", { std::make_pair("attributename", "attributevalue") }, "text", std::vector<Element>())));
}

}

}
