#pragma once

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>

namespace fasc::server::tests::utils {

///Проверяет, что JSON-строка содержит поле со строковым значением.
///@param body  JSON-строка.
///@param key   имя проверяемого поля.
///@param value ожидаемое строковое значение.
inline void expectJsonStringField(const std::string& body,
                                  const std::string& key,
                                  const std::string& value) {
  const auto json = nlohmann::json::parse(body);
  EXPECT_EQ(json.at(key).get<std::string>(), value);
}

///Проверяет, что JSON-строка содержит поле со std::uint64_t значением.
///@param body  JSON-строка.
///@param key   имя проверяемого поля.
///@param value ожидаемое числовое значение.
inline void expectJsonUnsignedLongField(const std::string& body,
                                        const std::string& key,
                                        std::uint64_t value) {
  const auto json = nlohmann::json::parse(body);
  EXPECT_EQ(json.at(key).get<std::uint64_t>(), value);
}

} // namespace fasc::server::tests::utils
