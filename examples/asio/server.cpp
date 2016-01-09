
//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <asio.hpp>
#include <thread>
#include <chrono>
#include <async++.h>
#include <mutex>

#include <network/network.hpp>

using asio::ip::udp;


int
main()
{
  try {
    asio::io_service io_service;
    breeze::network::server s{ io_service };
    s.start_sending();
    io_service.run();
    // s.start_sending();
    // udp::socket socket(io_service, udp::endpoint(udp::v4(), 1234));
    // std::vector<udp::endpoint> endpoints;
    // for (;;) {
    //  boost::array<char, 1> recv_buf;
    //  udp::endpoint remote_endpoint;
    //  asio::error_code error;
    //  socket.async_receive_from(asio::buffer(recv_buf), remote_endpoint, 0,
    //                            [](auto& a, auto b) {

    //                            });
    //  endpoints.push_back(remote_endpoint);

    //  if (error && error != asio::error::message_size)
    //    throw asio::system_error(error);

    //  std::string message = make_daytime_string();

    //  for (auto& ep : endpoints) {
    //    asio::error_code ignored_error;
    //    socket.send_to(asio::buffer(message), ep, 0, ignored_error);
    //  }
    //}
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
