#include "listeningthread.h"

ListeningThread::ListeningThread()
    : mSocketDescriptor(-1)
{

}

void ListeningThread::stopListening()
{
    // no thread protection needed,
    mRunning = false; // done within the thread
}

ListeningThread::~ListeningThread()
{

}

