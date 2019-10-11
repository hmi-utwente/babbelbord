# Introduction
This repository contains the code to run the Babbelbord.

# Babbelbord web-app
The code for running the web-app can be found [here](https://github.com/hmi-utwente/babbelbord-app)

# Contents
* Arduino code for positioning of the pawns
* Processing code for connecting the arduino to the web-server

# Requirements
* A Babbelbord with an Adafruit ESP32 Feather (or another board with a WiFi module)
* Working Web-app for Babbelbord.
* Wi-Fi with simple SSID/PASSWORD login

# Arduino Feather
* Add this in Arduino for your Boards Manager (Add it via File -> Preferences -> `Additional Board Managers URLs`): https://dl.espressif.com/dl/package_esp32_index.json
* Make sure to install it via the Boards manager (Tools -> Boards Manager..)

# Running
* To connect the board to Wi-Fi, you need to set the SSID and PASSWORD in the `arduino_secrets.h` file. You can create this file by copying the `arduino_secrets_global.h` file and renaming it to `arduino_secrets.h`.
