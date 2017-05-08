#ifndef ANALYSISHANDLER_HPP_
#define ANALYSISHANDLER_HPP_

#include <iomanip>
#include <iostream>
#include "ConnectionHandler.hpp"

namespace handler {

class AnalysisHandler : public ConnectionHandler<AnalysisHandler> {
 public:
  explicit AnalysisHandler(const std::string& url, RingBuffer& queue)
      : ConnectionHandler<AnalysisHandler>(url, queue) {}
};

}  // namespace handler
#endif