#include <gtest/gtest.h>
#include "Helpers.h"
#include "AsyncSocket.h"

void go(Handler handler)
{
    getIOService().post(std::move(handler));
}

void run()
{
    getIOService().run();
}

void dispatch(int const threadCount = 0)
{
    const auto threads = threadCount > 0 ? threadCount : int(std::thread::hardware_concurrency());
    log("Threads: " + std::to_string(threads));
    for (auto i = 1; i < threads; ++i)
        go(run);

    run();
}

TEST(BACK_TO_FUTURE, AsyncServerTest)
{
    AsyncAcceptor acceptor(8800);
    log("Start accepting");

    Handler accepting = [&acceptor, &accepting]
    {
        struct Connection
        {
            Buffer buffer_;
            AsyncSocket socket_;

            void handling()
            {
                buffer_.resize(4000);
                socket_.readUntil(buffer_, HTTP_DELIM_BODY, 
                [this](Error const& error)
                {
                    if (!!error)
                    {
                        log("Error on reading: " + error.message());
                        delete this;
                        return;
                    }

                    log("redden");
                    buffer_ = makeHttpContent("<h1>Hello async!</h1>");

                    socket_.write(
                        buffer_,
                        [this](Error const& error)
                        {
                            if (!!error)
                            {
                                log("Error on writing: " + error.message());
                                delete this;
                                return;
                            }

                            log("written");
                            handling();
                        }
                    );
                });
            }
        };

        auto* connection = new Connection;
        acceptor.accept(
            connection->socket_, 
            [connection, &accepting](Error const& error)
            {
                if (!!error)
                {
                    log("Error on accepting: " + error.message());
                    delete connection;
                    return;
                }

                log("accepted");
                connection->handling();
                accepting();
            }
        );
    };


    accepting();
    dispatch();
}