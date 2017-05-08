#ifndef OAUTH_HPP_
#define OAUTH_HPP_

extern "C" {
#include <oauth.h>
}

#include <cstdlib>

#include <exception>
#include <stdexcept>
#include <string>

namespace handler {

namespace C_api_wrapper {
std::string oauth_sign_url2(const std::string& resource_url, char** argv,
                            OAuthMethod method,
                            const std::string& request_method,
                            const std::string& c_key,
                            const std::string& c_secret,
                            const std::string& t_key,
                            const std::string& t_secret) {
  char* oauth = ::oauth_sign_url2(
      resource_url.c_str(), nullptr, OA_HMAC, request_method.c_str(),
      c_key.c_str(), c_secret.c_str(), t_key.c_str(), t_secret.c_str());
  std::string result = oauth;
  free(oauth);

  return result;
}

}  // namespace C_api_wrapper

struct consumer_credentials {
  std::string consumer_key;
  std::string consumer_secret;
};

struct token_credentials {
  std::string token_key;
  std::string token_secret;
};

template <class ConsumerCredit, class TokenCredit>
class Oauth {
 public:
  Oauth() = default;
  Oauth(const ConsumerCredit& cp, const TokenCredit& ap)
      : m_consumer_p{cp}, m_token_p{ap} {}

  Oauth(const Oauth& that) = default;
  Oauth& operator=(const Oauth& that) = default;

  Oauth(Oauth&& that) = default;
  Oauth& operator=(Oauth&& that) = default;

  std::string get_authentic_url() const { return m_oauth_signed_url; }

  std::string sign_url(const std::string& resource_url,
                       const std::string& request_method) {
    // call oauth C API
    m_oauth_signed_url = C_api_wrapper::oauth_sign_url2(
        resource_url, NULL, OA_HMAC, request_method, m_consumer_p.consumer_key,
        m_consumer_p.consumer_secret, m_token_p.token_key,
        m_token_p.token_secret);

    if (m_oauth_signed_url.empty())
      throw std::runtime_error(
          "Cannot authenticate ConsumerCredit pair and token pair");
    return m_oauth_signed_url;
  }

  ~Oauth() = default;

 private:
  ConsumerCredit m_consumer_p;
  TokenCredit m_token_p;
  std::string m_oauth_signed_url;
};

}  // namespace Handler
#endif