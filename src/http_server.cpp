#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "http_server.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
namespace RobeHttpServer
{
HttpServer::HttpServer(int port) : port(port)
{
}
HttpServer::~HttpServer()
{
    stop();
}
std::string HttpServer::parseHtml(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void HttpServer::start()
{

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t peerAddressSize;
    this->serverSocket = serverSocket;
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)))
        return;
    listen(serverSocket, 5);
    struct sockaddr_in clientAddress;
    peerAddressSize = sizeof(struct sockaddr_in);

    while (true)
    {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &peerAddressSize);
        std::cout << "Client connected" << std::endl;
        char clientBuffer[1024];
        read(clientSocket, clientBuffer, 1024);
        HttpRequest request(clientBuffer);
        std::cout << request.getPath() << std::endl;

        std::string site = parseHtml("index.html");
        HttpResponse response(200, "OK", site);
        std::string httpResponse = response.httpResponse();
        write(clientSocket, httpResponse.c_str(), httpResponse.size());
    }
    stop();
}
void HttpServer::stop()
{
    close(serverSocket);
}

} // namespace RobeHttpServer
