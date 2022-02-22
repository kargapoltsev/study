#pragma once
#include <sstream>

using Buffer = std::string;

void log(std::string&& message);

#define HTTP_DELIM          "\r\n"
#define HTTP_DELIM_BODY     HTTP_DELIM HTTP_DELIM

Buffer makeHttpContent(const Buffer& body);

