/*
 * Practica1.h
 *
 *  Created on: Feb 4, 2022
 *      Author: Javier Presmanes e Ivan Vivas Pastor
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

/**
 * Create mask for K pins value
 *
 * Joystick pins:
 * J-Center 	: PK2
 * J-Down 		: PK3
 * J-Left 		: PK4
 * J-Right 		: PK5
 * J-Up 		: PK6
 *
 * As GPIOI_MODER is divided into 15 groups of 2 bits and for "general output"
 * a 01 value must be set:
 *
 * 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 > Group
 * 01 01 01 01 xx xx xx xx xx xx xx xx xx xx xx xx > Binary
 * | 5 | | 5 | | X | | X | | X | | X | | X | | X | > HEX
 *
 * */

#define GPIO_I_MODE_VAL (0x55FFFFFF)

/**
 * Create mask for K pins value
 *
 * Joystick pins:
 * J-Center 	: PK2
 * J-Down 		: PK3
 * J-Left 		: PK4
 * J-Right 		: PK5
 * J-Up 		: PK6
 *
 * As GPIO_PUPDR is divided into 15 groups of 2 bits and for "pull-up"
 * a 01 value must be set:
 *
 * 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 > Group
 * xx xx xx xx xx xx xx xx 00 01 01 01 01 01 xx xx > Binary
 * | X | | X | | X | | 0 | | 1 | | 5 | | 5 | | X | > HEX
 *
 * */
#define GPIO_K_PUPDR_VAL (0x00001550)

/**
 * Create mask for K pins value
 *
 * Joystick pins:
 * J-Center 	: PK2
 * J-Down 		: PK3
 * J-Left 		: PK4
 * J-Right 		: PK5
 * J-Up 		: PK6
 *
 * As GPIOK_MODER is divided into 15 groups of 2 bits and for "input"
 * a 00 value must be set:
 *
 * 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 > Group
 * 11 11 11 11 11 11 11 11 11 00 00 00 00 00 11 11 > Binary
 * | F | | F | | F | | F | | C | | 0 | | 0 | | F | > HEX
 *
 * */
#define GPIO_K_MODE_VAL (0xFFFFC00F)

int test = 0;

void Practica1() {

	// Ejercicio 1.1
	RCC_AHB4ENR |= GPIO_I_EN;
	RCC_AHB4ENR |= GPIO_K_EN;

	// Ejercicio 1.2
	GPIOI_MODER = GPIO_I_MODE_VAL;

	// Ejercicio 1.3

	for (uint8_t x = 0; x < 4; x++) {
		GPIOI_ODR |= (1 << 15);
		GPIOI_ODR |= (1 << 14);
		GPIOI_ODR |= (1 << 13);
		GPIOI_ODR |= (1 << 12);

		HAL_Delay(250);

		GPIOI_ODR &= ~(1 << 15);
		GPIOI_ODR &= ~(1 << 14);
		GPIOI_ODR &= ~(1 << 13);
		GPIOI_ODR &= ~(1 << 12);

		HAL_Delay(250);
	}

	// Ejercicio 1.4
	GPIOK_PUPDR = GPIO_K_PUPDR_VAL;
	GPIOK_MODER = GPIO_K_MODE_VAL;

	// Ejercicio 1.5

	while (1) {
		GPIOI_ODR |= ((GPIOK_IDR >> 6) << 15);
		GPIOI_ODR |= ((GPIOK_IDR >> 5) << 14);
		GPIOI_ODR |= ((GPIOK_IDR >> 4) << 13);
		GPIOI_ODR |= ((GPIOK_IDR >> 3) << 12);

		HAL_Delay(1);

		// Reset vale
		GPIOI_ODR &= ~(1 << 15);
		GPIOI_ODR &= ~(1 << 14);
		GPIOI_ODR &= ~(1 << 13);
		GPIOI_ODR &= ~(1 << 12);
	}
}

#endif /* SRC_PRACTICA1_PRACTICA1_H_ */
