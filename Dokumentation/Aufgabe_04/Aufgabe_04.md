﻿# Aufgabe 04 – Semaphoren in FreeRTOS: Arten, Konzepte und Anwendung

> 📝 **Bearbeitung:** [→ Bearbeitung_04.md](Bearbeitung_04.md)
>
> ⚠️ **Diese Datei ist die Aufgabenstellung und darf nicht verändert werden.**
> Alle Antworten, Notizen und Code-Snippets gehören ausschließlich in die Bearbeitungsdatei.

---

## Aufgabenbeschreibung

In dieser Aufgabe werden **Semaphoren** als zentrales Synchronisationsmittel in FreeRTOS erarbeitet.
Zunächst sollen die verschiedenen **Arten von Semaphoren** theoretisch verstanden werden, bevor sie in einer praktischen Aufgabe angewendet werden.

---

## Teil 1 – Theoretische Einführung: Semaphore-Arten in FreeRTOS

FreeRTOS stellt mehrere Typen von Semaphoren bereit, die sich in Anwendungszweck und Verhalten unterscheiden.
Erarbeitet euch die folgenden drei Typen und beschreibt jeweils Funktion, Erstellung und typische Einsatzszenarien.

### 1. Binary Semaphore

Eine **Binary Semaphore** kann nur die Werte **0** (nicht verfügbar) oder **1** (verfügbar) annehmen.
Sie eignet sich besonders zur **Signalisierung** zwischen Kontexten – z. B. von einer ISR an einen Task (»etwas ist passiert«).

| Operation | Task-Kontext | ISR-Kontext |
|-----------|-------------|-------------|
| Erstellen | `xSemaphoreCreateBinary()` | – |
| Anfordern | `xSemaphoreTake()` | `xSemaphoreTakeFromISR()` |
| Freigeben | `xSemaphoreGive()` | `xSemaphoreGiveFromISR()` |

> **Hinweis:** Nach `xSemaphoreCreateBinary()` ist die Semaphore initial **leer** (Wert 0).
> Um sie als Mutex-Ersatz (Ressourcenschutz) zu nutzen, muss einmalig `xSemaphoreGive()` aufgerufen werden.

**Typischer Einsatz:** ISR signalisiert einem Task, dass ein Ereignis eingetreten ist (z. B. Tastendruck).

### 2. Counting Semaphore

Eine **Counting Semaphore** besitzt einen **Zähler ≥ 0** statt eines einfachen Flags.
Jeder `Give`-Aufruf erhöht den Zähler, jeder `Take`-Aufruf verringert ihn (blockiert wenn Zähler = 0).

| Operation | Funktion |
|-----------|----------|
| Erstellen | `xSemaphoreCreateCounting(uxMaxCount, uxInitialCount)` |
| Anfordern | `xSemaphoreTake()` / `xSemaphoreTakeFromISR()` |
| Freigeben | `xSemaphoreGive()` / `xSemaphoreGiveFromISR()` |
| Zähler lesen | `uxSemaphoreGetCount()` |

**Typische Einsätze:**
- **Ereigniszählung:** Jeder Interrupt erhöht den Zähler; ein Task arbeitet die Ereignisse nacheinander ab.
- **Ressourcenpool:** Der Zähler repräsentiert die Anzahl verfügbarer Ressourcen (z. B. n freie Puffer).

### 3. Mutex (Mutual Exclusion Semaphore)

Ein **Mutex** ist ein spezialisierter Semaphore-Typ, der explizit für den **gegenseitigen Ausschluss** (engl. *mutual exclusion*) beim Zugriff auf gemeinsame Ressourcen konzipiert ist.

| Operation | Funktion |
|-----------|----------|
| Erstellen | `xSemaphoreCreateMutex()` |
| Anfordern (Sperren) | `xSemaphoreTake()` |
| Freigeben (Entsperren) | `xSemaphoreGive()` |

**Wichtige Unterschiede zum Binary Semaphore:**

| Eigenschaft | Binary Semaphore | Mutex |
|-------------|-----------------|-------|
| **Besitzer-Konzept** | Nein – jeder Kontext kann Give/Take aufrufen | Ja – nur der besitzende Task darf freigeben |
| **Prioritätsvererbung** | Nein | Ja – verhindert *Priority Inversion* |
| **ISR-Nutzung** | Ja (`…FromISR`-Varianten) | **Nein** – darf nicht aus einer ISR verwendet werden |
| **Typischer Einsatz** | Signalisierung (ISR → Task) | Ressourcenschutz (Task ↔ Task) |

> ⚠️ **Priority Inversion:** Wenn ein niedrig-priorisierter Task eine Ressource hält und ein hoch-priorisierter Task darauf wartet, kann ein mittel-priorisierter Task beide blockieren.
> Mutexes lösen dieses Problem durch **Prioritätsvererbung**: Der niedrig-priorisierte Task erbt temporär die höhere Priorität.

### Vergleichstabelle

| Kriterium | Binary Semaphore | Counting Semaphore | Mutex |
|-----------|-----------------|-------------------|-------|
| Wertebereich | 0 oder 1 | 0 bis Max | 0 oder 1 |
| Prioritätsvererbung | ❌ | ❌ | ✅ |
| ISR-tauglich | ✅ | ✅ | ❌ |
| Besitzer-Konzept | ❌ | ❌ | ✅ |
| Hauptzweck | Signalisierung | Zählung / Pool | Ressourcenschutz |

### Aufgabe (Theorie)

Beantwortet in der [Bearbeitung](Bearbeitung_04.md) folgende Fragen:

1. Beschreibt in eigenen Worten den Unterschied zwischen einer **Binary Semaphore** und einem **Mutex**. Warum sollte man für den Ressourcenschutz zwischen Tasks einen Mutex bevorzugen?
2. Nennt ein konkretes Beispiel, in dem eine **Counting Semaphore** sinnvoller ist als eine Binary Semaphore.
3. Warum darf ein **Mutex** nicht aus einer ISR heraus verwendet werden?
4. Was versteht man unter **Priority Inversion** und wie löst FreeRTOS dieses Problem?

---

## Teil 2 – Praktische Anwendung: Lauflicht mit Zustandsmaschine und Semaphore

In dieser Aufgabe wird ein **Lauflicht** über mindestens 3 GPIO-Pins (z. B. DBG1, DBG2, DBG3) implementiert.
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
> Blockierende Aufrufe (z. B. `xSemaphoreTake()` mit `portMAX_DELAY`) in einer ISR führen zu undefiniertem Verhalten und sind **verboten**.

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

1. Theoretische Erarbeitung der verschiedenen **Semaphore-Typen** in FreeRTOS (Binary Semaphore, Counting Semaphore, Mutex) sowie deren Unterschiede und Einsatzgebiete
2. Theoretische Betrachtung der Zustandsmaschine, des Interrupt-Mechanismus (`__weak` Callback-Konzept) und des **Semaphore-Musters** zum Schutz gemeinsamer Ressourcen
3. Praktische Implementierung des Lauflichts, der Zustandsmaschine, des Taster-Callbacks **sowie des Semaphore-Schutzes** auf dem STM32 Nucleo-H563ZI
4. Messtechnische Untersuchung über die GPIO-Pins sowie den **Trace-Debugger (WinIDEA / iC5700 BlueBox)**

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

### Binary Semaphore

| Thema | Link |
|-------|------|
| Binary Semaphore – Konzept & API | [freertos.org – xSemaphoreCreateBinary](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/01-xSemaphoreCreateBinary) |
| `xSemaphoreGive()` – Semaphore freigeben (Task-Kontext) | [freertos.org – xSemaphoreGive](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/15-xSemaphoreGive) |
| `xSemaphoreTake()` – Semaphore anfordern (Task-Kontext) | [freertos.org – xSemaphoreTake](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/12-xSemaphoreTake) |
| `xSemaphoreGiveFromISR()` – Semaphore freigeben (ISR-Kontext) | [freertos.org – xSemaphoreGiveFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/17-xSemaphoreGiveFromISR) |
| `xSemaphoreTakeFromISR()` – Semaphore anfordern (ISR-Kontext) | [freertos.org – xSemaphoreTakeFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/13-xSemaphoreTakeFromISR) |

### Counting Semaphore

| Thema | Link |
|-------|------|
| Counting Semaphore – Konzept & API | [freertos.org – xSemaphoreCreateCounting](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/03-xSemaphoreCreateCounting) |
| `uxSemaphoreGetCount()` – Zählerstand abfragen | [freertos.org – uxSemaphoreGetCount](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/18-uxSemaphoreGetCount) |

### Mutex

| Thema | Link |
|-------|------|
| Mutex – Konzept & API | [freertos.org – xSemaphoreCreateMutex](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/04-xSemaphoreCreateMutex) |
| Mutex vs. Binary Semaphore | [freertos.org – Mutexes](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/04-Mutexes) |

### Allgemein

| Thema | Link |
|-------|------|
| `vTaskNotifyGiveFromISR()` – Task aus ISR aufwecken | [freertos.org – vTaskNotifyGiveFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/05-Direct-to-task-notifications/02-vTaskNotifyGiveFromISR) |
| Interrupt-safe API (…FromISR) – Übersicht | [freertos.org – Deferred Interrupt Handling](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/11-Deferred-interrupt-handling) |
| Kritische Abschnitte & `taskENTER_CRITICAL()` | [freertos.org – taskENTER_CRITICAL](https://www.freertos.org/Documentation/02-Kernel/04-API-references/04-RTOS-kernel-control/01-taskENTER_CRITICAL_taskEXIT_CRITICAL) |
