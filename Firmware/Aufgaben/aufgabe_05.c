/**
 ******************************************************************************
 * @file    aufgabe_05.c
 * @brief   Aufgabe 05 – Lauflicht mit variabler Periode ueber
 *          Taster-Interrupt und Semaphore
 *
 *          Basiert auf Aufgabe 02: Das Lauflicht wird um eine
 *          Taster-Steuerung erweitert. Ein externer Interrupt
 *          (User-Button) aendert die Lauflicht-Periode zyklisch.
 *          Die ISR-/Task-Kommunikation erfolgt ueber eine
 *          Binary Semaphore.
 *
 *          Hinweis: Nur ...FromISR()-Varianten der FreeRTOS-API in den
 *          EXTI-Callbacks verwenden!
 ******************************************************************************
 */

#include "aufgaben.h"

void aufgabe_05_init(void)
{

}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{

}
