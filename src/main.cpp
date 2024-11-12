#include "server.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <print>
#include <sstream>
#include <string>
#include <sys/socket.h>

using namespace std;

auto home(int fd) -> void;

auto main() -> int {
  HTTP::Server *server = HTTP::Server::getInstance(80);

  (*server).addRoute("/get", home);

  (*server).start();
  return 0;
}

auto home(int fd) -> void {
  HTTP::Server *server = HTTP::Server::getInstance();
  auto headers = (*server).getHeaders();

  if (headers.empty()) {
    close(fd);
    return;
  }

  std::string data =
      "<head></head><body><h1>" + headers["Method"] + "</h1></body>\r\n\r\n";

  std::stringstream res;

  res << "HTTP/1.1 200 OK\r\n";
  res << "Content-Type: text/html; charset=UTF-8\r\n";
  res << "Content-Length: " << data.length() << "\r\n";
  res << "\r\n";
  res << data;

  int bytesSent = send(fd, res.str().c_str(), res.str().length(), 0);
  if (bytesSent < 0) {
    std::println("Send failed");
    return;
  }
}
