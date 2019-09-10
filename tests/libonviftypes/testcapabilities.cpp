// testcapabilities.cpp
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

TEST(TestCapabilities, Compare)
{
  Compare(Capabilities(AnalyticsCapabilities(std::string("xaddr"), true, true),
                       DeviceCapabilities(std::string("xaddr"),
                                          NetworkCapabilities(true, true, true, true, true),
                                          SystemCapabilities(true, true, true, true, true, true, { onvif::OnvifVersion(5, 6), onvif::OnvifVersion(7, 8) }, true, true, true, true),
                                          IOCapabilities(5, 5),
                                          SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, true)),
                       EventCapabilities(std::string("xaddr"), true, true, true),
                       ImagingCapabilities(std::string("xaddr")),
                       MediaCapabilities(std::string("xaddr"),
                                         RealTimeStreamingCapabilities(true, true, true),
                                         ProfileCapabilities(5)),
                       PTZCapabilities(std::string("xaddr")),
                       DeviceIOCapabilities(std::string("xaddr"), 5, 5, 5, 5, 5),
                       DisplayCapabilities(std::string("xaddr"), true),
                       RecordingCapabilities(std::string("xaddr"), true, true, true, true, 5),
                       SearchCapabilities(std::string("xaddr"), true),
                       ReplayCapabilities(std::string("xaddr")),
                       ReceiverCapabilities(std::string("xaddr"), true, true, true, 5, 5),
                       AnalyticsDeviceCapabilities(std::string("xaddr"), true)));
}

}

}
