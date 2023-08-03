#include "FunctionServer.h"

namespace functionserver{

    FunctionServer::FunctionServer(unsigned short server_port, bool case_sensitive) : _server(TCPServer(IPV::V4, server_port)), _caseSensitive(case_sensitive){
        
        this->_regexReqMsg=std::regex(this->_REGEX_EXPR);

        this->_server.OnJoin = [this](TCPConnection::pointer conn){
            std::cout<<"[>] New client connected from " << conn->socket().remote_endpoint() << "!"<< std::endl;
        };

        this->_server.OnLeave = [this](TCPConnection::pointer conn){
            std::cout<<"[<] Client disconnected!"<< std::endl;
        };

        this->_server.OnClientMessage = [this](TCPConnection* conn, std::string msg){

            msg.erase(std::remove( msg.begin(), msg.end(), '\n'), msg.end());
            msg.erase(std::remove( msg.begin(), msg.end(), '\r'), msg.end());

            std::cout<<"[-] Receiving from client " << conn->socket().remote_endpoint() << " : " << msg << std::endl;

            FunctionRequestMessage req_msg;
            FunctionResponseMessage res_msg;

            if(this->_parseRequestMessage(msg,&req_msg)){

                res_msg.errorCode = -1000;
                res_msg.functionName = req_msg.functionName;
                res_msg.params = req_msg.params;

                if(!this->_caseSensitive){
                    std::transform(req_msg.functionName.begin(), req_msg.functionName.end(), req_msg.functionName.begin(), ::toupper);
                }

                auto it = this->_serverFunctions.find(req_msg.functionName);

                if(it!=this->_serverFunctions.end()){
                    res_msg.errorCode = 0;
                    (it->second)(&req_msg,&res_msg);
                }

                conn->post(this->_buildResponseMessage(&res_msg)+"\r\n");
            }else{
                conn->post("-1000,{},CommandMalformed()\r\n");
            }
        };

    }

    bool FunctionServer::_parseRequestMessage(std::string msg, FunctionRequestMessage* req_msg){
        std::smatch m;
        if(std::regex_search(msg, m, this->_regexReqMsg)){
            //std::cout<<"[-] Parsing..."<<std::endl;
            
            req_msg->functionName = m.str(1);
            //std::cout<<"[-] \tFunction Name: "<<req_msg->functionName<<std::endl;
            if(m.size()>1 && m.str(2)!=""){
                //std::cout<<"[-] \tParams:"<<std::endl;
                
                boost::split(req_msg->params, m.str(2), [](char c){return c == ',';});

                /*for(int i=0;i<req_msg->params.size();i++){
                    std::cout<<"[-]\t\t"<<req_msg->params[i]<<std::endl;
                }*/

            }
            return true;
        }else{
            return false;
        }

    }

    std::string FunctionServer::_buildResponseMessage(FunctionResponseMessage* res_msg){
        const auto paramList = std::accumulate(begin(res_msg->params), end(res_msg->params), std::string{}, [](const std::string& so_far, const auto& next) {
            return so_far + (so_far.empty() ? "" : ",") + std::string(next);
        });
        const auto returnList = std::accumulate(begin(res_msg->returnValues), end(res_msg->returnValues), std::string{}, [](const std::string& so_far, const auto& next) {
            return so_far + (so_far.empty() ? "" : ",") + std::string(next);
        });
        return std::string("") + std::to_string(res_msg->errorCode) + ",{" + returnList  + "}," + res_msg->functionName + "(" + paramList + ")";
    }

    void FunctionServer::addFunction(std::string functionName, FunctionType callbackFunction){
        if(!this->_caseSensitive){
            std::transform(functionName.begin(), functionName.end(), functionName.begin(), ::toupper);
        }
        this->_serverFunctions[functionName] = callbackFunction;
    }

    void FunctionServer::removeFunction(std::string functionName){
        if(!this->_caseSensitive){
            std::transform(functionName.begin(), functionName.end(), functionName.begin(), ::toupper);
        }
        this->_serverFunctions.erase(functionName);
    }

    void FunctionServer::start(){
        this->_server.start();
    }

}