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

    void read(Buffer&, async::AsyncHandler);
    void readSome(Buffer&, async::AsyncHandler);
    void readUntil(Buffer&, Buffer stopSign, async::AsyncHandler);
    void write(const Buffer&, async::AsyncHandler);
    void close();

private:
    boost::asio::ip::tcp::socket socket_;
};

struct AsyncAcceptor
{
    explicit AsyncAcceptor(int port);

    void accept(AsyncSocket&, async::AsyncHandler);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};
