/**
 ******************************************************************************
 * @file    aufgabe_03.c
 * @brief   Implementierung Aufgabe 03
 *
 *          Struktur:
 *            aufgabe_03_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 3
 *            aufgabe03Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE03_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE03_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe03TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe03Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 03 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 3.
 */
void aufgabe_03_init(void)
{
    xTaskCreate(aufgabe03Task,
                "aufgabe03Task",
                AUFGABE03_TASK_STACK,
                NULL,
                AUFGABE03_TASK_PRIO,
                &aufgabe03TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 03.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe03Task(void *argument)
{
    /* TODO: Aufgabe 03 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
