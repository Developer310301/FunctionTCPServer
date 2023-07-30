#ifndef FUNCTION_SERVER_H
#define FUNCTION_SERVER_H

#include <iostream>
#include <boost/asio.hpp>

namespace functionserver{

    using namespace boost::asio;
    using namespace boost::asio::ip;

    class FunctionServer {

        private:

            ip::tcp::acceptor acceptor_;
            std::ostream* output_stream;
            std::ostream* error_stream;

            void startAccept();
            void handleClient(tcp::socket socket);

        public:
            FunctionServer(io_service& ioService, unsigned short port, std::ostream* output_stream = &std::cout, std::ostream* error_stream = &std::cerr)
                : acceptor_(ioService, ip::tcp::endpoint(ip::tcp::v4(), port)), output_stream(output_stream), error_stream(error_stream){
                startAccept();
            }
            
    };

}

#endif