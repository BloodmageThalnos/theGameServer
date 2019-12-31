#include "Server.h"
#include "WebsocketManager.h"

int main() {
  WebsocketManager wm;
  wm.startListen(18888);
  wm.wait();

  return 0;
}