#pragma once

#include <string>
#include <string_view>

namespace fasc::server::security {

std::string base64_encode(const unsigned char* data, std::size_t size);
std::string base64_decode(std::string_view encoded);
std::string base64_url_encode(const unsigned char* data, std::size_t size);
std::string base64_url_encode(std::string_view data);

} // namespace fasc::server::security
