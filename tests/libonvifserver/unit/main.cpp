// main.cpp
//

///// Includes /////

#include "main.hpp"

#include <boost/scope_exit.hpp>

#ifdef _WIN32
  #include <Windows.h>
#endif

///// Functions /////

GTEST_API_ int main(int argc, char** argv)
{
#ifdef _WIN32
  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
  {

    return 1;
  }

  BOOST_SCOPE_EXIT(void)
  {
    CoUninitialize();
  } BOOST_SCOPE_EXIT_END

    const HRESULT error = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
  if ((error != RPC_E_TOO_LATE) && FAILED(error))
  {

    return 1;
  }
#endif
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
