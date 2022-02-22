#include "Socket.h"

namespace asio = boost::asio;

namespace 
{

asio::io_service& getIOService()
{
    static auto ioService = boost::asio::io_service();
    return ioService;
}

bool hasEnd(size_t const posEnd, Buffer const& b, Buffer const& stopSign)
{
    return posEnd >= stopSign.size() &&
        b.rfind(stopSign, posEnd - stopSign.size()) != std::string::npos;
}

auto makeBuffer(Buffer const& b, size_t const offset = 0)
{
    return asio::buffer(&b[offset], b.size() - offset);
}

auto makeBuffer(Buffer& b, size_t const offset = 0)
{
    return asio::buffer(&b[offset], b.size() - offset);
}

}

Socket::Socket()
    : socket_(getIOService())
{}

void Socket::read(Buffer& b)
{
    asio::read(socket_, makeBuffer(b));
}

void Socket::readSome(Buffer& b)
{
    b.resize(socket_.read_some(makeBuffer(b)));
}

int Socket::readUntil(Buffer& b, Buffer const& stopSign)
{
    size_t offset = 0;
    while(true)
    {
        auto const bytes = socket_.read_some(
            makeBuffer(b, offset));
        offset += bytes;

        if (hasEnd(offset, b, stopSign))
        {
            b.resize(offset);
            return offset;
        }

        if (offset == b.size())
        {
            log("Not enough size: " + std::to_string(b.size()));
            b.resize(b.size() * 2);
        }
    }
}

void Socket::write(Buffer const& b)
{
    asio::write(socket_, makeBuffer(b));
}

void Socket::close()
{
    socket_.close();
}

Acceptor::Acceptor(int32_t const port)
    : acceptor_(getIOService(), 
        asio::ip::tcp::endpoint(
            asio::ip::tcp::v4(), 
            port))
{}

void Acceptor::accept(Socket& socket)
{
    acceptor_.accept(socket.socket_);
}

