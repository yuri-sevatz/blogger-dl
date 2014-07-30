#include "app.hpp"

#include <QApplication>

int main(int argc, char * argv[]) {

    QApplication app(argc, argv);

    App ui;
    ui.init(app);

    return app.exec();
}
