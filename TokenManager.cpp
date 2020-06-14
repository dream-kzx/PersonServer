#include "TokenManager.h"

#include <future>

#include "Tools.h"

namespace lookupman {
void TokenManager::PutToken(const std::string& token, const std::string& account_number,
                            const std::chrono::nanoseconds& generate_time) {
  TokenMsg token_msg = {account_number, generate_time};
  {
    std::lock_guard<std::mutex> token_lock(token_mutex_);
    tokens_queue_.push(token);
    tokens_[token] = token_msg;
  }
}

void TokenManager::DeleteToken(const std::string& token) {
  std::lock_guard<std::mutex> token_lock(token_mutex_);
  auto iter = tokens_.find(token);
  if (iter != tokens_.end()) tokens_.erase(iter);
}

std::string TokenManager::GetUserInToken(const std::string& token) {
  std::lock_guard<std::mutex> token_lock(token_mutex_);
  auto iter = tokens_.find(token);
  if (iter != tokens_.end()) {
    return iter->second.account_Number;
  }
  return "";
}

void TokenManager::CheckToken() {
  std::thread th([this] { checkTimeout(); });
  th.detach();
}

void TokenManager::checkTimeout() {
  printf("token检查开启：\n");
  while (status_) {
    while (true) {
      std::lock_guard<std::mutex> token_lock(token_mutex_);
      if (tokens_.size() == 0) break;

      auto front = tokens_queue_.front();
      auto iter = tokens_.find(front);
      if (iter == tokens_.end()) break;

      auto token_generate_time = iter->second.generate_time;
      auto current_time = KGetCurrentTimestamp() - timeout_;
      auto difference_time = current_time - token_generate_time;
      if (difference_time.count() > 0) {
        tokens_.erase(iter);
        tokens_queue_.pop();
      } else {
        break;
      }
    }
    std::this_thread::sleep_for(std::chrono::minutes(1));
  }
}

}  // namespace lookupman
