#ifndef SOBER_NETWORK_HTTP_DELEGATE_STRING_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_STRING_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.hpp>

#include <string>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class String: virtual public Interface {
 public:
  String() noexcept;

  void body_start() noexcept override;

  /**
    * @throws @c std::runtime_error if already finished
    */
  void body_write(const char* buffer, std::size_t size) override;

  /**
    * @throws @c std::runtime_error if already finished
    */
  void body_finish() override;

  /**
    * @return true - body can be read
    * @return false - body is not valid
    * @note Requests with status code != OK have valid body
    */
  bool ready() const noexcept;

  /**
    * @throws @c std::runtime_error if body not ready
    */
  const std::string& body() const;

 private:
  bool ready_;
  std::string body_;
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_STRING_HPP_