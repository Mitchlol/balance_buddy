/*----------------------------------------------------------------------------------------
Settings for balance_beeper
*/
#define BEEPER_PIN 27

#define PLAY_STARTUP true
#define DUTY_CYCLE_ALERT 0.80          // 0 to disable
#define SWITCH_ERPM 1000               // 0 to disable
#define LOW_VOLTAGE 40.0               // 0 to disable
#define LOW_VOLTAGE_INTERVAL 30 * 1000 // every 30 seconds

/*----------------------------------------------------------------------------------------
Settings for balance_display
*/

/* Uncomment below if using I2C, otherwise use SPI
i2c is very slow, using it will cause inconsistent beeper timings
long i2c wires are not recommended and may cause crashing */

// #define USE_I2C

#ifdef USE_I2C
#include <Wire.h>
#include "SSD1306AsciiWire.h"
// other pins: SDA=21, SCL=22
#else
#include <SPI.h>
#include "SSD1306AsciiSpi.h"
#define CS_PIN 5
#define DC_PIN 16
#define RES_PIN 17
// other pins: SCK=18, D1(MOSI)=23
#endif

#define REFRESH_INTERVAL 100 // 100ms = 0.1s = 10hz

/*----------------------------------------------------------------------------------------
Settings for balance_leds
*/
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 64

// Note: Forward in this instance means postive ERPM, which can actually be backwards if your motor direction is reversed.
#define LED_PIN_FORWARD 32
#define LED_PIN_BACKWARD 33
#define NUM_LEDS_FORWARD 1
#define NUM_LEDS_BACKWARD 1
#define STARTUP_FORWARD true

// Dims leds when switches are off, 1.0 is full brightness, 0.0 is off when switch is off
#define SWITCH_OFF_BRIGHTNESS_SCALE 1.0

// For full color list see http://fastled.io/docs/3.1/struct_c_r_g_b.html
#define COLOR_FORWARD CRGB::White
#define COLOR_BACKWARD CRGB::Red

/*----------------------------------------------------------------------------------------
Settings for esc
*/
#define ESC_CAN_ID 2
#define BALANCE_BUDDY_CAN_ID 3

#define CAN_RX_PIN 12
#define CAN_TX_PIN 13