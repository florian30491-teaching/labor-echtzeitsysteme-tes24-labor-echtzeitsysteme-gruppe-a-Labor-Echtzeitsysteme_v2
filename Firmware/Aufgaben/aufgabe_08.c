/**
 ******************************************************************************
 * @file    aufgabe_08.c
 * @brief   Implementierung Aufgabe 08
 *
 *          Struktur:
 *            aufgabe_08_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 8
 *            aufgabe08Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE08_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE08_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe08TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe08Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 08 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 8.
 */
void aufgabe_08_init(void)
{
    xTaskCreate(aufgabe08Task,
                "aufgabe08Task",
                AUFGABE08_TASK_STACK,
                NULL,
                AUFGABE08_TASK_PRIO,
                &aufgabe08TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 08.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe08Task(void *argument)
{
    /* TODO: Aufgabe 08 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}