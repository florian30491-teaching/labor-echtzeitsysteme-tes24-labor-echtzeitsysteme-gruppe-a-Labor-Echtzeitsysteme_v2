﻿# Aufgabe 04 – Lauflicht mit Taster-Interrupt, Zustandsmaschine und Semaphore

> 📝 **Bearbeitung:** [→ Bearbeitung_04.md](Bearbeitung_04.md)
>
> ⚠️ **Diese Datei ist die Aufgabenstellung und darf nicht verändert werden.**
> Alle Antworten, Notizen und Code-Snippets gehören ausschließlich in die Bearbeitungsdatei.

---

## Aufgabenbeschreibung

In dieser Aufgabe wird ein **Lauflicht** über mindestens 3 GPIO-Pins (z.B. DBG1, DBG2, DBG3) implementiert.  
Ein einzelner FreeRTOS-Task steuert das Lauflicht mit einer **variablen Taskperiode** und einem **externen Taster** zur Zustandssteuerung.  
Der gemeinsame Zugriff auf die Zustandsvariable zwischen ISR-Kontext (Callback) und Task-Kontext wird durch eine **Binary Semaphore** geschützt.

### Task-Anforderungen

- Der Task hat eine **höhere Priorität als der IDLE-Task**
- Beim **Eintritt** in den Task wird immer ein dedizierter Pin **getoggelt** (zur messtechnischen Auswertung)
- Die Taskperiode ist variabel und wechselt zwischen folgenden Werten:

| Zustand | Taskperiode |
|---------|-------------|
| Lauflicht aus | Task wird **nicht ausgeführt** |
| Periode 1 | 250 ms |
| Periode 2 | 500 ms |
| Periode 3 | 750 ms |

### Zustandsmaschine

Das Verhalten des Tasks wird über eine **Zustandsvariable** gesteuert:

```
Zustand 0 (initial): Lauflicht AUS  → Task wird nicht ausgeführt
Zustand 1:           Lauflicht EIN  → Periode 250 ms
Zustand 2:           Lauflicht EIN  → Periode 500 ms
Zustand 3:           Lauflicht EIN  → Periode 750 ms
                             │
               Taster betätigt → nächster Zustand (zyklisch)
```

> Wenn das Lauflicht **aus** ist (Zustand 0), darf der Task **nicht ausgeführt** werden.  
> Erst nach Betätigung des Tasters wechselt der Zustand und der Task wird wieder aktiviert.

### Ressourcenschutz mit Binary Semaphore

Die Zustandsvariable wird sowohl vom Interrupt-Callback (ISR-Kontext) als auch vom Task (Task-Kontext) gelesen und geschrieben.  
Da beide Ausführungskontexte **gleichzeitig** auf dieselbe Variable zugreifen können, entsteht ein **kritischer Abschnitt** (engl. *race condition*), der explizit geschützt werden muss.

In dieser Aufgabe wird dazu eine **Binary Semaphore** verwendet:

- Die Semaphore wird beim Start mit `xSemaphoreCreateBinary()` erstellt und initial mit `xSemaphoreGive()` freigegeben (Mutex-Muster)
- Der **Task** fordert die Semaphore vor dem Lesen/Schreiben der Zustandsvariablen mit `xSemaphoreTake()` an und gibt sie danach mit `xSemaphoreGive()` wieder frei
- Der **ISR-Callback** (`HAL_GPIO_EXTI_Falling_Callback`) verwendet ausschließlich die ISR-sichere Variante `xSemaphoreTakeFromISR()` / `xSemaphoreGiveFromISR()`, da aus einem Interrupt-Kontext heraus **keine blockierenden FreeRTOS-Funktionen** aufgerufen werden dürfen

> ⚠️ **Wichtig:** Aus einem ISR-Kontext dürfen ausschließlich die `...FromISR()`-Varianten der FreeRTOS-API verwendet werden.  
> Blockierende Aufrufe (z.B. `xSemaphoreTake()` mit `portMAX_DELAY`) in einer ISR führen zu undefiniertem Verhalten und sind **verboten**.

### Taster-Interrupt & Callback

Die Projektvorlage ist so konfiguriert, dass ein **externer Interrupt** ausgelöst wird, sobald der Taster betätigt wird.  
Beim STM32H5 unterscheidet der HAL zwischen steigender und fallender Flanke – es existieren zwei separate `__weak`-Callbacks:

```c
/* Fallende Flanke – wird beim Drücken des Tasters aufgerufen */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) { /* ... */ }

/* Steigende Flanke – wird beim Loslassen des Tasters aufgerufen */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)  { /* ... */ }
```

> Beide Callbacks sind in `aufgabe_04.c` als Gerüst vorbereitet.  
> Entscheidet selbst welcher Callback für diese Aufgabe geeignet ist und begründet eure Wahl.

### Ziel der Aufgabe

1. Theoretische Betrachtung der Zustandsmaschine, des Interrupt-Mechanismus (`__weak` Callback-Konzept) und des **Semaphore-Musters** zum Schutz gemeinsamer Ressourcen
2. Praktische Implementierung des Lauflichts, der Zustandsmaschine, des Taster-Callbacks **sowie des Semaphore-Schutzes** auf dem STM32 Nucleo-H563ZI
3. Messtechnische Untersuchung über die GPIO-Pins sowie den **Trace-Debugger (WinIDEA / iC5700 BlueBox)**

> 📌 **GPIO-Pins für Lauflicht und Messung:** Verwenden Sie die dedizierten Debug-Pins des Boards (Connector **CN10**, rechts unten):
> | Pin | MCU-Pin | CN10-Pin |
> |-----|---------|----------|
> | DBG1 | PA3 | Pin 34 (D35) |
> | DBG2 | PE15 | Pin 30 (D37) |
> | DBG3 | PE12 | Pin 26 (D39) |
> | DBG4 | PE10 | Pin 24 (D40) |
>
> ![Nucleo Board Pinout – CN10 Connector](../Stm32/pinout_nucleo_board.png)
>
> Genaue Pin-Belegung: Board User Manual [**UM3115**](../Stm32/um3115-stm32h5-nucleo144-board-mb1404-stmicroelectronics.pdf), Seite 32.

---

## 📚 FreeRTOS-Referenzen

| Thema | Link |
|-------|------|
| Binary Semaphore – Konzept & API | [freertos.org – xSemaphoreCreateBinary](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/01-xSemaphoreCreateBinary) |
| `xSemaphoreGive()` – Semaphore freigeben (Task-Kontext) | [freertos.org – xSemaphoreGive](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/15-xSemaphoreGive) |
| `xSemaphoreTake()` – Semaphore anfordern (Task-Kontext) | [freertos.org – xSemaphoreTake](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/12-xSemaphoreTake) |
| `xSemaphoreGiveFromISR()` – Semaphore freigeben (ISR-Kontext) | [freertos.org – xSemaphoreGiveFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/17-xSemaphoreGiveFromISR) |
| `xSemaphoreTakeFromISR()` – Semaphore anfordern (ISR-Kontext) | [freertos.org – xSemaphoreTakeFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/13-xSemaphoreTakeFromISR) |
| `vTaskNotifyGiveFromISR()` – Task aus ISR aufwecken | [freertos.org – vTaskNotifyGiveFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/05-Direct-to-task-notifications/02-vTaskNotifyGiveFromISR) |
| Interrupt-safe API (…FromISR) – Übersicht | [freertos.org – Deferred Interrupt Handling](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/11-Deferred-interrupt-handling) |
| Kritische Abschnitte & `taskENTER_CRITICAL()` | [freertos.org – taskENTER_CRITICAL](https://www.freertos.org/Documentation/02-Kernel/04-API-references/04-RTOS-kernel-control/01-taskENTER_CRITICAL_taskEXIT_CRITICAL) |
