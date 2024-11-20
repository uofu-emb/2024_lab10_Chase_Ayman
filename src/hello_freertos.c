/**
*Chase and Ayman 2024 Lab 10
 */

#include <stdio.h> // Standard I/O for input/output functions

#include "FreeRTOS.h" // FreeRTOS includes for multitasking
#include "task.h"

#include "pico/stdlib.h" // Pico SDK standard library for GPIO and other basic functions
#include "pico/multicore.h" // For multicore handling (not used here)
#include "pico/cyw43_arch.h" // Includes for using onboard Wi-Fi and LEDs

// GPIO pin for the external LED
#define OUT_PIN 1 // GPIO pin 1

// Variables for blinking logic
int count = 0;      // Counter to toggle LED
bool on = false;    // Current LED state (on or off)

// Task priority and stack size definitions
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL ) // Main task priority
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL ) // Blink task priority
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE      // Stack size for main task
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE     // Stack size for blink task

/**
 * Blink task
 * - Toggles the onboard LED or GPIO-connected LED every 500ms.
 * - Uses FreeRTOS delay function for timing.
 */
void blink_task(__unused void *params) {
    // Ensure the Wi-Fi and onboard LED system is initialized
    hard_assert(cyw43_arch_init() == PICO_OK);

    // Initialize the GPIO pin for the external LED
    gpio_init(OUT_PIN);            // Initialize the pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set the pin direction to output

    while (true) {
        // Toggle the onboard Wi-Fi LED
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        // Toggle the external LED
        gpio_put(OUT_PIN, on);

        // Update the LED state every 11 counts
        if (count++ % 11) on = !on;

        // Wait 500ms before the next toggle
        vTaskDelay(500);
    }
}

/**
 * Main function
 * - Initializes the standard I/O and FreeRTOS.
 * - Starts the scheduler to begin multitasking.
 */
int main(void) {
    // Initialize standard I/O for printing messages
    stdio_init_all();

    // Create the blink task directly
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    return 0; // Should never reach here unless scheduler fails
}