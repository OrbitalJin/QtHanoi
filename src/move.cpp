// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include "move.h"

Move::Move(int f, int t): from(f), to(t){};
Move::Move(): from(0), to(0){};

int Move::originPole(){
    return this->from;
}

int Move::targetPole(){
    return this->to;
}

