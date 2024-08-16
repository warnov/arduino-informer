#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>  // Include BearSSL
#include "config.h"  // Include the Wi-Fi credentials AND URL

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define the I2C address for the display. 0x3C is common for many SSD1306 displays.
#define OLED_I2C_ADDRESS 0x3C

// Create the display object connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// URL for the GET request

// Variables to manage the refresh interval
unsigned long previousMillis = 0;
const long interval = 3600000; // 1 hour in milliseconds

// Button configuration
const int buttonPin = 0;  // Adjust to your button pin (e.g., GPIO 0)

// Variables for button state
int buttonState = HIGH;    // Variable for reading the button status

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);

  // Initialize the display with the I2C address
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the display buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("War Informer v1.77");
  display.display();
  delay(3000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to " + String(ssid) + "...");
    display.display();
  }
  
  Serial.println("Connected to Wi-Fi");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();

  
  // Initialize the button pin as input with pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Make the initial request
  makeHttpRequest();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to refresh the display (every hour)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    makeHttpRequest(); // Call the endpoint again to refresh the data
  }

  // Read the button state
  buttonState = digitalRead(buttonPin);

  // If the button is pressed, execute a refresh and reset the timer
  if (buttonState == LOW) { // Button is pressed (active LOW)
    makeHttpRequest();  // Call the endpoint to refresh the data
    previousMillis = millis();  // Reset the timer
  }
}

// Function to make the HTTP GET request
void makeHttpRequest() {

  // Display "Refreshing..." on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Refreshing...");
  display.display();

  delay(1000);  // Debounce delay


  // Prepare to make HTTPS GET request using BearSSL
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

  // Option 1: Skip certificate validation (not recommended for production)
  client->setInsecure();  // This will ignore the SSL certificate validation
  
  HTTPClient http;
  http.begin(*client, url);
  http.setTimeout(10000);
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    // Check for a successful response
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      
      // Process the response and format it for the OLED
      displayFormattedResponse(payload);
      
    } else {
      String errorMsg = "GET failed: " + http.errorToString(httpCode);
      Serial.println(errorMsg);
      
      // Display error on OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(errorMsg);
      display.display();
    }
  } else {
    String errorMsg = "Error: " + http.errorToString(httpCode);
    Serial.println(errorMsg);
    
    // Display error on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(errorMsg);
    display.display();
  }

  // Close the connection
  http.end();
}

// Function to format and display the response on the OLED
void displayFormattedResponse(String response) {
  display.clearDisplay();
  
  // Split the response into lines based on the '\n' character
  int firstNewLine = response.indexOf('\n');
  int secondNewLine = response.indexOf('\n', firstNewLine + 1);
  
  String firstLine = response.substring(0, firstNewLine);
  String restOfLines = response.substring(secondNewLine + 1);

  // Display the first line
  display.setCursor(0, 0);
  display.println(firstLine);

  // Leave two blank lines (adjust as necessary for your display size)
  display.setCursor(0, 20); // Adjust the y-coordinate to create two blank lines
  
  // Display the remaining lines
  display.println(restOfLines);

  // Finally, update the display
  display.display();
}
