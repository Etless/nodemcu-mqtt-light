long firstPixelHue = 0;
void rainbow(uint8_t arr[LED_COUNT][3]) {
  if (firstPixelHue < 5*65536) long firstPixelHue = 0;
  for(int i=0; i<LED_COUNT; i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      hex2rgb(strip.ColorHSV(pixelHue),arr[i]);
    }
    
  firstPixelHue += 256;
}

void hex2rgb(int hex, uint8_t * rgb) {
  rgb[0] = hex >> 16 ;
  rgb[1] = (hex & 0x00ff00) >> 8;
  rgb[2] = (hex & 0x0000ff);
}
