#include <unistd.h>

#include <http_server.hpp>

using namespace RobeHttpServer;

int main()
{
    HttpServer server(8080);
    server.start();

    return 0;
}
