<h1 align="center">Scoreboard Controller System 🏀</h1>

<p align="justify">This repository provides the code and documentation for controlling a scoreboard and shotclock system using multiple microcontrollers, including Arduino Mega 2560, NodeMCU 8266, and ESP8266-based boards. The system allows for real-time control and synchronization of the scoreboard and shotclock via wireless communication.</p>

---

## 📑 Table of Contents

1. [Overview](#overview)
2. [Hardware Setup](#hardware-setup)
3. [A0 Modes and Pairing](#a0-modes-and-pairing)
4. [Firmware Programming](#firmware-programming)
5. [Connections and DIP Switches](#connections-and-dip-switches)
6. [Troubleshooting](#troubleshooting)
7. [License](#license)
8. [Author](#author)

---

<a id="overview"></a>
## Overview

The Mega2560+WiFi combo board is one physical board with two processors. The Mega 2560 runs the controller and owns the buttons, buzzer, and controller display. Its onboard ESP8266 runs the Wi-Fi firmware and communicates with the Mega over the board's internal Serial3 connection. That ESP8266 sends scoreboard updates to display boards over ESP-NOW.

The usual installation has one scoreboard and two shotclocks. The scoreboard and both shotclocks are separate ESP8266 display boards.

- `Scoreboard_Controller_FW`: runs on the Mega 2560 processor.
- `Scoreboard_Controller_WiFi_FW`: runs on the combo board's onboard ESP8266.
- `Scoreboard_FW`: runs on the scoreboard display ESP8266. Select `BOARD 1` or `BOARD 2` to match the display wiring.
- `Shotclock_FW`: runs on both shotclock display ESP8266 boards.

---

<a id="hardware-setup"></a>
## Hardware Setup

Required hardware:

- 1 Mega2560+WiFi combo board. Its Mega 2560 processor runs the controller; its onboard ESP8266 runs the ESP-NOW bridge.
- 1 ESP8266 display board for the scoreboard.
- 2 ESP8266 display boards for the shotclocks.

The Mega's buttons, buzzer, and four-digit display are used for pairing feedback. The onboard Mega-to-ESP8266 Serial3 connection is internal to the combo board; no separate Wi-Fi bridge NodeMCU or external UART wiring is needed.

---

<a id="a0-modes-and-pairing"></a>
## A0 Modes and Pairing

The onboard ESP8266 reads A0 at startup to choose a mode and radio channel:

| A0 reading | Mode | Channel | Routing |
| --- | --- | ---: | --- |
| Below 450 (near 0 V) | Group 1 | 1 | Send to the three MAC addresses stored for Group 1. |
| 450-899 (about 1.5 V) | Group 2 | 6 | Send to the three MAC addresses stored for Group 2. |
| 900 or above (near 3.3 V) | All | 11, plus channels 1 and 6 | Send the ESP-NOW broadcast MAC `FF:FF:FF:FF:FF:FF` so devices on any group channel receive the update. |

The A0 level must be selected before powering the Mega2560+WiFi board. The scoreboard and shotclocks do not use A0.

### Pair a group

Pairing stores one scoreboard and two shotclock MAC addresses in the selected group bank in the onboard ESP8266's EEPROM. Group 1 and Group 2 have separate banks. A bank is replaced only after all three devices have been found and acknowledged.

1. Set A0 to Group 1 or Group 2, then power on the Mega2560+WiFi board.
2. Hold the Mega's `HOME_FOUL` and `AWAY_FOUL` buttons together for three seconds. The controller display shows `PAIR` and the buzzer sounds.
3. Within the 15-second listening window, power-cycle the scoreboard and both shotclock boards. Each display repeatedly advertises its role and MAC across channels 1, 6, and 11 until it receives a group assignment or live controller data.
4. The bridge assigns the selected group's channel to each display. The Mega gives a short beep as each device is acknowledged.
5. When one scoreboard and two shotclocks are confirmed, the bridge saves the three addresses to EEPROM. `PAIR` clears and a longer beep signals success.

If three valid devices are not confirmed before the window expires, pairing fails and the previous saved group remains intact. Unsynchronized displays continue advertising, so you can hold the two foul buttons again to reopen pairing without rebooting them.

Repeat these steps with A0 set to the other group to create or replace that group's separate set. Use separate display sets for Group 1 and Group 2 because each display stores one operating channel.

### All mode

All mode does not use a saved MAC list. The bridge sends the broadcast MAC on channels 1, 6, and 11 so all in-range display boards can receive, regardless of which group channel they last stored. Pairing is not available while A0 selects All; select Group 1 or Group 2 to pair a saved set.

---

<a id="firmware-programming"></a>
## Firmware Programming

Use the Arduino IDE with the Mega 2560 and ESP8266 board packages installed. Select the correct serial port and processor for each upload.

1. **Scoreboard:** In `Scoreboard_FW/Scoreboard_FW.ino`, set `#define BOARD 1` or `#define BOARD 2` to match the scoreboard hardware, then upload to the scoreboard ESP8266.
2. **Shotclocks:** Upload `Shotclock_FW/Shotclock_FW.ino` to both shotclock ESP8266 boards.
3. **Onboard Wi-Fi bridge:** Set the combo board's DIP switches to the ESP8266 programming position (`00001110` per the board diagram). Select the ESP8266 board and upload `Scoreboard_Controller_WiFi_FW/Scoreboard_Controller_WiFi_FW.ino` to the onboard ESP8266.
4. **Mega controller:** Set the DIP switches to the Mega programming position (`00110000` per the board diagram). Select Arduino Mega 2560 and upload `Scoreboard_Controller_FW/Scoreboard_Controller_FW.ino` to the same combo board.
5. Return the DIP switches to the normal run position and power-cycle the system.

The onboard ESP8266 must run the Wi-Fi bridge firmware; do not erase it or leave it blank. No MAC addresses are entered by hand. The ESP8266 flash settings image is available at `docs/wemosd1mini_flashsettings.png`.

---

<a id="connections-and-dip-switches"></a>
## Connections and DIP Switches

The Mega 2560 communicates with the onboard ESP8266 through the combo board's internal Serial3 connection at 115200 baud. There are no external TX/RX wires between these two processors. Use the board's DIP switches to select the Mega or ESP8266 for programming, then restore the normal run position after flashing. Refer to `docs/dipswitch.png` and the board's silkscreen for switch orientation.

The three display ESP8266 boards are powered independently and communicate wirelessly with the onboard ESP8266 using ESP-NOW. No router, SSID, password, or Internet connection is required.

---

<a id="troubleshooting"></a>
## Troubleshooting

### Pairing finds fewer than three devices

- Confirm A0 selects Group 1 or Group 2, not All.
- Enter `PAIR`, then reboot the scoreboard and both shotclocks. Unsynchronized devices continue advertising until assigned or until they receive live controller data.
- Confirm the scoreboard firmware is installed on one board and shotclock firmware on two boards. Pairing accepts one scoreboard and two shotclocks.
- Keep the bridge and display boards nearby during pairing.

### Pairing fails or no beep is heard

- Verify both controller and onboard ESP8266 firmware are installed on the Mega2560+WiFi combo board.
- Check that the board is in normal run mode after flashing and that its internal Serial3 routing is enabled.
- The short beep indicates a device acknowledgement; the longer beep and cleared `PAIR` display indicate all three MACs were saved.

### Displays do not update

- Check the A0-selected mode and ensure that the boards are paired into the matching Group 1 or Group 2 bank, or select All to broadcast.
- Group 1 uses channel 1 and Group 2 uses channel 6. The firmware assigns and saves this channel on each display during pairing.
- Power-cycle the Mega2560+WiFi board after changing A0; the mode is read at startup.

### Firmware will not upload

- Check the DIP switch programming position, selected processor, USB port, and cable.
- Confirm the scoreboard `BOARD` setting matches the physical scoreboard wiring.

---

<a id="license"></a>
## 📜 License

This project is licensed under the MIT License. See the LICENSE file for details.

---

<a id="author"></a>
## 👨‍💻 Author

This project was created by Jezreel Tan. Feel free to contact me at jztan25@gmail.com.

