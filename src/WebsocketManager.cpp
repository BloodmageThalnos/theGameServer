#include "WebsocketManager.h"
#include "Log.h"

// opens a thread to listen for new connections from a port
void WebsocketManager::startListen(unsigned short port) {
  t_listen = new std::thread{[port, this]() {
    try {
      net::io_context ioc{1};
      auto endpoint = tcp::endpoint{net::ip::make_address("0.0.0.0"), port};
      net::spawn(ioc,
                 std::bind(&WebsocketManager::doListen, this, std::ref(ioc),
                           endpoint, std::placeholders::_1));
      ioc.run();
      LOG("Listen thread quited.");
    } catch (const std::exception& e) {
      ERROR(e.what());
    }
  }};
}

// handles a new-come connection, i.e. assign a new port.
void WebsocketManager::doListen(net::io_context& ioc, tcp::endpoint endpoint,
                                net::yield_context yield) {
  tcp::acceptor acceptor(ioc);
  try {
    acceptor.open(endpoint.protocol());
    acceptor.set_option(net::socket_base::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen(net::socket_base::max_listen_connections);
    LOG("Listening on " << endpoint.address().to_string() << ":"
                        << endpoint.port() << ". ");

    while (true) {
      // boost::system::error_code ec;
      tcp::socket sock(ioc);
      acceptor.async_accept(sock, yield);
      LOG("Accepted new connection.");
      boost::asio::spawn(acceptor.get_executor(),
                         std::bind(&WebsocketManager::sockHandler, this,
                                   std::move(sock), std::placeholders::_1));
    }
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void WebsocketManager::sockHandler(tcp::socket& sock,
                                   net::yield_context yield) {
  auto ws = websocket::stream<beast::tcp_stream>(std::move(sock));
  boost::system::error_code ec;
  ws.set_option(
      websocket::stream_base::timeout::suggested(beast::role_type::server));
  ws.set_option(
      websocket::stream_base::decorator([](websocket::response_type& res) {
        res.set(http::field::server, "the-game-server");
      }));
  ws.async_accept(yield[ec]);
  if (ec) {
    ERROR("Accept websocket connection failed.");
  }

  for (;;) {
    beast::flat_buffer buffer;
    ws.async_read(buffer, yield[ec]);
    if (ec) {
      if (ec == websocket::error::closed) {
        return;
      }
    }
    onRead(net::buffer_cast<const char*>(buffer.data()), buffer.size());
  }
}