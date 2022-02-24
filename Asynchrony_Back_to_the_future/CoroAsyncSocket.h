#pragma once
#include "Helpers.h"
#include "AsyncSocket.h"

namespace coro_async
{

struct Acceptor;
struct Socket
{
    friend struct Acceptor;

    void read(Buffer&);
    void readSome(Buffer&);
    void readUntil(Buffer&, Buffer stopSign);
    void write(const Buffer&);
    void close();

private:
    AsyncSocket socket_;
};

struct Acceptor
{
    explicit Acceptor(int port);

    void accept(Socket&);

private:
    AsyncAcceptor acceptor_;

};

}
