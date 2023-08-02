#ifndef FUNCTIONSERVER_H
#define FUNCTIONSERVER_H

#include <unordered_map>
#include <functional>
#include <regex>

#include "server/TCPServer.h"
#include "FunctionMessages.h"

namespace functionserver{

    class FunctionServer{

        using FunctionType = std::function<void(FunctionRequestMessage*,FunctionResponseMessage*)>;

        private:
            //const std::string REGEX_EXPR = "([a-zA-Z0-9]+)\\(([a-zA-Z0-9\\s]*(\\s*,\\s*[a-zA-Z0-9\\s]+)*)\\)";
            const std::string REGEX_EXPR = "^([a-zA-Z0-9]+)$";

            TCPServer _server;
            std::unordered_map<std::string, FunctionType> _serverFunctions;
            std::regex _regexReqMsg;

        private:
            void _parseRequestMessage(std::string msg,FunctionRequestMessage* req_msg);
            std::string _buildResponseMessage(FunctionResponseMessage* res_msg);

        public:
            FunctionServer(unsigned short server_port);
            void addFunction(std::string functionName, FunctionType callbackFunction);
            void removeFunction(std::string functionName);
            void start();

    };

}

#endif