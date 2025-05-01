# Scoreboard Controller System 🏀

This repository provides the code and documentation for controlling a scoreboard and shot clock system using multiple microcontrollers, including Arduino Mega 2560, NodeMCU 8266, and ESP8266-based boards. The system allows for real-time control and synchronization of the scoreboard and shot clock via wireless communication.

---

## 📑 Table of Contents

1. [📝 Overview](#overview)
2. [🛠 Hardware Setup](#hardware-setup)
3. [📶 ESP-NOW Configuration](#wi-fi-configuration)
4. [💻 Programming Method](#programming-method)
5. [🛠 Programming Steps](#programming-steps)
6. [🔌 Connections and Pinouts](#connections-and-pinouts)
7. [🕹 DIP Switch Settings](#dip-switch-settings)
8. [🆘 Troubleshooting](#troubleshooting)
9. [📜 License](#license)
10. [👨‍💻 Author](#author)

---

<a id="overview"></a>
## 📝 Overview

The project consists of multiple components that communicate with each other over ESP-NOW and serial communication:

- **Remote Controller Board:** Arduino Mega 2560 + ESP8266
- **Wi-Fi Module for Remote Controller:** NodeMCU 8266
- **Scoreboard/Shotclock:** NodeMCU 8266 for controlling the shotclock & scoreboard

### Key Notes:

- **USE LEVEL SHIFTER WHEN CONNECTING MEGA+WIFI TO NODEMCU8266.**
- The ESP8266 on the Mega+WiFi setup is **disabled** and not used due to performance issues.
- The system includes firmware for the different components:
  - **Scoreboard_Controller_FW:** Runs on the Arduino Mega 2560.
  - **Scoreboard_Controller_WiFi_FW:** Runs on the NodeMCU 8266 for the Wi-Fi module.
  - **Scoreboard_FW:** Runs on the NodeMCU 8266 for the scoreboard.
  - **Shotclock_FW:** Runs on the NodeMCU 8266 for the shotclock.

---

<a id="hardware-setup"></a>
## 🛠 Hardware Setup

### Components:

- **Arduino Mega 2560:** Acts as the main controller for the scoreboard.
- **NodeMCU 8266 x 1 (Wi-Fi Module):** Communicates with the Mega+WiFi board via UART.
- **NodeMCU 8266 x 3 (Scoreboard/Shotclock):** Controls the shot clock functionality.
- **Level Shifter:** Converts voltage between Arduino Mega (5V) and NodeMCU 8266 (3.3V) for UART communication.

### Required Components:

- Arduino Mega 2560
- NodeMCU 8266 (for Wi-Fi communication)
- NodeMCU 8266 (for Scoreboard/Shotclock control)
- Level shifter (3.3V to 5V for serial communication)

---

<a id="wi-fi-configuration"></a>
## 📶 ESP-NOW Configuration

To set up the ESP-NOW communication protocol for the system, follow these steps to configure the firmware files for the Wi-Fi modules:

1. Open the `Scoreboard_Controller_WiFi_FW.ino` file.
2. Locate the section where the ESP-NOW peer devices are defined.
3. Add the MAC addresses of the peer devices (e.g., the shotclock module and scoreboard controller) to the ESP-NOW peer list.

```cpp
uint8_t broadcast1_1[] = {0xBC, 0xDD, 0xC2, 0x13, 0x30, 0x9F};
uint8_t broadcast1_2[] = {0x2C, 0xF4, 0x32, 0x79, 0x15, 0x0F};
uint8_t broadcast1_3[] = {0x40, 0x91, 0x51, 0x48, 0x27, 0xEA};
uint8_t broadcast1_4[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
```
### 👯‍♀️ Setting Other Groups

To set the second group (optional) fill in the rest of the MAC Addresses

```cpp
uint8_t broadcast2_1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast2_2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast2_3[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast2_4[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
```

The device will have 3 modes.
- Group 1 Connect
- Group 2 Connect
- Connect to All

NOTE: This configuration is only for the Scoreboard_Controller_WiFi_FW and the ESP8266 it is programmed on. To change modes, set the voltage of A0 in the controller as follows:

- 0V: Group 1
- 1.5V: Group 2
- 3.3V: Connect to All

The rest of the other NodeMCU devices will not use A0; only this one does.

> To know the MAC Address of device, refer to [🛠 Programming Steps](#programming-steps).

---

<a id="programming-method"></a>
## 💻 Programming Method

Follow these steps to flash the correct firmware onto each board:

1. **Flash the Shotclock and Scoreboard Firmware:** Flash the NodeMCU 8266 shot clock with the Shotclock_FW and the scoreboard with the Scoreboard_FW.
2. **Clear the Mega+WiFi ESP8266 Code:** Flash the ESP8266 module with blank Arduino code.
3. **Flash the Mega+WiFi Controller Firmware:** Flash the Arduino Mega 2560 with the Scoreboard_Controller_FW firmware.
4. **Flash the Wi-Fi Module for Mega+WiFi:** Flash the NodeMCU 8266 for the Mega+WiFi board with the Scoreboard_Controller_WiFi_FW.

---

<a id="programming-steps"></a>
## 🛠 Programming Steps

### Step 1: Flash the Scoreboard

1. **For Setup Board 1:**  
   - In the code, define **Board 1** by setting `#define BOARD 1`.

2. **For Setup Board 2:**  
   - In the code, define **Board 2** by setting `#define BOARD 2`.

3. **Upload Firmware to the NodeMCU 8266:**  
   - After setting the correct board definition, upload the firmware to the corresponding board.  
   - For **Board 1**, upload the Scoreboard firmware 1 to the NodeMCU 8266 controlling the scoreboard.  
   - For **Board 2**, upload the Scoreboard firmware 2 to the NodeMCU 8266 controlling the Scoreboard.  
   - In Arduino IDE, click the **Upload** button to send the firmware to the selected board.

```cpp
/* 
 * Choose the board you are using by setting the following:
 *   - If you are using Board 1, set:  #define BOARD 1
 *   - If you are using Board 2, set:  #define BOARD 2
 * 
 * Only **one** of the options should be set at a time. 
 * Change the correct line based on your setup.
 */

#define BOARD 1
```
> Part of code to choose which fw version to upload to NODEMCU

![](/docs/macaddress.png)
> ESP8266 MAC Address

### Step 2: Flash the Shotclock

1. **Upload Shotclock Firmware:** Using Arduino IDE, upload the Shotclock_FW firmware to the NodeMCU 8266 controlling the shotclock.

### Step 3: Programming the Mega+WiFi ESP8266

1. **Set DIP Switch to 00001110:** Configures the ESP8266 on the Mega board to programming mode.
2. **Flash the ESP8266:** Using Arduino IDE, upload blank Arduino code to the ESP8266 on the Mega to clear its memory.
3. **Unplug Power:** Disconnect the power from the Mega+WiFi board.

### Step 4: Flash the Arduino Mega 2560

1. **Set DIP Switch to 00110000:** Configures the Mega board to load the scoreboard controller firmware.
2. **Upload the Firmware:** Using Arduino IDE, upload the Scoreboard_Controller_FW firmware to the Mega 2560.
3. **Unplug Power:** Disconnect the power from the Mega+WiFi board.

### Step 5: Flash the Wi-Fi Module (NodeMCU 8266 for Mega+WiFi)

1. **Configure the MAC Addresses:** Use the MAC addresses from the other modules and set them in the code before flashing.
2. **Flash the Wi-Fi Firmware:** Using Arduino IDE, upload the Scoreboard_Controller_WiFi_FW firmware to the NodeMCU 8266 Wi-Fi module.
3. **Unplug Power:** Disconnect the power after flashing the firmware.

![](/docs/wemosd1mini_flashsettings.png)
> ESP8266 Flash Settings

---

<a id="connections-and-pinouts"></a>
## 🔌 Connections and Pinouts

### Main Board (Mega+WiFi):

- Set the DIP switch to 00110000 to configure the Mega for scoreboard control.
- Set serial connection switch to RXD0 and TXD0 to ensure no collision with RXD3 and TXD3.

### Wi-Fi Module (NodeMCU 8266 for Mega+WiFi):

- **RX/TX Communication:** The Mega's Serial3 (TX/RX – PIN 14 & 15) must be connected to the RX/TX pins on the NodeMCU 8266.
  - Use a level shifter for proper voltage conversion between the 5V logic of the Mega and the 3.3V logic of the NodeMCU.
  - **Connections:** RX > TX and TX > RX.

### Shotclock Board (NodeMCU 8266):

- The shot clock board is pre-configured with its firmware (Shotclock_FW) and works independently.

---

<a id="dip-switch-settings"></a>
## 🕹 DIP Switch Settings

Ensure the following DIP switch settings are correctly configured for each step:

1. **For Clearing Mega+WiFi ESP8266:**
   - Set DIP switch to 00001110.
2. **For Flashing the Mega+WiFi 2560 with the Scoreboard Controller Firmware:**
   - Set DIP switch to 00110000.
   - Set Serial switch to RX0/TX0.

Refer to the DIP switch diagram for further details.

![](/docs/dipswitch.png)

---

<a id="troubleshooting"></a>
## 🆘 Troubleshooting

### Issue: Communication Errors

- **Solution:** Ensure the serial communication lines between the Mega 2560 and the NodeMCU are connected properly, and that level shifters are used to protect the 3.3V NodeMCU from the 5V Mega.

### Issue: Firmware Not Uploading

- **Solution:** Check the DIP switch settings and ensure that the correct board is selected in the Arduino IDE. Verify that the correct port is being used for uploading.

### Issue: No Wi-Fi Connection

- **Solution:** Double-check the Wi-Fi credentials in the Scoreboard_Controller_WiFi_FW. Ensure the NodeMCU 8266 is correctly configured.

---

<a id="license"></a>
## 📜 License

This project is licensed under the MIT License. See the LICENSE file for details.

---

<a id="author"></a>
## 👨‍💻 Author

This project was created by Jezreel Tan. Feel free to contact me at jztan25@gmail.com.

