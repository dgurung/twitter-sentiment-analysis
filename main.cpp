#include "include/ExceptionHandler.hpp"
#include "include/Oauth.hpp"
#include "include/ConnectionHandler.hpp"
#include "include/TwitterHandler.hpp"
#include "include/AnalysisHandler.hpp"
#include "include/utility.hpp"

#include <json.hpp>

#include <future>
#include <memory>

using json = nlohmann::json;

int main(int argc, const char* argv[]) {

    try {
    	if (argc != 3) {
        	throw std::invalid_argument("Usage: ./TwitterStream <consumer-key,secret> <access-token,secret>");
    	}

    	using namespace handler;

        // Read oauth conusmer Key-secret, access token id - secret from file and twitter api url from file       	
    	const consumer_credentials c_p 		= utility::read_settings<consumer_credentials>(argv[1]);  
    	const token_credentials t_p 		= utility::read_settings<token_credentials>(argv[2]);       
		constexpr auto twitter_url 			= "https://stream.twitter.com/1.1/statuses/sample.json";
		constexpr auto twitter_http_method 	= "GET";
		// constexpr auto sentiment140_url 	= "http://www.sentiment140.com/api/bulkClassifyJson?appid=deepak.gurung@nevada.unr.edu";

		RingBuffer queue;

		Oauth<consumer_credentials, token_credentials> twitter_oauth_url(c_p, t_p);
		std::string signed_url = twitter_oauth_url.sign_url(twitter_url, twitter_http_method);

		// Create twitter stream handler with oauth2 protocol
		TwitterHandler twitter_stream_handler(signed_url, queue);

		// Asynchronously launch connection thread 
		std::future<void> twitter_thread = std::async( std::launch::async, [&twitter_stream_handler](){ 
							try{															
								twitter_stream_handler.get_request();
							} catch(...) {
								Exception::eptr = std::current_exception(); 
							}
						});	
/*
		// Asynchronously launch connection thread 
		std::future<void> sentiment140_thread = std::async( std::launch::async, [&sentiment140_handler, &j](){ 
							try{															
								std::string data = j.dump();
								sentiment140_handler.post_request(data);
							} catch(...) {
								Exception::eptr = std::current_exception(); 
							}
						});	
*/
	} catch (const std::invalid_argument& e) {
		std::cout << e.what() << '\n';
		return 1;
	} catch (...) {
		Exception::eptr = std::current_exception();
	}

	Exception::exceptions_handler(Exception::eptr);
}
