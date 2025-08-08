#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>

class TestServer
{
  private:
  public:
    TestServer();
    void startServer();
};
