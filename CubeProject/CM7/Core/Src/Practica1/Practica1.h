/*
 * Practica1.h
 *
 *  Created on: Feb 4, 2022
 *      Author: Javier Presmanes
 */

#ifndef SRC_PRACTICA1_PRACTICA1_H_
#define SRC_PRACTICA1_PRACTICA1_H_

#include "main.h"

// #define RCC_BASE (0x58024400UL)
#define RCC_AHB4ENR (*((volatile uint32_t*)(RCC_BASE+0xE0)))
#define RCC_C1_AHB4ENR_OFFSET (0x140UL)	// Offset for Core 1 RCC
#define RCC_C2_AHB4ENR_OFFSET (0x1A0UL)	// Offset for Core 1 RCC

#define GPIO_I_EN (1 << 8) //
#define GPIO_K_EN (1 << 10) //

// ========== GPIO ==========
#define GPIOI_BASE (0x58022000UL)
#define GPIOK_BASE (0x58022800UL)

#define GPIOI_MODER ( *((volatile uint32_t*)(GPIOI_BASE+0x00)))
#define GPIOI_ODR   ( *((volatile uint32_t*)(GPIOI_BASE+0x14)))
#define GPIOK_MODER ( *((volatile uint32_t*)(GPIOK_BASE+0x00)))
#define GPIOK_IDR   ( *((volatile uint32_t*)(GPIOK_BASE+0x10)))
#define GPIOK_PUPDR ( *((volatile uint32_t*)(GPIOK_BASE+0x0C)))

#define GPIO_MODE_OUTPUT (0b01)
#define GPIO_I_MODE_VAL (0x55FFFFFF) // 0101 0101

int test = 0;

void Practica1() {

	// Ejercicio 1.1
	RCC_AHB4ENR |= GPIO_I_EN;
	RCC_AHB4ENR |= GPIO_K_EN;

	// Ejercicio 1.2
	GPIOI_MODER = GPIO_I_MODE_VAL;

	// Ejercicio 1.3

	GPIOI_ODR |= (1 << 15);
	GPIOI_ODR |= (1 << 14);
	GPIOI_ODR |= (1 << 13);
	GPIOI_ODR |= (1 << 12);

	HAL_Delay(1000);

	GPIOI_ODR &= ~(1 << 15);
	GPIOI_ODR &= ~(1 << 14);
	GPIOI_ODR &= ~(1 << 13);
	GPIOI_ODR &= ~(1 << 12);

// Ejercicio 1.4

// Ejercicio 1.5
}

#endif /* SRC_PRACTICA1_PRACTICA1_H_ */
