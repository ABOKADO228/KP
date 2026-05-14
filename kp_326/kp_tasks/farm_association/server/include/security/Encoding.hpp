#pragma once

#include <string>
#include <string_view>

namespace fasc::server::security {

///Кодирует бинарные данные в Base64.
///@param data указатель на начало буфера.
///@param size размер буфера в байтах.
///@returns Base64-строка.
std::string base64Encode(const unsigned char* data, std::size_t size);

///Декодирует Base64-строку.
///@param encoded строка в Base64.
///@returns декодированные бинарные данные в строке.
std::string base64Decode(std::string_view encoded);

///Кодирует бинарные данные в URL-safe Base64 без padding.
///@param data указатель на начало буфера.
///@param size размер буфера в байтах.
///@returns URL-safe Base64-строка.
std::string base64UrlEncode(const unsigned char* data, std::size_t size);

///Кодирует строку в URL-safe Base64 без padding.
///@param data исходная строка.
///@returns URL-safe Base64-строка.
std::string base64UrlEncode(std::string_view data);

} // namespace fasc::server::security
