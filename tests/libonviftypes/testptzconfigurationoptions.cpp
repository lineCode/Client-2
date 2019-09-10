// testptzconfigurationoptions.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestPTZConfigurationOptions, Compare)
{
  Compare(PTZConfigurationOptions(onvif::IntAttrList(std::vector<int>({ 1, 3 })),
                                  PTZSpaces({ Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) })));
}

}

}
