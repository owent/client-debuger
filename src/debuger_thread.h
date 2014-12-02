#ifndef DEBUGER_THREAD__H
#define DEBUGER_THREAD__H

#include "debuger_global.h"

#include "qthread.h"

class debuger_thread_runner : public QObject
{
    Q_OBJECT

public:
    typedef int(*fn_t)(int, char**);

    debuger_thread_runner(fn_t, int, char**);
    ~debuger_thread_runner();

private:
    fn_t fn_;
    int argc_;
    char** argv_;

public Q_SLOTS:
    void start();

Q_SIGNALS:
    void finished();
};

class debuger_thread_actor : public QObject
{
    Q_OBJECT

public:
    ~debuger_thread_actor();


Q_SIGNALS:
    void start();


public Q_SLOTS:
    void finished();
};

#endif // DEBUGER_THREAD__H
