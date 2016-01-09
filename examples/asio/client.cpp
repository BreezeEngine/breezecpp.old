//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <asio.hpp>
#include <thread>
#include <chrono>
#include <fstream>
#include <msgpack.hpp>
#include <network/network.hpp>

using asio::ip::udp;
struct foo
{
  // friend class boost::serialization::access;
  int i;
  int i2;
};

int
main(int argc, char* argv[])
{
  foo f{ 10, 2 };
  std::stringstream buffer;
  int i = 10;
  std::vector<int> v{ 1, 2, 3, 4, 5, 10000000 };
  std::vector<int> v2;
  msgpack::type::tuple<int> src(f.i);
  msgpack::pack(buffer, v);
  buffer.seekg(0);

  std::stringstream ss;
  int testi = 42;

  msgpack::pack(ss, testi);
  std::string datas(ss.str());
  breeze::network::net_vector<int> nv;
  nv.resize(20);

  // ss.seekg(0, std::ios::end);
  nv.insert_bytes_at(0, datas.data(), datas.size());

  std::cout << nv[0] << std::endl;
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    asio::io_service io_service;

    udp::resolver resolver(io_service);
    udp::endpoint receiver_endpoint =
      *resolver.resolve(udp::v4(), argv[1], "1234");

    udp::socket socket(io_service);
    socket.open(udp::v4());
    std::array<char, 1> send_buf = { { 0 } };
    socket.send_to(asio::buffer(buffer.str()), receiver_endpoint);
    for (;;) {
      std::this_thread::sleep_for(std::chrono::seconds(1));

      std::array<char, 128> recv_buf;
      udp::endpoint sender_endpoint;
      auto len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
      std::cout.write(recv_buf.data(), len);
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
