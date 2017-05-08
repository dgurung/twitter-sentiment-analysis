#ifndef EXCEPTIONHANDLER_HPP_
#define EXCEPTIONHANDLER_HPP_

#include <exception>
#include <iostream>

namespace Exception {

static std::exception_ptr eptr = nullptr;

void exceptions_handler(std::exception_ptr eptr) {
  try {
    if (eptr) {
      std::rethrow_exception(eptr);
    }
  } catch (const std::exception& e) {
    std::cout << "Exception Caught:" << e.what() << '\n';
  }
}

}  // namespace
#endif