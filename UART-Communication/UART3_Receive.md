# ✅ Code Overview
This program:

- Initializes USART3.
- Configures PB10 as TX and PB11 as RX.
- Receives a character from the serial input at 9600 baud and stores it.

## 📘 Full Code with Documentation

```c
#include "stm32f10x.h"  // Device header file for STM32F10x series

int main(void)
{
    /* -------- 1. Enable Peripheral Clocks -------- */

    // Enable GPIOB (bit 3) and AFIO (bit 0) clocks
    RCC->APB2ENR |= 0x09;  // 0b00001001 = 0x09

    // Enable USART3 clock on APB1 (bit 18)
    RCC->APB1ENR |= (1 << 18); // 0x40000

    /* -------- 2. Configure GPIO Pins PB10 (TX) and PB11 (RX) -------- */

    // Clear PB8 to PB15 configuration (16 bits in CRH)
    GPIOB->CRH &= ~(0xFFFF); // Clear all bits

    // Configure PB10 (USART3 TX) as AF Push-Pull Output @ 50 MHz
    // MODE10 = 0b11, CNF10 = 0b10 => 0b1011 = 0xB => bits [11:8]
    GPIOB->CRH |= (0xB << 8); // Sets bits 11:8

    // Configure PB11 (USART3 RX) as Floating Input
    // MODE11 = 0b00, CNF11 = 0b01 => 0b1000 = 0x8 => bits [15:12]
    GPIOB->CRH |= (0x8 << 12);

    /* -------- 3. Configure USART3 -------- */

    // Setup baud rate = 9600, with APB1 = 36 MHz
    uint32_t system_clock = 36000000;
    uint32_t baud_rate = 9600;
    USART3->BRR = (system_clock + (baud_rate / 2)) / baud_rate;

    // Enable Transmitter (bit 3) and Receiver (bit 2)
    USART3->CR1 |= (1 << 3); // TE: Transmit enable
    USART3->CR1 |= (1 << 2); // RE: Receive enable

    // Set word length to 9 bits (bit 12 = 1)
    USART3->CR1 |= (1 << 12);

    // Enable USART (bit 13 = UE)
    USART3->CR1 |= (1 << 13);

    /* -------- 4. Main Loop: Receive Characters -------- */

    while (1) {
        char chat;

        // Wait until a character is received (bit 5, RXNE = Read Data Register Not Empty)
        while (!(USART3->SR & (1 << 5))) {
            // Busy wait for character
        }

        // Read the received data (this clears RXNE automatically)
        chat = USART3->DR;

        // Optional: Store received char as null-terminated string
        char message[2];
        message[0] = chat;
        message[1] = '\0';

        // Display message if a display or debug interface is available
        // display(message); // Uncomment and implement if needed
    }
}
```

