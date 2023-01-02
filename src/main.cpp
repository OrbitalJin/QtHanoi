// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include <QApplication>
#include "hanoi.h"

// Entry Point
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Hanoi instance;
    instance.show();
    return app.exec();
}
