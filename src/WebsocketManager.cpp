#include "WebsocketManager.h"

void fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

// opens a thread to listen for new connections from a port
void WebsocketManager::startListen(unsigned short port) {
  t_listen = new std::thread{[&]() { listen(port, 1); }};
}

// listen for new connections from a port, sync
void WebsocketManager::listen(unsigned short port, int threads) {
  try {
    const auto address = net::ip::make_address("0.0.0.0");

    net::io_context ioc{threads};
    net::spawn(ioc, std::bind(&WebsocketManager::doListen, this, std::ref(ioc),
                              tcp::endpoint{address, port}));
    ioc.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

// handles a new-come connection, i.e. assign a new port.
void WebsocketManager::doListen(net::io_context& ioc, tcp::endpoint endpoint) {
  std::cout << "WebsocketManager::doListen : Begin." << std::endl;
  tcp::acceptor acceptor(ioc);
  try {
    acceptor.open(endpoint.protocol());
    acceptor.set_option(net::socket_base::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen(net::socket_base::max_listen_connections);

    while (true) {
      tcp::socket sock(ioc);

      // acceptor.async_accept(sock, [](beast::error_code) {});
      std::cout << "WebsocketManager::doListen : before Accept." << std::endl;
      acceptor.accept(sock);
      boost::asio::spawn(
          acceptor.get_executor(),
          std::bind(&WebsocketManager::handle, this, std::move(sock)));
      std::cout << "WebsocketManager::doListen : after Accept." << std::endl;
    }
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  std::cout << "WebsocketManager::doListen : End." << std::endl;
}

void WebsocketManager::handle(tcp::socket& sock) {
  std::cout << "WebsocketManager::handle : Begin." << std::endl;
  auto ws = websocket::stream<beast::tcp_stream>(std::move(sock));
  ws.set_option(
      websocket::stream_base::timeout::suggested(beast::role_type::server));
  ws.set_option(
      websocket::stream_base::decorator([](websocket::response_type& res) {
        res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) +
                                         " websocket-server-coro");
      }));
  ws.async_accept([](beast::error_code) {});

  // for (;;) {
  // This buffer will hold the incoming message
  beast::flat_buffer buffer;

  // Read a message
  ws.async_read(buffer,
                [&buffer, this](beast::error_code const& err, std::size_t siz) {
                  onRead(beast::buffers_to_string(buffer.data()));
                });
  //}
  std::cout << "WebsocketManager::handle : End." << std::endl;
}