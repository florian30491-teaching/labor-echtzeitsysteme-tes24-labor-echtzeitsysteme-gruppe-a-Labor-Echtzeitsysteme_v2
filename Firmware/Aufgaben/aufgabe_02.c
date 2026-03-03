/**
 ******************************************************************************
 * @file    aufgabe_02.c
 * @brief   Implementierung Aufgabe 02
 *
 *          Struktur:
 *            aufgabe_02_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 2
 *            aufgabe02Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE02_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE02_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe02TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe02Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 02 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 2.
 */
void aufgabe_02_init(void)
{
    xTaskCreate(aufgabe02Task,
                "aufgabe02Task",
                AUFGABE02_TASK_STACK,
                NULL,
                AUFGABE02_TASK_PRIO,
                &aufgabe02TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 02.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe02Task(void *argument)
{
    /* TODO: Aufgabe 02 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
