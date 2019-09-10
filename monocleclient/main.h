// main.hpp
//

#ifndef IDLVWBHMI355555ZUBUSAAKW4PS7JG6RCJ
#define IDLVWBHMI355555ZUBUSAAKW4PS7JG6RCJ

///// Includes /////

#include <QApplication>
#include <QString>

///// Globals /////

const QString ORGANISATIONNAME = "MonocleSecurity";
const QString APPLICATIONNAME = "Client";

///// Prototypes /////

#if defined(BOOST_OS_WINDOWS) && !defined(DEBUG)
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdline, int show);
#else
int main(int argc, char** argv);
#endif

#endif
