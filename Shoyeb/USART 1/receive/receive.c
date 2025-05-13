#include "stm32f10x.h"

int main(){
	
	RCC->APB2ENR |= 1; // Enable alternating function
	RCC->APB2ENR |= 4; // Enable clock for GPIOA
	
	RCC->APB2ENR |= 0x4000; // Enable UART1
	
	GPIOA->CRH &=~(0xFF0); // Reset A9 and A10 pin
	GPIOA->CRH |=	(0x8B0); // Configure A9 as TX(alternating function output) and A10 as RX (input push/pull)
	
	uint32_t system_clock = 36000000;
	uint32_t baud_rate = 9600;
	
	USART1->BRR = (system_clock + baud_rate/2)/baud_rate; // Set baud rate
	
	
	USART1->CR1 |= 0x2000; // Set UART1 enable
	
	USART1->CR1 |= 0xC; // Enable Tx and Rx pin
	
	char chat = 'A';
	
	while(1){
	
		while(!(USART1->SR & 1<<6)){}
	
		chat = USART1->DR ;
	}
	
	
}