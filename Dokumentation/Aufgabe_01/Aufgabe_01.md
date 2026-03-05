# Aufgabe 01

| | |
|---|---|
| **Gruppe** | |
| **Datum** | |
| **Branch** | `feature/aufgabe01` |

---

## Aufgabenbeschreibung

Erarbeiten Sie sich die Grundlagen zum **Scheduling** von FreeRTOS.

**Quellen (Pflichtlektüre – bitte alle studieren):**

| Quelle | Inhalt |
|---|---|
| https://www.freertos.org/implementation/a00002.html | FreeRTOS Scheduling-Grundlagen |
| https://www.freertos.org/RTOS-task-states.html | Task-Zustände (Running, Ready, Blocked, Suspended) |
| https://www.freertos.org/a00106.html | `xTaskCreate` API |
| https://www.freertos.org/a00022.html | `vTaskDelay` / `vTaskDelayUntil` |
| https://www.freertos.org/taskandcr.html | Task-Grundlagen und Prioritäten |
| https://www.freertos.org/RTOS-Cortex-M3-M4.html | FreeRTOS auf ARM Cortex-M (Kontextwechsel, PendSV) |

Stellen Sie auf Basis dieser Grundlagen dar, wie sich das FreeRTOS-Echtzeitbetriebssystem bei **präemptivem Scheduling von zwei Tasks mit gleicher Priorität** verhält.

1. Betrachten Sie das Scheduling zunächst **theoretisch**.
2. **Programmieren** Sie anschließend ein Beispiel mit zwei Tasks gleicher Priorität.
3. Untersuchen Sie das Beispiel **messtechnisch** und ermitteln Sie dabei die Dauer des Taskwechsels.

**Hinweise zur messtechnischen Untersuchung:**

Für die Messung stehen im Labor mehrere Methoden zur Verfügung – wählen Sie mindestens eine davon und begründen Sie Ihre Wahl:

- **Trace-Debugger (iC5700 BlueBox + WinIDEA):** Der im Labor verfügbare Trace-Debugger kann Task-Wechsel auf Instruktionsebene aufzeichnen. In WinIDEA lässt sich über die Trace-Analyse (z. B. *Task Execution Chart*) direkt ablesen, wann welcher Task aktiv ist und wie lange der Kontextwechsel dauert.
- **GPIO-Toggle + Oszilloskop:** Jeder Task setzt beim Eintritt einen dedizierten GPIO-Ausgangspin auf High und zieht ihn vor dem Kontextwechsel wieder auf Low. Am Oszilloskop ist die Lücke zwischen den Flanken direkt als Kontextwechselzeit messbar.
- **GPIO-Toggle + Logikanalysator:** Wie oben, jedoch mit einem Logikanalysator zur digitalen Aufzeichnung und Zeitauswertung.

---

## Bearbeitung

### Theoretisch

<!-- Vorüberlegungen, Konzepte, Berechnungen, Schaltpläne -->
<!-- Bilder direkt in diesem Ordner ablegen und wie folgt einbinden: -->
<!-- ![Beschreibung](dateiname.png) -->

### Praktisch

<!-- Umsetzung am Board/Gerät, Messergebnisse, Code-Snippets, Screenshots -->

```c
// Code-Snippet hier einfügen
```

