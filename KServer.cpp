#include "KServer.h"

namespace lookupman {
const std::string KServer::GetUserInToken(std::string account_number) {
  auto iter = tokens_.find(account_number);

  if (iter != tokens_.end()) return iter->second;

  return "";
}
}  // namespace lookupman
