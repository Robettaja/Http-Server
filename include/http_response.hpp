#pragma once
#include <string>
#include <vector>
#include "http_headers.hpp"

namespace RobeHttpServer
{

enum ResponseType
{
    information = 1,
    successful = 2,
    redirection = 3,
    clientError = 4,
    serverError = 5
};
class HttpResponse
{
  private:
    // Can add more if nessessary
    std::vector<std::string> allowedFileTypes = {".html", ".css", ".js", ".json", ".jpg", ".png", ".webp"};
    int statusCode;
    std::string statusMessage;
    HttpHeaders headers;
    std::string body;
    ResponseType getResponseType(int code);

  public:
    HttpResponse(int code, const std::string& bodyContent, const std::string& filePath);
    std::string httpResponse();
};

} // namespace RobeHttpServer
