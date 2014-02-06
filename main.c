/*
 * WS2812 LED bar rainbow fader
 *
 * Created: 2013 Nov 21
 * Author: Ben K (github.com/McSpider)
 * Licence: MIT
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "color_util.c"
#include "light_ws2812.c"



#define led_num 8
#define led_brightness 100 // 0 - 255
#define led_saturation 255 // 0 - 255

#define fade_speed 10 // ms
#define fade_increment 1

struct cHSV led_hsv[led_num];
struct cRGB led_color[led_num];

// Define functions
void io_init(void);


int main (void)
{
  io_init();
  uint8_t i;
  
  // Setup RGB and HSV data arrays
  for (i = 0; i <= led_num - 1; i++)
  {
    // Shift hue along length of bar
    uint16_t hue = (767 / (led_num - 1)) * i;
    led_hsv[i] = ColorHSV(hue,led_saturation,led_brightness);
    
    // Convert the HSV color to RGB
    led_color[i] = hsb2rgb(led_hsv[i]);
  }
  
  // Loop till infinity (Well, till the battery runs dry anyways)
  while (1)
  {
    cli(); // Disable any interrupts
    ws2812_setleds(led_color, 8);
    sei(); // Re-enable any interrupts
    
    _delay_ms(fade_speed);
    
    for (i = 0; i <= led_num - 1; i++)
    {
      // Increment each LEDs hue (If it is greater than the max hue set it to 0)
      uint16_t new_hue = (led_hsv[i].h + fade_increment);
      if (new_hue > 767) new_hue = 0;
      led_hsv[i].h = new_hue;
      
      // Convert the HSV color to RGB
      led_color[i] = hsb2rgb(led_hsv[i]);
    }
  }
}

void io_init(void)
{
  // Set PortB pins 0 and 1 to output
  DDRB = 0b00000011;
  // Our dev board has a status LED connected to 5V on PD0, turn the LED off.
  PORTB = 0b00000001;
}

