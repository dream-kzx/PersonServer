#ifndef PERSONSERVER_KSERVER_H
#define PERSONSERVER_KSERVER_H
// using namespace httplib;

// Server server;
// server.Get("/hi", [](const Request& req, Response& res) {
//  res.set_content("Hello World!", "text/plain");
//});

// server.Get("/stop",
//           [&](const Request& req, Response& res) { server.stop(); });
// server.listen("localhost", 1234);
//

#include <httplib.h>
#include <rapidjson/document.h>
#include <string>

namespace lookupman {

using Func = std::function<void()>;

class KServer {
 public:
  KServer(const std::string& address, int port)
      : address_(address), port_(port) {}

  ~KServer() { server_.stop(); }

  void Get(const std::string& url_path, httplib::Server::Handler handler) {
    server_.Get(url_path.c_str(), handler);
  }

  void Post(const std::string& url_path, httplib::Server::Handler handler) {
    server_.Post(url_path.c_str(), handler);
  }

  bool Listen() { return server_.listen(address_.c_str(), port_); }

 private:
  httplib::Server server_;
  std::string address_;
  int port_;


};
}  // namespace lookupman

#endif