#ifndef FUNCTIONMESSAGES_H
#define FUNCTIONMESSAGES_H

#include <string>

namespace functionserver{

    struct FunctionRequestMessage{

        std::string functionName="";
        uint64_t param_number = 0;
        std::string* params=nullptr;

        ~FunctionRequestMessage(){
            if(params)
                delete[] params;
        }

    };

    struct FunctionResponseMessage{

        int errorCode=-1;
        std::string* returnValues=nullptr;
        std::string functionName="";
        std::string* params=nullptr;

        ~FunctionResponseMessage(){
            if(returnValues)
                delete[] returnValues;
            if(params)
                delete[] params;
        }

    };

};

#endif