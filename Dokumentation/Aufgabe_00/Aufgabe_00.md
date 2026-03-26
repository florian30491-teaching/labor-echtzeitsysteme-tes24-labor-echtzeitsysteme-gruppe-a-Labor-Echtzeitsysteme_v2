# Aufgabe 00 – Einstieg: LED-Toggle mit einem FreeRTOS-Task

| | |
|---|---|
| **Gruppe** | |
| **Datum** | |
| **Branch** | `feature/aufgabe00` |

---

## Aufgabenbeschreibung

Erstellen Sie einen einzelnen FreeRTOS-Task, der eine der On-Board LEDs des Nucleo-H563ZI Boards periodisch **toggelt**.

Ziel dieser Aufgabe ist es, den grundlegenden Umgang mit dem Projekt, der Toolchain und FreeRTOS kennenzulernen:

- Einen **FreeRTOS-Task** mit `xTaskCreate()` anlegen
- Die Task-Funktion als **Endlosschleife** (`for (;;)`) implementieren
- Innerhalb des Tasks eine LED über `HAL_GPIO_TogglePin()` periodisch umschalten
- Die Periodendauer mit `vTaskDelay(pdMS_TO_TICKS(ms))` festlegen

### Verfügbare LEDs

| LED | Farbe | GPIO-Port & Pin |
|-----|-------|-----------------|
| LD1 | Grün | `LED_GREEN_GPIO_Port`, `LED_GREEN_Pin` |
| LD2 | Gelb | `LED_YELLOW_GPIO_Port`, `LED_YELLOW_Pin` |
| LD3 | Rot | `LED_RED_GPIO_Port`, `LED_RED_Pin` |

**Beispiel – LED toggeln:**
```c
HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
```

### Quellen

| Quelle | Inhalt |
|---|---|
| [freertos.org – xTaskCreate](https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/01-xTaskCreate) | `xTaskCreate` |
| [freertos.org – vTaskDelay](https://www.freertos.org/Documentation/02-Kernel/04-API-references/02-Task-control/01-vTaskDelay) | `vTaskDelay` |
| [freertos.org – Task Priorities](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/03-Task-priorities) | Task-Grundlagen und Prioritäten |

Betrachten Sie die Aufgabenstellung zunächst **theoretisch** und anschließend **praktisch**, indem Sie das Beispiel auf dem Board implementieren und die blinkende LED verifizieren.

---

## Bearbeitung

### Theoretisch

<!-- Vorüberlegungen: Was ist ein FreeRTOS-Task? Was macht xTaskCreate? -->
<!-- Bilder direkt in diesem Ordner ablegen: ![Beschreibung](dateiname.png) -->

### Praktisch

<!-- Umsetzung, Beobachtungen -->

```c
// Code-Snippet hier einfügen
```

