// media.hpp
//

#ifndef ID7MFMGV1DK8848D3ZZTMXW8CM2G2VHRGX
#define ID7MFMGV1DK8848D3ZZTMXW8CM2G2VHRGX

///// Includes /////

#include <boost/optional.hpp>
#include <stdint.h>
#include <string>
#include <vector>

#include "mediaprotocol.hpp"
#include "mediatype.hpp"

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class Media
{
public:

  Media();
  Media(const std::string& text);
  Media(MEDIATYPE mediatype, const boost::optional<uint16_t>& port, const boost::optional<uint16_t>& numports, MEDIAPROTOCOL mediaprotocol, const std::vector<unsigned int>& formats);
  ~Media();

  void Clear();
  bool IsValid() const;
  std::string ToString() const;

  bool operator==(const Media& rhs) const;

  boost::optional<MEDIATYPE> mediatype_;
  boost::optional<uint16_t> port_;
  boost::optional<uint16_t> numports_;
  boost::optional<MEDIAPROTOCOL> mediaprotocol_;
  std::vector<unsigned int> formats_;

};

}

}

#endif
