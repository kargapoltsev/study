#include "CoroAsyncSocket.h"
#include "Async.h"
#include "Coro.h"
#include "CoroAsync.h"

namespace coro_async
{

void Socket::read(Buffer& buffer)
{
    VERIFY(coro::isInsideCoro(), "read must be called inside coro");
    defer([this, &buffer](coro::Coro* coro)
    {
        VERIFY(!coro::isInsideCoro(), "read completion must be called outside coro");
        socket_.read(buffer, onCompleteHandler(coro));
        log("read scheduled");
    });
}

void Socket::readSome(Buffer& buffer)
{
    VERIFY(coro::isInsideCoro(), "readSome must be called inside coro");
    defer([this, &buffer](coro::Coro* coro)
    {
        VERIFY(!coro::isInsideCoro(), "readSome completion must be called outside coro");
        socket_.readSome(buffer, onCompleteHandler(coro));
        log("readSome scheduled");
    });
}

void Socket::readUntil(Buffer& buffer, Buffer stopSign)
{
    VERIFY(coro::isInsideCoro(), "readUntil must be called inside coro");
    defer([this, &buffer, stopSign = std::move(stopSign)](coro::Coro* coro)
    {
        VERIFY(!coro::isInsideCoro(), "readUntil completion must be called outside coro");
        socket_.readUntil(buffer, stopSign, onCompleteHandler(coro));
        log("readUntil scheduled");
    });
}

void Socket::write(Buffer const& buffer)
{
    VERIFY(coro::isInsideCoro(), "write must be called inside coro");
    defer([this, &buffer](coro::Coro* coro)
    {
        VERIFY(!coro::isInsideCoro(), "write completion must be called outside coro");
        socket_.write(buffer, onCompleteHandler(coro));
        log("write scheduled");
    });
}

void Socket::close()
{
    socket_.close();
}

Acceptor::Acceptor(int const port)
    : acceptor_(port)
{}

void Acceptor::accept(Socket& socket)
{
    VERIFY(coro::isInsideCoro(), "deffer accept must be called inside coro");
    defer(
        [this, &socket](coro::Coro* coro)
        {
            VERIFY(!coro::isInsideCoro(), "deffer accept completion must be called outside coro");

            acceptor_.accept(
                socket.socket_, 
                onCompleteHandler(coro)
            );

            log("accept scheduled");
        }
    );
}

}
