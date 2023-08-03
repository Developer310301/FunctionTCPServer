#ifndef FUNCTIONMESSAGES_H
#define FUNCTIONMESSAGES_H

#include <string>
#include <vector>

namespace functionserver{

    struct FunctionRequestMessage{
        std::string functionName="";
        std::vector<std::string> params;
    };

    struct FunctionResponseMessage{
        int errorCode=-1;
        std::vector<std::string> returnValues;
        std::string functionName="";
        std::vector<std::string> params;
    };

};

#endif