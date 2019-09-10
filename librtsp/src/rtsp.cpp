// rtsp.cpp
//

///// Includes /////

#include "rtsp/rtsp.hpp"

///// Namespaces /////

namespace rtsp
{

// Globals

const uint8_t LUMDCCODELENS[] = { 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
const uint8_t LUMDCSYMBOLS[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const uint8_t LUMACCODELENS[] = { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
const uint8_t LUMACSYMBOLS[] = { 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa };
const uint8_t CHMDCCODELENS[] = { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
const uint8_t CHMDCSYMBOLS[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const uint8_t CHMACCODELENS[] = { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };
const uint8_t CHMACSYMBOLS[] = { 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa };
const int JPEGLUMAQUANTIZER[64] = { 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99 };
const int JPEGCHROMAQUANTIZER[64] = { 17, 18, 24, 47, 99, 99, 99, 99, 18, 21, 26, 66, 99, 99, 99, 99, 24, 26, 56, 99, 99, 99, 99, 99, 47, 66, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 };

///// Functions /////

void MakeDRIHeader(std::vector<unsigned char>& data, uint16_t dri)
{
  data.push_back(0xff);
  data.push_back(0xdd); // DRI
  data.push_back(0); // Length MSB
  data.push_back(4); // Length LSB
  data.push_back(dri >> 8); // DRI MSB
  data.push_back(dri & 0xff); // DRI LSB
}

void MakeHuffmanHeader(std::vector<unsigned char>& data, const uint8_t* codelens, int ncodes, const uint8_t* symbols, int nsymbols, int tableNo, int tableclass)
{
  data.push_back(0xff);
  data.push_back(0xc4); // DHT
  data.push_back(0); // Length MSB
  data.push_back(3 + ncodes + nsymbols); // Length LSB
  data.push_back((tableclass << 4) | tableNo);
  data.insert(data.end(), codelens, codelens + ncodes);
  data.insert(data.end(), symbols, symbols + nsymbols);
}

void MakeQuantHeader(std::vector<unsigned char>& data, const uint8_t* qt, int tableno)
{
  data.push_back(0xff);
  data.push_back(0xdb); // DQT
  data.push_back(0); // Length MSB
  data.push_back(67); // Length LSB
  data.push_back(tableno);
  data.insert(data.end(), qt, qt + 64);
}

void MakeHeaders(std::vector<unsigned char>& data, int type, int width, int height, const uint8_t* lqt, const uint8_t* cqt, uint16_t dri)
{
  // Convert from blocks to pixels(multiply by 8)
  width *= 8;
  height *= 8;

  data.push_back(0xff);
  data.push_back(0xd8); // SOI

  MakeQuantHeader(data, lqt, 0);
  MakeQuantHeader(data, cqt, 1);

  if (dri != 0)
  {
    MakeDRIHeader(data, dri);

  }

  data.push_back(0xff);
  data.push_back(0xc0); // SOF
  data.push_back(0); // Length MSB
  data.push_back(17); // Length LSB
  data.push_back(8); // 8-bit precision
  data.push_back(height >> 8); // Height MSB
  data.push_back(height); // Height LSB
  data.push_back(width >> 8); // Width MSB
  data.push_back(width); // Width LSB
  data.push_back(3); // 3 components
  data.push_back(0); // Comp 0
  if ((type == 0) || (type == 64))
  {
    data.push_back(0x21); // hsamp = 2, vsamp = 1

  }
  else
  {
    data.push_back(0x22); // hsamp = 2, vsamp = 2

  }
  data.push_back(0); // Quant table 0
  data.push_back(1); // Comp 1
  data.push_back(0x11); // hsamp = 1, vsamp = 1
  data.push_back(1); // Quant table 1
  data.push_back(2); // Comp 2
  data.push_back(0x11); // hsamp = 1, vsamp = 1
  data.push_back(1); // Quant table 1

  MakeHuffmanHeader(data, LUMDCCODELENS, sizeof(LUMDCCODELENS), LUMDCSYMBOLS, sizeof(LUMDCSYMBOLS), 0, 0);
  MakeHuffmanHeader(data, LUMACCODELENS, sizeof(LUMACCODELENS), LUMACSYMBOLS, sizeof(LUMACSYMBOLS), 0, 1);
  MakeHuffmanHeader(data, CHMDCCODELENS, sizeof(CHMDCCODELENS), CHMDCSYMBOLS, sizeof(CHMDCSYMBOLS), 1, 0);
  MakeHuffmanHeader(data, CHMACCODELENS, sizeof(CHMACCODELENS), CHMACSYMBOLS, sizeof(CHMACSYMBOLS), 1, 1);

  data.push_back(0xff);
  data.push_back(0xda); // SOS
  data.push_back(0); // Length MSB
  data.push_back(12); // Length LSB
  data.push_back(3); // 3 components
  data.push_back(0); // Comp 0
  data.push_back(0); // Huffman table 0
  data.push_back(1); // Comp 1
  data.push_back(0x11); // Huffman table 1
  data.push_back(2); // Comp 2
  data.push_back(0x11); // Huffman table 1
  data.push_back(0); // First DCT coeff
  data.push_back(63); // Last DCT coeff
  data.push_back(0); // Successive approximation
}

void MakeTables(int q, uint8_t* lqt, uint8_t* cqt)
{
  const int factor = utility::Clip(q, 1, 99);
  if (q < 50)
  {
    q = 5000 / factor;

  }
  else
  {
    q = 200 - (factor * 2);

  }

  for (int i = 0; i < 64; ++i)
  {
    int lq = (JPEGLUMAQUANTIZER[i] * q + 50) / 100;
    int cq = (JPEGCHROMAQUANTIZER[i] * q + 50) / 100;
    lqt[i] = utility::Clip(lq, 1, 255);
    cqt[i] = utility::Clip(cq, 1, 255);
  }
}

}
