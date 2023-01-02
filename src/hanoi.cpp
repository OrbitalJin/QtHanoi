// Data Structures & Algorithms.
// TP5: Towers Of Hanoi.
// Author: Ouzaher Saad

#include "hanoi.h"
#include "ui_hanoi.h"

Disk* MOVING = NULL;

// Constructor
Hanoi::Hanoi(QWidget *parent): QMainWindow(parent), ui(new Ui::Hanoi){
    this->ui->setupUi(this);
    this->adjustUi();

    this->defaultCount = MAX_DISKS;
    this->autoSolve = false;
    this->deltaTime = 100;
    this->eventScheduler = new QTimer(this);
    this->Player = new QMediaPlayer(this);

    this->initPoles();
    this->toggleControls(true);
    this->connectSignalsAndSlots();
}

// Ui adjustements
void Hanoi::adjustUi(){
    const QIcon appIcon(":/assets/assets/imgs/TowerOfHanoi.png");
    this->setWindowIcon(appIcon);
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);

    this->ui->spinBox->setMaximum(MAX_DISKS);
    this->ui->spinBox->setValue(MAX_DISKS);
    this->ui->spinBox->setSuffix(" Disk(s).");

    this->ui->deltaTimeSlider->setRange(100, 1000);
    this->ui->deltaTimeSlider->setValue(100);
    this->ui->lockedCheckBox->setChecked(false);
    this->ui->sfxCheckBox->setChecked(true);
}

// Initializing the poles & keeping track of their references.
void Hanoi::initPoles(){
    this->poles[0] = new Pole(0, this->defaultCount, this->ui->fromPoleBtn);
    this->poles[1] = new Pole(1, 0, this->ui->auxPoleBtn);
    this->poles[2] = new Pole(2, 0, this->ui->toPoleBtn);
}

// Connect Signals to Slots and point them to their appropriate Callbacks.
void Hanoi::connectSignalsAndSlots(){
    this->connect(this->ui->closeAppBtn, &QPushButton::clicked, this, &Hanoi::close);
    this->connect(this->ui->minimizeAppBtn, &QPushButton::clicked, this, &Hanoi::showMinimized);
    this->connect(this->ui->maximizeAppBtn, &QPushButton::clicked, this, &Hanoi::toggleMaximized);

    this->connect(this->ui->fromPoleBtn, &QPushButton::clicked, this, &Hanoi::fromPoleBtnCallback);
    this->connect(this->ui->auxPoleBtn, &QPushButton::clicked, this, &Hanoi::auxPoleBtnCallback);
    this->connect(this->ui->toPoleBtn, &QPushButton::clicked, this, &Hanoi::toPoleBtnCallback);

    this->connect(this->ui->resetGameBtn, &QPushButton::clicked, this, &Hanoi::resetGame);
    this->connect(this->ui->undoMoveBtn, &QPushButton::clicked, this, &Hanoi::undoMoveCallback);
    this->connect(this->ui->startAutoSolveBtn, &QPushButton::clicked, this, &Hanoi::autoSolveBtnCallback);
    this->connect(this->ui->generateStepsBtn, &QPushButton::clicked, this, &Hanoi::generateStepsCallback);

    this->connect(this->ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Hanoi::spinBoxSlot);
    this->connect(this->ui->lockedCheckBox, &QCheckBox::stateChanged, this, &Hanoi::lockedCheckBoxSlot);
    this->connect(this->ui->sfxCheckBox, &QCheckBox::stateChanged, this, &Hanoi::sfxCheckBoxSlot);
    this->connect(this->ui->deltaTimeSlider, &QSlider::valueChanged, this, &Hanoi::deltaTimeSliderSlot);

    this->connect(this->eventScheduler, &QTimer::timeout, this, &Hanoi::autoHanoiSolver);
}

// Toggle Controls.
void Hanoi::toggleControls(bool enable){
    this->ui->fromPoleBtn->setEnabled(enable);
    this->ui->auxPoleBtn->setEnabled(enable);
    this->ui->toPoleBtn->setEnabled(enable);
}

// Toggle Settings.
void Hanoi::toggleSettings(bool enabled){
    this->ui->configGroupBox->setEnabled(enabled);
    this->ui->startAutoSolveBtn->setEnabled(enabled);
    this->ui->generateStepsBtn->setEnabled(enabled);
    this->ui->undoMoveBtn->setEnabled(enabled);
    this->ui->stepsViewer->setEnabled(enabled);
}

// Reset Simulation to it's original state.
void Hanoi::resetGame(){
    for(auto pole: this->poles) pole->deleteLater();
    if(MOVING) MOVING->deleteLater();
    if(MOVING) MOVING = NULL;

    this->ui->moveCountLCD->display(0);
    this->ui->stepsViewer->clear();

    this->toggleControls(true);
    this->eventScheduler->stop();

    this->history.clear();
    this->scheduledSteps.clear();
    this->initPoles();
}

// Recursive algorithm to compute & enqueue the moves to solve the game.
void Hanoi::computeMoves(int diskCount, int from, int to, int aux){
    if(diskCount == 0 ) return;

    this->computeMoves(diskCount - 1, from, aux, to);
    this->pushStep(diskCount, from, to);
    this->computeMoves(diskCount - 1, aux, to, from);
}

// Write the steps to the QListWidget.
void Hanoi::pushStep(int diskCount, int from, int to){
    QString stepLabel = QString("Move %0 from Pole %1 to Pole %2")
                    .arg(
                    QString::number(diskCount),
                    QString::number(from),
                    QString::number(to)
                );
    this->scheduledSteps.enqueue(Move(from, to));
    this->ui->stepsViewer->addItem(new QListWidgetItem(stepLabel, this->ui->stepsViewer));
}

// Handle incrementation of Step (signed).
void Hanoi::incrementStepCount(int increment){
    this->ui->stepsViewer->setCurrentRow(this->ui->stepsViewer->currentRow() + increment);
    this->ui->moveCountLCD->display(this->ui->moveCountLCD->value() + increment);
}

// Handle pole click events.
void Hanoi::poleClickHandler(int poleNum){
    if(!MOVING){
        MOVING = this->poles[poleNum]->popTop();
        this->Player->setMedia(QUrl("qrc:/assets/assets/audio/Lift.wav"));
    }

    else{
        int from = MOVING->getPole()->getIndex();
        if(this->poles[poleNum]->push(MOVING)){
            Move move(from, this->poles[poleNum]->getIndex());
            this->Player->setMedia(QUrl("qrc:/assets/assets/audio/Place.wav"));
            this->incrementStepCount(1);
            history.push(move);
        }
        else{
            MOVING->getPole()->push(MOVING);
            this->Player->setMedia(QUrl("qrc:/assets/assets/audio/BadMove.wav"));
        }
        MOVING = NULL;
    }
    this->Player->play();
}

// Auto Solver
void Hanoi::autoHanoiSolver(){
    if(!this->autoSolve) return;
    if(this->scheduledSteps.empty()){
        this->autoSolve = false;
        this->toggleSettings(true);
        this->eventScheduler->stop();
        return;
    }

    Move Step = this->scheduledSteps.dequeue();
    MOVING = poles[Step.originPole()]->popTop();
    poles[Step.targetPole()]->push(MOVING);
    this->ui->lockedCheckBox->setChecked(false);
    this->toggleSettings(false);
    this->incrementStepCount(1);
    this->history.push(Step);
}

////// Callbacks

// Starting Pole Button Callback
void Hanoi::fromPoleBtnCallback(){
    poleClickHandler(0);
}
// Auxiliary Pole Button Callback
void Hanoi::auxPoleBtnCallback(){
    poleClickHandler(1);
}

// Destination Pole Button Callback
void Hanoi::toPoleBtnCallback(){
    poleClickHandler(2);
}

// Generate Steps Callback
void Hanoi::generateStepsCallback(){
    this->ui->stepsViewer->clear();
    this->computeMoves(this->poles[0]->getDiskCount(), 0, 2, 1);
    this->ui->stepsViewer->setCurrentRow(0);
}

// Autoplay Button Callback
void Hanoi::autoSolveBtnCallback(){
    this->autoSolve = true;
    this->toggleControls(false);

    this->resetGame();
    this->generateStepsCallback();
    this->eventScheduler->start(this->deltaTime);
}

void Hanoi::undoMoveCallback(){
   if(this->history.empty()) return;
   Move prevStep = history.pop();
   Pole* to = poles[prevStep.targetPole()];
   Pole* from = poles[prevStep.originPole()];
   this->incrementStepCount(-1);
   if(MOVING) MOVING->getPole()->push(MOVING);
   MOVING = to->popTop();
   from->push(MOVING);
   MOVING = NULL;
}

////// Slots

// SpinBox valueChanged Callback
void Hanoi::spinBoxSlot(int value){
    this->defaultCount = value;
    this->resetGame();
}

// Manually toggle the Pole Widgets.
void Hanoi::lockedCheckBoxSlot(bool enabled){
    this->toggleControls(!enabled);
}

// Toggle SFX on or Off
void Hanoi::sfxCheckBoxSlot(bool enabled){
    this->Player->setMuted(!enabled);
}

// Slider valueChanged Callback
void Hanoi::deltaTimeSliderSlot(int value){
    this->deltaTime = value;
}

////// Events

// Paint Event
void Hanoi::paintEvent(QPaintEvent* event){
    (void) event;
    QPainter pen(this);
    pen.setRenderHint(QPainter::Antialiasing);
}

// MousePress Event
void Hanoi::mousePressEvent(QMouseEvent* event){
    this->currentPos = event->globalPos();
}

// MouseMove Event
void Hanoi::mouseMoveEvent(QMouseEvent* event){
    if(!this->ui->topFrame->underMouse()) return;
    this->newPos = QPoint(event->globalPos() - currentPos);
    this->move(this->x() + this->newPos.x(), this->y() + this->newPos.y());
    this->currentPos = event->globalPos();
}

void Hanoi::toggleMaximized(){
    if(this->isMaximized()) this->showNormal();
    else this->showMaximized();
}

// Destructor
Hanoi::~Hanoi(){
    MOVING = NULL;
    this->history.clear();
    this->scheduledSteps.clear();
    for(auto pole: poles) pole->deleteLater();
    delete this->ui;
}

//void Hanoi::poleClickHandler(int poleNum){
//    if(!MOVING) MOVING = this->poles[poleNum]->popTop();
//    else{
//        int from = MOVING->getPole()->getIndex();
//        if(this->poles[poleNum]->push(MOVING)){
//            Move move(from, this->poles[poleNum]->getIndex());
//            this->incrementStepCount(1);
//            history.push(move);
//        }
//        else MOVING->getPole()->push(MOVING);
//        MOVING = NULL;
//    }
//}


