#pragma once

#include <string>

#include "http_headers.hpp"
namespace RobeHttpServer
{
class HttpRequest
{
  private:
    std::string path;
    std::string method;
    HttpHeaders headers;
    std::string body;

    std::string sanitizePath(const std::string& path);

  public:
    HttpRequest(const std::string& request);
    std::string parseRequest(const std::string& request);
    std::string getPath() const
    {
        return path;
    }
    std::string getMethod() const
    {
        return method;
    }
};

} // namespace RobeHttpServer
