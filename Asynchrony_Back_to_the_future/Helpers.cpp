#include "Helpers.h"
#include <iostream>

void log(std::string&& message)
{
    std::cout
        << "Server: "
        << std::move(message)
        << std::endl;
}

Buffer makeHttpContent(const Buffer& body)
{
    std::ostringstream o;
    o << "HTTP/1.1 200 Ok" HTTP_DELIM
        "Content-Type: text/html" HTTP_DELIM
        "Content-Length: " << body.size() << HTTP_DELIM_BODY
        << body;
    return o.str();
}
