#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "http_response.hpp"

namespace RobeHttpServer
{

HttpResponse::HttpResponse(int code, const std::string& bodyContent, const std::string& filePath)
    : statusCode(code), body(bodyContent)
{
    std::unordered_map<std::string, std::string> contentTypeMap = {
        {".html", "text/html"},        {".css", "text/css"},  {".js", "application/javascript"},
        {".json", "application/json"}, {".png", "image/png"}, {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},       {".gif", "image/gif"}, {".svg", "image/svg+xml"},
        {".txt", "text/plain"}};

    std::string fileExtension = "";
    if (!filePath.empty())
    {
        try
        {
            fileExtension = filePath.substr(filePath.find_last_of('.'));
            if (std::find(allowedFileTypes.begin(), allowedFileTypes.end(), fileExtension) == allowedFileTypes.end())
            {
                code = 403;
                body = "403 Forbidden";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error extracting file extension: " << e.what() << std::endl;
        }
    }

    ResponseType responseType = getResponseType(code);
    HttpHeaders headers;
    std::string correctExtension = contentTypeMap[fileExtension];
    switch (responseType)
    {
    case ResponseType::information:
        break;

    case ResponseType::successful:
        headers.addHeader("Server", "RobeHttpServer");
        headers.addHeader("Content-Length", std::to_string(body.size()));
        headers.addHeader("Content-Type", correctExtension);
        break;

    case ResponseType::redirection:
        headers.addHeader("Location", "https://localhost:8443/" + filePath);
        headers.addHeader("Content-Length", std::to_string(body.size()));
        headers.addHeader("Content-Type", correctExtension);
        break;

    case ResponseType::clientError:
        headers.addHeader("Server", "RobeHttpServer");
        headers.addHeader("Content-Length", std::to_string(body.size()));
        headers.addHeader("Content-Type", correctExtension);
        break;

    case ResponseType::serverError:

        break;
    }

    this->headers = headers;
}
ResponseType HttpResponse::getResponseType(int code)
{
    int enumNum = code;
    while (enumNum >= 10)
    {
        enumNum /= 10;
    }
    return static_cast<ResponseType>(enumNum);
}

std::string HttpResponse::httpResponse()
{
    std::unordered_map<int, const char*> responseTypesFromCode = {
        {200, "OK"}, {303, "See Other"}, {403, "Forbidden"}, {404, "Not Found"}};
    statusMessage = responseTypesFromCode[statusCode];

    std::string readyHeaders;
    for (const auto& [key, value] : headers.getHeaders())
    {
        readyHeaders += key + value + "\r\n";
    }

    return "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n" + readyHeaders + "\r\n" + body;
}
} // namespace RobeHttpServer
