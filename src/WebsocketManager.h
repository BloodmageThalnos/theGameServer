#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "NetworkManager.h"

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

class WebsocketManager : public NetworkManager {
 public:
  WebsocketManager() : t_listen(nullptr) {}
  ~WebsocketManager() {
    if (t_listen) {
      delete (t_listen), t_listen = nullptr;
    }
  }
  virtual void startListen(unsigned short port);
  // void listen(unsigned short port, int threads);

  void onRead(const char* buf, int len) {
    std::cout << "Read :" << std::string(buf, len) << std::endl;
  };
  void doWrite(beast::flat_buffer){};

  // private:
  void doListen(net::io_context& ioc, tcp::endpoint endpoint,
                net::yield_context yield);
  void sockHandler(tcp::socket& sock, net::yield_context yield);
  std::thread* t_listen;
};