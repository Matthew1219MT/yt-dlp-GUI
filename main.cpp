#include "view.h"
#include "model.h"
#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Model      model;
    View       view;
    Controller controller(&view, &model);

    view.show();
    return a.exec();
}