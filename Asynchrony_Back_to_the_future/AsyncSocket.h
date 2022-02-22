#pragma once
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include "Helpers.h"

using Error = boost::system::error_code;
using IoHandler = std::function<void(const Error&)>;

struct AsyncAcceptor;
struct AsyncSocket
{
    friend struct AsyncAcceptor;

    AsyncSocket();

    void read(Buffer&, IoHandler);
    void readSome(Buffer&, IoHandler);
    void readUntil(Buffer&, Buffer stopSign, IoHandler);
    void write(const Buffer&, IoHandler);
    void close();

private:
    boost::asio::ip::tcp::socket socket_;
};

struct AsyncAcceptor
{
    explicit AsyncAcceptor(int port);

    void accept(AsyncSocket&, IoHandler);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};
