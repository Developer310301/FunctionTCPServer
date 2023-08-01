#ifndef FUNCTION_SERVER_H
#define FUNCTION_SERVER_H

#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>
#include <optional>
#include <boost/asio.hpp>
#include "TCPConnection.h"

namespace functionserver{

    
    using namespace boost::asio;
    using namespace boost::asio::ip;
    namespace io = boost::asio;

    enum IPV{
        V4,
        V6
    };

    class FunctionServer {

        using OnJoinHandler = std::function<void(TCPConnection::pointer)>;
        using OnLeaveHandler = std::function<void(TCPConnection::pointer)>;
        using OnClientMessageHandler = std::function<void(TCPConnection*, std::string)>;

        private:

            IPV _ipVersion;
            unsigned short _port;
            std::ostream* _outputStream;
            std::ostream* _errorStream;
            io_context _ioContext;
            tcp::acceptor _acceptor;
            std::optional<io::ip::tcp::socket> _socket;
            std::unordered_set<TCPConnection::pointer> _connections{};

        public:
            OnJoinHandler OnJoin;
            OnLeaveHandler OnLeave;
            OnClientMessageHandler OnClientMessage;           

        private:
            void startAccept();

        public:
            FunctionServer(IPV ipv=V4, unsigned short port=6789, std::ostream* output_stream = &std::cout, std::ostream* error_stream = &std::cerr);
            int start();
    };

}

#endif