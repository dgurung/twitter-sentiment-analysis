#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include "RingBuffer.hpp"
#include "Oauth.hpp"

extern "C" {
	#include <stdlib.h>
	#include <curl/curl.h>
}

#include <memory>
#include <string>
#include <exception>
#include <stdexcept>

#include <iostream>
#include <type_traits>

namespace handler{

template <class Derived_t>
class ConnectionHandler {
public:
	explicit ConnectionHandler(const std::string& url, RingBuffer& queue) 
		: m_url(url)
		, m_queue(queue) {
			// Initialize curl Session
			init_curl_session();
	} 
	ConnectionHandler(const ConnectionHandler&) 			= delete;
	ConnectionHandler& operator=(const ConnectionHandler&) 	= delete;

	ConnectionHandler(ConnectionHandler&& other) {
		m_queue	= other.m_queue;
		m_curl_handler = other.m_curl_handler;
		other.m_curl_handler = nullptr; // Release the curl handler of source object.
	}
	
	ConnectionHandler& operator=(ConnectionHandler&& other) {
		if (this != other) {	

			m_queue = std::move(other.m_queue);

			// Free existing resource
			curl_easy_cleanup(m_curl_handler);

			m_curl_handler = other.m_curl_handler;

			other.m_curl_handler = nullptr;
			return *this;
		}
	}

	~ConnectionHandler() {
		curl_easy_cleanup(m_curl_handler);
        curl_global_cleanup();
	}

	void get_request();
	void post_request(const std::string& data);

protected:
	void init_curl_session();
    void setup_curl_options();
    void curl_perform();
    void setup_callback();
	
    std::string m_url;
    CURL* 		m_curl_handler	= nullptr;
	CURLcode 	m_status;
	RingBuffer& m_queue;
};

template <class Derived_t>
void ConnectionHandler<Derived_t>::init_curl_session() {
	// Initialize curl Session
    curl_global_init(CURL_GLOBAL_ALL);	    
    m_curl_handler = curl_easy_init();
    if (m_curl_handler == nullptr) {
    	throw std::runtime_error("Unable to initialize curl");
    }
}

template <class Derived_t>
void ConnectionHandler<Derived_t>::setup_curl_options() {

    // - Set URL
	m_status = curl_easy_setopt(m_curl_handler, CURLOPT_URL, m_url.c_str());

	std::cout << m_url <<'\n';

	if (m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
	}

    // - User agent name
	m_status = curl_easy_setopt(m_curl_handler, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	if (m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
	}

  	// - HTTP m_status >=400 ---> ERROR
    m_status =  curl_easy_setopt(m_curl_handler, CURLOPT_FAILONERROR, 1);
    if (m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
	}

	// - Disable progress bar
	m_status = curl_easy_setopt(m_curl_handler, CURLOPT_NOPROGRESS, 1L);
    if ( m_status != CURLE_OK ) {
        throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
    }	
}

template <class Derived_t>
void ConnectionHandler<Derived_t>::setup_callback() {

   	m_status = static_cast<Derived_t*>(this)->customize_callback(m_curl_handler);
	if (m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
	}

    // - Write data
    m_status = curl_easy_setopt(m_curl_handler, CURLOPT_WRITEDATA, (void *)&m_queue);
	if (m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
	}
}

template <class Derived_t>
void ConnectionHandler<Derived_t>::curl_perform() {
	// Twitter API server keeps the connection alive.
	// As soon as curl gets the data it invokes write function (callback function)
	// Blocks until write function (callback func) is finished being invoked.
	m_status = curl_easy_perform(m_curl_handler);	
    if (m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
    }
}


template <class Derived_t>
void ConnectionHandler<Derived_t>::get_request() {
	// Setup curl session
	setup_curl_options();
	// SetUp curl callback function
	setup_callback();
	// get the string stream
	curl_perform();
}

template <class Derived_t>
void ConnectionHandler<Derived_t>::post_request(const std::string& data){

    m_status = curl_easy_setopt(m_curl_handler, CURLOPT_POST, 1);
    if ( m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
    }

    m_status = curl_easy_setopt(m_curl_handler, CURLOPT_POSTFIELDS, data.c_str());
    if ( m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
    }

    m_status = curl_easy_setopt(m_curl_handler, CURLOPT_POSTFIELDSIZE, data.length());
    if ( m_status != CURLE_OK) {
		throw std::runtime_error( std::string{curl_easy_strerror(m_status)} );
	}

	// Setup curl session
	setup_curl_options();
	// SetUp curl callback function
	setup_callback();
	// get the string stream
	curl_perform();
}

}
 // namespace Handler

#endif