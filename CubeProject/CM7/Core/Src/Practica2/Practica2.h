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
#include "string.h"
#include "stdio.h"

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

struct custom_char {
		char *buff;
		bool valid;
};

void recibir_cadena(custom_char *chr);
void enviar_cadena(char *str, int size);

custom_char received;

void Practica2() {

	// Ejercicio 2.1
	RCC->APB2ENR |= UART1_EN;

	// Ejercicio 2.2
	// Activate HSE clock
	// Activate GPIO I
	RCC->APB4ENR |= APB4ENR_HSE_EN | GPIO_I_EN;
//	RCC->APB4ENR |= GPIO_I_EN;
	GPIOI->MODER = GPIO_I_MODE_VAL;

	GPIOA->MODER = 0xABFFFFFF;
	GPIOA->MODER &= ~((0x01 << GPIOA_9_UART) | (0x01 << GPIOA_10_UART));

	GPIOA->AFR[1] |= (0x00000000 | (0x07 << 4) | (0x07 << 8));

// Ejercicio 2.3
	USART1->CR1 |= (1 << 29) + (1 << 3) + (1 << 2) + (1 << 0);
	USART1->BRR = 556;

// Ejercicio 2.4

	char str[14] = "Hola Mundo\r\n";

	uint32_t timeStart = HAL_GetTick();
//	while (HAL_GetTick() - timeStart < 1000) {
//		enviar_cadena(str, 12);
//	}

// Ejercicio 2.5

// Configuramos los pines para salida

	timeStart = HAL_GetTick();

	// Turn off all leds
	change_led_state(1, 0);
	change_led_state(2, 0);
	change_led_state(3, 0);
	change_led_state(4, 0);

//	while (true) {
//		recibir_cadena();
//	}

// Ejercicio 2.6

	char buff[128];

	// Pedimos un numero
	char *msg = "Porfavor, escriba un numero de menos de 3 cifras: ";
	enviar_cadena(msg, strlen(msg));

	// Esperamos a recibir el numero
	do {
		recibir_cadena(&received);
	} while (!received.valid);

	int first_number = atoi(received.buff);

	// Escribimos el numero por pantalla
	memset(buff, '\0', strlen(buff));
	snprintf(buff, 128, "El primer numero es : %d\n ", first_number);

	enviar_cadena(buff, strlen(buff));

	// Pedimos otro numero
	msg = "Porfavor, escriba un numero de menos de 3 cifras: ";
	enviar_cadena(msg, strlen(msg));

	// Esperamos a recibir el segundo numero
	do {
		recibir_cadena(&received);
	} while (!received.valid);

	int second_number = atoi(received.buff);

	// Escribimos el numero por pantalla
	memset(buff, '\0', strlen(buff));
	snprintf(buff, 128, "El segundo numero es : %d\n ", second_number);

	enviar_cadena(buff, strlen(buff));

	int total = first_number + second_number;

	// Escribimos el numero total por pantalla
	memset(buff, '\0', strlen(buff));
	snprintf(buff, 128, "La suma es : %d\n ", total);

	enviar_cadena(buff, strlen(buff));

//	HAL_UART_Init(&huart1);

}

void configure_out_leds() {

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

	if (mode == 0)
		GPIOI->ODR |= (1 << offset);
	else
		GPIOI->ODR &= ~(1 << offset);
}

void recibir_cadena(custom_char *chr) {

	chr->valid = false;
	memset(chr->buff, '\0', strlen(chr->buff));

	uint8_t x = 0;

	uint32_t start_time = HAL_GetTick();
	while ((HAL_GetTick() - start_time < 10) && !chr->valid) {
		while (((USART1->ISR >> 5) & 0x01)) {
			chr->buff[x] = USART1->RDR;

			if (chr->buff[x] == 0x0A) {
				if (chr->buff[x - 1] == 0x0D)
					chr->buff[x - 1] = '\0';
				chr->buff[x] = '\0';
				chr->valid = true;
				break;
			}
			x++;
		}

	}
}

void enviar_cadena(char *str, int size) {

	if (str[size - 1] != '\0')
		str[size - 1] = (char) '\0';

	for (uint8_t x = 0; x < size; x++) {
		USART1->TDR = str[x];

		bool byte_sent = 0;

		do {
			byte_sent = ((USART1->ISR >> 7) & 0x01);
		} while (!byte_sent);
	}

}

#endif /* SRC_PRACTICA2_PRACTICA2_H_ */
