// main.hpp
//

#ifndef IDHM3D09BPNI92BHZ0KPTKDHR7BXIEFU94
#define IDHM3D09BPNI92BHZ0KPTKDHR7BXIEFU94

///// Includes /////

#include <gtest/gtest.h>
#include <socket/proxyparams.hpp>
#include <string>

///// Globals /////

extern sock::ProxyParams g_proxyparams;
extern std::string g_address;
extern std::string g_username;
extern std::string g_password;

///// Prototypes /////

GTEST_API_ int main(int argc, char** argv);

#endif
