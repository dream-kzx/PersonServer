#ifndef PERSONSERVER_TOKENMANAGER_H
#define PERSONSERVER_TOKENMANAGER_H
#include <chrono>
#include <map>
#include <mutex>
#include <queue>
#include <string>

namespace lookupman {
class TokenManager {
  struct TokenMsg {
    std::string account_Number;
    std::chrono::nanoseconds generate_time;
  };

 public:
  explicit TokenManager(const std::chrono::minutes& timeout)
      : timeout_(timeout), status_(true) {}
  ~TokenManager() { status_ = false; }

  void PutToken(const std::string& token, const std::string& account_number,
                const std::chrono::nanoseconds& generate_time);
  void DeleteToken(const std::string& token);
  std::string GetUserInToken(const std::string& token);
  void CheckToken();

 private:
  std::map<std::string, TokenMsg> tokens_;
  std::queue<std::string> tokens_queue_;
  std::chrono::minutes timeout_;
  std::mutex token_mutex_;
  bool status_;

  void checkTimeout();
};
}  // namespace lookupman

#endif
