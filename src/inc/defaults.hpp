#ifndef HTTP_DEFAULTS_H
#define HTTP_DEFAULTS_H

#include <cstdio>
#include <cstring>
#include <fstream>
#include <print>
#include <sstream>
#include <sys/socket.h>
#include <filesystem>


namespace HTTP {
auto notfound(int fd) -> void;
auto sendfile(int fd, std::string path, std::string file) -> void;
auto sendimage(int fd, std::string path, std::string file) -> void;

const std::string HEADERS = "Server: TheKrew\r\n\r\n";
const std::string WWW = "/home/kopy/Dokumenty/lab/www"; 

} // namespace HTTP

#endif // !HTTP_DEFAULTS_H
