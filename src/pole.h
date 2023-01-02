// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include <QWidget>

const int MAX_DISKS = 8;

class Disk;

class Pole: public QWidget{
private:
    int index;
    int diskCount;

    QWidget* parent;
    Disk* stack[MAX_DISKS];

public:
    Pole(int,int, QWidget*);
    void initDisks(int);
    Disk* popTop();
    bool push(Disk*);// false means size is wrong or pointer NULL. put the disk on the pole, true success.
    int getIndex();
    int getDiskCount();
    void paintEvent(QPaintEvent*);
    ~Pole();
};

