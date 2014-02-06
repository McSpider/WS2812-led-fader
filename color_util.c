/******************************************************************************
 * http://www.johngineer.com
 *
 * modified by Ben K (McSpider) to accept cHSV and output cRGB
 *
 * saturation (sat) and brightness (bright) are 8-bit values.
 *
 * hue (index) is a value between 0 and 767. hue values out of range are
 * rendered as 0.
 *
 *****************************************************************************/

#include "light_ws2812.h"

// The light_ws2812 cRGB struct, incase this is used without the light_ws2812 lib.
// struct cRGB { uint8_t g; uint8_t r; uint8_t b; };
// Define a HSV struct similar to the RGB one found in light_ws2812.
struct cHSV { uint16_t h; uint8_t s; uint8_t v; };

// Convenience methods
struct cRGB ColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
  struct cRGB color;
  color.r = r; color.g = g; color.b = b;
  return color;
}
struct cHSV ColorHSV(uint16_t h, uint8_t s, uint8_t v)
{
  struct cHSV color;
  color.h = h; color.s = s; color.v = v;
  return color;
}


// Main HSV to RGB
struct cRGB hsb2rgb(struct cHSV hsv)
{
  uint16_t index = hsv.h; uint8_t sat = hsv.s; uint8_t bright = hsv.v;
	uint16_t r_temp, g_temp, b_temp;
	uint8_t index_mod;
	uint8_t inverse_sat = (sat ^ 255);
  
	index = index % 768;
	index_mod = index % 256;
  
	if (index < 256)
	{
		r_temp = index_mod ^ 255;
		g_temp = index_mod;
		b_temp = 0;
	}
  
	else if (index < 512)
	{
		r_temp = 0;
		g_temp = index_mod ^ 255;
		b_temp = index_mod;
	}
  
	else if ( index < 768)
	{
		r_temp = index_mod;
		g_temp = 0;
		b_temp = index_mod ^ 255;
	}
  
	else
	{
		r_temp = 0;
		g_temp = 0;
		b_temp = 0;
	}
  
	r_temp = ((r_temp * sat) / 255) + inverse_sat;
	g_temp = ((g_temp * sat) / 255) + inverse_sat;
	b_temp = ((b_temp * sat) / 255) + inverse_sat;
  
	r_temp = (r_temp * bright) / 255;
	g_temp = (g_temp * bright) / 255;
	b_temp = (b_temp * bright) / 255;
  
  return ColorRGB(r_temp,g_temp,b_temp);
}
