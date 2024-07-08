#include <Arduino.h>

#define ADC_PIN A0 // Analog input pin (ADC)
#define PWM_PIN D1 // PWM output pin (GPIO5)

int previousAdcValue = 0;
const int MinFrequency = 0;
const int MaxFrequency = 20000;
const int MinimumADCFrequency= 10 ;
unsigned long previousMillis = 0;
const unsigned long interval = 10; // Interval in milliseconds
void setFanFrequency(int frequency);
void setup() {
  Serial.begin(9600); // Start serial communication at 115200 baud
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, 0); // Initialize PWM pin to off
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to read the ADC value again
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time you read the ADC value

    int adcValue = analogRead(ADC_PIN);

    // Check if ADC value differs by 32
    if (abs(adcValue - previousAdcValue) >= 32) {
      // Update frequency based on ADC value
      int frequency = map(adcValue, 0, 1023, MinFrequency, MaxFrequency);
      
      if (frequency <= MinimumADCFrequency) {
        analogWrite(PWM_PIN, 0); // Stop PWM if frequency is too low
        frequency = 0;
      } else {
        setFanFrequency(frequency); // Update PWM frequency
      }
      
      // Store current ADC value as previous for next comparison
      previousAdcValue = adcValue;
    }
  }
}

void setFanFrequency(int frequency) {
  analogWriteFreq(frequency); // Set PWM frequency
  analogWrite(PWM_PIN, 128);  // Set 50% duty cycle
  // Print the frequency to the Serial Monitor
  Serial.print("Frequency: ");
  Serial.println(frequency);
}
