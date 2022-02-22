#include <gtest/gtest.h>
#include <thread>
#include "Socket.h"

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
