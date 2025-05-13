
# ✅ Code Overview
This program:

- Initializes USART3 on STM32F103xx.
- Configures GPIO PB10 as TX and PB11 as RX (USART3 pins).
- Sends the character 'Z' repeatedly over UART at 9600 baud.

## 📘 Full Code with Documentation

```c
// Include device-specific header for STM32F10x series
#include "stm32f10x.h"    

int main(void)
{
    /* -------- 1. Enable Peripheral Clocks -------- */

    // Enable GPIOB and AFIO clocks
    // RCC->APB2ENR bit 3 (IOPBEN) and bit 0 (AFIOEN) => 0b00001001 = 0x09
    RCC->APB2ENR |= 0x09;

    // Enable USART3 clock (bit 18 of APB1ENR)
    RCC->APB1ENR |= (1 << 18);  // 0x40000

    /* -------- 2. Configure GPIO Pins (PB10 = TX, PB11 = RX) -------- */

    // Clear PB8 to PB15 configuration bits (CRH handles PB8–PB15)
    GPIOB->CRH &= ~(0xFFFF); // Clear all 16 bits (for safety)

    // Configure PB10 (TX) as Alternate Function Push-Pull (50 MHz)
    // MODE10 = 0b11 (50 MHz), CNF10 = 0b10 (AF Push-Pull)
    // Combined: 0b1011 = 0xB => placed at bits [11:8]
    GPIOB->CRH |= (0xB << 8); // Sets bits 11:8

    // Configure PB11 (RX) as Input Floating
    // MODE11 = 0b00, CNF11 = 0b01 => 0b1000 = 0x8 => bits [15:12]
    GPIOB->CRH |= (0x8 << 12);

    /* -------- 3. Configure USART3 -------- */

    // Set baud rate to 9600 (with 36 MHz APB1 clock)
    uint32_t system_clock = 36000000;  // 36 MHz
    uint32_t baud_rate = 9600;
    
    // BRR = Fck / BaudRate
    // Add (baud_rate / 2) for rounding
    USART3->BRR = (system_clock + (baud_rate / 2)) / baud_rate;

    // Enable Transmitter (bit 3, TE)
    USART3->CR1 |= (1 << 3);

    // Enable Receiver (bit 2, RE)
    USART3->CR1 |= (1 << 2);

    // Set word length to 9 bits (bit 12, M = 1)
    USART3->CR1 |= (1 << 12);

    // Enable USART3 (bit 13, UE)
    USART3->CR1 |= (1 << 13);

    /* -------- 4. Main Loop: Send Character 'Z' -------- */

    while (1) {
        char chat = 'Z';

        // Wait until Transmission Complete (bit 6, TC)
        while (!(USART3->SR & (1 << 6))) {
            // Busy-wait for transmit complete flag
        }

        // Load data into Data Register to transmit
        USART3->DR = chat;
    }
}
```

### 🔍 Pin Summary

| Peripheral | Pin  | Function       |
|------------|------|----------------|
| USART3_TX  | PB10 | TX (Transmit)  |
| USART3_RX  | PB11 | RX (Receive)   |

### ⚠ Notes
- Make sure USART3 is mapped correctly to PB10/PB11 (default mapping).
- Word length is set to 9 bits; change CR1 |= (1 << 12) to CR1 &= ~(1 << 12) for 8-bit.
- For receiving, a read from USART3->DR should be handled with RXNE (SR & (1 << 5)).
- No delay or wait between transmissions — 'Z' will be spammed rapidly.