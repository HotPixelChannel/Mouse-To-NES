#include "Gamepad.h"
#include "Arduino.h"


Gamepad::Gamepad() {

  pinMode(2, INPUT);   //CLK
  pinMode(4, OUTPUT);  //DATA
  writeHi();
  pinMode(3, INPUT);   //LATCH

  reset();
}

int Gamepad::getData() {
  return dataPad;
}

void Gamepad::reset() {
  dataPad = 0;
}

void Gamepad::writeData() {

  /* Эмуляция крестовины работает через счетчики,
      чтобы уменьшить вероятность ложного срабатывания
      или пропуска команд*/

  if ((x > 0 - DIR_SENS * 4 & x < DIR_SENS * 4) | x == 0) {
    dataPad &= ~(1 << 7);
    dataPad &= ~(1 << 6);
  } else if (x > 0) {
    dataPad |= (1 << 7);
    x--;
  } else {
    dataPad |= (1 << 6);
    x++;
  }

  if ((y > 0 - DIR_SENS * 4 & y < DIR_SENS * 4) | y == 0) {
    dataPad &= ~(1 << 4);
    dataPad &= ~(1 << 5);
  }  else if (y > 0) {
    dataPad |= (1 << 4);
    y--;
  } else {
    dataPad |= (1 << 5);
    y++;
  }


  if (dataPad == 0) {
    return;
  }


  waitLatch(); // Ждем сигнал сброса от NES
  cli();       // Отключаем прерывания, чтобы ничего не мешало правильному формированию пакета

  // Передаем каждый бит при изменении сигнала Clock
  for (int i = 0; i < 8; i++) {
    if (dataPad & (1 << i))
      writeLo();
    else
      writeHi();

    waitClock(HIGH);
  }
  writeHi();

  sei(); // Включаем прерывания для опроса мыши
}

/**
   Запись состояний кнопок в посылку
*/
void Gamepad::pressButton(int button, int state) {
  switch (button) {
    case A:

      if (state == HIGH) {
        dataPad |= (1 << 0);
      } else
        dataPad &= ~(1 << 0);
      break;

    case B:
      if (state == HIGH) {
        dataPad |= (1 << 1);
      } else
        dataPad &= ~(1 << 1);
      break;

    case SELECT:
      if (state == HIGH)
        dataPad |= (1 << 2);
      else
        dataPad &= ~(1 << 2);
      break;

    case START:
      if (state == HIGH)
        dataPad |= (1 << 3);
      else
        dataPad &= ~(1 << 3);
      break;

    case UP:
      if (state == 0)
        break;
      if (y < 0)
        y = 0;
      y = y + DIR_SENS;
      break;

    case DOWN:
      if (state == 0)
        break;
      if (y > 0)
        y = 0;
      y = y - DIR_SENS;

      break;

    case LEFT:
      if (state == 0)
        break;
      if (x > 0)
        x = 0;
      x = x - DIR_SENS;
      break;

    case RIGHT:
      if (state == 0)
        break;
      if (x < 0)
        x = 0;
      x = x + DIR_SENS;
      break;
  }
}

// DigitalRead() работает очень тормознуто, юзаем прямой доступ к портам

void Gamepad::waitClock(int state) {
  if (state) {
    while (PIND & (1 << PIND2)) {};
  } else {
    while (!(PIND & (1 << PIND2))) {};
  }
}


void Gamepad::waitLatch() {
  while (!(PIND & (1 << PIND3))) {};
}

void Gamepad::writeLo() {
  PORTD &= ~(1 << 4);
}

void Gamepad::writeHi() {
  PORTD |= (1 << 4);
}
