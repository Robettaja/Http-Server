#pragma once
#include <string>
#include <unordered_map>

namespace RobeHttpServer
{

class HttpHeaders
{
  private:
    std::unordered_map<std::string, std::string> headers;

  public:
    HttpHeaders();
    std::unordered_map<std::string, std::string> getHeaders() const;
    void addHeader(const std::string& key, const std::string& value);
    static HttpHeaders fromRequest(const std::string& request);
};
} // namespace RobeHttpServer
