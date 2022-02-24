#pragma once
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

#include "Async.h"
#include "Helpers.h"

struct AsyncAcceptor;
struct AsyncSocket
{
    friend struct AsyncAcceptor;

    AsyncSocket();

    void read(Buffer&, async::IoHandler);
    void readSome(Buffer&, async::IoHandler);
    void readUntil(Buffer&, Buffer stopSign, async::IoHandler);
    void write(const Buffer&, async::IoHandler);
    void close();

private:
    boost::asio::ip::tcp::socket socket_;
};

struct AsyncAcceptor
{
    explicit AsyncAcceptor(int port);

    void accept(AsyncSocket&, async::IoHandler);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};
