# Aufgabe 05 – Lauflicht mit variabler Periode über Taster-Interrupt und Semaphore

> 📝 **Bearbeitung:** [→ Bearbeitung_05.md](Bearbeitung_05.md)
>
> ⚠️ **Diese Datei ist die Aufgabenstellung und darf nicht verändert werden.**
> Alle Antworten, Notizen und Code-Snippets gehören ausschließlich in die Bearbeitungsdatei.

---

## Aufgabenbeschreibung

Diese Aufgabe baut auf **Aufgabe 02** (Lauflicht mit periodischem Task) auf.
Das bestehende Lauflicht wird um eine **Taster-Steuerung** erweitert: Bei jedem Tastendruck soll die **Periode des Lauflichts** zyklisch verändert werden.

Die Kommunikation zwischen dem **Taster-Interrupt** (ISR-Kontext) und dem **Lauflicht-Task** (Task-Kontext) wird über eine **Binary Semaphore** realisiert, wie sie in Aufgabe 04 eingeführt wurde.

> ### ℹ️ Abgrenzung zu Aufgabe 04
>
> In **Aufgabe 04** wurde die Binary Semaphore als **Ressourcenschutz** (Mutex-Muster) eingesetzt:
> Die ISR und der Task greifen beide lesend/schreibend auf eine gemeinsame Zustandsvariable zu – die Semaphore schützt diesen kritischen Abschnitt (`TakeFromISR` → Zustand ändern → `GiveFromISR`).
>
> In **Aufgabe 05** wird die Binary Semaphore als **Signalisierungsmechanismus** (*Deferred Interrupt Processing*) verwendet:
> Die ISR signalisiert dem Task lediglich, dass ein Ereignis eingetreten ist (`GiveFromISR`), und der Task reagiert darauf (`Take`).
> Zusätzlich kommen hier **Software-Entprellung** und das **`__weak`-Callback-Konzept** als neue Themen hinzu.
> Es gibt keinen »Aus«-Zustand – das Lauflicht läuft immer.

---

## Teil 1 – Theorie: Das `__weak`-Callback-Konzept

### Was bedeutet `__weak`?

Das Schlüsselwort `__weak` ist ein **Compiler-Attribut** (GCC: `__attribute__((weak))`), das eine Funktion als **schwach gebunden** (engl. *weakly linked*) markiert.
Eine `__weak`-Funktion dient als **Standardimplementierung**, die vom Linker automatisch durch eine **gleichnamige, nicht-schwache Funktion** (»starke Definition«) ersetzt wird, sofern eine solche existiert.

**Prinzip:**

```
┌─────────────────────────────────────────────────────┐
│  HAL-Treiber (stm32h5xx_hal_gpio.c)                │
│                                                     │
│  __weak void HAL_GPIO_EXTI_Falling_Callback(...)    │
│  {                                                  │
│      UNUSED(GPIO_Pin);  /* leere Implementierung */ │
│  }                                                  │
└─────────────────────┬───────────────────────────────┘
                      │  Linker ersetzt automatisch,
                      │  wenn eine starke Definition existiert
                      ▼
┌─────────────────────────────────────────────────────┐
│  Eigener Code (aufgabe_05.c)                        │
│                                                     │
│  void HAL_GPIO_EXTI_Falling_Callback(...)           │
│  {                                                  │
│      /* eigene Implementierung – überschreibt die   │
│         __weak-Version des HAL-Treibers */          │
│  }                                                  │
└─────────────────────────────────────────────────────┘
```

**Vorteile dieses Musters:**
- Der HAL-Treiber bleibt **unverändert** – es muss kein fremder Code editiert werden
- Wird kein Callback benötigt, sorgt die `__weak`-Standardversion dafür, dass kein Linker-Fehler entsteht
- Mehrere Aufgaben können **denselben Callback unabhängig überschreiben**, da immer nur eine Aufgabe gleichzeitig kompiliert wird

### Interrupt-Aufrufkette im STM32H5

Der vollständige Weg vom Hardware-Ereignis bis zum eigenen Code:

```
Taster betätigt (Hardware-Flanke auf PC13)
    │
    ▼
EXTI13_IRQHandler()                    ← Interrupt-Vektor (stm32h5xx_it.c)
    │
    ▼
HAL_GPIO_EXTI_IRQHandler(GPIO_Pin)     ← HAL erkennt Flankenrichtung
    │
    ├─ steigende Flanke → HAL_GPIO_EXTI_Rising_Callback(GPIO_Pin)
    │
    └─ fallende Flanke  → HAL_GPIO_EXTI_Falling_Callback(GPIO_Pin)
                                        ↑
                           Hier greift eure __weak-Überschreibung
```

### Aufgabe (Theorie)

Beantwortet in der [Bearbeitung](Bearbeitung_05.md) folgende Fragen:

1. Erklärt in eigenen Worten, was `__weak` bedeutet und warum der HAL-Treiber dieses Muster für Callbacks verwendet.
2. Was passiert, wenn **keine** starke Definition des Callbacks existiert? Warum führt das nicht zu einem Fehler?
3. In welcher Datei und Funktion befindet sich der Interrupt-Handler für den User-Button (`EXTI13_IRQHandler`)? Zeichnet den vollständigen Aufrufpfad vom Hardware-Interrupt bis zu eurem Callback nach.

---

## Teil 2 – Praktische Umsetzung

### Ausgangsbasis

Übernehmt das **Lauflicht aus Aufgabe 02** (mindestens 3 GPIO-Pins, z. B. DBG1, DBG2, DBG3) in `aufgabe_05.c`.
Der Lauflicht-Task läuft initial mit einer Periode von **250 ms**.

### Callback-Gerüst in `aufgabe_05.c`

Die Codevorlage `aufgabe_05.c` enthält neben der leeren `aufgabe_05_init()` bereits die beiden **Callback-Gerüste**, mit denen die `__weak`-Versionen des HAL-Treibers überschrieben werden:

```c
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    /* Eigene Implementierung hier – überschreibt die __weak-Version */
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    /* Eigene Implementierung hier – überschreibt die __weak-Version */
}
```

> ⚠️ **Wichtig:** Durch die gleichnamige, starke Definition werden die `__weak`-Versionen aus dem HAL-Treiber automatisch vom Linker ersetzt.
> Entscheidet selbst, welcher der beiden Callbacks für diese Aufgabe geeignet ist und begründet eure Wahl.

### Taster-Interrupt: Pin prüfen und Entprellen

Im Callback muss zunächst geprüft werden, **welcher Pin** den Interrupt ausgelöst hat, da der Callback für **alle** EXTI-Pins aufgerufen wird:

```c
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_BUTTON_Pin)
    {
        /* Nur auf den User-Button reagieren */
    }
}
```

**Entprellung (Debouncing):**
Mechanische Taster erzeugen beim Betätigen mehrere schnelle Flanken (*Prellen*).
Ohne Entprellung wird der Callback mehrfach hintereinander aufgerufen.

Einfache Software-Entprellung per Zeitstempel:

```c
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin != USER_BUTTON_Pin) return;

    static TickType_t xLastPress = 0;
    TickType_t xNow = xTaskGetTickCountFromISR();

    if ((xNow - xLastPress) < pdMS_TO_TICKS(200))  /* 200 ms Sperrzeit */
        return;

    xLastPress = xNow;

    /* Ab hier: gültiger Tastendruck → Semaphore signalisieren */
}
```

> ⚠️ Innerhalb eines ISR-Callbacks dürfen nur **`…FromISR()`**-Varianten der FreeRTOS-API verwendet werden!

### Semaphore für Task/Interrupt-Kommunikation

Die Kommunikation zwischen ISR und Task folgt dem **Signalisierungs-Muster** (engl. *deferred interrupt processing*):

1. **Initialisierung** (`aufgabe_05_init`): Binary Semaphore mit `xSemaphoreCreateBinary()` erstellen (initial leer)
2. **ISR-Callback**: Nach erfolgreicher Entprellung die Semaphore mit `xSemaphoreGiveFromISR()` freigeben, um den Task aufzuwecken
3. **Task**: Wartet mit `xSemaphoreTake()` auf die Semaphore und wechselt bei Empfang zur nächsten Periode

```
ISR (Taster-Callback)                   Task (Lauflicht)
─────────────────────                   ─────────────────
  Pin prüfen                              wartet auf Semaphore
  Entprellen                              (xSemaphoreTake, Timeout = 0 oder kurz)
  xSemaphoreGiveFromISR(sem)  ──────►   Semaphore empfangen
                                          → Periode wechseln
                                          Lauflicht-Schritt ausführen
                                          vTaskDelay(aktuelle Periode)
```

### Periodenwechsel

Bei jedem gültigen Tastendruck soll die Lauflicht-Periode **zyklisch** zwischen folgenden Werten wechseln:

| Tastendruck | Periode |
|-------------|---------|
| Initial / 4. Druck | 250 ms |
| 1. Druck | 500 ms |
| 2. Druck | 750 ms |
| 3. Druck | 250 ms |
| … | zyklisch weiter |

> **Hinweis:** Das Lauflicht läuft immer – es gibt in dieser Aufgabe keinen »Aus«-Zustand.

### Ziel der Aufgabe

1. Verständnis des **`__weak`-Callback-Konzepts** und der Interrupt-Aufrufkette im STM32 HAL
2. Implementierung eines **Taster-Interrupts** mit **Pin-Prüfung** und **Software-Entprellung**
3. Umsetzung der **Semaphore-basierten Kommunikation** zwischen ISR-Kontext und Task-Kontext (Signalisierungsmuster)
4. Praktische Erweiterung des Lauflichts aus Aufgabe 02 um eine **variable Periode**
5. Messtechnische Untersuchung über die GPIO-Pins sowie den **Trace-Debugger (WinIDEA / iC5700 BlueBox)**

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
| `xSemaphoreGiveFromISR()` – Semaphore aus ISR freigeben | [freertos.org – xSemaphoreGiveFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/17-xSemaphoreGiveFromISR) |
| `xSemaphoreTake()` – Semaphore anfordern (Task-Kontext) | [freertos.org – xSemaphoreTake](https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/12-xSemaphoreTake) |
| `xTaskGetTickCountFromISR()` – Tick-Zähler aus ISR lesen | [freertos.org – xTaskGetTickCountFromISR](https://www.freertos.org/Documentation/02-Kernel/04-API-references/04-RTOS-kernel-control/06-xTaskGetTickCountFromISR) |
| Deferred Interrupt Handling – Übersicht | [freertos.org – Deferred Interrupt Handling](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/11-Deferred-interrupt-handling) |
| `vTaskDelay()` – Task verzögern | [freertos.org – vTaskDelay](https://www.freertos.org/Documentation/02-Kernel/04-API-references/02-Task-control/01-vTaskDelay) |
