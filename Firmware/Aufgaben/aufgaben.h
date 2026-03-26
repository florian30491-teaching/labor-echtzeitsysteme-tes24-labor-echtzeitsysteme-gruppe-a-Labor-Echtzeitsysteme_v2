/**
 ******************************************************************************
 * @file    aufgaben.h
 * @brief   Gemeinsamer Header fuer alle Aufgaben-Implementierungen.
 *
 *          Einstiegspunkt fuer app_freertos.c ist immer:
 *            aufgabe_freertos_init()
 *
 *          Diese Funktion ist in aufgaben.c implementiert und ruft zur
 *          Compile-Zeit per #if AUFGABE_NR die passende aufgabe_XX_init()
 *          auf.
 *
 *          Welche Aufgabe aktiv ist, steuert der CMake-Parameter
 *          -DAUFGABE=X (1-8).  Das Praeprozessor-Symbol AUFGABE_NR traegt
 *          den gewaehlten Wert.
 ******************************************************************************
 */

#ifndef AUFGABEN_H
#define AUFGABEN_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* ---------------------------------------------------------------------------
 * Zentraler Einstiegspunkt – wird aus MX_FREERTOS_Init() aufgerufen.
 * Leitet intern per #if AUFGABE_NR an die aktive Aufgabe weiter.
 * ---------------------------------------------------------------------------*/
void aufgabe_freertos_init(void);

/* ---------------------------------------------------------------------------
 * Aufgaben-spezifische Init-Funktionen (je eine pro aufgabe_XX.c)
 * ---------------------------------------------------------------------------*/
void aufgabe_00_init(void);
void aufgabe_01_init(void);
void aufgabe_02_init(void);
void aufgabe_03_init(void);
void aufgabe_04_init(void);
void aufgabe_05_init(void);
void aufgabe_06_init(void);
void aufgabe_07_init(void);
void aufgabe_08_init(void);

#endif /* AUFGABEN_H */

