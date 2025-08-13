#include <unistd.h>

#include <http_server.hpp>

using namespace RobeHttpServer;

int main()
{
    HttpServer server(8443);
    server.start();

    return 0;
}
