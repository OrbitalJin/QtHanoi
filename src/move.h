// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

class Move{
private:
    int from;
    int to;

public:
    Move();
    Move(int f, int t);
    int originPole();
    int targetPole();
};
