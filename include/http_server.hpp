#pragma once
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>

namespace RobeHttpServer
{
class HttpServer
{
  private:
    int port;
    int serverSocket;

    std::string parseHtml(const std::string& path);

  public:
    HttpServer(int port = 8080);
    ~HttpServer();
    void start();
    void stop();
};

} // namespace RobeHttpServer
