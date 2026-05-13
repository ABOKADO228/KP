#pragma once

#include <string>
#include <unordered_map>

struct HttpRequest {
  std::string method;
  std::string target;
  std::string body;
  std::unordered_map<std::string, std::string> headers;
  std::unordered_map<std::string, std::string> query_params;
};

struct HttpResponse {
  unsigned status{200};
  std::string content_type{"application/json"};
  std::string body;
};
