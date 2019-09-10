// rtsp.hpp
//

#ifndef ID3EQLO85D20ON2TAN73OIGQCCJBOAUYKX
#define ID3EQLO85D20ON2TAN73OIGQCCJBOAUYKX

///// Includes /////

#include <socket/socket.hpp>
#include <stdint.h>
#include <string>
#include <vector>

#include "client/client.hpp"
#include "server/server.hpp"

///// Namespaces /////

namespace rtsp
{

///// Globals /////

const std::string USERAGENT = std::string("User-Agent: Monocle");

///// Prototypes /////

void MakeHeaders(std::vector<unsigned char>& data, int type, int width, int height, const uint8_t* lqt, const uint8_t* cqt, uint16_t dri);
void MakeTables(int q, uint8_t* lqt, uint8_t* cqt);

}

#endif
