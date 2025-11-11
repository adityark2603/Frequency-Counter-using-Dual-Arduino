/*
 * Arduino Frequency Counter with SH1106 OLED
 * ------------------------------------------
 * Measures frequency on D5 using FreqCount library.
 * Displays averaged frequency and period on OLED.
 *
 * Libraries Required:
 *  - Adafruit_GFX
 *  - Adafruit_SH1106
 *  - FreqCount
 *
 * Connections:
 *  - Signal → D5 (through 1k resistor)
 *  - OLED: VCC→5V, GND→GND, SDA→A4, SCL→A5
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <FreqCount.h>

#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

// ===== SETTINGS =====
#define GATE_TIME 1000     // Gate time in milliseconds (longer = more stable)
#define AVG_SAMPLES 5      // Number of readings for rolling average

float freqBuffer[AVG_SAMPLES];
int bufIndex = 0;

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize OLED
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); // 0x3C or 0x3D depending on your OLED
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Freq Counter Ready");
  display.display();
  delay(1500);

  // Initialize frequency counter
  FreqCount.begin(GATE_TIME);
  Serial.println("Frequency counter started...");
}

void loop() {
  // Wait for new data from FreqCount
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();  // Rising edges counted in gate time

    // Calculate frequency (Hz) based on gate time
    float freq = (float)count * (1000.0 / GATE_TIME);
    freqBuffer[bufIndex++] = freq;
    if (bufIndex >= AVG_SAMPLES) bufIndex = 0;
