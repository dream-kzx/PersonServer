#ifndef PERSONSERVER_MYCRYPTO_H
#define PERSONSERVER_MYCRYPTO_H
#include <string>

namespace lookupman {
class MyCrypto {
 public:
  static std::string Hash(const std::string& msg);
};
}  // namespace lookupman

#endif
