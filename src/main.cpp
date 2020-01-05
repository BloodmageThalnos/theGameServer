#include "Server.h"
#include "WebsocketManager.h"

int main() {
  WebsocketManager wm;
  wm.startListen(20011);
  wm.t_listen->join();

  return 0;
}