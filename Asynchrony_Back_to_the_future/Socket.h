#pragma once
#include <string>
#include <boost/asio.hpp>
#include "Helpers.h"

struct Acceptor;

struct Socket
{
    friend struct Acceptor;

    Socket();

    void read(Buffer& b);
    void readSome(Buffer& b);
    int readUntil(Buffer& b, Buffer const& stopSign);
    void write(Buffer const& b);
    void close();

private:
    boost::asio::ip::tcp::socket socket_;
};

struct Acceptor
{
    explicit Acceptor(int32_t port);
    void accept(Socket& socket);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};