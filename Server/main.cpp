#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UdpServer {
public:
    UdpServer(boost::asio::io_context& io_context, short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                this->handle_receive(ec, bytes_recvd);
            });
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error && bytes_transferred > 0) {
            std::cout << "Received message: " << std::string(recv_buffer_.data(), bytes_transferred) << std::endl;
        }
        start_receive();
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: UdpServer <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        UdpServer server(io_context, std::atoi(argv[1]));
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
