#include "http_headers.hpp"
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>

namespace RobeHttpServer
{
HttpHeaders::HttpHeaders()
{
}
std::unordered_map<std::string, std::string> HttpHeaders::getHeaders() const
{
    return headers;
}
void HttpHeaders::addHeader(const std::string& key, const std::string& value)
{
    headers[key + ": "] = value;
}
HttpHeaders HttpHeaders::fromRequest(const std::string& request)
{
    HttpHeaders httpHeaders;
    std::vector<std::string> lines;
    std::stringstream ss(request);
    std::string line;

    while (std::getline(ss, line))
    {
        if (line == "\r" || line == "\n" || line.empty())
            break;

        lines.emplace_back(line);
    }

    lines.erase(lines.begin());
    for (auto const line : lines)
    {
        std::string key = line.substr(0, line.find(':'));
        httpHeaders.headers[key] = line.substr(line.find(':') + 1);
    }
    return httpHeaders;
}

} // namespace RobeHttpServer
