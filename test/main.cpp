#include "FunctionServer.h"

int main(){

    boost::asio::io_service ioService;
    functionserver::FunctionServer server(ioService,5555);

    ioService.run();

}