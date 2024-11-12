#include "inc/defaults.hpp"

namespace HTTP {
auto notfound(int fd) -> void {
  std::string res = "HTTP/1.1 404 Not Found\r\n\r\n";
  int bytesSent = send(fd, res.c_str(), std::strlen(res.c_str()), 0);
  if (bytesSent < 0) {
    std::println("Send failed");
    return;
  }
}

auto sendfile(int fd, std::string path, std::string file) -> void {

  std::string fullpath = WWW + path;

  auto fileExist = std::filesystem::exists(fullpath);
  if (!fileExist) {
    notfound(fd);
    return;
  }
  std::ifstream index(fullpath);
  std::stringstream buffer;
  buffer << index.rdbuf();

  std::string data = buffer.str();
  std::stringstream res;

  // Build the HTTP response
  res << "HTTP/1.1 200 OK\r\n";
  res << "Content-Type: text/" + file + "; charset=UTF-8\r\n";
  res << "Content-Length: " << data.length() << "\r\n";
  res << "\r\n"; // Add an extra blank line separating headers and body
  res << data;   // Add the body of the response

  int bytesSent = send(fd, res.str().c_str(), res.str().length(), 0);
  if (bytesSent < 0) {
    std::println("Send failed");
    return;
  }
}

auto sendimage(int fd, std::string path, std::string file) -> void {

  std::string fullpath = WWW + path;

  std::ifstream f(fullpath, std::ios::binary);

  if (!f.is_open()) {
    notfound(fd);
    return;
  }
  std::ifstream index(fullpath);
  std::stringstream buffer;
  buffer << index.rdbuf();

  std::string data = buffer.str();
  std::stringstream res;

  // Build the HTTP response
  res << "HTTP/1.1 200 OK\r\n";
  res << "Content-Type: image/" + file + "; charset=UTF-8\r\n";
  res << "Content-Length: " << data.length() << "\r\n";
  res << "Connection: close\r\n\r\n"; // Add an extra blank line separating
                                      // headers and body

  int bytesSent = send(fd, res.str().c_str(), res.str().length(), 0);
  if (bytesSent < 0) {
    std::println("Send failed");
    return;
  }

  bytesSent = send(fd, data.c_str(), data.size(), 0);
  if (bytesSent < 0) {
    std::println("Send failed");
    return;
  }
}
} // namespace HTTP
