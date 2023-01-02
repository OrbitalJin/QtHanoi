// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include <QPainter>

#include "pole.h"
#include "disk.h"

// Constructor
Disk::Disk(int size, int pos, Pole* pole, QWidget* parent){
    this->size = size;
    this->pole = pole;
    this->pos = pos;
    this->parent = parent;

    this->setParent(parent);
    this->show();
}

// Method that visually lifts the `Disk` on top of the stack
void Disk::Lift(){
    this->pos = 12;
    this->repaint();
}

// Returns the size of the current stack
int Disk::getSize(){
    return this->size;
}

// Returns the current `Pole` parent of the current `Disk`
Pole* Disk::getPole(){
    return this->pole;
}

// Change the parent of the current `Disk` to ``
void Disk::setPolePos(Pole* to, int p){
    if(this->getPole() != to){                  // Makes sure that the `Disk` isn't already on the target `Pole`
        this->pole = to;
        this->setParent(to->parentWidget());
        this->show();
    }

    this->pos = p;
    this->repaint();
}

// Onverloadint the paintEvent in order to draw & adjust the colors of any given `Disk` depending on it's size & index
void Disk::paintEvent(QPaintEvent *event){
    (void) event;
    int width = (size * 20) + 20;
    int centerPoint[2] = {
        this->parent->width()/2,
        this->parent->height()/2
    };

    QPainter pen(this);
    QColor palette[] = {
        QColor(170, 90, 99), QColor(192, 130, 113),
        QColor(208, 181, 127), QColor(147, 170, 128),
        QColor(137, 175, 175), QColor(118, 145, 174),
        QColor(163, 130, 158), QColor(126, 174, 188)
    };

    QColor diskColor = palette[size-1];

    // Smooths the rendering (using interpolation I think?)
    pen.setRenderHint(QPainter::Antialiasing);
    pen.setBrush(diskColor);
    pen.drawRect(centerPoint[0] - width/2, (100 + 260 - 5) - 23 * (pos + 1), width, 22);
    this->raise();
}

