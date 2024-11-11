#include "server.hpp"
#include <cstdio>
#include <cstring>


using namespace std;


auto main() -> int {
  HTTP::Server server = HTTP::Server(80);

  server.start();
  return 0;
}


