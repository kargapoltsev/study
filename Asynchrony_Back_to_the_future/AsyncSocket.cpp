#include "AsyncSocket.h"

namespace asio = boost::asio;

namespace
{
    auto makeBuffer(Buffer const& b, size_t const offset = 0)
    {
        return asio::buffer(&b[offset], b.size() - offset);
    }

    auto makeBuffer(Buffer& b, size_t const offset = 0)
    {
        return asio::buffer(&b[offset], b.size() - offset);
    }

}

AsyncSocket::AsyncSocket()
    : socket_(getIOService())
{}

void AsyncSocket::read(Buffer& b, async::IoHandler handler)
{
    async_read(
        socket_, 
        makeBuffer(b),
        asio::transfer_all(),
        [handler = std::move(handler)](const async::Error& error, std::size_t) -> void 
        {
            handler(error);
        }
    );
}

void AsyncSocket::readSome(Buffer& b, async::IoHandler handler)
{
    socket_.async_read_some(
        makeBuffer(b),
        [&b, handler = std::move(handler)](const async::Error& error, std::size_t bytes) -> void
        {
            handler(error);
            b.resize(bytes);
        }
    );
}

void AsyncSocket::readUntil(Buffer& b, Buffer stopSign, async::IoHandler handler)
{
    struct UntilHandler
    {
        UntilHandler(
            asio::ip::tcp::socket& socket,
            Buffer& buffer,
            Buffer stopSign,
            async::IoHandler handler)
            : offset_(0)
            , socket_(socket)
            , buffer_(buffer)
            , stopSign_(std::move(stopSign))
            , handler_(std::move(handler))
        {}

        void read()
        {
            log("read at offset: " + std::to_string(offset_));
            socket_.async_read_some(makeBuffer(buffer_, offset_), *this);
        }

        void complete(const async::Error& error) const
        {
            handler_(error);
        }

        void operator()(const async::Error& error, std::size_t bytes)
        {
            if (!!error) {
                return complete(error);
            }

            offset_ += bytes;
            log("buffer: '" + buffer_.substr(0, offset_) + "'");

            if (hasEnd(offset_, buffer_, stopSign_))
            {
                // found end
                buffer_.resize(offset_);
                return complete(error);
            }
            if (offset_ == buffer_.size())
            {
                log("not enough size: " + std::to_string(buffer_.size()));
                buffer_.resize(buffer_.size() * 2);
            }

            read();
        }

    private:
        size_t offset_;
        asio::ip::tcp::socket& socket_;
        Buffer& buffer_;
        Buffer stopSign_;
        async::IoHandler handler_;
    };

    UntilHandler(
        socket_,
        b,
        std::move(stopSign),
        std::move(handler)).read();
}

void AsyncSocket::write(const Buffer& b, async::IoHandler handler)
{
    boost::asio::async_write(
        socket_, 
        makeBuffer(b),
        [handler = std::move(handler)](async::Error const& error, size_t)
        {
            handler(error);
        }
    );
}

void AsyncSocket::close()
{
    socket_.close();
}

AsyncAcceptor::AsyncAcceptor(int const port)
    : acceptor_(getIOService(), 
        asio::ip::tcp::endpoint(
            asio::ip::tcp::v4(), port))
{}

void AsyncAcceptor::accept(AsyncSocket& socket, async::IoHandler handler)
{
    acceptor_.async_accept(socket.socket_, handler);
}


