#include <Arduino.h>

// ==========================================
// Task A: The "Chatty Intern" (Priority 1)
// ==========================================
void TaskA(void *pvParameters) {
  // SETUP AREA: Code here runs only ONCE when the task starts
  // (Like putting on an apron)

  // LOOP AREA: The "Shift" that never ends
  for (;;) { 
    Serial.println("Hello! from Serial A (Priority 1)");
    
    // CRITICAL: You must block! 
    // This tells the Scheduler: "I am done for now, go run someone else."
    vTaskDelay(100 / portTICK_PERIOD_MS); 
  }

  // If you ever break out of the loop (which you shouldn't), 
  // you MUST delete the task explicitly or the CPU crashes.
  vTaskDelete(NULL); 
}

// ==========================================
// Task B: The "Important Manager" (Priority 2)
// ==========================================
void TaskB(void *pvParameters) {
  for (;;) {
    Serial.println(">>> System Active... (Priority 2)");
    
    // Notice: Even though this is higher priority, 
    // vTaskDelay forces it to sleep, letting Priority 1 run.
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Give a moment for Serial to start
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("--- Starting RTOS ---");

  // Create Tasks
  // 'Serial.println' uses a lot of RAM. 1000 bytes is risky and causes crashes!
  xTaskCreate(TaskA, "Task A", 2048, NULL, 1, NULL);
  xTaskCreate(TaskB, "Task B", 2048, NULL, 2, NULL);
  
  Serial.println("Setup done. handing over to Scheduler...");
}

void loop() {
  // The 'Idle' loop. 
  // In pure FreeRTOS, this wouldn't even exist.
  // In Arduino, we leave it empty.
}

