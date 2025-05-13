#include "stm32f10x.h"

int main(){
	
	RCC->APB2ENR |= 1; // Enable alternating function
	RCC->APB2ENR |= 8; // Enable clock for GPIOB
	
	RCC->APB1ENR |= 0x40000; // Enable UART3
	
	GPIOB->CRH &=~(0xFF00); // Reset B10 and B11 pin
	GPIOB->CRH |=	(0x8B00); // Configure B10 as TX(alternating function output) and B11 as RX (input push/pull)
	
	uint32_t system_clock = 36000000;
	uint32_t baud_rate = 9600;
	
	USART3->BRR = (system_clock + baud_rate/2)/baud_rate; // Set baud rate
	
	
	USART3->CR1 |= 0x2000; // Set UART3 enable
	
	USART3->CR1 |= 0xC; // Enable Tx and Rx pin
	
	char chat = 'A';
	
	while(1){
	
		while(!(USART3->SR & 1<<6)){}
	
		USART3->DR = chat;
	}
	
	
}