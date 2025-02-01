#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

// Define LED and button pins
#define LED1_PIN 11  // Blue LED
#define LED2_PIN 12  // Red LED
#define LED3_PIN 13  // Green LED
#define BUTTON_PIN 5  // Pushbutton

// Timer interval in milliseconds
#define TIMER_INTERVAL 3000
#define PRINT_INTERVAL 1000

// State variable for the LED control
typedef enum {
    ALL_ON,
    TWO_ON,
    ONE_ON,
    OFF
} LEDState;

LEDState current_state = OFF;
struct repeating_timer timer;

// Function prototypes
void turn_off_callback(struct repeating_timer *t);
void button_callback(uint gpio, uint32_t events);
void print_status();

int main() {
    // Initialize the standard I/O
    stdio_init_all();

    // Initialize LED pins
    gpio_init(LED1_PIN);
    gpio_init(LED2_PIN);
    gpio_init(LED3_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_set_dir(LED3_PIN, GPIO_OUT);

    // Initialize button pin
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Enable pull-up resistor

    // Set up the button callback
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Main loop
    while (true) {
        print_status();
        sleep_ms(PRINT_INTERVAL);
    }

    return 0;
}

// Button callback function to handle button presses
void button_callback(uint gpio, uint32_t events) {
    if (current_state == OFF) {
        // Start the LED sequence
        current_state = ALL_ON;
        gpio_put(LED1_PIN, 1); // Turn on all LEDs
        gpio_put(LED2_PIN, 1);
        gpio_put(LED3_PIN, 1);

        // Start the timer for LED control
        add_repeating_timer_ms(TIMER_INTERVAL, turn_off_callback, NULL, &timer);
    }
}

// Timer callback function to change the LED state
void turn_off_callback(struct repeating_timer *t) {
    // Change the LED state based on the current state
    switch (current_state) {
        case ALL_ON:
            gpio_put(LED1_PIN, 0); // Turn off first LED
            current_state = TWO_ON; // Next state
            break;
        case TWO_ON:
            gpio_put(LED2_PIN, 0); // Turn off second LED
            current_state = ONE_ON; // Next state
            break;
        case ONE_ON:
            gpio_put(LED3_PIN, 0); // Turn off last LED
            current_state = OFF; // Next state
            break;
        case OFF:
            // Stop the timer and reset the state
            cancel_repeating_timer(&timer);
            break;
    }
}

// Function to print the current status of the LEDs
void print_status() {
    switch (current_state) {
        case ALL_ON:
            printf("Current State: ALL ON\n");
            break;
        case TWO_ON:
            printf("Current State: TWO ON\n");
            break;
        case ONE_ON:
            printf("Current State: ONE ON\n");
            break;
        case OFF:
            printf("Current State: OFF\n");
            break;
    }
}