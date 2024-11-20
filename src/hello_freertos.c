/**
*Chase and Ayman 2024 Lab 10
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define OUT_PIN 1 // GPIO pin for the external LED

// Task priority and stack size definitions
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL ) // Blink task priority
#define BLINK_TASK_PRIORITY2    ( tskIDLE_PRIORITY + 5UL ) // Blink task priority
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE     // Stack size for blink task

// Global variables
int count = 0;      // Counter to toggle LED
bool on = false;    // Current LED state (on or off)

// 1. Blink an LED using sleep_ms to delay iterations
void blink_task(__unused void *params) {
    // Initialize the GPIO pin for the external LED
    gpio_init(OUT_PIN);            // Initialize the pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set the pin direction to output

    while (true) {
        gpio_put(OUT_PIN, on); // Set the LED state
        on = !on;             // Toggle the LED state
        sleep_ms(2500);        // Delay for 2500ms
    }
}

// 2. Blink an LED using FreeRTOS with multiple threads
void blink_task_thread(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    while (true) {
        gpio_put(OUT_PIN, true); // Turn on LED
        vTaskDelay(1500);         // Wait 1500ms
        gpio_put(OUT_PIN, false); // Turn off LED
        vTaskDelay(250);         // Wait 1500ms
    }
}

void blink_task_thread2(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    while (true) {
        gpio_put(OUT_PIN, true); // Turn on LED
        vTaskDelay(2000);         // Wait 2000ms
        gpio_put(OUT_PIN, false); // Turn off LED
        vTaskDelay(2000);         // Wait 2000ms
    }
}

// 3. Run a busy loop to get 100% CPU utilization
void blink_task_busy_loop(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    while (true) {
        // Blink LED rapidly in a busy loop
        gpio_put(OUT_PIN, true);
        for (volatile int i = 0; i < 1000000; i++); // Busy delay
        gpio_put(OUT_PIN, false);
        for (volatile int i = 0; i < 1000000; i++); // Busy delay
    }
}

// 4. Disable compiler optimizations with a tight loop
#pragma GCC optimize ("O0") // Disable compiler optimizations for this function
void blink_task_no_optimize(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    while (true) {
        gpio_put(OUT_PIN, true);    // Turn on LED

        // Tight loop for CPU usage (no delay)
        for (int i = 0; i < 10000000; i++) {
            // Perform some trivial but CPU-heavy operations (e.g., multiplication)
            volatile int dummy = i * 3;  // Keeps CPU busy with computation
        }

        gpio_put(OUT_PIN, false);   // Turn off LED

        // Tight loop for CPU usage (no delay)
        for (int i = 0; i < 10000000; i++) {
            // Another CPU-heavy operation
            volatile int dummy = i * 2;  // Keeps CPU busy with computation
        }
    }
}

// 5. Run a tight loop to trigger high CPU activity
void blink_task_tight_loop(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    volatile int result = 0;
    while (true) {
        for (volatile int i = 1; i <= 10000; i++) {
            result += i * i; // Perform multiply and accumulate
        }
        gpio_put(OUT_PIN, !gpio_get(OUT_PIN)); // Toggle LED
    }
}

int main(void) {
    // Initialize standard I/O for printing messages
    stdio_init_all();

    // Uncomment the desired task to run ---------

    //Scenario 1
    // xTaskCreate(blink_task, "BlinkThread",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    //Scenario 2            
    // xTaskCreate(blink_task_thread, "BlinkThread",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    // xTaskCreate(blink_task_thread2, "BlinkThread2",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY2, NULL);

    //Scenario 3   
    // xTaskCreate(blink_task_busy_loop, "BlinkThread",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    //Scenario 4
    // xTaskCreate(blink_task_no_optimize, "BlinkThread",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    //Scenario 5
    xTaskCreate(blink_task_tight_loop, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    return 0; // Should never reach here unless scheduler fails
}

// ----- Original Code to debug that was the first pushed to work_branch
// #include <stdio.h> // Standard I/O for input/output functions

// #include "FreeRTOS.h" // FreeRTOS includes for multitasking
// #include "task.h"

// #include "pico/stdlib.h" // Pico SDK standard library for GPIO and other basic functions
// #include "pico/cyw43_arch.h" // Includes for using onboard Wi-Fi and LEDs

// // GPIO pin for the external LED
// #define OUT_PIN 1 // GPIO pin 1

// // Variables for blinking logic
// int count = 0;      // Counter to toggle LED
// bool on = false;    // Current LED state (on or off)

// // Task priority and stack size definitions
// #define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL ) // Main task priority
// #define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL ) // Blink task priority
// #define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE      // Stack size for main task
// #define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE     // Stack size for blink task

// /**
//  * Blink task
//  * - Toggles the onboard LED or GPIO-connected LED every 500ms.
//  * - Uses FreeRTOS delay function for timing.
//  */
// void blink_task(__unused void *params) {
//     // Ensure the Wi-Fi and onboard LED system is initialized
//     hard_assert(cyw43_arch_init() == PICO_OK);

//     // Initialize the GPIO pin for the external LED
//     gpio_init(OUT_PIN);            // Initialize the pin
//     gpio_set_dir(OUT_PIN, GPIO_OUT); // Set the pin direction to output

//     while (true) {
//         // Toggle the onboard Wi-Fi LED
//         //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
//         // Toggle the external LED
//         gpio_put(OUT_PIN, on);

//         // Update the LED state every 11 counts
//         if (count++ % 11) on = !on;

//         // Wait 500ms before the next toggle
//         vTaskDelay(500);
//     }
// }

// /**
//  * Main function
//  * - Initializes the standard I/O and FreeRTOS.
//  * - Starts the scheduler to begin multitasking.
//  */
// int main(void) {
//     // Initialize standard I/O for printing messages
//     stdio_init_all();

//     // Create the blink task directly
//     xTaskCreate(blink_task, "BlinkThread",
//                 BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

//     // Start the FreeRTOS scheduler
//     vTaskStartScheduler();

//     return 0; // Should never reach here unless scheduler fails
// }