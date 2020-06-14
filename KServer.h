// KServer.h
//此类是构建整个后台服务的类，包含了http服务的开启、POST请求和GET请求注册、token的管理等功能
//

#ifndef PERSONSERVER_KSERVER_H
#define PERSONSERVER_KSERVER_H

#include <string>

#include "TokenManager.h"
#include "httplib.h"

namespace lookupman {

using Func = std::function<void()>;

class KServer {
 public:
  KServer(const std::string& address, int port,
          const std::chrono::minutes& token_timeout)
      : address_(address),
        port_(port),
        staff_token_manager_(token_timeout),
        admin_token_manager_(token_timeout) {
    staff_token_manager_.CheckToken();
    admin_token_manager_.CheckToken();
  }

  ~KServer() { server_.stop(); }

  void Get(const std::string& url_path, httplib::Server::Handler handler) {
    server_.Get(url_path.c_str(), handler);
  }

  void Post(const std::string& url_path, httplib::Server::Handler handler) {
    server_.Post(url_path.c_str(), handler);
  }

  bool Listen() { return server_.listen(address_.c_str(), port_); }

  void PutTokenInStaff(const std::string& token,
                       const std::string& account_number,
                       const std::chrono::nanoseconds& generate_time) {
    staff_token_manager_.PutToken(token, account_number, generate_time);
  }

  void DeleteTokenInStaff(const std::string& token) {
    staff_token_manager_.DeleteToken(token);
  }

  const std::string GetUserInTokenInStaff(const std::string& token) {
    return staff_token_manager_.GetUserInToken(token);
  }

  void PutTokenInAdmin(const std::string& token,
                       const std::string& account_number,
                       const std::chrono::nanoseconds& generate_time) {
    admin_token_manager_.PutToken(token, account_number, generate_time);
  }

  void DeleteTokenInAdmin(const std::string& token) {
    admin_token_manager_.DeleteToken(token);
  }

  const std::string GetUserInTokenInAdmin(const std::string& token) {
    return admin_token_manager_.GetUserInToken(token);
  }

 private:
  httplib::Server server_;
  std::string address_;
  int port_;

  TokenManager staff_token_manager_;
  TokenManager admin_token_manager_;
};
}  // namespace lookupman

#endif