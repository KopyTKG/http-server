#ifndef HTTP_DEFAULTS_H
#define HTTP_DEFAULTS_H

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>
#include <sys/socket.h>
#include <unordered_map>

namespace HTTP {
typedef std::unordered_map<std::string, std::string> Headers;

auto notfound(int fd) -> void;
auto sendfile(int fd, std::string path, std::string file) -> void;
auto sendimage(int fd, std::string path, std::string file) -> void;

const std::string HEADERS = "Server: TheKrew\r\n\r\n";
const std::string WWW = "/var/www";

} // namespace HTTP

#endif // !HTTP_DEFAULTS_H
