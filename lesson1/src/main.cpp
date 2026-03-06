#include <Arduino.h>

// Define pins
const int ledPin = 2;
const int buttonPin = 4;

// 1. Define the Tasks (The "Jobs")
// This is the "Heartbeat" task. It runs independently.
// Task A: Blinks a red LED every 500ms (The "Heartbeat").
void TaskBlink(void *pvParameters) {
  pinMode(ledPin, OUTPUT);
  for (;;) { // Infinite loop for this task
    digitalWrite(ledPin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS); // Non-blocking delay!
    digitalWrite(ledPin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// This is the "Input" task. It watches the button.
// Task B: Monitors a button. When pressed, it instantly prints a message without stopping the blinking.
void TaskButton(void *pvParameters) {
  pinMode(buttonPin, INPUT_PULLUP);
  for (;;) {
    if (digitalRead(buttonPin) == LOW) { // Button pressed (Active Low)
      Serial.println("INTERRUPT! Button Pressed!");
      // Simple debounce logic
      vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
    // Very important: yield to other tasks slightly so the CPU doesn't freeze
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

void setup() {
  Serial.begin(115200);

  /*
    (2)
    Create the Tasks
    xTaskCreate(Function, Name, Stack Size, Parameters, Priority, Handle);
  */
  xTaskCreate(TaskBlink, "Blink Task", 1000, NULL, 1, NULL);
  xTaskCreate(TaskButton,"Button Task", 1000, NULL, 1, NULL);
  
  Serial.println("System Started. Multitasking active...");
}

void loop() {
  // 3. The Empty Loop
  // In a Pro RTOS system, the loop() is often empty! 
  // The tasks above are doing all the work.
}