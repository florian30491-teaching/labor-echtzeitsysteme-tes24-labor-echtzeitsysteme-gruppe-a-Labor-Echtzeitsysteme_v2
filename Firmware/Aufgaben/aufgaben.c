/**
 ******************************************************************************
 * @file    aufgaben.c
 * @brief   Zentraler Dispatch fuer die aktive Aufgabe.
 *
 *          aufgabe_freertos_init() wird aus MX_FREERTOS_Init() aufgerufen
 *          und leitet zur Compile-Zeit per #if AUFGABE_NR an die jeweils
 *          aktive Aufgabe weiter.
 *
 *          Aufgabe wechseln:
 *            cmake -DAUFGABE=3 ..
 *          oder in CLion:
 *            Settings -> Build, Execution, Deployment -> CMake -> CMake options
 *            Eintrag: -DAUFGABE=3
 ******************************************************************************
 */

#include "aufgaben.h"

/**
 * @brief  Zentraler Einstiegspunkt – ruft die Init-Funktion der aktiven
 *         Aufgabe auf.  Wird aus MX_FREERTOS_Init() aufgerufen, bevor
 *         osKernelStart() den Scheduler startet.
 */
void aufgabe_freertos_init(void)
{
#if   AUFGABE_NR == 0
    aufgabe_00_init();
#elif AUFGABE_NR == 1
    aufgabe_01_init();
#elif AUFGABE_NR == 2
    aufgabe_02_init();
#elif AUFGABE_NR == 3
    aufgabe_03_init();
#elif AUFGABE_NR == 4
    aufgabe_04_init();
#elif AUFGABE_NR == 5
    aufgabe_05_init();
#elif AUFGABE_NR == 6
    aufgabe_06_init();
#elif AUFGABE_NR == 7
    aufgabe_07_init();
#elif AUFGABE_NR == 8
    aufgabe_08_init();
#else
#error "Ungueltige Aufgabe: AUFGABE_NR muss zwischen 0 und 8 liegen."
#endif
}

