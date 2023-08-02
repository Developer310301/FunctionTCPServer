#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <iostream>
#include <boost/asio.hpp>
#include <functional>
#include <queue>

namespace functionserver{

    using namespace boost::asio;
    using boost::asio::ip::tcp;
    namespace io = boost::asio;

    class TCPConnection{

        using MessageHandler = std::function<void(TCPConnection*, std::string)>;
        using ErrorHandler = std::function<void()>;

        private:
            tcp::socket _socket;
            std::string _message;
            std::ostream* _outputStream;
            std::ostream* _errorStream;

            std::queue<std::string> _outgoingMessages;
            io::streambuf _streamBuf {65536};

            MessageHandler _messageHandler;
            ErrorHandler _errorHandler;
        
        public:
            using pointer = std::shared_ptr<TCPConnection>;

            tcp::socket& socket(){ return this->_socket; }    
            std::string message(){ return this->_message; }


        private:

            explicit TCPConnection(io::ip::tcp::socket&& socket,std::ostream* output_stream, std::ostream* error_stream);

            void asyncRead();
            void onRead(boost::system::error_code ec, size_t bytesTransferred);
            
            void asyncWrite();
            void onWrite(boost::system::error_code ec, size_t bytesTransferred);

        
        public:
            void start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
            void post(const std::string& str);
            static pointer createTCPConnection(io::ip::tcp::socket&& socket, std::ostream* output_stream = &std::cout, std::ostream* error_stream = &std::cerr) {
                return pointer(new TCPConnection(std::move(socket), output_stream, error_stream));
            }


    };

}

#endif