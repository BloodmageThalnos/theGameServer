#include "Server.h"

const unsigned short kServerPort = 18888;

void Server::init() {}

void Server::run() { networkManager->startListen(kServerPort); }

int Server::newClient() {
  int id = generateClientId();
  assert(clients.find(id) == clients.end());
  Client& client = clients[id] = Client();
  client.id = id;
  client.state = ClientState::kConnecting;
  return id;
}

void Server::processMessage(int cid, Message msg) {
  switch (msg.type) {
    case MessageType::kHello: {
      HelloMessage hMsg(msg.buf, msg.len);
      Client& client = clients[cid];
      client.name = hMsg.name;
      client.state = ClientState::kWaiting;
    } break;
    default:
      break;
  }
}

// should generate a unique id for each client
int Server::generateClientId() {
  static int id = 1;
  return id++;
}