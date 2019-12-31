#include "WebsocketManager.h"

void fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

void WebsocketManager::startListen(unsigned short port, int threads) {
  t_listen = new std::thread{std::bind(listen, port, threads)};
}

// thread for each connection
void WebsocketManager::listen(unsigned short port, int threads) {
  try {
    const auto address = net::ip::make_address("0.0.0.0");

    net::io_context ioc{threads};
    tcp::endpoint endpoint{address, port};
    tcp::acceptor acceptor{ioc, endpoint};

    net::spawn(ioc, [&ioc, endpoint, this]() {
      tcp::acceptor acceptor(ioc);
      try {
        acceptor.open(endpoint.protocol());

        // Allow address reuse
        acceptor.set_option(net::socket_base::reuse_address(true));

        // Bind to the server address
        acceptor.bind(endpoint);

        // Start listening for connections
        acceptor.listen(net::socket_base::max_listen_connections);

        while (true) {
          tcp::socket socket(ioc);
          acceptor.async_accept(socket);
          net::spawn(acceptor.get_executor(), [&socket, this]() {
            auto ws = websocket::stream<beast::tcp_stream>{socket};
            ws.set_option(websocket::stream_base::timeout::suggested(
                beast::role_type::server));

            // Set a decorator to change the Server of the handshake
            ws.set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res) {
                  res.set(http::field::server,
                          std::string(BOOST_BEAST_VERSION_STRING) +
                              " websocket-server-coro");
                }));

            // Accept the websocket handshake
            ws.async_accept([](beast::error_code) {});

            for (;;) {
              // This buffer will hold the incoming message
              beast::flat_buffer buffer;

              // Read a message
              ws.async_read(buffer, [&buffer, this](beast::error_code) {
                onRead(beast::buffers_to_string(buffer.data()));
              });
            }
          });
        }
      } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    });
    ioc.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
