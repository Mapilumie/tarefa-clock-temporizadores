#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

// Define LED pins
#define RED_LED_PIN 11
#define YELLOW_LED_PIN 12
#define GREEN_LED_PIN 13

// Timer interval in milliseconds
#define TIMER_INTERVAL 3000
#define PRINT_INTERVAL 1000

// State variable for the traffic light
typedef enum {
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

TrafficLightState current_state;

// Function prototypes
void repeating_timer_callback(struct repeating_timer *t);
void print_status();

int main() {
    // Initialize the standard I/O
    stdio_init_all();

    // Initialize LED pins
    gpio_init(RED_LED_PIN);
    gpio_init(YELLOW_LED_PIN);
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_set_dir(YELLOW_LED_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    current_state = RED; // Initial state

    // Start the repeating timer
    struct repeating_timer timer;
    add_repeating_timer_ms(TIMER_INTERVAL, repeating_timer_callback, NULL, &timer);

    sleep_ms(3000); // Sleep for 3 seconds to let the timer callback run
    // Main loop
    while (true) {
        //printf("W\n");
        print_status();
        sleep_ms(PRINT_INTERVAL);
    }

    return 0;
}

// Timer callback function to change the traffic light state
void repeating_timer_callback(struct repeating_timer *t) {
    // Turn off all LEDs
    gpio_put(RED_LED_PIN, 0);
    gpio_put(YELLOW_LED_PIN, 0);
    gpio_put(GREEN_LED_PIN, 0);

    // Change state and turn on the corresponding LED
    switch (current_state) {
        case RED:
            gpio_put(RED_LED_PIN, 1);
            current_state = YELLOW; // Next state
            break;
        case YELLOW:
            gpio_put(YELLOW_LED_PIN, 1);
            current_state = GREEN; // Next state
            break;
        case GREEN:
            gpio_put(GREEN_LED_PIN, 1);
            current_state = RED; // Next state
            break;
    }
}

// Function to print the current status of the traffic light
void print_status() {
    switch (current_state) {
        case RED:
            printf("The light is green.\n");// Names do not match the colors so it is synchronized
            break;
        case YELLOW:
            printf("The light is red.\n");
            break;
        case GREEN:
            printf("The light is yellow.\n");
            break;
    }
}