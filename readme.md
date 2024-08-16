# ESP8266 WebClient with SSD1306 Display

This project is a simple sketch that demonstrates how to create a WebClient using the ESP8266 to fetch information from an API (e.g., stock values) and display it on an SSD1306 OLED display. A button is also included to manually refresh the data, while the data is automatically refreshed every hour.

## Features

- **HTTP GET Request**: Fetch data from an API over HTTP(S) using the ESP8266.
- **SSD1306 Display**: Show the fetched data on a 128x64 OLED display using the Adafruit SSD1306 library.
- **Auto-refresh**: The data is refreshed automatically every hour.
- **Manual Refresh**: A button allows for manual refreshing of the data.

## Hardware Requirements

- ESP8266 (e.g., NodeMCU, Wemos D1 Mini)
- SSD1306 OLED Display (I2C)
- Push Button
- Resistor (10kΩ recommended for pull-up if needed)

## Libraries Required

Ensure you have the following libraries installed in your Arduino IDE:

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [ESP8266WiFi Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
- [ESP8266HTTPClient Library](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient)
- [BearSSL Library](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/BearSSL)

## Circuit Diagram

1. **ESP8266 to SSD1306 OLED**:
   - VCC -> 3.3V
   - GND -> GND
   - SCL -> D1 (GPIO5)
   - SDA -> D2 (GPIO4)

2. **ESP8266 to Button**:
   - One side of the button to GPIO0 (D3 on NodeMCU)
   - Other side of the button to GND

## Configuration

1. **Wi-Fi Credentials**: Create a `config.h` file with your Wi-Fi credentials and API URL.

```cpp
// config.h
const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";
const char* url = "your-API-URL"; // For example, the API endpoint for stock values
```

2. **Optional Adjustments**: You can adjust the interval variable in the code to change the auto-refresh frequency (default is 1 hour).

## Usage
1. Upload the Sketch: Use the Arduino IDE to upload the sketch to your ESP8266.
2. Monitor Serial Output: Open the Serial Monitor to see the connection status and data fetched from the API.
3. View Data: The fetched data will be displayed on the SSD1306 OLED display.
4. efresh Manually: Press the button connected to GPIO0 to refresh the data immediately.

## Code Overview
* Setup: Initializes the display, connects to Wi-Fi, and makes an initial HTTP request to fetch data.
* Loop: Checks the elapsed time and refreshes the data every hour. Also, listens for button presses to trigger a manual refresh.
* HTTP Request: Uses BearSSL to make secure HTTP GET requests. Displays the fetched data or error messages on the OLED display.

## Future Improvements
* Add error handling for network issues.
* Enhance the display formatting for better readability.
* Implement more robust SSL certificate handling.

## License
This project is open-source and available under the MIT License.