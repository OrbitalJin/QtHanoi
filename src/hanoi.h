// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QPainter>
#include <QSound>
#include <QDebug>
#include <QStack>
#include <QQueue>
#include <QTimer>

#include "pole.h"
#include "disk.h"
#include "move.h"

// Qt's Quirk
namespace Ui{
    class Hanoi;
}

class Hanoi: public QMainWindow{
    // Q_Object Macro
    Q_OBJECT

public:
    Hanoi(QWidget *parent = 0);
    ~Hanoi();

// Construction Members
private:    
    Ui::Hanoi* ui;
    Pole* poles[3];

    QTimer* eventScheduler;
    QQueue<Move> scheduledSteps;
    QStack<Move> history;

    QPoint currentPos;
    QPoint newPos;

    QMediaPlayer *Player;

    int defaultCount;
    bool autoSolve;
    int deltaTime;

// Methods
private:
    void initPoles();
    void adjustUi();
    void toggleMaximized();
    void connectSignalsAndSlots();
    void autoHanoiSolver();
    void toggleControls(bool);
    void poleClickHandler(int);
    void computeMoves(int, int, int, int);
    void incrementStepCount(int);
    void pushStep(int, int, int);
    void toggleSettings(bool);
    void resetGame();


// Callbacks
private:
    void fromPoleBtnCallback();
    void auxPoleBtnCallback();
    void toPoleBtnCallback();
    void autoSolveBtnCallback();
    void generateStepsCallback();
    void undoMoveCallback();

// Slots
private slots:
    void spinBoxSlot(int);
    void deltaTimeSliderSlot(int);
    void lockedCheckBoxSlot(bool);
    void sfxCheckBoxSlot(bool);
// Events
private:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
};
