// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/Engine.hpp>
#include <sober/network/api/OpenWeatherMap.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/utils/Test.hpp>

namespace sober {
namespace network {
namespace api {
namespace unittest {

class OpenWeatherMap : public utils::Test {
 public:
  OpenWeatherMap(): stream_(engine_), delegate_(stream_) {
  }

  class Delegate: public api::OpenWeatherMap {
   public:
    using Base = api::OpenWeatherMap;
    Delegate(http::Stream& stream): Base(stream) {
    }

    virtual void on_success() override {
      try {
        Base::on_success();
      }
      catch (const std::exception& exc) {
        // ignore
      }
    }
  };

 protected:
  Engine engine_;
  http::Stream stream_;
  Delegate delegate_;
};

TEST_F(OpenWeatherMap, good) {
  delegate_.async_get_city("Madrid");
  engine_.run();

  ASSERT_FALSE(delegate_.attribute().temperature_human.empty());
}

TEST_F(OpenWeatherMap, bad) {
  delegate_.async_get_city("Dogville");
  engine_.run();

  ASSERT_THROW(delegate_.attribute(), std::runtime_error);
}

TEST_F(OpenWeatherMap, simultaniously) {
  http::Stream stream_A(engine_), stream_B(engine_);
  api::OpenWeatherMap api_A(stream_A), api_B(stream_B);

  api_A.async_get_city("San Francisco");
  api_B.async_get_city("San Francisco");

  engine_.run();

  ASSERT_FALSE(api_A.attribute().temperature_human.empty());
  ASSERT_STREQ(
      api_A.attribute().temperature_human.c_str(),
      api_B.attribute().temperature_human.c_str()
  );
}

} // namespace unittest
} // namespace api
} // namespace network
} // namespace sober
