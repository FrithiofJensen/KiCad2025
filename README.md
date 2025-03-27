# SemiKiCad 2025 - Fra Skematik til PCB for ESP-NOW RC System

Velkommen til materialerne for KiCad 2025 seminaret afholdt af Gert Lauritsen. Dette repository indeholder eksempler, projektfiler og Arduino-kode specifikt fokuseret på design af et printkort (PCB) til et fjernstyringssystem (RC) baseret på ESP-NOW protokollen.

Formålet med seminaret er at demonstrere designflowet i KiCad – fra skematisk diagram til færdigt PCB-layout – med et konkret projekt som eksempel: design af hardware til en ESP-NOW baseret sender eller modtager.

Vi vil se på, hvordan man designer et printkort, der integrerer en ESP8266 eller ESP32 microcontroller sammen med nødvendige komponenter som f.eks. input-enheder (joysticks, knapper) til en sender, eller output-drivere (servo-stik, motor-drivere) til en modtager. De medfølgende `.ino` filer implementerer selve ESP-NOW kommunikationen.

## Indhold

Repository'et indeholder (eller vil indeholde) følgende:

*   **KiCad Projektfiler:**
    *   Eksempler på skematiske diagrammer (`.kicad_sch`) for en RC-sender og/eller modtager.
    *   Eksempler på PCB layouts (`.kicad_pcb`) tilpasset ESP-moduler og RC-komponenter.
    *   Projektfil (`.kicad_pro`).
*   **Biblioteksfiler:**
    *   Specifikke symboler (`.kicad_sym`) og footprints (`.kicad_mod`) brugt i eksemplerne (hvis de ikke er standard i KiCad).
*   **ESP-NOW RC Arduino Sketches (`.ino`):**
    *   `RcRemote.ino`: Kode til ESP32 **senderen**. Læser typisk inputs (f.eks. fra potentiometre/joystick på det designede PCB) og sender data via ESP-NOW.
    *   `RCModtager.ino`: Kode til ESP32 **modtageren**. Modtager data via ESP-NOW og styrer outputs (f.eks. servoer eller motorer, potentielt via det designede PCB).
    *   Disse filer er beregnet til at blive kompileret og uploadet via Arduino IDE til separate ESP8266/ESP32 boards.
*   **Gerber/Produktionsfiler (Eksempel):**
    *   Eksempler på output-filer (`.gbr`, `.drl`) til PCB-produktion.

## Forudsætninger

For at få det fulde udbytte af seminaret og materialerne anbefales følgende software og hardware:

*   **Software:**
    *   **KiCad:**  Den seneste stabile version anbefales. Kan downloades gratis fra [https://www.kicad.org/](https://www.kicad.org/).
    *   **Arduino IDE:** Nødvendig for at arbejde med `.ino` filerne. Kan downloades gratis fra [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software).
        *   **VIGTIGT:** Sørg for at have **ESP8266** og/eller **ESP32 board support** installeret i Arduino IDE via "Boards Manager". Se guides online, f.eks.:
            *   ESP32: [https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-v2/](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-v2/)
            *   ESP8266: [https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
        *   Tjek `.ino` filerne for eventuelle yderligere nødvendige Arduino libraries (ud over ESP-NOW, som er indbygget) og installer dem via "Library Manager".
*   **Hardware:**
    *   **Mindst to ESP32 boards** (f.eks. NodeMCU, ESP32 DevKitC) – ét til senderen og ét til modtageren for at kunne teste ESP-NOW kommunikationen.
    *   (Valgfrit) Komponenter til at bygge prototyper af sender/modtager (breadboard, ledninger, joysticks, potentiometre, servoer etc.).

## Brug af Materialerne

1.  **Klon eller Download:** Hent materialerne ned på din computer ved at klone repository'et (`git clone https://github.com/gert-lauritsen/KiCad2025.git`) eller downloade det som en ZIP-fil.
2.  **KiCad:** Åbn `.kicad_pro` filen i KiCad for at udforske skematik og PCB layout. Brug filerne som reference eller udgangspunkt for dit eget RC-hardware design.
3.  **Arduino / ESP-NOW:**
    *   Åbn `[Sender_Filnavn].ino` i Arduino IDE.
    *   Åbn `[Modtager_Filnavn].ino` i et andet Arduino IDE vindue (eller efter hinanden).
    *   **Vigtigt:** ESP-NOW kræver, at enhederne kender hinandens MAC-adresser. Du skal typisk finde MAC-adressen på modtager-boardet og indsætte den i koden på sender-boardet (og/eller omvendt, afhængigt af koden). Find instruktioner til dette i kode-kommentarerne eller i ESP-NOW tutorials.
    *   Vælg det korrekte ESP32 board og port i "Tools" menuen for hvert board.
    *   Kompilér og upload sender-koden til det ene board.
    *   Kompilér og upload modtager-koden til det andet board.
    *   Test kommunikationen. Brug evt. Serial Monitor til debugging.



Vi håber, du får et lærerigt og udbytterigt seminar! Husk at installere KiCad, Arduino IDE og ESP board support *inden* seminaret starter for at få mest muligt ud af det.
