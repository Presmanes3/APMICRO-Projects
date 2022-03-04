/*
 * Practica3.h
 *
 *  Created on: 11 feb. 2022
 *      Author: Javier Presmanes
 */

#ifndef SRC_PRACTICA3_PRACTICA3_H_
#define SRC_PRACTICA3_PRACTICA3_H_

#include "main.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define DMA_1_AHB1ENR_POS 0
#define DMA_1_DIR_POS 6
#define DMA_1_PINC_POS 9
#define DMA_1_MINC_POS 10
#define DMA_1_PSIZE_POS 11
#define DMA_1_MSIZE_POS 13

enum DMA_Mode {
	PER_TO_MEM = 0,

	MEM_TO_PER = 1,

	MEM_TO_MEM = 2
};

enum DMA_Size {
	DMA_BYTE = 0,

	DMA_HALFWORD = 1,

	DMA_WORD = 2
};

enum DMA_INC_Mode {
	DMA_FIX = 0,

	DMA_INC = 1
};

void Practica3() {
	// Ejercicio 3.1
	int16_t datosA[] = { 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D' };
	int16_t datosB[16];

	RCC->AHB1ENR |= (1 << DMA_1_AHB1ENR_POS);    // Activate DMA 1 clk
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;
	DMA1_Stream7->CR = 0;
	DMA1->LISR = 0;
	DMA1->HISR = 0;

	uint32_t cr_value = ((DMA_Mode::MEM_TO_MEM << DMA_1_DIR_POS) | (DMA_INC_Mode::DMA_INC << DMA_1_PINC_POS) | (DMA_INC_Mode::DMA_INC << DMA_1_MINC_POS) | (DMA_Size::DMA_BYTE << DMA_1_PSIZE_POS)
	        | (DMA_Size::DMA_BYTE << DMA_1_MSIZE_POS));    //DIR,PINC,MINC,PSIZE,MSIZE
	DMA1_Stream7->CR = cr_value;
	DMA1_Stream7->M0AR = (uint32_t) &datosB[0];
	DMA1_Stream7->PAR = (uint32_t) &datosA[0];
	DMA1_Stream7->NDTR = sizeof(datosA);
	DMA1_Stream7->FCR = 0;
	DMA1_Stream7->CR |= (1 << 0);    //Inicio transferencia

	HAL_Delay(1000);

	uint8_t a = 8;

	// Ejercicio 3.2

	// Ejercicio 3.3
}

#endif /* SRC_PRACTICA3_PRACTICA3_H_ */
