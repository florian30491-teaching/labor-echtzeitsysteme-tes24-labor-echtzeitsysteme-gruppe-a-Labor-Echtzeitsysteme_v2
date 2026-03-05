# Laborhandbuch – Labor Echtzeitsysteme
### DHBW Stuttgart

Dieses Handbuch beschreibt alle relevanten Aspekte des Labors Echtzeitsysteme:  
die verwendete Hardware, die Toolchain, den Projektaufbau sowie den vollständigen Workflow von der Einrichtung bis zur Abgabe per Pull Request.  
Es dient als **zentrales Nachschlagewerk** für die gesamte Labordauer.

---

## Inhaltsverzeichnis

1. [Hardware – STM32 Nucleo-H563ZI](#1-hardware--stm32-nucleo-h563zi)
2. [Toolchain & Software-Übersicht](#2-toolchain--software-übersicht)
   - [2.1 STM32CubeMX](#21-stm32cubemx)
   - [2.2 STM32CubeCLT](#22-stm32cubeclt)
   - [2.3 CMake & Ninja](#23-cmake--ninja)
   - [2.4 FreeRTOS](#24-freertos)
   - [2.5 JetBrains CLion (IDE)](#25-jetbrains-clion-ide)
3. [Projektstruktur](#3-projektstruktur)
4. [Projekt einrichten](#4-projekt-einrichten)
   - [4.1 Repository klonen](#41-repository-klonen)
   - [4.2 CLion konfigurieren](#42-clion-konfigurieren)
   - [4.3 Aufgabe auswählen](#43-aufgabe-auswählen)
5. [Bauen & Flashen](#5-bauen--flashen)
   - [5.1 Projekt bauen](#51-projekt-bauen)
   - [5.2 Firmware flashen](#52-firmware-flashen)
6. [Debugging mit WinIDEA & iC5700 BlueBox](#6-debugging-mit-winidea--ic5700-bluebox)
   - [6.1 Hardware – iC5700 BlueBox Trace-Debugger](#61-hardware--ic5700-bluebox-trace-debugger)
   - [6.2 WinIDEA – IDE & Debugger-Frontend](#62-winidea--ide--debugger-frontend)
   - [6.3 Projekt in WinIDEA einrichten](#63-projekt-in-winidea-einrichten)
   - [6.4 Flashen & Debuggen](#64-flashen--debuggen)
   - [6.5 Trace-Analyse mit der iC5700 BlueBox](#65-trace-analyse-mit-der-ic5700-bluebox)
7. [Aufgaben-Framework](#7-aufgaben-framework)
   - [7.1 Konzept](#71-konzept)
   - [7.2 Aufgabe wechseln](#72-aufgabe-wechseln)
   - [7.3 Eigene Aufgabe implementieren](#73-eigene-aufgabe-implementieren)
8. [Abgabe-Workflow (GitHub)](#8-abgabe-workflow-github)
   - [8.1 Branch erstellen](#81-branch-erstellen)
   - [8.2 Dokumentation verfassen](#82-dokumentation-verfassen)
   - [8.3 Committen & Pushen](#83-committen--pushen)
   - [8.4 Pull Request erstellen](#84-pull-request-erstellen)
   - [8.5 Review & Nachbesserung](#85-review--nachbesserung)
9. [Namenskonventionen](#9-namenskonventionen)
10. [Markdown-Kurzreferenz](#10-markdown-kurzreferenz)
11. [Häufige Fehler & Lösungen](#11-häufige-fehler--lösungen)

---

## 1. Hardware – STM32 Nucleo-H563ZI

Das Labor verwendet das **STM32 Nucleo-H563ZI** Entwicklungsboard von STMicroelectronics.

| Eigenschaft | Detail |
|---|---|
| **Board** | NUCLEO-H563ZI |
| **Mikrocontroller** | STM32H563ZI |
| **Architektur** | ARM Cortex-M33, 32-Bit |
| **Takt** | bis zu 250 MHz |
| **Flash** | 2 MB |
| **RAM** | 640 KB (SRAM) |
| **On-Board Debugger** | ST-LINK v3 (USB, kein externes Gerät nötig) |
| **Formfaktor** | Nucleo-144 (144-Pin Stecker-Layout) |
| **Betriebsspannung** | 3,3 V (I/O tolerant 5 V) |
| **Schnittstellen** | UART, SPI, I²C, CAN-FD, USB, ETH, ... |

### On-Board Peripherie (relevant für das Labor)

| Peripherie | Beschreibung |
|---|---|
| **LD1 (grün)** | User-LED, steuerbar via `HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin)` |
| **LD2 (gelb)** | User-LED, steuerbar via `HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin)` |
| **LD3 (rot)** | User-LED / Error-Indikator, steuerbar via `HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin)` |
| **B1 (blau)** | User-Taster, konfiguriert als EXTI-Interrupt |
| **UART (Virtual COM)** | Über ST-LINK als serieller Port am PC sichtbar (115200 Baud) |

### Pinout-Referenz

Das vollständige Pinout ist in der Datei [`Labor-Echtzeitsyteme.ioc`](./Firmware/Labor-Echtzeitsyteme.ioc) hinterlegt und kann mit **STM32CubeMX** geöffnet werden.

---

## 2. Toolchain & Software-Übersicht

### 2.1 STM32CubeMX

**STM32CubeMX** ist ein grafisches Konfigurationswerkzeug von STMicroelectronics.

- Grafische Peripherie-Konfiguration (Takt, GPIO, Timer, UART, ...)
- Automatische Generierung von HAL-Initialisierungscode (`main.c`, `stm32h5xx_hal_msp.c`, ...)
- Verwaltung von Middleware (FreeRTOS, USB, FAT-FS, ...)
- Exportiert ein **CMake-Projekt** (`.ioc`-Datei ist die Konfigurationsquelle)

> ⚠️ Die `.ioc`-Datei und der generierte Code in `Core/` werden vom Dozenten gepflegt.  
> Studierende ändern **nur** Dateien im Ordner `Firmware/Aufgaben/`.

**Download:** [st.com/stm32cubemx](https://www.st.com/en/development-tools/stm32cubemx.html)

---

### 2.2 STM32CubeCLT

**STM32CubeCLT** (Command Line Tools) ist das Backend der ST-Toolchain und enthält:

| Komponente | Funktion |
|---|---|
| `clang` / `clang++` | **ST ARM Clang** – Cross-Compiler für ARM Cortex-M (LLVM-basiert) |
| `arm-none-eabi-gdb` | GDB-Debugger für ARM |
| `ST-LINK_gdbserver` | GDB-Server für die ST-LINK Verbindung |
| `STM32_Programmer_CLI` | Flash-Tool für die Firmware |

> ℹ️ Als Compiler wird **ST ARM Clang** (LLVM-basiert) verwendet, **nicht** GCC.  
> ST ARM Clang ist speziell für STM32 optimiert und liefert eine bessere Diagnose sowie modernere C-Sprachunterstützung als der klassische `arm-none-eabi-gcc`.  
> Die CMake-Toolchain-Datei `cmake/starm-clang.cmake` konfiguriert Clang automatisch als Compiler.

CLion nutzt diese Tools automatisch im Hintergrund – eine direkte Interaktion ist normalerweise nicht nötig.

**Download:** [st.com/stm32cubeclt](https://www.st.com/en/development-tools/stm32cubeclt.html)

---

### 2.3 CMake & Ninja

Das Projekt wird mit **CMake** als Build-System verwaltet.

```
CMakeLists.txt  →  CMake konfiguriert das Projekt
                →  Ninja führt den eigentlichen Build aus
                →  ST ARM Clang kompiliert den C-Code
                →  Ausgabe: Labor-Echtzeitsyteme.elf / .bin / .hex
```

Der zentrale Schalter zum Wechseln der aktiven Aufgabe ist die CMake-Cache-Variable `AUFGABE`:

```bash
cmake -DAUFGABE=3 ..   # Aufgabe 3 aktivieren
```

Intern wird daraus das Präprozessor-Symbol `AUFGABE_NR=3`, das in `aufgaben.c` den richtigen Initialisierungsaufruf aktiviert.

---

### 2.4 FreeRTOS

Das Projekt verwendet **FreeRTOS** als Echtzeit-Betriebssystem (RTOS).  
Die Integration erfolgt über die CMSIS-RTOS-V2-Schicht von STM32CubeMX, im Code werden jedoch **ausschließlich native FreeRTOS-API-Aufrufe** verwendet:

| Konzept | FreeRTOS-API |
|---|---|
| Task anlegen | `xTaskCreate(fn, name, stack, arg, prio, &handle)` |
| Task verzögern | `vTaskDelay(pdMS_TO_TICKS(ms))` |
| Queue erstellen | `xQueueCreate(length, itemSize)` |
| Semaphore erstellen | `xSemaphoreCreateBinary()` |
| Software-Timer | `xTimerCreate(...)` |
| Aus ISR signalisieren | `xSemaphoreGiveFromISR(...)` |

Der Scheduler wird in `main.c` durch `osKernelStart()` gestartet. Ab diesem Punkt läuft das System vollständig unter FreeRTOS-Kontrolle.

---

### 2.5 JetBrains CLion (IDE)

Als Entwicklungsumgebung wird **JetBrains CLion** verwendet.

#### Ersteinrichtung

1. CLion starten → **File → Open** → das **Hauptverzeichnis** `Labor-Echtzeitsysteme/` öffnen  
   *(nicht nur den `Firmware/`-Unterordner – das Hauptprojekt ermöglicht auch Git-Integration und Commits direkt aus CLion)*
2. Im **Project**-Fenster (links) mit **Rechtsklick auf `Firmware/CMakeLists.txt`** →  
   **„Load CMake Project"** auswählen  
   → CLion erkennt das Firmware-Projekt und konfiguriert Build-Targets automatisch
3. **Settings → Build, Execution, Deployment → Toolchains**  
   Neue Toolchain anlegen oder bestehende anpassen:

   | Feld | Wert |
   |---|---|
   | **Name** | `STM32CubeCLT` |
   | **C-Compiler** | `.../STM32CubeCLT/STM32CubeCLT/st/Ac6/SystemWorkbench/plugins/fr.ac6.mcu.externaltools.arm-none.linux64_.../tools/compiler/bin/clang` |
   | **C++-Compiler** | `…/clang++` |
   | **Debugger** | `.../STM32CubeCLT/GNU-tools-for-STM32/bin/arm-none-eabi-gdb` |

   > 💡 Der genaue Pfad zu Clang hängt von der installierten STM32CubeCLT-Version ab.  
   > Unter Windows liegt CLT typischerweise unter `C:\ST\STM32CubeCLT_<version>\`.  
   > Alternativ kann die CMake-Toolchain-Datei `cmake/starm-clang.cmake` direkt in den CMake-Optionen referenziert werden (siehe unten).

4. **Settings → Build, Execution, Deployment → CMake**  
   Im aktiven Profil unter **„CMake options"** eintragen:
   ```
   -DCMAKE_TOOLCHAIN_FILE=cmake/starm-clang.cmake -DAUFGABE=1
   ```
5. Rechtsklick auf `Firmware/CMakeLists.txt` → **„Reload CMake Project"**

#### Git direkt aus CLion verwenden

Da das **Hauptverzeichnis** `Labor-Echtzeitsysteme/` als Projekt geöffnet ist, steht die vollständige Git-Integration zur Verfügung:

| Aktion | Weg in CLion |
|---|---|
| Änderungen einsehen | **Git → Commit** (`Strg + K`) |
| Branch erstellen | **Git → Branches → New Branch** |
| Push | **Git → Push** (`Strg + Shift + K`) |
| Pull | **Git → Pull** |
| PR erstellen | Direkt über GitHub im Browser nach dem Push |

#### Nützliche Tastenkürzel

| Aktion | Tastenkürzel |
|---|---|
| Projekt bauen | `Strg + F9` |
| Run/Flash | `Shift + F10` |
| Debug starten | `Shift + F9` |
| Commit | `Strg + K` |
| Push | `Strg + Shift + K` |
| Datei suchen | `Strg + Shift + N` |
| Symbol suchen | `Strg + Alt + Shift + N` |
| CMake neu laden | Rechtsklick auf `CMakeLists.txt` → „Reload CMake Project" |

---

## 3. Projektstruktur

```
Labor-Echtzeitsysteme/
│
├── Firmware/                          # Gesamte Embedded-Firmware
│   ├── CMakeLists.txt                 # Haupt-Build-Konfiguration + Aufgaben-Switch
│   ├── CMakePresets.json              # Vordefinierte CMake-Profile
│   ├── Labor-Echtzeitsyteme.ioc       # STM32CubeMX Konfigurationsdatei
│   ├── startup_stm32h563xx.s          # Startup-Assembler (generiert)
│   ├── STM32H563xx_FLASH.ld           # Linker-Script Flash
│   ├── STM32H563xx_RAM.ld             # Linker-Script RAM
│   │
│   ├── Aufgaben/                      # ← Hier arbeiten Studierende
│   │   ├── aufgaben.h                 # Gemeinsamer Header (FreeRTOS-Includes, Deklarationen)
│   │   ├── aufgaben.c                 # Dispatcher: ruft per #if AUFGABE_NR die richtige Aufgabe auf
│   │   ├── aufgabe_01.c               # Implementierung Aufgabe 01
│   │   ├── aufgabe_02.c               # Implementierung Aufgabe 02
│   │   └── ...                        # bis aufgabe_08.c
│   │
│   ├── Core/
│   │   ├── Inc/                       # Header (main.h, FreeRTOSConfig.h, ...)
│   │   └── Src/                       # Quellcode (main.c, app_freertos.c, ...)
│   │
│   ├── Drivers/
│   │   ├── STM32H5xx_HAL_Driver/      # ST HAL (Hardware Abstraction Layer)
│   │   ├── BSP/STM32H5xx_Nucleo/      # Board Support Package (Pin-Definitionen, nicht direkt verwendet)
│   │   └── CMSIS/                     # ARM CMSIS-Core Header
│   │
│   ├── Middlewares/
│   │   └── Third_Party/FreeRTOS/      # FreeRTOS Quellcode
│   │
│   └── cmake/stm32cubemx/             # Generiertes CMake-Subprojekt (HAL, Treiber)
│
├── Dokumentation/                     # Aufgabendokumentation der Studierenden
│   ├── README.md                      # Inhaltsverzeichnis
│   ├── Aufgabe_01/
│   │   ├── Aufgabe_01.md              # Dokumentations-Template
│   │   └── (Bilder direkt hier)
│   └── ...
│
├── Bewertung/
│   └── Bewertung.md                   # 🔒 Nur für den Dozenten
│
├── .github/
│   ├── CODEOWNERS                     # Schützt Firmware & Bewertung
│   └── pull_request_template.md       # Automatische PR-Vorlage
│
├── Laborhandbuch.md                   # Dieses Dokument
└── README.md                          # Einstiegsseite
```

---

## 4. Projekt einrichten

### 4.1 Repository klonen

Du erhältst über **GitHub Classroom** einen Link zu deinem Gruppen-Repository.

```bash
git clone <deine-repo-url>
cd Labor-Echtzeitsysteme
```

Git-Identität setzen (einmalig):

```bash
git config --global user.name "Vorname Nachname"
git config --global user.email "deine@email.de"
```

---

### 4.2 CLion konfigurieren

1. CLion starten → **File → Open** → **Hauptverzeichnis** `Labor-Echtzeitsysteme/` auswählen
2. Im **Project**-Fenster (links): **Rechtsklick auf `Firmware/CMakeLists.txt`** →  
   **„Load CMake Project"**  
   → CLion lädt das Firmware-Build-System und indexiert alle Quellen
3. **Settings → Build, Execution, Deployment → CMake**  
   Im aktiven Profil unter **„CMake options"** eintragen:
   ```
   -DCMAKE_TOOLCHAIN_FILE=cmake/starm-clang.cmake -DAUFGABE=1
   ```
4. **Build directory** auf `cmake-build-debug` setzen
5. Rechtsklick auf `Firmware/CMakeLists.txt` → **„Reload CMake Project"**

> ℹ️ Das Öffnen des **Hauptverzeichnisses** (statt nur `Firmware/`) ist wichtig –  
> nur so steht die vollständige Git-Integration (Commit, Branch, Push) direkt in CLion zur Verfügung.

---

### 4.3 Aufgabe auswählen

Die aktive Aufgabe wird über die CMake-Option `AUFGABE` gesteuert.

**In CLion:**
> Settings → Build, Execution, Deployment → CMake → CMake options  
> Wert ändern auf z.B. `-DAUFGABE=3`  
> → **„Reload CMake Project"**

**Auf der Kommandozeile:**
```bash
cd Firmware
cmake -B cmake-build-debug -DAUFGABE=3
cmake --build cmake-build-debug
```

> ℹ️ Nach dem Ändern der Aufgabennummer muss CMake neu konfiguriert werden – ein reines „Build" reicht nicht.

---

## 5. Bauen & Flashen

### 5.1 Projekt bauen

In CLion: `Strg + F9` oder **Build → Build Project**

Das Ergebnis liegt unter:
```
Firmware/cmake-build-debug/Labor-Echtzeitsyteme.elf
```

---

### 5.2 Firmware flashen

#### Über CLion (Run-Konfiguration)

1. Oben rechts in CLion die **Run-Konfiguration** auswählen (z.B. „Labor-Echtzeitsyteme")
2. `Shift + F10` oder **Run → Run**  
   CLion startet automatisch den GDB-Server und flasht das Board.

#### Über STM32_Programmer_CLI (manuell)

Board per USB verbinden, dann:

```bash
STM32_Programmer_CLI -c port=SWD -w cmake-build-debug/Labor-Echtzeitsyteme.elf -rst
```

---

## 6. Debugging mit WinIDEA & iC5700 BlueBox

### 6.1 Hardware – iC5700 BlueBox Trace-Debugger

Als Debugger wird der **iC5700 BlueBox** von iSYSTEM eingesetzt.  
Es handelt sich um einen professionellen Trace-Debugger, der neben klassischem Debugging auch vollständige **Trace-Aufzeichnung** (Ausführungshistorie, Code-Coverage, Laufzeitmessung) unterstützt.

| Eigenschaft | Detail |
|---|---|
| **Gerät** | iSYSTEM iC5700 BlueBox |
| **Verbindung zum PC** | USB (High-Speed) |
| **Verbindung zum Board** | JTAG / SWD (10-Pin oder 20-Pin Stecker) |
| **Trace-Modus** | ETM (Embedded Trace Macrocell) – Cortex-M33 |
| **Unterstützte Features** | Breakpoints, Watchpoints, Trace, Code-Coverage, Profiling, RTOS-Awareness |

Die BlueBox wird zwischen PC und Nucleo-Board geschaltet:

```
PC  ──USB──►  iC5700 BlueBox  ──JTAG/SWD──►  NUCLEO-H563ZI
                                              (CN6 / Debug-Stecker)
```

> ℹ️ Der On-Board ST-LINK des Nucleo-Boards wird beim Einsatz der BlueBox **nicht** verwendet.  
> Sicherstellen, dass der **SB** (Solder Bridge) zur Trennung des On-Board ST-LINK geöffnet ist –  
> oder den JTAG/SWD-Stecker direkt an den CN6-Connector der BlueBox anschließen.

---

### 6.2 WinIDEA – IDE & Debugger-Frontend

**WinIDEA** ist die Entwicklungsumgebung von iSYSTEM und dient als Frontend für den iC5700.  
Für das **tägliche Coding und Editieren** wird CLion verwendet – WinIDEA übernimmt ausschließlich  
das **Flashen, Debuggen und die Trace-Analyse**.

**Download:** [isystem.com/winidea](https://www.isystem.com/downloads/winidea.html)

---

### 6.3 Projekt in WinIDEA einrichten

1. WinIDEA starten → **File → New Workspace** (oder bestehendes öffnen)
2. **Hardware → Hardware Configuration**:
   - Interface: `iC5700`
   - Verbindungstyp: `SWD` (empfohlen) oder `JTAG`
   - Target: `STM32H563ZI`
3. **Debug → Download File** → ELF-Datei auswählen:
   ```
   Firmware/cmake-build-debug/Labor-Echtzeitsyteme.elf
   ```
4. **Debug → Connect** → Verbindung zur BlueBox herstellen
5. Die ELF-Datei enthält vollständige Debug-Symbole (`-O0 -g3`, kein Inlining) –  
   WinIDEA zeigt C-Quellcode, Variablen und Call-Stack direkt an.

---

### 6.4 Flashen & Debuggen

#### Firmware flashen

1. ELF-Datei in CMake gebaut (siehe [Kapitel 5](#5-bauen--flashen))
2. In WinIDEA: **Debug → Download** (oder Toolbar-Icon)  
   → Firmware wird über die BlueBox in den Flash des STM32H563ZI geschrieben
3. **Debug → Reset & Run** → Board startet neu mit neuer Firmware

#### Debugging-Grundfunktionen

| Aktion | WinIDEA |
|---|---|
| Debug starten | **Debug → Connect**, dann **Run** (`F5`) |
| Anhalten | **Debug → Break** (`F6`) |
| Breakpoint setzen | Klick auf Zeilennummer im Quellcode |
| Step Over | `F10` |
| Step Into | `F11` |
| Step Out | `Shift + F11` |
| Variablen beobachten | **View → Watch** – Variablennamen eintragen |
| Register anzeigen | **View → Registers** |
| Memory anzeigen | **View → Memory** – Adresse eingeben |
| Disassembly | **View → Disassembly** |

#### Breakpoints & Watchpoints

- **Breakpoint:** Hält die Ausführung an einer bestimmten Codezeile an
- **Watchpoint (Data Breakpoint):** Hält an, sobald eine bestimmte Variable gelesen oder geschrieben wird –  
  besonders nützlich um Race Conditions zwischen FreeRTOS-Tasks zu finden:

  ```
  View → Breakpoints → New → Data Breakpoint → Adresse / Variablenname
  ```

---

### 6.5 Trace-Analyse mit der iC5700 BlueBox

Der iC5700 unterstützt **ETM-Trace** (Embedded Trace Macrocell) des Cortex-M33.  
Dabei wird jede ausgeführte Instruktion aufgezeichnet – ohne die Programmausführung zu beeinflussen.

#### Trace aktivieren

1. **Hardware → Hardware Configuration → Trace**: `ETM` aktivieren
2. **Debug → Trace → Start Recording**
3. Programm normal ausführen lassen (oder bis zu einem Breakpoint)
4. **Debug → Trace → Stop Recording**

#### Was Trace ermöglicht

| Feature | Nutzen im Labor |
|---|---|
| **Execution History** | Rückwärts durch den Code navigieren – was wurde vor dem Fehler ausgeführt? |
| **Code Coverage** | Welche Zeilen wurden tatsächlich ausgeführt? Tote Code-Pfade sichtbar machen |
| **Profiling / Timing** | Exakte Laufzeitmessung einzelner Funktionen und Tasks |
| **RTOS-Awareness** | Task-Wechsel, Scheduling-Entscheidungen und Wartezeiten sichtbar machen |
| **Exception Trace** | Alle Interrupts und deren Laufzeiten aufgezeichnet |

#### RTOS-Awareness (FreeRTOS)

WinIDEA unterstützt **FreeRTOS-Kernel-Awareness** – der Debugger kennt die internen FreeRTOS-Strukturen und zeigt:

- Alle laufenden **Tasks** mit Name, Priorität und aktuellem Zustand (`Running`, `Ready`, `Blocked`, `Suspended`)
- **Task-Stack-Auslastung** pro Task
- **Queue- und Semaphore-Zustände**
- **Task-Wechsel-Historie** aus dem Trace

Aktivieren unter: **Debug → RTOS → FreeRTOS** → Kernel-Symbol-Datei aus dem ELF automatisch geladen.

---

## 7. Aufgaben-Framework

### 7.1 Konzept

Alle Aufgaben teilen sich eine gemeinsame Firmware-Basis. Welche Aufgabe aktiv ist, wird **ausschließlich zur Compile-Zeit** über das CMake-Symbol `AUFGABE_NR` gesteuert – kein Code-Umbau nötig.

```
CMake -DAUFGABE=3
        │
        ▼
   AUFGABE_NR = 3  (Präprozessor-Define)
        │
        ▼
   aufgaben.c
   #if AUFGABE_NR == 3
       aufgabe_03_init()    ←── aufgabe_03.c
           xTaskCreate(aufgabe03Task, ...)
```

Der vollständige Aufrufpfad beim Systemstart:

```
main()
  └─ osKernelInitialize()
  └─ MX_FREERTOS_Init()          ← app_freertos.c
       └─ aufgabe_freertos_init() ← aufgaben.c  (Dispatcher)
            └─ aufgabe_0X_init()  ← aufgabe_0X.c
                 └─ xTaskCreate(aufgabe0XTask, ...)
  └─ osKernelStart()             ← Scheduler übernimmt
       └─ aufgabe0XTask()        ← läuft als FreeRTOS-Task
```

---

### 7.2 Aufgabe wechseln

**In CLion:**
1. Settings → Build, Execution, Deployment → CMake → CMake options
2. `-DAUFGABE=X` anpassen (X = 1 … 8)
3. Rechtsklick auf `CMakeLists.txt` → **„Reload CMake Project"**
4. `Strg + F9` → Build

**Auf der Kommandozeile:**
```bash
cmake -B cmake-build-debug -DAUFGABE=5
cmake --build cmake-build-debug
```

---

### 7.3 Eigene Aufgabe implementieren

Öffne die zugehörige Datei, z.B. für Aufgabe 3:

```
Firmware/Aufgaben/aufgabe_03.c
```

Die Datei hat folgende Grundstruktur:

```c
void aufgabe_03_init(void)
{
    // FreeRTOS-Objekte anlegen
    xTaskCreate(aufgabe03Task,
                "aufgabe03Task",
                AUFGABE03_TASK_STACK,   // Stack-Größe in Words
                NULL,
                AUFGABE03_TASK_PRIO,    // tskIDLE_PRIORITY + 1
                &aufgabe03TaskHandle);

    // Weitere Objekte möglich:
    // xQueueCreate(...)
    // xSemaphoreCreateBinary(...)
    // xTimerCreate(...)
}

static void aufgabe03Task(void *argument)
{
    // Initialisierung (einmalig)

    for (;;)
    {
        // Aufgaben-Logik hier

        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 Sekunde warten
    }
}
```

> ⚠️ **Nur Dateien in `Firmware/Aufgaben/` verändern.**  
> `main.c`, `app_freertos.c`, `aufgaben.c` und alle generierten Dateien werden nicht von Studierenden bearbeitet.

---

## 8. Abgabe-Workflow (GitHub)

Jede Aufgabe wird über einen eigenen **Feature-Branch** und **Pull Request** abgegeben.

### 8.1 Branch erstellen

```bash
git checkout main
git pull
git checkout -b feature/aufgabe03
```

> **Branch-Schema:** `feature/aufgabeXX`  
> Beispiele: `feature/aufgabe01`, `feature/aufgabe03`, ...

---

### 8.2 Dokumentation verfassen

Öffne:
```
Dokumentation/Aufgabe_03/Aufgabe_03.md
```

Fülle die **drei Pflichtabschnitte** aus:

| Abschnitt | Inhalt |
|---|---|
| **Aufgabenbeschreibung** | Aufgabe in eigenen Worten, Ziel |
| **Bearbeitung – Theoretisch** | Vorüberlegungen, Konzepte, Berechnungen, Schaltpläne |
| **Bearbeitung – Praktisch** | Umsetzung, Code-Snippets, Messergebnisse, Screenshots |

Bilder direkt im Aufgabenordner ablegen und verlinken:
```markdown
![Beschreibung](schaltplan.png)
```

---

### 8.3 Committen & Pushen

```bash
git add Dokumentation/Aufgabe_03/
git status                          # Prüfen was hinzugefügt wird
git commit -m "Aufgabe 03: Dokumentation fertiggestellt"
git push origin feature/aufgabe03
```

**Commit-Format:** `Aufgabe XX: <kurze Beschreibung>`

---

### 8.4 Pull Request erstellen

1. GitHub öffnen → **„Compare & pull request"** klicken
2. Felder ausfüllen:

   | Feld | Inhalt |
   |---|---|
   | **Title** | `Aufgabe 03 – Abgabe Gruppe [Name]` |
   | **Base** | `main` |
   | **Compare** | `feature/aufgabe03` |
   | **Reviewers** | Dozenten-Account eintragen |

3. Die **PR-Checkliste** (automatisch geladen) vollständig abhaken
4. **„Create pull request"** klicken

> ⚠️ Kein Selbst-Merge – nur der Dozent merged in `main`.

---

### 8.5 Review & Nachbesserung

```
PR erstellt  →  Dozent prüft  →  Kommentar im PR
                                      │
                        ┌─────────────┴──────────────┐
                        ▼                            ▼
                 ✅ Bestanden                 ❌ Nachbesserung
                 (Merge in main)         Auf demselben Branch weiterarbeiten:
                                         git checkout feature/aufgabe03
                                         # Änderungen vornehmen
                                         git commit -m "Aufgabe 03: Feedback eingearbeitet"
                                         git push origin feature/aufgabe03
                                         → PR aktualisiert sich automatisch
```

---

## 9. Namenskonventionen

| Element | Schema | Beispiel |
|---|---|---|
| **Branch** | `feature/aufgabeXX` | `feature/aufgabe05` |
| **Commit-Message** | `Aufgabe XX: <Beschreibung>` | `Aufgabe 05: Theoretischen Teil ergänzt` |
| **PR-Titel** | `Aufgabe XX – Abgabe Gruppe [Name]` | `Aufgabe 05 – Abgabe Gruppe Alpha` |
| **Bilddateien** | `beschreibung.png` (Kleinbuchstaben, kein Leerzeichen) | `schaltplan_timer.png` |
| **Funktionen (C)** | `snake_case` | `aufgabe_03_init()` |
| **Tasks (C)** | `aufgabe0XTask` | `aufgabe03Task` |
| **Makros (C)** | `SCREAMING_SNAKE_CASE` | `AUFGABE03_TASK_STACK` |

---

## 10. Markdown-Kurzreferenz

```markdown
# Überschrift 1        ## Überschrift 2        ### Überschrift 3

**fett**   *kursiv*   `inline-code`   ~~durchgestrichen~~

- Aufzählungspunkt
- Noch ein Punkt
  - Eingerückt

1. Nummeriert
2. Zweiter Punkt

| Spalte 1 | Spalte 2 |
|----------|----------|
| Wert A   | Wert B   |

![Bildbeschreibung](dateiname.png)
[Linktext](https://beispiel.de)
```

Code-Block mit Syntax-Highlighting:
````markdown
```c
xTaskCreate(meinTask, "meinTask", 256, NULL, tskIDLE_PRIORITY + 1, &handle);
```
````

---

## 11. Häufige Fehler & Lösungen

| Problem | Ursache | Lösung |
|---|---|---|
| CLion findet Compiler nicht | Toolchain nicht konfiguriert | Settings → Toolchains → STM32CubeCLT-Pfad, Clang als Compiler eintragen |
| `AUFGABE_NR` nicht definiert | CMake nicht neu geladen | Rechtsklick CMakeLists.txt → „Reload CMake Project" |
| BlueBox wird nicht erkannt | USB-Treiber fehlt oder falsche iC5700-Version | iSYSTEM USB-Treiber / WinIDEA neu installieren |
| WinIDEA verbindet nicht | Falscher Target oder SWD-Pins nicht verbunden | Hardware Configuration prüfen: Target `STM32H563ZI`, SWD-Kabel am CN6 |
| Direkt auf `main` committed | Branch vergessen | `git reset HEAD~1 --soft` → dann Branch anlegen |
| Bilder im PR nicht sichtbar | Leerzeichen oder Umlaut im Dateinamen | Datei umbenennen, nur Kleinbuchstaben und `_` |
| Build schlägt fehl: `undefined reference` | Funktion nicht implementiert | `aufgabe_XX_init()` in `aufgabe_XX.c` vollständig implementiert? |
| Trace zeigt keine Daten | ETM nicht aktiviert | Hardware Configuration → Trace → ETM aktivieren |
| PR ohne Reviewer | Vergessen | Im offenen PR rechts unter „Reviewers" Dozent eintragen |

---



*Bei Fragen wende dich direkt an den Dozenten oder hinterlasse einen Kommentar im jeweiligen Pull Request.*
