#include <boost/pool/pool_alloc.hpp>
#include <QMainWindow>
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.showMaximized();
    return app.exec();
}