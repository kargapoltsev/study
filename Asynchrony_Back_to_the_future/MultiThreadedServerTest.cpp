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

        runInNewThread([socket = std::move(socket)]()
        {
            try {
                Buffer buffer(4000, 0);
                (*socket).readUntil(buffer, HTTP_DELIM_BODY);
                log("Read: " + buffer);

                (*socket).write(makeHttpContent("<h1>Hello from sync multi thread server!</h1>"));
                log("Written");
                (*socket).close();
                log("Closed");
            }
            catch (std::exception const& e) {
                log("Error: " + std::string(e.what()));
            }
        });
    }
}
