#include <QApplication>
#include "Solving.h"
#include "ModifDialog.h"
#include "AddDialog.h"

int main(int argc, char *argv[])
{
    //git����
    QApplication  app(argc, argv);

    MyTimeSolveProject  *MyProject;

    MyProject = new MyTimeSolveProject;


    MyProject->show();

    return app.exec();
}


