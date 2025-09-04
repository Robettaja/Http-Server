#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "http_request.hpp"

namespace RobeHttpServer
{

HttpRequest::HttpRequest(const std::string& request)
{

    std::vector<std::string> lines;
    std::stringstream ss(request);
    std::string line;

    int bodyIndex = 0;
    while (std::getline(ss, line))
    {
        lines.emplace_back(line);
    }
    for (int i = 0; i < lines.size(); ++i)
    {
        line = lines[i];
        if (line == "\r" || line == "\n" || line.empty())
            bodyIndex = i + 1;
    }

    std::string firstLine = lines[0];
    int slashIndex = firstLine.find("/") + 1;
    path = firstLine.substr(slashIndex, firstLine.find(" ", 5) - slashIndex);
    if (path.empty())
    {
        path = "index.html";
    }
    path = sanitizePath(path);

    method = firstLine.substr(0, firstLine.find(" "));
    headers = HttpHeaders::fromRequest(request);
    for (int i = bodyIndex; i < lines.size(); ++i)
    {
        body += lines[i];
        if (i != lines.size() - 1)
            body += "\n";
    }
}
std::string HttpRequest::sanitizePath(const std::string& path)
{
    std::vector<std::string> illegalCharacters = {
        "..", "%2e", "%2E", "%2f", "%2F", "%252f", "//", "./", "\\\\", "%5c", "%5C", "%00",
    };
    std::string sanitized = path;
    for (std::string illegalCharacter : illegalCharacters)
    {
        int removeIndex = sanitized.find(illegalCharacter);
        while (removeIndex != std::string::npos)
        {
            if (removeIndex != std::string::npos)
                sanitized.erase(removeIndex, illegalCharacter.length());
            removeIndex = sanitized.find(illegalCharacter);
        }
    }
    if (sanitized[0] == '/')
        sanitized.erase(0, 1);
    return "www/" + sanitized;
}

} // namespace RobeHttpServer
