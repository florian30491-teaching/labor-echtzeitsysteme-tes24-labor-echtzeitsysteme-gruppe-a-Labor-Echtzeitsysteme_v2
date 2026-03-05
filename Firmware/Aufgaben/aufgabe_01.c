/**
 ******************************************************************************
 * @file    aufgabe_01.c
 * @brief   Implementierung Aufgabe 01
 *
 *          Struktur:
 *            aufgabe_01_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 1
 *            aufgabe01Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE01_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE01_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe01TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe01Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 01 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 1.
 */
void aufgabe_01_init(void)
{
    xTaskCreate(aufgabe01Task,
                "aufgabe01Task",
                AUFGABE01_TASK_STACK,
                NULL,
                AUFGABE01_TASK_PRIO,
                &aufgabe01TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 01.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe01Task(void *argument)
{
    /* TODO: Aufgabe 01 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
