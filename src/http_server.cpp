#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <openssl/ssl.h>
#include <openssl/err.h>

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
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clientSocket;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipAddr, str, sizeof(str));
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientSocket);
    if (SSL_accept(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(clientSocket);
        return;
    }

    char clientBuffer[1024];

    int bytes = SSL_read(ssl, clientBuffer, sizeof(clientBuffer) - 1);
    if (bytes <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(clientSocket);
        return;
    }
    // read(clientSocket, clientBuffer, 1024);
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
    // write(clientSocket, httpResponse.c_str(), httpResponse.size());
    SSL_write(ssl, httpResponse.c_str(), httpResponse.size());
    SSL_shutdown(ssl);
    SSL_free(ssl);
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

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    const SSL_METHOD* method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
    }

    if (SSL_CTX_use_certificate_file(ctx, "keys/localhost+2.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "keys/localhost+2-key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return;
    }
    if (!SSL_CTX_check_private_key(ctx))
    {
        std::cerr << "Private key does not match the certificate public key\n";
        return;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    this->serverSocket = serverSocket;
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    socklen_t peerAddressSize;
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
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

} // namespace RobeHttpServer
