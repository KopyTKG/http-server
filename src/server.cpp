#include "inc/server.hpp"
#include "inc/defaults.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace HTTP {

Server::Server(int port) {
  // create ip+port struct
  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  // creating empty socket
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (this->server_fd == -1) {
    printf("Socket creation failed: %s...\n", strerror(errno));
    exit(1);
  }

  // safty in case of used ip+port
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) <
      0) {
    printf("SO_REUSEPORT failed: %s \n", strerror(errno));
    exit(1);
  }

  // binding empty socket to ip and port
  if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("Bind failed: %s \n", strerror(errno));
    exit(1);
  }

  // max backlog setting
  int connection_backlog = 5;

  if (listen(server_fd, connection_backlog) != 0) {
    printf("Listening faild: %s \n", strerror(errno));
  }

  ip = "0.0.0.0:" + std::to_string(port);
  addRoute("/404", notfound);
}

Server::~Server() { close(Server::server_fd); }

// Public methods

auto Server::addRoute(const std::string &path, std::function<void(int)> handler)
    -> void {
  routes[path] = handler;
}

auto Server::start() -> void {
  std::printf("Starting server on %s \n", ip.c_str());
  while (1) {

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                           (socklen_t *)&client_addr_len);

    if (client_fd == -1) {
      std::printf("Failed to connect: %s \n", strerror(errno));
      continue;
    }

    std::thread(&Server::handleRequest, this, client_fd).detach();
  }
}

// Private methods

auto Server::handleRequest(int fd) const -> void {
  char readBuffer[1024];
  int bytesReceived = recv(fd, readBuffer, sizeof(readBuffer) - 1,
                           0); // leave room for null terminator
  if (bytesReceived == -1) {
    std::printf("Receiving failed: %s \n", strerror(errno));
    exit(1);
  }

  // Null-terminate the buffer to ensure it's a valid C-string
  readBuffer[bytesReceived] = '\0';

  auto headers = getHeaders(readBuffer);

  std::vector<std::string> fileTypes = {".html", ".css", ".htm", ".js"};
  std::vector<std::string> imageTypes = {".png", ".jpg"};

  std::unordered_map<std::string, std::string> fileToContent;
  fileToContent["js"] = "javascript";
  fileToContent["htm"] = "html";
  fileToContent["html"] = "html";
  fileToContent["css"] = "css";

  auto item = routes.find(headers["Path"]);
  if (item != routes.end()) {
    item->second(fd);
  } else {
    bool fileHandled = false;

    std::string path = headers["Path"];
    if (path == "/" || path[path.length() - 1] == (char)'/') {
      path += "index.html";
    }

    // Check file extensions
    for (const auto &ext : fileTypes) {
      if (path.size() >= ext.size() &&
          path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
        std::string extWithoutDot = ext.substr(1); // Remove the leading dot
        sendfile(fd, path,
                 fileToContent[extWithoutDot]); // Pass the content type
        fileHandled = true;
        break;
      }
    }

    // Check image file types
    if (!fileHandled) {
      for (const auto &ext : imageTypes) {
        if (path.size() >= ext.size() &&
            path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
          std::string extWithoutDot = ext.substr(1); // Remove the leading dot
          sendimage(fd, path, extWithoutDot);        // Pass the content type
          fileHandled = true;
          break;
        }
      }
    }

    // If no file or handler was found, serve 404
    if (!fileHandled) {
      if (routes.find("/404") != routes.end()) {
        routes.at("/404")(fd);
      } else {
        std::printf("404 handler not found, closing connection.\n");
      }
    }
  }

  close(fd);
}

auto Server::getHeaders(char buffer[1024]) const
    -> std::unordered_map<std::string, std::string> {
  std::unordered_map<std::string, std::string> headers;
  std::string request(buffer);
  std::istringstream requestStream(request);
  std::string line;
  bool firstLine = true;

  // Parsing the first line (Method, Path, Protocol)
  if (std::getline(requestStream, line)) {
    std::istringstream lineStream(line);
    std::string method, path, protocol;
    lineStream >> method >> path >> protocol;
    headers["Method"] = method;
    headers["Path"] = path;
    headers["Protocol"] = protocol;
  }

  // Parsing subsequent headers
  while (std::getline(requestStream, line)) {
    // Skip empty lines (they should be at the end of headers)
    if (line.empty()) {
      continue;
    }

    // Find the first colon (the delimiter between key and value)
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos) {
      std::string key = line.substr(0, colonPos);
      std::string value = line.substr(colonPos + 1);

      // Remove leading spaces in the value
      value.erase(0, value.find_first_not_of(" \t"));

      // Add to the map
      headers[key] = value;
    }
  }
  return headers;
}

} // namespace HTTP
