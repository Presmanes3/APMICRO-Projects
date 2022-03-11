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
#include "../Practica2/Practica2.h"

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

void ejercicio_3_1();
void ejercicio_3_2();
void ejercicio_3_3();

void Practica3() {
	// Ejercicio 3.1
//	ejercicio_3_1();

// Ejercicio 3.2
//	ejercicio_3_2();

// Ejercicio 3.3
	ejercicio_3_3();
}

void ejercicio_3_1() {
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

}

void ejercicio_3_2() {
	int8_t datosB[16];

// ================= CONFIGURE CLOCKS ================= //
// Configure clocks for UART
	RCC->APB2ENR |= UART1_EN;
	// Configure clocks for DMA
	RCC->AHB1ENR |= (1 << DMA_1_AHB1ENR_POS);
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;

	// ================= CONFIGURE UART ================= //
	GPIOA->MODER = 0xABFFFFFF;
	GPIOA->MODER &= ~((0x01 << GPIOA_9_UART) | (0x01 << GPIOA_10_UART));

	// Configure uart pins mode
	GPIOA->AFR[1] |= (0x00000000 | (0x07 << 4) | (0x07 << 8));

	// Configure BAUDRATE to 115200
	USART1->CR1 |= (1 << 29) + (1 << 3) + (1 << 2) + (1 << 0);
	USART1->BRR = 556;

	USART1->CR3 |= (1 << 6);    // DMAR = 1 in bit 6

	// Check if UART is working
//	char str[14] = "Hola Mundo\r\n";
//
//	uint32_t timeStart = HAL_GetTick();
//	while (HAL_GetTick() - timeStart < 1000) {
//		enviar_cadena(str, 12);
//	}

	// ================= CONFIGURE DMA ================= //
	DMA1_Stream7->CR = 0;
	DMA1->LISR = 0;
	DMA1->HISR = 0;
	DMAMUX1_Channel7->CCR = 41;    // 41 is for UART1 RX for DMA
	DMA1_Stream7->CR |= ((DMA_Mode::PER_TO_MEM << DMA_1_DIR_POS) | (DMA_INC_Mode::DMA_FIX << DMA_1_PINC_POS) | (DMA_INC_Mode::DMA_INC << DMA_1_MINC_POS) | (DMA_Size::DMA_BYTE << DMA_1_PSIZE_POS)
	        | (DMA_Size::DMA_BYTE << DMA_1_MSIZE_POS));    //DIR,PINC,MINC,PSIZE,MSIZE
	DMA1_Stream7->M0AR = (uint32_t) &datosB[0];
	DMA1_Stream7->PAR = (uint32_t) &(USART1->RDR);
	DMA1_Stream7->NDTR = sizeof(datosB);
	DMA1_Stream7->FCR = 0;
	DMA1_Stream7->CR |= (1 << 0);

	//HAL_Delay(10000);
}

void ejercicio_3_3() {
	int8_t datosB[16] = { 0 };

// ================= CONFIGURE CLOCKS ================= //
// Configure clocks for UART
	RCC->APB2ENR |= UART1_EN;
	// Configure clocks for DMA
	RCC->AHB1ENR |= (1 << DMA_1_AHB1ENR_POS);
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;

	// ================= CONFIGURE UART ================= //
	GPIOA->MODER = 0xABFFFFFF;
	GPIOA->MODER &= ~((0x01 << GPIOA_9_UART) | (0x01 << GPIOA_10_UART));

	// Configure uart pins mode
	GPIOA->AFR[1] |= (0x00000000 | (0x07 << 4) | (0x07 << 8));

	// Configure BAUDRATE to 115200
	USART1->CR1 |= (1 << 29) + (1 << 3) + (1 << 2) + (1 << 0);
	USART1->BRR = 556;

	USART1->CR3 |= (1 << 6);    // DMAR = 1 in bit 6

	// Check if UART is working
//	char str[14] = "Hola Mundo\r\n";
//
//	uint32_t timeStart = HAL_GetTick();
//	while (HAL_GetTick() - timeStart < 1000) {
//		enviar_cadena(str, 12);
//	}

	// ================= CONFIGURE DMA ================= //
	DMA1_Stream7->CR = 0;
	DMA1->LISR = 0;
	DMA1->HISR = 0;
	DMAMUX1_Channel7->CCR = 41;    // 41 is for UART1 RX for DMA
	DMA1_Stream7->CR |= ((DMA_Mode::PER_TO_MEM << DMA_1_DIR_POS) | (DMA_INC_Mode::DMA_FIX << DMA_1_PINC_POS) | (DMA_INC_Mode::DMA_INC << DMA_1_MINC_POS) | (DMA_Size::DMA_BYTE << DMA_1_PSIZE_POS)
	        | (DMA_Size::DMA_BYTE << DMA_1_MSIZE_POS));    //DIR,PINC,MINC,PSIZE,MSIZE
	DMA1_Stream7->M0AR = (uint32_t) &datosB[0];
	DMA1_Stream7->PAR = (uint32_t) &(USART1->RDR);
	DMA1_Stream7->NDTR = sizeof(datosB);
	DMA1_Stream7->FCR = 0;
	DMA1_Stream7->CR |= (1 << 0);

	datosB[0] = 'A';
	datosB[1] = 0;
	HAL_Delay(10000);
	enviar_cadena((char*) datosB, 16);
	SCB_InvalidateDCache_by_Addr(datosB, 32);
	HAL_Delay(10);
	enviar_cadena((char*) datosB, 16);
}

#endif /* SRC_PRACTICA3_PRACTICA3_H_ */
