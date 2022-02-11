/*
 * Practica2.h
 *
 *  Created on: 10 feb. 2022
 *      Author: Javier Presmanes
 */

#ifndef SRC_PRACTICA2_PRACTICA2_H_
#define SRC_PRACTICA2_PRACTICA2_H_

#include "main.h"
#include "stm32h7xx_hal_uart.h"
#include "../Practica1/Practica1.h"
#include "stdlib.h"

void change_led_state(uint8_t pin, uint8_t mode);
void configure_out_leds();

#define RCC_APB2ENR (*((volatile uint32_t*)(RCC_BASE+0xF0)))
#define RCC_C1_APB2ENR_OFFSET (0x150)
#define RCC_C2_APB2ENR_OFFSET (0x1B0)

extern UART_HandleTypeDef huart1;

#define UART1_EN (1 << 4)
#define APB4ENR_HSE_EN (1 << 25)

#define GPIOA_9_UART 18
#define GPIOA_10_UART 20

void Practica2() {

	// Ejercicio 2.1
	RCC->APB2ENR |= UART1_EN;

	// Ejercicio 2.2
	RCC->APB4ENR |= APB4ENR_HSE_EN;

	GPIOA->MODER = 0xABFFFFFF;
	GPIOA->MODER &= ~((0x01 << GPIOA_9_UART) | (0x01 << GPIOA_10_UART));

	GPIOA->AFR[1] |= (0x00000000 | (0x07 << 4) | (0x07 << 8));

// Ejercicio 2.3
	USART1->CR1 |= (1 << 29) + (1 << 3) + (1 << 2) + (1 << 0);
	USART1->BRR = 556;

// Ejercicio 2.4

	char str[13] = "Hola Mundo\r\n";

	uint32_t timeStart = HAL_GetTick();
	while (HAL_GetTick() - timeStart < 1000) {
//	while (true) {

		for (uint8_t x = 0; x < 11; x++) {
			USART1->TDR = str[x];

			bool byte_sent = 0;

			do {
				byte_sent = ((USART1->ISR >> 7) & 0x01);
			} while (!byte_sent);
		}

	}

// Ejercicio 2.5

// Configuramos los pines para salida

	timeStart = HAL_GetTick();

	while (true) {
		char *number;
		bool byte_received = 0;

		do {
			byte_received = ((USART1->ISR >> 5) & 0x01);
		} while (!byte_received);

		*number = USART1->RDR;

		int number_int = atoi(number);

		change_led_state(number_int, 1);
	}

// Ejercicio 2.6
//	HAL_UART_Init(&huart1);

}

void configure_out_leds() {
	RCC->AHB4ENR |= GPIO_I_EN;

	GPIOI_MODER = GPIO_I_MODE_VAL;
}

void change_led_state(uint8_t pin, uint8_t mode) {
	uint8_t offset = 0;

	switch (pin) {
		case 1: {
			offset = 15;
		}
		break;

		case 2: {
			offset = 14;
		}
		break;

		case 3: {
			offset = 13;
		}
		break;

		case 4: {
			offset = 12;
		}
		break;

		default:
		break;
	}

	if (mode == 1)
		GPIOI->ODR |= (1 << offset);
	else
		GPIOI->ODR &= ~(1 << offset);

}

#endif /* SRC_PRACTICA2_PRACTICA2_H_ */
