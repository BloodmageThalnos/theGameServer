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
  void startListen(unsigned short port, int threads = 1);

  void wait(){
    if(t_listen){
      t_listen->join();
    }
  }

  void onRead(std::string const str) {
    std::cout << "Read :" << str << std::endl;
  };
  void doWrite(beast::flat_buffer){};

 private:
  void listen(unsigned short port, int threads);
  std::thread* t_listen;
};