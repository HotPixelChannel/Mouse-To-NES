/* Подключение PS/2 мыши к NES/Famicom/Dendy вместо штатного геймпада

   (C)HotPixel, 2022
   https://www.youtube.com/hotpixel

  **********************
  Пины Arduino для геймпада NES
  2 - CLK
  4 - DATA
  3 - LATCH
  GND и +5 ардуины соединить с соответствующими пинами геймпада
*/

#include "PS2MouseHandler.h"
#include "Gamepad.h"

// Пины мыши
#define MOUSE_DATA A5
#define MOUSE_CLOCK A4

#define SENS 6 // Чувствительность срабатывания перемещения мыши

PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_STREAM);
Gamepad pad = Gamepad();

long lastTime;


/**
   Setup
*/
void setup() {
  mouse.initialise();
  mouse.set_remote_mode(); // Включаем ROMOTE режим на мышке, чтоб забирать данные только по нашему требованию
}


void loop() {

  // Читаем данные с мыша с периодом +-5мкс
  if (millis() - lastTime > 5 ) {
    pad.writeHi();
    mouse.get_data();
    lastTime = millis();
  }

  pad.pressButton(B, mouse.button(0));
  pad.pressButton(A, mouse.button(2));

  pad.pressButton(SELECT, mouse.z_movement() > 0);
  pad.pressButton(START, mouse.z_movement() < 0);

  pad.pressButton(RIGHT, mouse.x_movement() > SENS);
  pad.pressButton(LEFT, mouse.x_movement() < 0 - SENS );

  pad.pressButton(UP, mouse.y_movement() > SENS);
  pad.pressButton(DOWN, mouse.y_movement() < 0 - SENS);

  // Пишем в NES дважды, на случай пропуска команды (не уверен, что надо, но в некоторых играх помогает)
  for (int i = 0; i < 2; i++)
    pad.writeData();

}
