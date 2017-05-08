#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <fstream>

namespace handler {

namespace utility {

template <class user_pair>
const user_pair read_settings(const std::string& filename) {
  std::ifstream in_file;
  in_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  std::string first;
  std::string second;

  try {
    in_file.open(filename);

    std::getline(in_file, first);
    std::getline(in_file, second);
  } catch (const std::ifstream::failure& e) {
    throw e;
  }

  user_pair result{first, second};

  return result;
}

const std::string get_public_url(const std::string& filename) {
  std::ifstream in_file;
  in_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  std::string url;

  try {
    in_file.open(filename);
    std::getline(in_file, url);
  } catch (const std::ifstream::failure& e) {
    throw e;
  }

  return url;
}

}  // namepsace utility

}  // namespace Handler

#endif