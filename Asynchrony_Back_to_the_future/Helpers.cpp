#include "Helpers.h"
#include <iostream>
#include <thread>

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

bool hasEnd(size_t const posEnd, Buffer const& b, Buffer const& stopSign)
{
    return posEnd >= stopSign.size() &&
        b.rfind(stopSign, posEnd - stopSign.size()) != std::string::npos;
}

void startNewThread(Handler handler)
{
    log("startNewThread");
    std::thread([handler]()
    {
        try
        {
            log("startNewThread: thread has been created");
            handler();
            log("startNewThread: thread was ended successfully");
        }
        catch (std::exception const& e)
        {
            log("startNewThread: thread was ended with error " + std::string(e.what()));
        }
    }).detach();
}

boost::asio::io_service& getIOService()
{
    static auto ioService = boost::asio::io_service();
    return ioService;
}
