#ifndef Gamepad_h

#define Gamepad_h

class Gamepad {

#define DIR_SENS 2 // Чувствительность (продолжительность) нажатия крестовины

#define UP  1
#define DOWN  2
#define LEFT  3
#define RIGHT  4
#define A  5
#define B  6
#define SELECT  7
#define START  8



  private:
    int _port;
    volatile int dataPad;
    void writeLo();

    void waitLatch();
    void waitClock(int);
    int _lastClk;
    volatile int x, y;

  public:
    Gamepad();
    void pressButton(int, int);
    int getData();
    void writeData();
    int isWritting();
    void reset();
    void writeHi();
};

#endif Gamepad_h
