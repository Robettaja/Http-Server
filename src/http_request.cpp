#include <sstream>
#include <iostream>
#include <vector>

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
    path = lines[0].substr(lines[0].find("/") + 1, lines[0].find("."));
    path.erase(remove(path.begin(), path.end(), ' '), path.end());
    method = lines[0];
    headers = HttpHeaders::fromRequest(request);
    for (int i = bodyIndex; i < lines.size(); ++i)
    {
        body += lines[i];
        if (i != lines.size() - 1)
            body += "\n";
    }
}

} // namespace RobeHttpServer
