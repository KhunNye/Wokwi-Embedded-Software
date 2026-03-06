#include <Arduino.h>

const int ledPin = 2;
const int buttonPin = 4;

/*
  1. The "Volatile" Variables (Shared between ISR and Main)
  'volatile' tells the compiler: "Don't cache this! 
  It might change at ANY moment from an interrupt!" 
  cache: keep variable in its private pocket (CPU Register) to work faster*/
volatile bool buttonPressed = false;
volatile bool ledState = false; // Track if LED is ON or OFF

/*
  ------------------------------------------------
  2. The Interrupt Service Routine (ISR)
  IRAM_ATTR puts this function in RAM (Fast Memory) instead of Flash (Slow).
  This makes it run lightning fast.
  ------------------------------------------------ */
void IRAM_ATTR handleButtonPress() {
  // 2.1. Logic: Flip the state (True -> False -> True)
  ledState = !ledState; 
  
  // 2.2. Hardware: Write to the pin IMMEDIATELY
  // This happens in microseconds, even if Main Loop is asleep!
  digitalWrite(ledPin, ledState);

  // 2.3. Flag: Tell the main loop we did something
  buttonPressed = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  /*
  3. Attach Interrupt (the "Doorbell")
  When Pin 4 goes from HIGH to LOW (Falling edge), run 'handleButtonPress'.*/
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
  
  Serial.println("System Started. Press button to toggle LED!");
}

void loop() {

  /*
    ------------------------------------------------
    4. The "Deep Sleep" Simulation
    We are simulating a slow, heavy task.
    ------------------------------------------------ */
  Serial.println("Main Loop: I am going to sleep for 5 seconds...");
  
  // During this 5s delay, the CPU is technically "blocked" here.
  // BUT... because the LED toggle happens in the ISR (hardware interrupt),
  // the LED will still turn on/off instantly when you press the button!
  delay(5000);

  /*
    ------------------------------------------------
    5. The "Wake Up" Check
    When we wake up, we check if the interrupt happened.
    ------------------------------------------------ */
  if (buttonPressed) {
    // NOW it is safe to print!
    Serial.println("Main Loop: Woke up! Someone pressed the button while I was sleeping.");
    buttonPressed = false; // Reset flag
  }
}