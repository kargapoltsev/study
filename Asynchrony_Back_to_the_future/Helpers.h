#pragma once
#include <functional>
#include <boost/asio.hpp>

using Buffer = std::string;

void log(std::string&& message);

#define HTTP_DELIM          "\r\n"
#define HTTP_DELIM_BODY     HTTP_DELIM HTTP_DELIM

Buffer makeHttpContent(const Buffer& body);

bool hasEnd(size_t const posEnd, Buffer const& b, Buffer const& stopSign);

using Handler = std::function<void()>;

void startNewThread(Handler handler);

boost::asio::io_service& getIOService();

#define RAISE(D_str)                throw std::runtime_error(D_str)
#define VERIFY(D_cond, D_str)       if (!(D_cond)) RAISE("Verification failed: " #D_cond ": " D_str)
