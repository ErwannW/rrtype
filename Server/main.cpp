#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <set>

using boost::asio::ip::udp;

class UDPServer {
public:
    UDPServer(boost::asio::io_context& io_context, short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)),
          timer_(io_context, boost::posix_time::seconds(0)) {

        startReceive();
        startTimer();
    }

private:
    void startReceive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&UDPServer::handleReceive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void handleReceive(const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
        if (!error) {
            std::cout << "New client connected from: " << remote_endpoint_ << std::endl;
            clients_.insert(remote_endpoint_);  // Add the client to the set of clients
            startReceive();
        } else {
            std::cerr << "Error receiving: " << error.message() << std::endl;
        }
    }

    void startTimer() {
        timer_.async_wait(boost::bind(&UDPServer::handleSend, this));
    }

    void handleSend() {
        std::string current_time = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());

        for (const auto& client : clients_) {
            std::cout << "Sending to client " << client << std::endl;
            socket_.send_to(boost::asio::buffer(current_time), client, 0);
        }

        // Restart the timer to send the next message in 3 seconds
        timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(3));
        startTimer();
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::set<udp::endpoint> clients_;  // Store client endpoints
    boost::asio::deadline_timer timer_;
    std::array<char, 1> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        UDPServer server(io_context, 8080);  // UDP Server on port 8080
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
