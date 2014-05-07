#include "ThreadPool.h"
#include "nethead.h"
#include "UdpServer.h"
#include "Configure.h"
#include "Log.h"

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[])
{

    //把当前进程变为守护进程
    if (daemon(1, 1))
    {
        throw runtime_error("daemon");
        WRITE_STR(string(" turn main to deamon error"));
    }
    // config
    Configure *conf = Configure::getInstance();
    string sport = conf->getConfigByName("port");
    istringstream ss(sport);
    short port = 0;
    if (!(ss >> port))
    {
        throw runtime_error("sstream");
        WRITE_STR(string("trans str-port to int-port error"));
    }

    // prepair
    Address server_addr(htonl(INADDR_ANY), htons(port), AF_INET);
    Socket socket("udp");
    //bind
    socket.bindToAddess(server_addr);
    //start server;
    ThreadPool pool(10);
    UdpServer server(socket, server_addr, pool);
    server.start();
}
