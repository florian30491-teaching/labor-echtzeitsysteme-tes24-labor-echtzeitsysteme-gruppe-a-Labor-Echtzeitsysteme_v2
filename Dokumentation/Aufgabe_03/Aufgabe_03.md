# Aufgabe 03 – Rate Monotonic Scheduling mit drei Tasks

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

- Beim **Eintritt** in den Task wird der zugehörige Pin **gesetzt**.
- Beim **Verlassen** des Tasks wird der Pin **zurückgesetzt**.
- Die Tasklänge wird durch eine **Warteschleife** simuliert – die Anzahl der Schleifendurchläufe ist experimentell zu ermitteln.

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