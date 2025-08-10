#pragma once
#include <string>
#include "http_headers.hpp"

namespace RobeHttpServer
{
class HttpResponse
{
  private:
    int statusCode;
    std::string statusMessage;
    HttpHeaders headers;
    std::string body;

  public:
    HttpResponse(int code, const std::string& message, const std::string& bodyContent, const std::string& filePath);
    std::string httpResponse();
};

} // namespace RobeHttpServer
