#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

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
void HttpServer::handleRequest(int clientSocket)
{
    char clientBuffer[1024];
    read(clientSocket, clientBuffer, 1024);
    HttpRequest request(clientBuffer);
    std::string body = "";
    if (request.getPath() != "")
        body = readFile(request.getPath());
    std::string httpResponse = "";
    if (body.empty())
    {
        HttpResponse response(404, "Not Found ", body, request.getPath());
        httpResponse = response.httpResponse();
    }
    else
    {
        HttpResponse response(200, "OK", body, request.getPath());
        httpResponse = response.httpResponse();
    }
    write(clientSocket, httpResponse.c_str(), httpResponse.size());
    close(clientSocket);
}
std::string HttpServer::readFile(const std::string& path)
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
    std::cout << "Started Server" << std::endl;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t peerAddressSize;
    this->serverSocket = serverSocket;
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)))
    {
        std::cout << "Failed to bind server to socket" << std::endl;
        return;
    }
    listen(serverSocket, 5);
    struct sockaddr_in clientAddress;
    peerAddressSize = sizeof(struct sockaddr_in);

    while (true)
    {

        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &peerAddressSize);
        std::thread requestThread(&HttpServer::handleRequest, this, clientSocket);
        requestThread.detach();
    }
    stop();
}
void HttpServer::stop()
{
    close(serverSocket);
}

} // namespace RobeHttpServer
