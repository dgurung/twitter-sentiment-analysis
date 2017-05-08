#ifndef TWITTERHANDLER_HPP_
#define TWITTERHANDLER_HPP_

#include "ConnectionHandler.hpp"
#include <iostream>
#include <iomanip>

#include <json.hpp>

namespace handler{

using json = nlohmann::json;

class TwitterHandler : public ConnectionHandler<TwitterHandler> {
public:	
	explicit TwitterHandler (const std::string& url_oauth, RingBuffer& queue) 
		: ConnectionHandler<TwitterHandler>(url_oauth, queue) {
	}

	CURLcode customize_callback(CURL* curl_handler) {
		 // - Callback function
    	CURLcode status = curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, this->call);
    	return status;
	}

	static size_t call(char* ptr, size_t size, size_t nmemb, RingBuffer* queue){
		std::cout << std::setw(4) << size << '\t' << ptr << '\t' << nmemb << '\n';

		json j = json::parse(ptr);

		queue->spsc_ring_buffer->push(shm::shared_string(ptr, queue->char_alloc));        
		std::cout << std::setw(4) << size << '\t' << static_cast<void*>(ptr) << '\t' << nmemb << '\n';
		return size * nmemb;
	}
};

}	// namespace handler
#endif