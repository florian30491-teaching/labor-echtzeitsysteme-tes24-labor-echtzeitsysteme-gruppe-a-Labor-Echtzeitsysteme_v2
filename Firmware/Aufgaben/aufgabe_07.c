/**
 ******************************************************************************
 * @file    aufgabe_07.c
 * @brief   Implementierung Aufgabe 07
 *
 *          Struktur:
 *            aufgabe_07_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 7
 *            aufgabe07Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE07_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE07_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe07TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe07Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 07 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 7.
 */
void aufgabe_07_init(void)
{
    xTaskCreate(aufgabe07Task,
                "aufgabe07Task",
                AUFGABE07_TASK_STACK,
                NULL,
                AUFGABE07_TASK_PRIO,
                &aufgabe07TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 07.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe07Task(void *argument)
{
    /* TODO: Aufgabe 07 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}