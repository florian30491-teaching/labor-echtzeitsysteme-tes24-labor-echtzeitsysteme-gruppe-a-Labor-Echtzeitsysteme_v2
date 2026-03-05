/**
 ******************************************************************************
 * @file    aufgabe_04.c
 * @brief   Implementierung Aufgabe 04
 *
 *          Struktur:
 *            aufgabe_04_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 4
 *            aufgabe04Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE04_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE04_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe04TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe04Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 04 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 4.
 */
void aufgabe_04_init(void)
{
    xTaskCreate(aufgabe04Task,
                "aufgabe04Task",
                AUFGABE04_TASK_STACK,
                NULL,
                AUFGABE04_TASK_PRIO,
                &aufgabe04TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 04.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe04Task(void *argument)
{
    /* TODO: Aufgabe 04 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
