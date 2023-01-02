// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include<QWidget>

class Pole;

// Pos refers to index relative to the pole, n being the top, and 0 the bottom.
class Disk: public QWidget{
private:
    int size;
    int pos;
    Pole* pole;
    QWidget* parent;

public:
    Disk(int size, int pos, Pole* on, QWidget* parent);
    void Lift();
    int getSize();
    Pole* getPole();
    void setPolePos(Pole* d, int p);
    void paintEvent(QPaintEvent * event);
};


