#include "MyCrypto.h"

#include "cryptopp/cryptlib.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"

namespace lookupman {
std::string MyCrypto::Hash(const std::string& msg) {
  using namespace CryptoPP;

  std::string digest_hex;

  std::string digest;

  SHA1 hash;
  hash.Update((const byte*)msg.data(), msg.size());
  digest.resize(hash.DigestSize());
  hash.Final((byte*)&digest[0]);

  StringSource(digest, true,
               new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest_hex)));

  return digest_hex;
}
}  // namespace lookupman
