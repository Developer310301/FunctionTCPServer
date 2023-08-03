#include "FunctionServer.h"

using namespace functionserver;

int main(){

    FunctionServer server(6789,false);

    server.addFunction("test",[](FunctionRequestMessage* reqMsg,FunctionResponseMessage* res_msg){

        res_msg->returnValues.push_back("ciao");

    });

    server.start();

    return 0;

}