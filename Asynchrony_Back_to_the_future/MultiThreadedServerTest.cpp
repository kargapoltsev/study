#include <gtest/gtest.h>
#include <thread>
#include "Socket.h"

using Handler = std::function<void()>;

void startNewThread(Handler handler)
{
    log("startNewThread");
    std::thread([handler]()
    {
        try
        {
            log("startNewThread: thread has been created");
            handler();
            log("startNewThread: thread was ended successfully");
        } catch(std::exception const& e)
        {
            log("startNewThread: thread was ended with error " + std::string(e.what()));
        }
    }).detach();
}

TEST(BACK_TO_FUTURE, MultiThreadedServerTest)
{
    Acceptor acceptor(8800);
    log("Accepting");


    while(true)
    {
        auto socket = new Socket;
        acceptor.accept(*socket);
        log("Accepted");

        startNewThread([socket]()
        {
            try {
                Buffer buffer(4000, 0);
                (*socket).readUntil(buffer, HTTP_DELIM_BODY);
                log("Red: " + buffer);

                (*socket).write(makeHttpContent("<h1>Hello sync singlethread!</h1>"));
                log("Wrote");
                (*socket).close();
                log("Closed");
            }
            catch (std::exception const& e) {
                log("error: " + std::string(e.what()));
            }
        });
    }
}
