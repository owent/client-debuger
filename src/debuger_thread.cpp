#include <qvariant.h>
#include <atomic>
#include "debuger_thread.h"
#include "debuger.h"

debuger_thread_runner::debuger_thread_runner(fn_t fn, int c, char** v) : fn_(fn), argc_(c), argv_(v) {
}

debuger_thread_runner::~debuger_thread_runner(){
}


void debuger_thread_runner::start() {
    if (fn_) {
        fn_(argc_, argv_);
    }

    finished();
}

debuger_thread_actor::~debuger_thread_actor(){
}


void debuger_thread_actor::finished() {
    // TODO END
    if (debuger::app){
        debuger::app->closeAllWindows();
    }
}
