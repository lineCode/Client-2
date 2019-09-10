// testmetadataconfigurationoptions.cpp
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

TEST(TestMetadataConfigurationOptions, Compare)
{
  Compare(MetadataConfigurationOptions(PTZStatusFilterOptions(true, false, true, false), { METADATACOMPRESSIONTYPE_GZIP, METADATACOMPRESSIONTYPE_EXI }));
}

}

}
