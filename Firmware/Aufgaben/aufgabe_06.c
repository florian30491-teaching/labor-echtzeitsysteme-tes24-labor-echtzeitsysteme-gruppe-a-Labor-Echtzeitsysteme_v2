/**
 ******************************************************************************
 * @file    aufgabe_06.c
 * @brief   Implementierung Aufgabe 06
 *
 *          Struktur:
 *            aufgabe_06_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 6
 *            aufgabe06Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE06_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE06_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe06TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe06Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 06 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 6.
 */
void aufgabe_06_init(void)
{
    xTaskCreate(aufgabe06Task,
                "aufgabe06Task",
                AUFGABE06_TASK_STACK,
                NULL,
                AUFGABE06_TASK_PRIO,
                &aufgabe06TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 06.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe06Task(void *argument)
{
    /* TODO: Aufgabe 06 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
