#include "FunctionServer.h"

namespace functionserver{

    FunctionServer::FunctionServer(unsigned short server_port) : _server(TCPServer(IPV::V4, server_port)), _regexReqMsg(std::regex(REGEX_EXPR)){

        this->_server.OnClientMessage = [this](TCPConnection* conn, std::string msg){

            msg.erase(std::remove( msg.begin(), msg.end(), '\n'), msg.end());

            std::cout<<"[-] Receiving from client " << conn->socket().remote_endpoint() << " : " << msg << std::endl;

            FunctionRequestMessage req_msg;
            FunctionResponseMessage res_msg;
            std::string resp_msg;

            this->_parseRequestMessage(msg,&req_msg);

            res_msg.errorCode = -1;
            res_msg.functionName = req_msg.functionName;
            res_msg.params = req_msg.params;

            auto it = this->_serverFunctions.find(req_msg.functionName);

            if(it!=this->_serverFunctions.end()){
                (it->second)(&req_msg,&res_msg);
            }

            conn->post(this->_buildResponseMessage(&res_msg)+"\n");
        };

    }

    void FunctionServer::_parseRequestMessage(std::string msg, FunctionRequestMessage* req_msg){

        std::smatch m;

        if(std::regex_search(msg, m, this->_regexReqMsg)){
            std::cout<<"Parsing..."<<std::endl;
            /*req_msg->functionName = m.str(1);

            if(m.size()>1){

                req_msg->params = new std::string[m.size()-1];

                for(int i=0;i<m.size()-1;i++){
                    req_msg->params[i] = m.str(i+2);
                    std::cout<<req_msg->params[i]<<std::endl;
                }

            }*/

        }else{
            std::cerr<<"Cannot Parsing!"<<std::endl;
        }

    }

    std::string FunctionServer::_buildResponseMessage(FunctionResponseMessage* res_msg){
        return "ok";
    }

    void FunctionServer::addFunction(std::string functionName, FunctionType callbackFunction){
        this->_serverFunctions[functionName] = callbackFunction;
    }

    void FunctionServer::removeFunction(std::string functionName){
        this->_serverFunctions.erase(functionName);
    }

    void FunctionServer::start(){
        this->_server.start();
    }

}