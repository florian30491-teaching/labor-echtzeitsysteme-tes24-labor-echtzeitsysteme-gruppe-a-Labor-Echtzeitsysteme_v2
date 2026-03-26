﻿# Aufgabe 03 – Rate Monotonic Scheduling mit drei Tasks

| | |
|---|---|
| **Gruppe** | |
| **Datum** | |
| **Branch** | `feature/aufgabe03` |

---

## Aufgabenbeschreibung

Legen Sie drei Tasks an, jeder Task steuert einen Pin mit unterschiedlicher Taskperiode und Tasklänge gemäß folgender Tabelle:

| Taskname | Tasklänge | Taskperiode |
|----------|-----------|-------------|
| Task 1   | 2 ms      | 3 ms        |
| Task 2   | 27 ms     | 40 ms       |
| Task 3   | 67 ms     | 100 ms      |

- Beim **Eintritt** in den Task wird der zugehörige Pin **gesetzt (High)**.
- Beim **Verlassen** des Tasks wird der Pin **zurückgesetzt (Low)**.
- Die Tasklänge wird durch eine **Warteschleife** simuliert – die Anzahl der Schleifendurchläufe ist experimentell zu ermitteln.

> 📌 **GPIO-Pins für die Tasks:** Verwenden Sie die dedizierten Debug-Pins des Boards (Connector **CN10**, rechts unten).  
> Jedem Task wird ein eigener Pin zugeordnet, sodass im Trace-Debugger die vollständige Abarbeitungsdauer jedes Tasks direkt als High-Pegel sichtbar ist:
>
> | Task | empfohlener Pin | MCU-Pin | CN10-Pin |
> |------|-----------------|---------|----------|
> | Task 1 | DBG1 | PA3 | Pin 34 (D35) |
> | Task 2 | DBG2 | PE15 | Pin 30 (D37) |
> | Task 3 | DBG3 | PE12 | Pin 26 (D39) |
>
>
> ![Nucleo Board Pinout – CN10 Connector](../Stm32/pinout_nucleo_board.png)
>
> Genaue Pin-Belegung: Board User Manual [**UM3115**](../Stm32/um3115-stm32h5-nucleo144-board-mb1404-stmicroelectronics.pdf), Seite 32.

### Prioritätsvergabe

Die Task-Prioritäten sind nach dem Prinzip des **Rate Monotonic Schedulings (RMS)** zu vergeben:  
kürzeste Periode → höchste Priorität.

### Messtechnische Untersuchung

Untersuchen Sie das Scheduling-Verhalten über den **Trace-Debugger (iC5700 BlueBox + WinIDEA)**.  
Alternativ können die GPIO-Pins am Oszilloskop oder Logikanalysator ausgewertet werden.

Betrachten Sie die Aufgabenstellung zunächst **theoretisch** und anschließend **praktisch**.

---

## Bearbeitung

### Theoretisch

<!-- Vorüberlegungen, RMS-Analyse, Timing-Diagramm -->
<!-- Bilder direkt in diesem Ordner ablegen: ![Beschreibung](dateiname.png) -->

### Praktisch

<!-- Umsetzung, Messergebnisse, Code-Snippets -->

```c
// Code-Snippet hier einfügen
```