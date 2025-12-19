# STM32 Snake Game with LED Dot Matrix & Bluetooth Control

Link for video demonstration: https://youtube.com/shorts/KJ2IRY3PYMU?si=vGa0bGhHLsTQnHkt

---

## `Introduction`
This project runs a classic Snake Game application on an 8x8 LED dot matrix driven by a MAX7219 display driver, running on an STM32 Nucleo-F302R8 development board. The game can be played using Bluetooth control via an HC-05 module, and real-time direction input can be made from a smartphone.

![ezgif-76fb343b474b0f72](https://github.com/user-attachments/assets/a23272c1-cd91-490f-b0c1-7e5c59dfb0b7)

## `Hardware Compenents`
- STM32 Nucleo F302R8
- 8×8 LED Dot Matrix
- MAX7219 LED Driver
- HC-05 Bluetooth Module
- Jumper Wires (Male–Female)

<img width="449" height="534" alt="Screenshot 2025-11-18 161919" src="https://github.com/user-attachments/assets/1dcc0e3c-2744-4db4-abeb-94077e4802b9" />


## `Pin Configuration`

<img width="572" height="492" alt="Screenshot 2025-11-18 145800" src="https://github.com/user-attachments/assets/56cae7e8-617c-48dd-b6ef-18d2bc729ee0" />

- PC4 -> USART1_TX -> Bluetooth_RX
- PC5 -> USART1_RX -> Bluetooth_TX
- PC10 -> SPI3_SCK (Clock) -> MAX7219_CLK
- PC11 -> CS (Chip Select) -> MAX7219_CS
- PC12 -> SPI3_MOSI (Master Out Slave In) -> MAX7219_DIN
- MAX7219_VCC -> +5V
- Bluetooth_VCC -> +5V


## `MAX7219 LED Driver`

MAX7219 communicates with STM32 using an SPI-compatible interface.
The device expects a **16-bit frame**. This frame consists of a **4-bit register address** (D11-D8) and **8-bit data** (D7-D0). The most significant four bits (D15-D12) are typically 'Don't Care' bits.
The STM32 SPI peripheral is configured in Half-Duplex Master mode to transmit this 16-bit data.

<img width="769" height="61" alt="Screenshot 2025-11-18 160507" src="https://github.com/user-attachments/assets/0a08b13b-9e6c-4e6a-9f3d-d6c4dd1d5080" />

---

The MAX7219 CS pin is not automatic and is controlled manually as a GPIO output.
- The default GPIO output level of the CS pin is set to **HIGH**.
- Before starting a data transfer, the CS pin is pulled **LOW**.
- After the SPI transfer is complete, CS is set back to **HIGH**.

```c
void MAX7219_SendData(uint8_t Addr, uint8_t data)
{
	uint16_t writeData = (Addr<<8)|data;
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);               //enable slave
	HAL_SPI_Transmit(&hspi3, (uint8_t *)&writeData, 1, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1); 			  //disable slave
}
```


## `Bluetooth Control`

System control was achieved via wireless communication through a mobile platform-based application.The Arduino Bluetooth Control Application was integrated onto the mobile device, and specific command values were assigned to the directional buttons using the application's Gamepad interface.The physical layer connection between the mobile device and the embedded system (STM32) was established using an HC-05 Bluetooth Module . User inputs were carried via the Bluetooth SPP (Serial Port Profile) protocol, and this data was finally transmitted asynchronously to the main microcontroller (STM32) using the UART (Universal Asynchronous Receiver-Transmitter) serial communication interface.

<img width="1058" height="493" alt="Screenshot 2025-11-18 154638" src="https://github.com/user-attachments/assets/e6dfd7f0-c8ab-43fa-80bf-8dfe2bb5ace8" />

	- w: Move Up
	- s: Move Down
	- a: Move Left
	- d: Move Right

---

**The HAL_UART_RxCpltCallback()** function processes incoming Bluetooth commands using an interrupt-driven mechanism. Each time a character is received, **SetDirection()** is called to update the snake’s movement direction. After handling the input, UART reception is re-enabled in non-blocking mode.
```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){

		SetDirection(rx_data);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	}
}
```

**The SetDirection()** function checks the received 'w', 'a', 's', 'd' commands and prevents the snake from reversing its direction, ensuring safe direction updates.
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

<details>
  <summary><b>Yardımcı Pin Fonksiyonlarını Görüntüle (AD0, INT, XDA)</b></summary>
  <ul>
    <li><b>AD0:</b> I2C adres seçimi (0x68 veya 0x69).</li>
    <li><b>INT:</b> İşlemci verimliliği için kesme sinyali.</li>
    <li><b>XDA/XCL:</b> Harici sensör genişletme portları.</li>
  </ul>
</details>

