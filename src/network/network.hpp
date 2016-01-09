#ifndef NETWORK_H
#define NETWORK_H

#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <thread>
#include <chrono>
#include <async++.h>
#include <mutex>
#include <chrono>
#include <msgpack.hpp>

using asio::ip::udp;
namespace breeze {
namespace network {

std::string
make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}
class remote_connection
{
public:
  udp::endpoint endpoint;
  std::chrono::time_point<std::chrono::system_clock> last_received_packet;
  remote_connection(udp::endpoint endpoint,
                    std::chrono::time_point<std::chrono::system_clock> time)
    : endpoint(endpoint)
    , last_received_packet(time)
  {
  }
  bool operator==(const remote_connection& rc)
  {
    return rc.endpoint == endpoint;
  }
};

class server
{
public:
  udp::socket socket;
  std::mutex m;
  std::vector<remote_connection> remote_connections;

  server(asio::io_service& service)
    : socket(service, udp::endpoint(udp::v4(), 1234))
  {
    add_connection();
  }

  void add_connection()
  {
    async::spawn([this]() {
      udp::endpoint remote_endpoint;
      std::array<char, 100> recv_buf;
      socket.receive_from(asio::buffer(recv_buf), remote_endpoint, 0);
      m.lock();
      remote_connections.push_back(
        remote_connection{ remote_endpoint, std::chrono::system_clock::now() });
      m.unlock();
      msgpack::unpacked result;
      msgpack::unpack(result, recv_buf.data(), recv_buf.size());
      auto obj = result.get();
      std::cout << obj << std::endl;
      //      int dst;
      //      obj.convert(&dst);
      //      std::cout << "Connection: "  << std::endl;
      //      std::cout << "Connection: " << dst << std::endl;
      this->add_connection();
    });
  }
  void start_sending()
  {
    for (;;) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      for (auto& rc : remote_connections) {
        std::string message = make_daytime_string();
        asio::error_code ignored_error;
        socket.send_to(asio::buffer(message), rc.endpoint, 0, ignored_error);
      }
    }
  }
};

struct remote_vector
{
  virtual void insert_bytes_at(std::size_t pos, const char* bytes,
                               std::size_t size) = 0;
  virtual ~remote_vector(){}
};

template <class T>
class net_vector : public std::vector<T>, public remote_vector
{
public:
  virtual void insert_bytes_at(std::size_t pos, const char* bytes,
                               std::size_t size) override
  {
    msgpack::unpacked result;
    msgpack::unpack(result, bytes, size);
    auto obj = result.get();
    T t;
    obj.convert(t);
    this->at(pos) = t;
  }
};
}
}
#endif // NETWORK_H
