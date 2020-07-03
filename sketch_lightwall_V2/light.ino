#define TransitionTicks 0.02

double s_state = 0.0;
double t_state = 0.0;
double d_state = 0.0;

// Source & Target variables
uint8_t s_mode = 0;
uint8_t t_mode = 0;

uint8_t s_array[LED_COUNT][3];
uint8_t t_array[LED_COUNT][3];

uint8_t s_brightness = 255;
uint8_t t_brightness = 255;
int d_brightness = 0;

// Set light state (On Off)
void setState(boolean state) {
  if (state == light_state) return;
  light_state = state;
  transition_state = 1;

  if (light_state) t_state = 1.0;
  else t_state = 0.0;
}

// Set static color
void staticColor(uint8_t r, uint8_t g, uint8_t b) {
  transition_color = 1;
  t_mode = 0;
  
  for (int i = 0; i < LED_COUNT; i++) {
    t_array[i][0] = r;
    t_array[i][1] = g;
    t_array[i][2] = b;
  }
}

// Set effect
void setEffect(int var) {
  transition_color = 1;
  t_mode = var;
}

// Set brightness
void setBrightness(uint8_t brightness) {
  if (transition_brightness!=0) {
    s_brightness = t_brightness;
    d_brightness = 0;
  }
  
  transition_brightness = 1;
  t_brightness = brightness;
}

// Transitions between two states
void changeStateMode() {
  if (custom_TransitionTicks != -1) transition_state-=custom_TransitionTicks;
  else transition_state-=TransitionTicks;
    
  if (transition_state<=0)
    transition_state=0;

  d_state = s_state - (s_state - t_state)*(1.0-transition_state);
  
  if (transition_state==0) {
    s_state = t_state;
    d_state = s_state;
  }
}

// Transitions between two modes
void changeColorMode() {
    if (custom_TransitionTicks != -1) transition_color-=custom_TransitionTicks;
    else transition_color-=TransitionTicks;
    
    if (transition_color<=0)
      transition_color=0;


    // Render the modes (0 is static)
    render(s_array, s_mode);
    render(t_array, t_mode);
    
    // Step difference between source and target
    for (int i = 0; i < LED_COUNT; i++) {
      uint8_t d_array[3];
      
      d_array[0] = t_array[i][0] - (t_array[i][0] - s_array[i][0])*transition_color;
      d_array[1] = t_array[i][1] - (t_array[i][1] - s_array[i][1])*transition_color;
      d_array[2] = t_array[i][2] - (t_array[i][2] - s_array[i][2])*transition_color;

      // Complete transition between source and target
      if (transition_color==0) {
        
        // If state is true copy old source variable to buffer variable
        uint8_t b_mode = 0;
        uint8_t b_array[3];
        
//        if (!state) {
//          b_mode = s_mode;
//
//          b_array[0] = s_array[i][0];
//          b_array[1] = s_array[i][1];
//          b_array[2] = s_array[i][2];
//        }

        // Set new source variable
        s_mode = t_mode;
        s_array[i][0] = t_array[i][0];
        s_array[i][1] = t_array[i][1];
        s_array[i][2] = t_array[i][2];

//        // If state is true copy buffer variable to target variable
//        if (!state) {
//          b_mode = t_mode;
//
//          t_array[i][0] = b_array[0];
//          t_array[i][1] = b_array[1];
//          t_array[i][2] = b_array[2];
//        }
      }
      // Set color to pixel on strip
      strip.setPixelColor(i, getColor(d_array));
    }

  updateColor();
}

// Uppdates effect for animation beside transitions
void updateLight() {
  render(s_array, s_mode);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, getColor(s_array[i]));
  }
  updateColor();
}

// Render effect and write it to the given array
void render(uint8_t arr[LED_COUNT][3], int var) {
  switch(var) {
    case 1:
      rainbow(arr);
      break;
    default:
      break;
  }
}

// Transitions between two brightness variables
void changeBrightnessMode() {

  if (custom_TransitionTicks != -1) transition_brightness-=custom_TransitionTicks;
  else transition_brightness-=TransitionTicks;
    
  if (transition_brightness<=0)
      transition_brightness=0;

  d_brightness = (s_brightness - t_brightness)*(1.0-transition_brightness);
  
  if (transition_brightness==0) {
    s_brightness = t_brightness;
    d_brightness = 0;
  }
}

// Set brightness and add gamma to rgb color
uint32_t getColor(uint8_t rgb[3]) {
  double bp = (double)(s_brightness - d_brightness) / 255.0; // Convert from 255-0 to 1-0 
  bp *= d_state;
  
  uint8_t r = (double)rgb[0] * bp;
  uint8_t g = (double)rgb[1] * bp;
  uint8_t b = (double)rgb[2] * bp;
  
  return strip.gamma32(strip.Color(r, g, b));
}

// Update light strip
void updateColor() {
  strip.show();
}
