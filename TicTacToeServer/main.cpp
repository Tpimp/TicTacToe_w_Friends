#include <QCoreApplication>
#include "tictactoeserver.h"
#include"signal.h"
#include"unistd.h"

/******************************************************************************
 * Attempting to make as small as possible
 *  C++ lambdas are used instead of writing a bunch of classes to handle signals
 * TicTacToeServer takes care of the "work" to
 *
 * ****************************************************************************/


// set the server to terminate on ctrl-c signal
static TicTacToeServer * GLOBAL_TTTSERVER;

static void kill_server(int sig)
{
        printf("\nquit the application (user request signal = %d).\n", sig);
        GLOBAL_TTTSERVER->shutdownServer();
        QCoreApplication::exit();
}



void handleUnixSignals(const std::vector<int>& quitSignals,
                      const std::vector<int>& ignoreSignals = std::vector<int>()) {


    // all these signals will be ignored.
    for ( int sig : ignoreSignals )
        signal(sig, SIG_IGN);

    // each of these signals calls the handler (quits the QCoreApplication).
    for ( int sig : quitSignals )
        signal(sig, kill_server);
}

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv); // console application -> use QCore only
    handleUnixSignals({SIGINT,SIGQUIT,SIGTERM});
    qsrand(9392832);
    TicTacToeServer       ttt_server(&a);  // create the server
    GLOBAL_TTTSERVER = &ttt_server;
    ttt_server.startListening();        // begin asynchronous listen

    return a.exec();
}
