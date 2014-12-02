#ifndef DEBUGER_H
#define DEBUGER_H

#include "debuger_global.h"

#include <QtWebKit>
#include <QtWebKitWidgets/qwebview.h>
#include <QtWebKitWidgets/qwebframe.h>
#include <QApplication>
#include <qwindow.h>

extern "C" {
    typedef void(*debuger_js_fn)(const char **, size_t*, size_t, char**, size_t*);
}

class DEBUGER_EXPORT debuger : public QWidget
{
    Q_OBJECT

public:
    ~debuger();

    static debuger* create_debuger();
    static debuger* instance;
    static std::shared_ptr<QApplication> app;

    QWebView* webview_;

    void open_local_file(const char* path, size_t len);
    void open_url(const char* path, size_t len);

    void invoke(const char* name, debuger_js_fn fn);

    void eval(const char* code, size_t len, char* ret, size_t* ret_len);

protected Q_SLOTS:
    void OnLoadFinished(bool ok);
    void OnTitleChanged(const QString& title);
    void OnResizeWebView(QSize size);
    void OnPopulateJavaScriptWindowObject();

public Q_SLOTS:
    void OnAttachJobFinished(
#if !defined(Q_QDOC)
        QPrivateSignal
#endif
        );

public:
    Q_INVOKABLE QString call(QString func_name, QStringList args);

    Q_INVOKABLE void setWindowSize(int w, int h);
    Q_INVOKABLE void setMinWindowSize(int w, int h);
    Q_INVOKABLE void setMaxWindowSize(int w, int h);

    Q_INVOKABLE QString getFileContent(QString path);

    Q_INVOKABLE QString getWebFileContent(QString path);

private:
    debuger();
    std::map<std::string, debuger_js_fn> js_fns_;
    bool last_;
    QString open_path_dir_;
    
};

#endif // DEBUGER_H
