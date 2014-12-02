#include <string>
#include <thread>
#include <qtconcurrentrun.h>
#include <qthread.h>
#include <memory>
#include "debuger.h"
#include "debuger_thread.h"

static int app_argc = 0;
static char** app_argv = nullptr;

extern "C" {
    DEBUGER_EXPORT void debuger_init_app(int argc, char** argv) {
        debuger::app = std::make_shared<QApplication>(argc, argv);
        app_argc = argc;
        app_argv = argv;
    }


    DEBUGER_EXPORT void debuger_exit(int code) {
        if (debuger::app) {
            debuger::app->exit(code);
            debuger::app.reset();
        }
    }

    DEBUGER_EXPORT int debuger_exec(int (*fn)(int, char**), void (*exit_fn)()) {
        if (fn) {
            debuger_thread_runner runner(fn, app_argc, app_argv);
            debuger_thread_actor actor;
            QThread child;

            QObject::connect(&actor, SIGNAL(start()), &runner, SLOT(start()));
            QObject::connect(&runner, SIGNAL(finished()), &actor, SLOT(finished()));

            runner.moveToThread(&child);
            child.start();
            actor.start();

            int ret = debuger::app->exec();

            if (exit_fn)
                exit_fn();

            // ==== child join
            child.quit();
            child.wait();
            return ret;
        }

        return debuger::app->exec();
    }

    DEBUGER_EXPORT void debuger_open_local_file(const char* path, size_t len) {
        debuger::create_debuger();
        debuger::instance->open_local_file(path, len);
    }

    DEBUGER_EXPORT void debuger_open_url(const char* path, size_t len) {
        debuger::create_debuger();
        debuger::instance->open_url(path, len);
    }

    DEBUGER_EXPORT void debuger_get_charset(char* name, size_t len) {
        strncpy(name, QTextCodec::codecForLocale()->name(), len);
    }

    DEBUGER_EXPORT void debuger_set_charset(const char* name) {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName(name));
    }

    DEBUGER_EXPORT void debuger_eval(const char* code, size_t len, char* ret, size_t* ret_len) {
        debuger::instance->eval(code, len, ret, ret_len);
    }

    DEBUGER_EXPORT void debuger_invoke(const char* name, debuger_js_fn fn) {
        debuger::instance->invoke(name, fn);
    }

    void test_fn(const char * argv[], size_t* arg_len, size_t argc, char** ret, size_t* ret_len) {
        if (argc > 0) {
            char buffer[4096] = { 0 };
            size_t buffer_len = 0;
            debuger_eval(argv[0], arg_len[0], buffer, &buffer_len);

            if (buffer_len > 0) {
                *ret = reinterpret_cast<char*>(malloc(buffer_len));
                *ret_len = buffer_len;
                memcpy(*ret, buffer, buffer_len);
            }
        }
    }
}

//int main(int argc, char** argv) {
//    debuger_init_app(argc, argv);
//
//    debuger_open_local_file("../debuger.webview/index.html", 0);
//    //debuger_open_url("http://www.baidu.com", 0);
//    
//
//    debuger_invoke("test_func", test_fn);
//
//    return debuger_exec();
//}