# STM32 Snake Game with MAX7219 & Bluetooth Control

Link for video demonstration: https://youtube.com/shorts/KJ2IRY3PYMU?si=vGa0bGhHLsTQnHkt

## `Introduction`
This project runs a classic Snake Game application on an 8x8 LED dot matrix driven by a MAX7219 display driver, running on an STM32 Nucleo-F302R8 development board. The game can be played using Bluetooth control via an HC-06 module, and real-time direction input can be made from a smartphone.

![ezgif-76fb343b474b0f72](https://github.com/user-attachments/assets/a23272c1-cd91-490f-b0c1-7e5c59dfb0b7)

## `Hardware Compenents`
- STM32 Nucleo F302R8
- 8×8 LED Dot Matrix
- MAX7219 LED driver
- HC-06 Bluetooth Module
- Jumper Wires (Male–Female)

<img width="687" height="794" alt="Screenshot 2025-11-18 144349" src="https://github.com/user-attachments/assets/0a06e01f-cf34-401a-ad33-f378c493f5f1" />

## `Pin Configuration`

<img width="572" height="492" alt="Screenshot 2025-11-18 145800" src="https://github.com/user-attachments/assets/56cae7e8-617c-48dd-b6ef-18d2bc729ee0" />

## `Bluetooth Control`

- w: Move Up
- s: Move Down
- a: Move Left
- d: Move Right

```c
void SetDirection(char cmd) {
    switch (cmd) {
        case 'w': if (SnakeDirection != DOWN)  SnakeDirection = UP;    break;
        case 's': if (SnakeDirection != UP)    SnakeDirection = DOWN;  break;
        case 'a': if (SnakeDirection != RIGHT) SnakeDirection = LEFT;  break;
        case 'd': if (SnakeDirection != LEFT)  SnakeDirection = RIGHT; break;
    }
}```

## asdasdasdada

