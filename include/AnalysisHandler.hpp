#ifndef ANALYSISHANDLER_HPP_
#define ANALYSISHANDLER_HPP_

#include "ConnectionHandler.hpp"
#include <iostream>
#include <iomanip>

namespace handler{

class AnalysisHandler : public ConnectionHandler<AnalysisHandler> {
public:	
	explicit AnalysisHandler (const std::string& url, RingBuffer& queue)  
		:  ConnectionHandler<AnalysisHandler>(url, queue) {
	}
};

} // namespace handler
#endif