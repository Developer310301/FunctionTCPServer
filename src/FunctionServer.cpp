#include "FunctionServer.h"

namespace functionserver{
    void FunctionServer::startAccept() {
        tcp::socket socket(acceptor_.get_executor());
        acceptor_.async_accept(socket, [this, &socket](const boost::system::error_code& error) {
            if (!error) {
                handleClient(std::move(socket));
            }
            startAccept();
        });
    }

    void FunctionServer::handleClient(tcp::socket socket) {
        try {
            *this->output_stream << "Client connected: " << socket.remote_endpoint() << std::endl;

            // Handle client communication here
            // Example: echo back received data
            char data[1024];
            size_t dataSize = socket.read_some(buffer(data));
            socket.write_some(buffer(data, dataSize));
        }
        catch (std::exception& e) {
            *this->error_stream << "Exception in handleClient: " << e.what() << std::endl;
        }

        *this->output_stream << "Client disconnected: " << socket.remote_endpoint() << std::endl;
    }
}