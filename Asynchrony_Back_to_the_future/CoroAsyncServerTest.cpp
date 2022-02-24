#include <gtest/gtest.h>
#include "Helpers.h"
#include "CoroAsyncSocket.h"
#include "CoroAsync.h"

using namespace coro_async;

TEST(BACK_TO_FUTURE, CoroAsyncServerTest)
{
    Acceptor acceptor(8800);
    log("accepting");
    go([&acceptor] ()
    {
        while (true)
        {
            auto* toAccept = new Socket;
            acceptor.accept(*toAccept);
            log("accepted");
            go([toAccept]
            {
                try
                {
                    auto socket(std::move(*toAccept));
                    delete toAccept;

                    Buffer buffer;
                    while(true)
                    {
                        buffer.resize(4000);
                        socket.readUntil(buffer, HTTP_DELIM_BODY);
                        socket.write(makeHttpContent("<h1>Hello synca!</h1>"));
                    }
                }
                catch (std::exception const& e)
                {
                    log("error: " + std::string(e.what()));
                }
            });

        }
    });

    async::dispatch();
}
