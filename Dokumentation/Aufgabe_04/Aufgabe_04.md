# Aufgabe 04 – Lauflicht mit Taster-Interrupt, Zustandsmaschine und Semaphore

| | |
|---|---|
| **Gruppe** | |
| **Datum** | |
| **Branch** | `feature/aufgabe04` |

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

---

## Bearbeitung

### Theoretisch

<!-- Vorüberlegungen, Konzepte, Zustandsdiagramm -->

### Praktisch

<!-- Umsetzung, Messergebnisse, Code-Snippets -->

```c
// Code-Snippet hier einfügen
```

---

## 📚 FreeRTOS-Referenzen

| Thema | Link |
|-------|------|
| Binary Semaphore – Konzept & API | [freertos.org/xSemaphoreCreateBinary](https://www.freertos.org/xSemaphoreCreateBinary.html) |
| `xSemaphoreGive()` – Semaphore freigeben (Task-Kontext) | [freertos.org/xSemaphoreGive](https://www.freertos.org/xSemaphoreGive.html) |
| `xSemaphoreTake()` – Semaphore anfordern (Task-Kontext) | [freertos.org/xSemaphoreTake](https://www.freertos.org/xSemaphoreTake.html) |
| `xSemaphoreGiveFromISR()` – Semaphore freigeben (ISR-Kontext) | [freertos.org/xSemaphoreGiveFromISR](https://www.freertos.org/xSemaphoreGiveFromISR.html) |
| `xSemaphoreTakeFromISR()` – Semaphore anfordern (ISR-Kontext) | [freertos.org/xSemaphoreTakeFromISR](https://www.freertos.org/xSemaphoreTakeFromISR.html) |
| `portYIELD_FROM_ISR()` – Context Switch nach ISR anstoßen | [freertos.org/portYIELD_FROM_ISR](https://www.freertos.org/portYIELD_FROM_ISR.html) |
| Interrupt-safe API (…FromISR-Funktionen) – Übersicht | [freertos.org/a00270](https://www.freertos.org/a00270.html) |
| Kritische Abschnitte & `taskENTER_CRITICAL()` | [freertos.org/taskENTER_CRITICAL_taskEXIT_CRITICAL](https://www.freertos.org/taskENTER_CRITICAL_taskEXIT_CRITICAL.html) |


