// testptznode.cpp
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

TEST(TestPTZNode, Compare)
{
  Compare(PTZNode(std::string("token"),
          true,
          std::string("name"),
          PTZSpaces({ Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }, { Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }),
          5,
          true,
          { std::string("auxiliarycommands1"), std::string("auxiliarycommands2") },
          { PTZPresetTourSupported(5, { PTZPRESETTOUROPERATION_STOP, PTZPRESETTOUROPERATION_PAUSE }), PTZPresetTourSupported(6, { PTZPRESETTOUROPERATION_START, PTZPRESETTOUROPERATION_PAUSE }) }));
}

}

}
