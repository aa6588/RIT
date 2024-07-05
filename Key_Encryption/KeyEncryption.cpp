/** Implementation file for key encryption
 *
 *  \file imgs/ipcv/key_encryption/KeyEncryption.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 10 January 2020
 *  Edited: Andrea Avendano (aa6588@rit.edu)
 *  Date: 27 March 2020
 */

#include "imgs/ipcv/key_encryption/KeyEncryption.h"

namespace ipcv {

std::string Encrypt(const std::string &key, const std::string &str) {
  std::string encrypted_str;
  std::string k;
  encrypted_str.reserve(str.size());

  /* Make k to cycle the key through the original string (str)
   */
  size_t cycle = 0;
  for (size_t idx = 0; idx < str.size(); idx++) {
    k += key[cycle];
    cycle = (cycle + 1) % key.size();
  }
  /* Add k and the original string (str) together then use mod to restore
   * it to a printable range
   */
  for (size_t idx = 0; idx < str.size(); idx++) {
    encrypted_str.push_back(((str[idx] - 32) + (k[idx] - 32)) % (127 - 32) +
                            32);
  }

  return encrypted_str;
}

std::string Decrypt(const std::string &key, const std::string &str) {
  std::string decrypted_str;
  std::string k;
  decrypted_str.reserve(str.size());

  /* Make k to cycle the key through the original string (str)
   */
  size_t cycle = 0;
  for (size_t idx = 0; idx < str.size(); idx++) {
    k += key[cycle];
    cycle = (cycle + 1) % key.size();
  }

  /* if subtraction results in negative value, add modulo to bring
   *  back to printable range.
   *  otherwise, just add 32
   */

  for (size_t idx = 0; idx < str.size(); idx++) {
    if (((str[idx] - 32) - (k[idx] - 32)) < 0) {
      decrypted_str.push_back(((str[idx] - 32) - (k[idx] - 32))
                                            + (127 - 32) + 32);
    } else {
      decrypted_str.push_back((str[idx] - 32) - (k[idx] - 32) + 32);
    }
  }
  return decrypted_str;
}
} // namespace ipcv
