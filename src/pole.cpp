// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include <QWidget>
#include <QPainter>

#include "disk.h"
#include "pole.h"

Pole::Pole(int index, int diskCount, QWidget *parent){
    this->index = index;
    this->diskCount = diskCount;
    this->parent = parent;

    this->initDisks(diskCount);
    this->setEnabled(true);
    this->setParent(parent);
    this->show();
}

void Pole::initDisks(int n){
    for(int i = 0; i < n; i++){
        Disk* disk = new Disk(n - i, i, this, parent);
        this->stack[i] = disk;
    }
}

// Pushes the target disk on top of the stack
bool Pole::push(Disk* disk){
    if(this->diskCount == 0){
        this->stack[0] = disk;
        this->diskCount++;
        disk->setPolePos(this, 0);
        this->raise();
        return true;
    }
    if(this->stack[this->diskCount-1]->getSize() < disk->getSize()) return false;

    stack[this->diskCount] = disk;
    this->diskCount++;
    disk->setPolePos(this, this->diskCount - 1);
    this->raise();
    return true;
}

// Pops & returns the disk that's on top of the stack
Disk* Pole::popTop(){
    Disk* disk = NULL;
    if(this->diskCount > 0){
      disk = this->stack[--this->diskCount];
      this->stack[this->diskCount] = NULL;
      disk->Lift();
    }
    return disk;
}

// `Index` Property
int Pole::getIndex(){
    return index;
}

// `diskCount` Property
int Pole::getDiskCount(){
    return this->diskCount;
}

// Paint Event
void Pole::paintEvent(QPaintEvent *event){
    (void) event;
    int centerPoint[2] = {
        this->parent->width()/2,
        this->parent->height()/2
    };

    QPainter pen(this);
    QColor poleColor(71, 80, 99);

    pen.setBrush(poleColor);
    // Pole
    pen.drawRect(centerPoint[0] - 8, centerPoint[1] + 130, 15, -centerPoint[1]);
    // Stage
    pen.drawRect(centerPoint[0] - 200/2, centerPoint[1] + 128, 200, 25);
}

// Destructor
Pole::~Pole(){
    this->setParent(NULL);
    for(int i = 0; i< this->diskCount; i++){
        this->stack[i]->deleteLater();
    }
}
