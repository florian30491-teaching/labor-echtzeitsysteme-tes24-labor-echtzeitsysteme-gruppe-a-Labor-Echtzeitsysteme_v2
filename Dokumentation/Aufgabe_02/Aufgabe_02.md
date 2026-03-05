# Aufgabe 02

| | |
|---|---|
| **Gruppe** | |
| **Datum** | |
| **Branch** | `feature/aufgabe02` |

---

## Aufgabenbeschreibung

Legen Sie einen Task an, welcher ein **Lauflicht** von n ≥ 3 Pins (z. B. DBG1, DBG2 und DBG3) programmiert.

- Der Task soll eine **höhere Priorität als der IDLE-Task** haben.
- Der Task soll eine **Taskperiode von 250 ms** haben.
- Bei jedem Aufruf des Tasks soll das Lauflicht **um eine weitere Stelle verschoben** werden.
- Wenn das Ende der Kette erreicht ist, soll das Lauflicht **wieder von vorne beginnen**.

Betrachten Sie die Aufgabenstellung zunächst **theoretisch** und anschließend **praktisch**, indem Sie ein Beispiel programmieren und dieses messtechnisch geeignet untersuchen.

**Hinweise zur messtechnischen Untersuchung:**

Für die Messung stehen im Labor mehrere Methoden zur Verfügung – wählen Sie mindestens eine davon und begründen Sie Ihre Wahl:

- **Trace-Debugger (iC5700 BlueBox + WinIDEA):** Der im Labor verfügbare Trace-Debugger kann Task-Ausführungszeiten und -perioden aufzeichnen. In WinIDEA lässt sich über die Trace-Analyse (z. B. *Task Execution Chart*) die Taskperiode direkt ablesen.
- **GPIO-Toggle + Oszilloskop:** Der Task setzt beim Eintritt einen dedizierten GPIO-Ausgangspin. Am Oszilloskop ist die Periode der Flanken direkt als Taskperiode messbar.
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