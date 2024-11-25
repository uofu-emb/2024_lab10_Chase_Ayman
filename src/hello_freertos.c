/**
*Chase and Ayman 2024 Lab 10
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
//#include "pico/sleep.h"

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

// // 3. Run a busy loop to get 100% CPU utilization
// void blink_task_busy_loop(__unused void *params) {
//     gpio_init(OUT_PIN);            // Initialize the GPIO pin
//     gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

//     while (true) {
//         // Blink LED rapidly in a busy loop
//         gpio_put(OUT_PIN, true);
//         for (volatile int i = 0; i < 1000000; i++); // Busy delay
//         gpio_put(OUT_PIN, false);
//         for (volatile int i = 0; i < 1000000; i++); // Busy delay
//     }
// }

// 3. Run a busy loop to get 100% CPU utilization
void scenario_3_busy_loop(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    while (true) {
        gpio_put(OUT_PIN, true); // Turn on LED
        for (volatile int i = 0; i < 500000; i++); // Busy delay
        gpio_put(OUT_PIN, false); // Turn off LED
        for (volatile int i = 0; i < 500000; i++); // Busy delay
    }
}

// 4. Run a tight loop with specific operations and disable compiler optimizations
#pragma GCC optimize ("O0") // Disable compiler optimizations
void scenario_4_tight_loop(__unused void *params) {
    gpio_init(OUT_PIN);            // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    while (true) {
        gpio_put(OUT_PIN, true); // Turn on LED
        volatile uint32_t k;    // Use volatile to prevent optimization
        for (int i = 0; i < 30; i++) {
            uint32_t j = 0;
            j = ((~j >> i) + 1) * 27644437;
            k = j;
        }
        gpio_put(OUT_PIN, false); // Turn off LED
    }
}

// 5. Set up an interrupt and demonstrate energy modes
void scenario_5_interrupt_and_energy(__unused void *params) {
    gpio_init(OUT_PIN);              // Initialize the GPIO pin
    gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

    // Set up an interrupt pin for external signal
    const uint PIN_INTERRUPT = 2; // Example GPIO pin for interrupt
    gpio_set_dir(PIN_INTERRUPT, GPIO_IN);
    gpio_set_irq_enabled_with_callback(PIN_INTERRUPT, GPIO_IRQ_EDGE_RISE, true, &interrupt_handler);

    // Drive the pin with an external signal generator to test responsiveness

    while (true) {
        // Invoke wait-for-interrupt instruction
        __wfi(); // Wait for Interrupt
    }
}

// Interrupt handler for GPIO
void interrupt_handler(uint gpio, uint32_t events) {
    if (gpio == OUT_PIN) {
        gpio_put(OUT_PIN, !gpio_get(OUT_PIN)); // Toggle LED on interrupt
    }
}

// 5 (continued): Sleep and dormant demos
void scenario_5_sleep_demo(__unused void *params) {
    // Sleep demo code adapted from hello_sleep example
    // Refer to: https://github.com/raspberrypi/pico-playground/tree/master/sleep/hello_sleep/hello_sleep_alarm.c

    printf("Scenario 5: Entering sleep mode with alarm...\n");

    // Ensure sleep functions are initialized
    sleep_init();

    // Specify the duration for the alarm (in microseconds)
    uint64_t sleep_duration_us = 5000000; // 5 seconds

    while (true) {
        // Calculate the target time for waking up
        datetime_t target_time;
        rtc_get_datetime(&target_time);
        datetime_add_us(&target_time, sleep_duration_us);

        printf("Setting alarm for 5 seconds from now...\n");

        // Enter sleep mode and wake up after the alarm
        sleep_goto_sleep_until(&target_time);

        // Code resumes here after waking up
        printf("Woke up from sleep mode!\n");

        // Perform tasks or delays before going back to sleep
        sleep_ms(1000); // Example delay before re-entering sleep
    }
}

void scenario_5_dormant_demo(__unused void *params) {
    // Dormant demo code adapted from hello_dormant example
    // Refer to: https://github.com/raspberrypi/pico-playground/tree/master/sleep/hello_dormant/hello_dormant_gpio.c

    printf("Scenario 5: Entering dormant mode. Waiting for GPIO event to wake up...\n");

    const uint PIN_WAKEUP = 2; // Example GPIO pin for wakeup
    gpio_init(PIN_WAKEUP);     // Initialize GPIO pin
    gpio_set_dir(PIN_WAKEUP, GPIO_IN); // Set GPIO pin as input

    while (true) {
        // Enter dormant mode and wait for GPIO pin to wake up
        sleep_run_from_dormant(PIN_WAKEUP, true, false); // Wake on high level

        // Code resumes here after waking up from dormant mode
        printf("Woke up from dormant mode! Performing tasks...\n");

        // Perform necessary tasks after wakeup
        sleep_ms(1000); // Example delay before re-entering dormant mode
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

    // Scenario 3
    // xTaskCreate(scenario_3_busy_loop, "Scenario3_BusyLoop",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Scenario 4
    // xTaskCreate(scenario_4_tight_loop, "Scenario4_TightLoop",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Scenario 5
    xTaskCreate(scenario_5_interrupt_and_energy, "Scenario5_InterruptAndEnergy",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Scenario 5 Sleep Demo
//  xTaskCreate(scenario_5_sleep_demo, "Scenario5_SleepDemo",
//             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Scenario 5 Dormant Demo
    // xTaskCreate(scenario_5_dormant_demo, "Scenario5_DormantDemo",
    //             BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    return 0; // Should never reach here unless scheduler fails
}

// //-----Old Instructions version-----
// // 3. Run a busy loop to get 100% CPU utilization
// void blink_task_busy_loop(__unused void *params) {
//     gpio_init(OUT_PIN);            // Initialize the GPIO pin
//     gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

//     while (true) {
//         // Blink LED rapidly in a busy loop
//         gpio_put(OUT_PIN, true);
//         for (volatile int i = 0; i < 1000000; i++); // Busy delay
//         gpio_put(OUT_PIN, false);
//         for (volatile int i = 0; i < 1000000; i++); // Busy delay
//     }
// }

// // 4. Disable compiler optimizations with a tight loop
// #pragma GCC optimize ("O0") // Disable compiler optimizations for this function
// void blink_task_no_optimize(__unused void *params) {
//     gpio_init(OUT_PIN);            // Initialize the GPIO pin
//     gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

//     while (true) {
//         gpio_put(OUT_PIN, true);    // Turn on LED

//         // Tight loop for CPU usage (no delay)
//         for (int i = 0; i < 10000000; i++) {
//             // Perform some trivial but CPU-heavy operations (e.g., multiplication)
//             volatile int dummy = i * 3;  // Keeps CPU busy with computation
//         }

//         gpio_put(OUT_PIN, false);   // Turn off LED

//         // Tight loop for CPU usage (no delay)
//         for (int i = 0; i < 10000000; i++) {
//             // Another CPU-heavy operation
//             volatile int dummy = i * 2;  // Keeps CPU busy with computation
//         }
//     }
// }

// // 5. Run a tight loop to trigger high CPU activity
// void blink_task_tight_loop(__unused void *params) {
//     gpio_init(OUT_PIN);            // Initialize the GPIO pin
//     gpio_set_dir(OUT_PIN, GPIO_OUT); // Set it as an output pin

//     volatile int result = 0;
//     while (true) {
//         for (volatile int i = 1; i <= 10000; i++) {
//             result += i * i; // Perform multiply and accumulate
//         }
//         gpio_put(OUT_PIN, !gpio_get(OUT_PIN)); // Toggle LED
//     }
// }



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