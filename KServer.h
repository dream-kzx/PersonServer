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

#include <map>
#include <string>

#include "TokenManager.h"
#include "httplib.h"

namespace lookupman {

using Func = std::function<void()>;

class KServer {
 public:
  KServer(const std::string& address, int port,
          std::chrono::minutes token_timeout)
      : address_(address), port_(port), token_manager_(token_timeout) {
    token_manager_.CheckToken();
  }

  ~KServer() { server_.stop(); }

  void Get(const std::string& url_path, httplib::Server::Handler handler) {
    server_.Get(url_path.c_str(), handler);
  }

  void Post(const std::string& url_path, httplib::Server::Handler handler) {
    server_.Post(url_path.c_str(), handler);
  }

  bool Listen() { return server_.listen(address_.c_str(), port_); }

  void PutToken(std::string token, std::string account_number,
                std::chrono::nanoseconds generate_time) {
    token_manager_.PutToken(token, account_number, generate_time);
  }

  void DeleteToken(std::string token) { token_manager_.DeleteToken(token); }

  const std::string GetUserInToken(std::string token) {
    return token_manager_.GetUserInToken(token);
  }

 private:
  httplib::Server server_;
  std::string address_;
  int port_;

  TokenManager token_manager_;
};
}  // namespace lookupman

#endif