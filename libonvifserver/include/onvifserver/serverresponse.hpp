// serverresponse.hpp
//

#ifndef IDUJV0H0DUEB4M5IC7WRNA6NP82WWH8FPL
#define IDUJV0H0DUEB4M5IC7WRNA6NP82WWH8FPL

///// Includes /////

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "header.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Enuemrations /////

enum class HTTPSTATUSCODE : int
{
  OK = 200,
  BADREQUEST = 400,
  UNAUTHORIZED = 401,
  NOTFOUND = 404,
  METHODNOTFOUND = 405,
  INTERNALSERVERERROR = 500,
};

///// Globals /////

extern Header CONTENT_TYPE;

///// Classes /////

class ServerResponse
{
 public:

  ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::vector<Header>& headers, const std::string& body);
  ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::string& body, const std::map< std::string, std::vector<char> >& mtomdatas);
  ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::vector<Header>& headers, const std::string& title, const std::string& text);
  ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::string& title, const std::string& text);
  ServerResponse(HTTPSTATUSCODE httpstatuscode, const std::string& body);

  HTTPSTATUSCODE GetHttpStatusCode() const { return httpstatuscode_; }
  std::string GetReason() const { return Reason(httpstatuscode_); }
  const std::vector<Header>& GetHeaders() const { return headers_; }
  const std::string& GetBody() const { return body_; }

 private:

  std::string Reason(HTTPSTATUSCODE code) const;

  HTTPSTATUSCODE httpstatuscode_;
  std::vector<Header> headers_;
  std::string body_;

};

}

}

#endif
