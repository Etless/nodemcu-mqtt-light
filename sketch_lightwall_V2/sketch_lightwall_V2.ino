#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>

#include <Adafruit_NeoPixel.h>

// --- WIFI SETTINGS ---
const char* WIFI_SSID = "WIFI SSID";
const char* WIFI_PASS = "PASSWORD";

// --- MQTT SETTINGS ---
const char* MQTT_SERVER = "hassio.local";
const int   MQTT_PORT   = 0001;
const char* MQTT_USER   = "USER";
const char* MQTT_PASS   = "PASSWORD";

#define SUBSCRIPTION_PATH "example/living_room/decore/light/"

WiFiClient espClient;
PubSubClient client(espClient);

// --- LED SETTINGS ---
#define LED_PIN    5
#define LED_COUNT 20
#define LED_ARGS  NEO_RGB + NEO_KHZ800

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, LED_ARGS);

// --- VARIABLES ----
boolean light_state = false;

double transition_state = 0;
double transition_color = 0;
double transition_brightness = 0;

double custom_TransitionTicks = -1;
#define delay_loop 10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);delay(100);
  reconnect();

  strip.begin();
  strip.setBrightness(255);
  strip.show();
  
  // Set starting color and state
  staticColor(200,30,50);
  setState(false);
  custom_TransitionTicks = 1;

  

  client.publish("living_room/decore/light/status", "{\"color\":{\"r\":200,\"g\":30,\"b\":50},\"state\":\"OFF\"}");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.loop())
    reconnect();

  // Change state
  if (transition_state!=0) changeStateMode();
  
  // Change brightness
  if (transition_brightness!=0) changeBrightnessMode();
  
  // Fade between two modes
  if (transition_color!=0) changeColorMode();
  else updateLight();

  // Main delay loop
  delay(delay_loop);
}
