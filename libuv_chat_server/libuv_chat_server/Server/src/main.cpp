#include "chatserver.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "uv.h"
#include <csignal>

#include "optionargs.h"

enum optionIndex { UNKNOWN, HELP, PORT };
const option::Descriptor usage[] =
{
    {UNKNOWN, 0, "", "", Arg::Unknown, "USAGE: Client -p PORT -n NICKNAME -a SERVER_ADDRESS" },
    {HELP, 0, "", "help", Arg::None, "--help \t Print usage and exit." },
    {PORT, 0, "p", "port", Arg::Numeric, "-p <port>, \t --port=<port> \t(number)"},
    { 0, 0, 0, 0, 0, 0 },
};

void term(int signum)
{
    fprintf(stderr, "Terminating Server\n");
    ChatServer::GetInstance()->Broadcast("Server Terminating");
    exit(signum);    
}

int main(int argc, char* argv[])
{
    if (argc > 0)
    {
        argc--;
        argv++;
    }

    option::Stats stats(usage, argc, argv);
    option::Option* options = new option::Option[stats.options_max];
    option::Option* buffer = new option::Option[stats.buffer_max];

    option::Parser parse(usage, argc, argv, options, buffer);
    if (parse.error())
    {
        fprintf(stderr, "Error while parsing options\n");
        return 1;
    }

    if (options[HELP] || argc == 0)
    {
        option::printUsage(fwrite, stderr, usage, 80);
    }

    if (!options[PORT])
    {
        fprintf(stderr, "port missing");
        return 1;
    }

    signal(SIGTERM, term);
    signal(SIGINT, term);

    ChatServer* server = ChatServer::GetInstance();
    int err = server->Init(strtol(options[PORT].arg, NULL, 10));

    if (err != 0)
    {
        std::cout << "Error listening" << uv_strerror(err) << std::endl;
    }
    else
    {
    }

    std::cout << "Server application exits" << std::endl;

    if (options) {
        delete[] options;
    }
    if (buffer) {
        delete[] buffer;
    }
    return 0;
}
