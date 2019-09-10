// testvideoencoderconfigurationoptions.cpp
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

TEST(TestVideoEncoderConfigurationOptions, Compare)
{
  Compare(VideoEncoderConfigurationOptions(IntRange(5, 5),
                                           JpegOptions({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5)),
                                           Mpeg4Options({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { MPEG4PROFILE_ASP }),
                                           H264Options({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { H264PROFILE_HIGH, H264PROFILE_MAIN }),
                                           JpegOptions2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5)),
                                           Mpeg4Options2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { MPEG4PROFILE_ASP }, IntRange(5, 5)),
                                           H264Options2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { H264PROFILE_HIGH, H264PROFILE_MAIN }, IntRange(5, 5))));
}

}

}
