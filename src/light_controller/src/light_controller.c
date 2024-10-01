/**
 * @file light_controller.c
 * @brief Module to control light based on power state.
 */

#include "light_controller.h"
#include "rte.h"
#include "autoconf.h"

 /**
  * @rst
  * .. impl:: Light state
  *    :id: SWIMPL_LC-001
  *    :implements: SWDD_LC-001
  * @endrst
  *
  * @enum LightState
  * @brief Represents the states of the light.
  */
typedef enum {
    LIGHT_OFF,  /**< Represents a state where the light is turned off. */
    LIGHT_ON    /**< Represents a state where the light is turned on with a specific color. */
} LightState;

static LightState currentLightState = LIGHT_OFF;  /**< Current state of the light. */
#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
static int blinkCounter = 0;
static boolean blinkState = FALSE;
#endif
const RGBColor OFF_COLOR = { .red = 0, .green = 0, .blue = 0 };

static unsigned int getBrightnessValue() {
#if CONFIG_BRIGHTNESS_ADJUSTMENT
    /**
     * @rst
     * .. impl:: Variable brightness
     *    :id: SWIMPL_LC-005
     *    :implements: SWDD_LC-005
     * @endrst
     */
    return RteGetBrightnessValue();
#else
    return 128;
#endif
}

/**
 * @rst
 * .. impl:: Turn light off
 *    :id: SWIMPL_LC-002
 *    :implements: SWDD_LC-003
 * @endrst
 */
static void turnLightOff(void) {
#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
    blinkState = FALSE;
#endif
    RteSetLightValue(OFF_COLOR);
}

/**
 * @rst
 * .. impl:: Turn light on
 *    :id: SWIMPL_LC-003
 *    :implements: SWDD_LC-003
 * @endrst
 */
static void turnLightOn(void) {
    RGBColor color = OFF_COLOR;
#if CONFIG_COLOR_BLUE
    color.blue = getBrightnessValue();
#elif CONFIG_COLOR_GREEN
    color.green = getBrightnessValue();
#elif CONFIG_COLOR_RED
    color.red = getBrightnessValue();
#elif CONFIG_COLOR_PURPLE
    color.red = getBrightnessValue() / 2;
    color.blue = getBrightnessValue();
#endif
#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
    blinkState = TRUE;
#endif
    RteSetLightValue(color);
}

#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
/**
 * @rst
 *
 * .. impl:: Calculate blink period
 *    :id: SWIMPL_LC-004
 *    :implements: SWDD_LC-002
 * @endrst
 */
static_scope_file unsigned int calculateBlinkPeriod(percentage_t mainKnobValue) {
    // Calculate blink period based on main knob value
    unsigned int blinkPeriod = 100 - (mainKnobValue); // Adjust this formula as needed

    // Ensure there's a minimum blink period
    blinkPeriod = (blinkPeriod > 10) ? blinkPeriod : 10; // Adjust the minimum period as needed

    return blinkPeriod;
}
#endif

/**
 * @rst
 * .. impl:: Light Controller's main function
 *    :id: SWIMPL_LC-006
 *    :implements: SWDD_LC-001
 * @endrst
 * 
 * @brief Controls the light state.
 *
 * Uses a state machine to determine the light state based on several inputs,
 * e.g., the system's power state.
 */
void lightController(void) {

    PowerState powerState = RteGetPowerState();
#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
    percentage_t mainKnobValue = RteGetMainKnobValue();
    unsigned int blinkPeriod = calculateBlinkPeriod(mainKnobValue);
#endif

#if LOGGING_ENABLED
    RteLoggerPrintToConsole(LOG_LEVEL_DEBUG, "Light controller: power state = %d, main knob value = %d, blink period = %d", powerState, mainKnobValue, blinkPeriod);
#endif

    switch (currentLightState) {
    case LIGHT_OFF:
#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
        blinkCounter = 0;
#endif
        if (powerState != POWER_STATE_OFF) {
            turnLightOn();
            currentLightState = LIGHT_ON;
        }
        break;

    default: // LIGHT_ON
        if (powerState == POWER_STATE_OFF) {
            turnLightOff();
            currentLightState = LIGHT_OFF;
        }
#if CONFIG_BLINKING_RATE_AUTO_ADJUSTABLE 
        else {
            blinkCounter++;
            if (blinkCounter >= blinkPeriod) {
                // Toggle the LED state
                if (blinkState == TRUE) {
                    turnLightOff();
                }
                else {
                    turnLightOn();
                }
                blinkCounter = 0;
            }
        }
#endif
#if CONFIG_BRIGHTNESS_ADJUSTMENT
        else {
            turnLightOn();
        }
#endif
        break;
    }
}
