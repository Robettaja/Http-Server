#include <iostream>
#include <string>
#include <unordered_map>
#include "http_response.hpp"

namespace RobeHttpServer
{

HttpResponse::HttpResponse(int code, const std::string& message, const std::string& bodyContent,
                           const std::string& filePath)
    : statusCode(code), statusMessage(message), body(bodyContent)
{
    std::unordered_map<std::string, std::string> contentTypeMap = {
        {".html", "text/html"},        {".css", "text/css"},  {".js", "application/javascript"},
        {".json", "application/json"}, {".png", "image/png"}, {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},       {".gif", "image/gif"}, {".svg", "image/svg+xml"},
        {".txt", "text/plain"}};
    std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
    HttpHeaders headers;
    std::string correctExtension = contentTypeMap[fileExtension];
    std::cout << correctExtension << std::endl;

    headers.addHeader("Server", "RobeHttpServer");
    headers.addHeader("Content-Length", std::to_string(body.size()));
    headers.addHeader("Content-Type", correctExtension);
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
