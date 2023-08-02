#include "FunctionServer.h"

using namespace functionserver;

int main(){

    FunctionServer server(6789);


    server.start();

    return 0;

}