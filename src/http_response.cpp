#include <iostream>
#include <string>
#include <unordered_map>
#include "http_response.hpp"

namespace RobeHttpServer
{

HttpResponse::HttpResponse(int code, const std::string& message, const std::string& bodyContent)
    : statusCode(code), statusMessage(message), body(bodyContent)
{
    HttpHeaders headers;
    headers.addHeader("Server", "RobeHttpServer");
    headers.addHeader("Content-Length", std::to_string(body.size()));
    headers.addHeader("Content-Type", "text/html");
    this->headers = headers;
}

std::string HttpResponse::httpResponse()
{
    std::string readyHeaders;
    for (const auto& [key, value] : headers.getHeaders())
    {
        readyHeaders += key + value + "\r\n";
    }

    return "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n" + readyHeaders +
           "\r\n"

           "\r\n" +
           body;
}
} // namespace RobeHttpServer
