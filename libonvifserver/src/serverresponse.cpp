// serverresponse.hpp
//

///// Includes /////

#include "onvifserver/serverresponse.hpp"

#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Globals /////

Header CONTENT_TYPE = Header("Content-Type", "application/soap+xml; charset=utf-8");

///// Methods /////

ServerResponse::ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::vector<Header>& headers, const std::string& body) :
  httpstatuscode_(httpstatuscode),
  headers_(headers),
  body_(body)
{

}

ServerResponse::ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::string& body, const std::map< std::string, std::vector<char> >& mtomdatas) :
  httpstatuscode_(httpstatuscode)
{
  if (mtomdatas.size())
  {
    const std::string boundary = utility::GenerateRandomString(40);

    const std::string contentboundary(std::string("\r\n--") + boundary);
    const std::string newline("\r\n");
    body_.append(contentboundary.begin(), contentboundary.end());
    body_.append(newline.begin(), newline.end());

    const std::string soapheaders = "Content-Type: application/xop+xml; charset=UTF-8; type=\"application/soap+xml\"\r\nContent-ID: <soap@soap>\r\n\r\n";
    body_.append(soapheaders.begin(), soapheaders.end());
    body_.append(body.begin(), body.end());

    // MTOM data
    body_.append(contentboundary.begin(), contentboundary.end());
    for (const auto& mtomdata : mtomdatas)
    {
      body_.append(newline.begin(), newline.end());
      const std::string binaryheaders = std::string("Content-Transfer-Encoding: binary\r\nContent-ID: <") + mtomdata.first + std::string(">\r\n\r\n");
      body_.append(binaryheaders.begin(), binaryheaders.end());
      body_.append(mtomdata.second.begin(), mtomdata.second.end());
      body_.append(contentboundary.begin(), contentboundary.end());
    }
    body_.push_back('-');
    body_.push_back('-');

    // Headers
    headers_.push_back(Header(std::string("Content-Type"), std::string("Multipart/Related; boundary=\"") + boundary + std::string("\" type=\"application/xop+xml\" start=\"<soap@soap>\" start-info=\"application/soap+xml\"")));
    headers_.push_back(Header(std::string("Content-length"), std::to_string(body_.size())));
  }
  else
  {
    body_ = body;

  }
}

ServerResponse::ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::vector<Header>& headers, const std::string& title, const std::string& text) :
  httpstatuscode_(httpstatuscode),
  headers_(headers),
  body_("<!DOCTYPE html><html lang=en><title>" + title + "</title><body><p>" + text + "</p></body></html>")
{

}

ServerResponse::ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::string& title, const std::string& text) :
  ServerResponse(httpstatuscode, std::vector<Header>(), title, text)
{

}

ServerResponse::ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::string& body) :
  httpstatuscode_(httpstatuscode),
  body_(body)
{

}

std::string ServerResponse::Reason(HTTPSTATUSCODE httpstatuscode) const
{
  switch (httpstatuscode)
  {
    case HTTPSTATUSCODE::OK:
    {

      return std::string("OK");
    }
    case HTTPSTATUSCODE::BADREQUEST:
    {

      return std::string("Bad Request");
    }
    case HTTPSTATUSCODE::UNAUTHORIZED:
    {

      return std::string("Unauthorized");
    }
    case HTTPSTATUSCODE::NOTFOUND:
    {

      return std::string("Not Found");
    }
    case HTTPSTATUSCODE::METHODNOTFOUND:
    {

      return std::string("Method Not Found");
    }
    case HTTPSTATUSCODE::INTERNALSERVERERROR:
    {

      return std::string("Internal Server Error");
    }
    default:
    {

      return std::string();
    }
  }
}

}

}
