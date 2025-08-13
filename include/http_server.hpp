#pragma once
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <openssl/ssl.h>

namespace RobeHttpServer
{
class HttpServer
{
  private:
    int port;
    int serverSocket;
    SSL_CTX* ctx;

    std::string readFile(const std::string& path);
    void handleRequest(int clientSocket);

  public:
    HttpServer(int port = 8080);
    ~HttpServer();
    void start();
    void stop();
};

} // namespace RobeHttpServer
