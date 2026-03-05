/**
 ******************************************************************************
 * @file    aufgabe_05.c
 * @brief   Implementierung Aufgabe 05
 *
 *          Struktur:
 *            aufgabe_05_init()  -> legt FreeRTOS-Tasks / Queues / ... an,
 *                                  wird aus aufgaben.c aufgerufen wenn
 *                                  AUFGABE_NR == 5
 *            aufgabe05Task()    -> eigentliche Task-Funktion
 ******************************************************************************
 */

#include "aufgaben.h"

/* Private Definitionen ------------------------------------------------------*/
#define AUFGABE05_TASK_STACK   256u              /* Stack in Words             */
#define AUFGABE05_TASK_PRIO    (tskIDLE_PRIORITY + 1)

/* Private Variablen ---------------------------------------------------------*/
static TaskHandle_t aufgabe05TaskHandle = NULL;

/* Private Funktionsprototypen -----------------------------------------------*/
static void aufgabe05Task(void *argument);

/* Implementierung -----------------------------------------------------------*/

/**
 * @brief  Legt alle FreeRTOS-Objekte der Aufgabe 05 an.
 *         Wird aus aufgaben.c aufgerufen wenn AUFGABE_NR == 5.
 */
void aufgabe_05_init(void)
{
    xTaskCreate(aufgabe05Task,
                "aufgabe05Task",
                AUFGABE05_TASK_STACK,
                NULL,
                AUFGABE05_TASK_PRIO,
                &aufgabe05TaskHandle);

    /* TODO: weitere FreeRTOS-Objekte hier anlegen, z.B.:
     *   xQueueCreate(...)
     *   xSemaphoreCreateBinary(...)
     *   xTimerCreate(...)
     */
}

/**
 * @brief  Haupt-Task der Aufgabe 05.
 * @param  argument  Nicht verwendet.
 */
static void aufgabe05Task(void *argument)
{
    /* TODO: Aufgabe 05 hier implementieren */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

