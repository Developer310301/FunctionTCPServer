#include "TCPConnection.h"

namespace functionserver{

    TCPConnection::TCPConnection(io::ip::tcp::socket&& socket, std::ostream* output_stream, std::ostream* error_stream)
        : _socket(std::move(socket)), _outputStream(output_stream), _errorStream(error_stream){   
    }

    void TCPConnection::start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler){
        this->_messageHandler = std::move(messageHandler);
        this->_errorHandler = std::move(errorHandler);

        this->asyncRead();
    }

    void TCPConnection::post(const std::string& str){
        bool queueIdle = this->_outgoingMessages.empty();
        _outgoingMessages.push(str);

        if(queueIdle){
            asyncWrite();
        }
    }

    void TCPConnection::asyncRead(){
        async_read_until(this->_socket, this->_streamBuf, '\n',[this]
        (boost::system::error_code ec, size_t bytesTransferred){
            this->onRead(ec,bytesTransferred);
        });
    }

    void TCPConnection::onRead(boost::system::error_code ec, size_t bytesTransferred){

        if(ec){
            this->_socket.close(ec);
            this->_errorHandler();
            return;
        }
        std::stringstream msg;
        msg << std::istream(&this->_streamBuf).rdbuf();
        this->_streamBuf.consume(bytesTransferred);
        this->_messageHandler(this,msg.str());
        this->asyncRead();
    }

    void TCPConnection::asyncWrite(){
        async_write(this->_socket, buffer(this->_outgoingMessages.front()), [this]
        (boost::system::error_code ec, size_t bytesTransferred){
            this->onWrite(ec, bytesTransferred);
        });
    }

    void TCPConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred){

        if(ec){
            this->_socket.close(ec);
            this->_errorHandler();
            return;
        }

        this->_outgoingMessages.pop();

        if(!this->_outgoingMessages.empty()){
            this->asyncWrite();
        }

    }
}