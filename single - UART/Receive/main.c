#include "stm32f10x.h"

// Define Pins
#define RS_HIGH   (GPIOA->BSRR = (1<<0))
#define RS_LOW    (GPIOA->BRR  = (1<<0))

#define EN_HIGH   (GPIOA->BSRR = (1<<1))
#define EN_LOW    (GPIOA->BRR  = (1<<1))

void delay(volatile uint32_t t) {
    while(t--);
}

void pulse_enable() {
    EN_HIGH;
    delay(2000);
    EN_LOW;
    delay(2000);
}

void send_4bit(uint8_t data) {
    if(data & 0x01) GPIOA->BSRR = (1<<4); else GPIOA->BRR = (1<<4);
    if(data & 0x02) GPIOA->BSRR = (1<<5); else GPIOA->BRR = (1<<5);
    if(data & 0x04) GPIOA->BSRR = (1<<6); else GPIOA->BRR = (1<<6);
    if(data & 0x08) GPIOA->BSRR = (1<<7); else GPIOA->BRR = (1<<7);
}

void send_command(uint8_t cmd) {
    RS_LOW;
    send_4bit(cmd >> 4);
    pulse_enable();
    send_4bit(cmd & 0x0F);
    pulse_enable();
    delay(40000);
}

void send_data(uint8_t data) {
    RS_HIGH;
    send_4bit(data >> 4);
    pulse_enable();
    send_4bit(data & 0x0F);
    pulse_enable();
    delay(40000);
}

void LCD_Init() {
    delay(800000); // Wait LCD power up

    RS_LOW;
    send_4bit(0x03);
    pulse_enable();
    delay(50000);

    send_4bit(0x03);
    pulse_enable();
    delay(50000);

    send_4bit(0x03);
    pulse_enable();
    delay(50000);

    send_4bit(0x02); // Set 4-bit mode
    pulse_enable();
    delay(50000);

    send_command(0x28); // 4-bit, 2 line, 5x8 dots
    send_command(0x0C); // Display ON, cursor OFF
    send_command(0x06); // Entry mode
    send_command(0x01); // Clear display
    delay(50000);
}

void LCD_SendString(char *str) {
    while(*str) {
        send_data(*str++);
    }
}

int main() {
    // Clock enable for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Set PA0–PA7 as output push-pull 2MHz
    GPIOA->CRL &= ~(0xFFFFFFFF);
    GPIOA->CRL |=  (0x22222222);

    LCD_Init();
    LCD_SendString("HELLO GUYS");

    while(1);
}
