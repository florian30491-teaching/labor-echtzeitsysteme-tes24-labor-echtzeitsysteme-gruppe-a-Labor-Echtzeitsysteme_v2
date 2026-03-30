# Aufgabe 02 – Lauflicht mit periodischem Task

> 📝 **Bearbeitung:** [→ Bearbeitung_02.md](Bearbeitung_02.md)
>
> ⚠️ **Diese Datei ist die Aufgabenstellung und darf nicht verändert werden.**
> Alle Antworten, Notizen und Code-Snippets gehören ausschließlich in die Bearbeitungsdatei.

---

## Aufgabenbeschreibung

Legen Sie einen Task an, welcher ein **Lauflicht** von n ≥ 3 Pins (z. B. DBG1, DBG2 und DBG3) programmiert.

- Der Task soll eine **höhere Priorität als der IDLE-Task** haben.
- Der Task soll eine **Taskperiode von 250 ms** haben.
- Bei jedem Aufruf des Tasks soll das Lauflicht **um eine weitere Stelle verschoben** werden.
- Wenn das Ende der Kette erreicht ist, soll das Lauflicht **wieder von vorne beginnen**.

Betrachten Sie die Aufgabenstellung zunächst **theoretisch** und anschließend **praktisch**, indem Sie ein Beispiel programmieren und dieses messtechnisch untersuchen.

> Für die messtechnische Untersuchung steht der **Trace-Debugger (iC5700 BlueBox + WinIDEA)** zur Verfügung.  
> Alternativ können GPIO-Toggle-Pins am Oszilloskop oder Logikanalysator ausgewertet werden.

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

