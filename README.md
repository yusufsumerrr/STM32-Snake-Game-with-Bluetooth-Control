# STM32 Snake Game with LED Dot Matrix & Bluetooth Control

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

- PC4 -> USART1_TX
- PC5 -> USART1_RX
- PC10 -> SPI3_SCK (Clock)
- PC11 -> CS (Chip Select)
- PC12 -> SPI3_MOSI (Master Out Slave In)
- MAX7219_VCC -> 5V
## `Bluetooth Control`

- w: Move Up
- s: Move Down
- a: Move Left
- d: Move Right

![4d4c545a-e758-4b28-9b92-c54a6463f355](https://github.com/user-attachments/assets/5d89a9aa-3f6d-4fd4-ad15-d90aac2e596a)

```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){

		SetDirection(rx_data);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	}
}
```

```c
void SetDirection(char cmd) {
    switch (cmd) {
        case 'w': if (SnakeDirection != DOWN)  SnakeDirection = UP;    break;
        case 's': if (SnakeDirection != UP)    SnakeDirection = DOWN;  break;
        case 'a': if (SnakeDirection != RIGHT) SnakeDirection = LEFT;  break;
        case 'd': if (SnakeDirection != LEFT)  SnakeDirection = RIGHT; break;
    }
}
```



