#include "FunctionServer.h"

using namespace functionserver;

int main(){

    FunctionServer server{};

    server.OnJoin = [](TCPConnection::pointer conn){
        std::cout<< "Connection from " << conn->socket().remote_endpoint() << std::endl;
    };

    server.OnLeave = [](TCPConnection::pointer conn){
        std::cout << "Client disconnected" << std::endl;
    };

    server.OnClientMessage = [](TCPConnection* conn, std::string msg){
        conn->post(msg);
    };

    server.start();

    return 0;

}