#include "server/TCPServer.h"

namespace functionserver{

    TCPServer::TCPServer(IPV ipv, unsigned short port, std::ostream* output_stream, std::ostream* error_stream) 
        : _ipVersion(ipv), _port(port), _outputStream(output_stream), _errorStream(error_stream),
        _acceptor(tcp::acceptor(this->_ioContext, tcp::endpoint(ipv==IPV::V4 ? tcp::v4() : tcp::v6(), this->_port))){
    }

    int TCPServer::start(){
        try{
            startAccept();
            this->_ioContext.run();
        }catch(std::exception e){
            *this->_errorStream << "Error while starting server: " << e.what() << std::endl;
            return -1;
        }
        return 0;
    }

    void TCPServer::startAccept(){
        this->_socket.emplace(this->_ioContext);
        using namespace std::placeholders;
        // create a connection
        
        //async accept the connection
        
        this->_acceptor.async_accept(*this->_socket, [this](const boost::system::error_code& error){
            
            auto conn = TCPConnection::createTCPConnection(std::move(*this->_socket), this->_outputStream, this->_errorStream);

            this->_connections.insert(conn);

            if(this->OnJoin){
                OnJoin(conn);
            }

            if(!error){
                conn->start(
                    [this](TCPConnection* conn, const std::string& message) { if (OnClientMessage) OnClientMessage(conn, message); },
                    [&, weak =std::weak_ptr(conn)] {
                        if (auto shared = weak.lock(); shared && this->_connections.erase(shared)) {
                            if (OnLeave) OnLeave(shared);
                        }
                    }
                );
            }

            this->startAccept();
        });
    }
}