#include <qvariant.h>
#include <atomic>
#include "debuger.h"

debuger* debuger::instance;
std::shared_ptr<QApplication> debuger::app;
static std::atomic_uint debuger_count;

extern "C" void debuger_exit(int code);

debuger* debuger::create_debuger() {
    if (debuger::instance) {
        debuger::instance->last_ = false;
        delete debuger::instance;
    }

    return debuger::instance = new debuger();
}

debuger::debuger()
{
    last_ = true;
    webview_ = new QWebView();

    connect(webview_, SIGNAL(loadFinished(bool)), this, SLOT(OnLoadFinished(bool)));
    connect(webview_, SIGNAL(titleChanged(const QString&)), this, SLOT(OnTitleChanged(const QString&)));

    //大小改变
    connect(webview_->page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)), this, SLOT(OnResizeWebView(QSize))); 

    // javascript 对象注入
    connect(webview_->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(OnPopulateJavaScriptWindowObject()));
}

debuger::~debuger()
{
    if (last_) {
        debuger_exit(0);
    }
}


void debuger::open_local_file(const char* path, size_t len) {
    std::string local_path;
    if (len > 0)
        local_path.assign(path, len);
    else
        local_path = path;

    QString abs_path = QFileInfo(QString::fromStdString(local_path)).absoluteFilePath();
    webview_->load(QUrl("file:///" + abs_path));
    webview_->show();

    open_path_dir_ = QFileInfo(abs_path).absoluteDir().path();
}

void debuger::open_url(const char* path, size_t len) {
    std::string local_path;
    if (len > 0)
        local_path.assign(path, len);
    else
        local_path = path;

    webview_->load(QUrl(QString::fromStdString(local_path)));
    webview_->show();

    open_path_dir_ = QUrl(QString::fromStdString(local_path)).url(QUrl::RemoveFilename);
}

void debuger::invoke(const char* name, debuger_js_fn fn) {
    if (nullptr == fn) {
        js_fns_.erase(name);
        return;
    }

    js_fns_[name] = fn;
}

void debuger::eval(const char* code, size_t len, char* ret, size_t* ret_len) {
    QVariant r = webview_->page()->currentFrame()->evaluateJavaScript(QString::fromLocal8Bit(code, len > 0 ? len : -1));
    std::string r_str = r.toString().toLocal8Bit();

    if (ret) {
        if (r.isValid()) {
            if (ret_len && *ret_len > 0) {
                strncpy(ret, r_str.c_str(), *ret_len);
            } else {
                strcpy(ret, r_str.c_str());
                *ret_len = r_str.size();
            }
        } else {
            if (ret)
                ret[0] = 0;
            if (ret_len)
                *ret_len = 0;
        }
    }
}

QString debuger::call(QString func_name, QStringList args) {
    std::string fn_name = func_name.toLocal8Bit();
    QString js_ret;

    if (js_fns_.end() == js_fns_.find(fn_name)) {
        return js_ret;
    }

    typedef const char* pt;
    std::vector<pt> param;
    std::vector<size_t> param_len;
    std::vector<QByteArray> param_utf8;
    param.reserve(static_cast<size_t>(args.size()));
    param_len.reserve(static_cast<size_t>(args.size()));
    param_utf8.reserve(static_cast<size_t>(args.size()));

    for (int i = 0; i < args.size(); ++i) {
        param_utf8.push_back(args[i].toLocal8Bit());
        param.push_back(param_utf8.back().constData());
        param_len.push_back(static_cast<size_t>(param_utf8.back().size()));
    }

    char* ret_str = nullptr;
    size_t ret_len = 0;
    debuger_js_fn fn = js_fns_[fn_name];
    fn(param.data(), param_len.data(), param_utf8.size(), &ret_str, &ret_len);

    if (nullptr != ret_str) {
        js_ret = QString::fromLocal8Bit(ret_str, ret_len > 0 ? static_cast<int>(ret_len): -1);
        free(ret_str);
    }

    return js_ret;
}

void debuger::setWindowSize(int w, int h) {
    webview_->setFixedSize(w, h);
}

void debuger::setMinWindowSize(int w, int h) {
    webview_->setMinimumSize(w, h);
}

void debuger::setMaxWindowSize(int w, int h) {
    webview_->setMaximumSize(w, h);
}

QString debuger::getFileContent(QString path) {
    QFile f(path);
    f.open(QFile::OpenMode::enum_type::ReadOnly);
    QString ret = f.readAll();
    return ret;
}

QString debuger::getWebFileContent(QString path) {
    QFile f(QDir(open_path_dir_).absoluteFilePath(path));
    f.open(QFile::OpenMode::enum_type::ReadOnly);
    QString ret = f.readAll();
    return ret;
}

void debuger::OnLoadFinished(bool ok) {
    //webview_->page()->sec
}


void debuger::OnTitleChanged(const QString& title) {
    webview_->setWindowTitle(title);
}

void debuger::OnResizeWebView(QSize size)
{
    //QSize contentSize = webview_->page()->currentFrame()->contentsSize(); //内容高度(获得的高度是正确的)
    //webview_->setMinimumHeight(contentSize.height()); //设置最小高度
}

void debuger::OnPopulateJavaScriptWindowObject() {
    webview_->page()->mainFrame()->addToJavaScriptWindowObject("debuger", this);
}

void debuger::OnAttachJobFinished(
#if !defined(Q_QDOC)
    QPrivateSignal
#endif
    ) {
    debuger_exit(0);
}
