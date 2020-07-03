void reconnect() {
  Serial.println();
  
  //WIFI
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(2000); Serial.print("."); }
  Serial.println("\nConnected to the WiFi network");

  //MQTT
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

  Serial.print("Connecting to MQTT");
  while(!client.connected()) 
    if (!client.connect("decore_light", MQTT_USER, MQTT_PASS)) { delay(2000); Serial.print("."); }
  Serial.println("\nConnected to the MQTT network");

  //MQTT Subscriptions
  client.subscribe(SUBSCRIPTION_PATH);
}


void callback(char* t, byte* p, unsigned int length) {

  String payload = "";
  String topic   = t;
  
  for (int i = 0; i < length; i++)
    payload += (char)p[i];
  
  Serial.println("Json: " + payload);

  String short_topic = topic.substring(0, topic.lastIndexOf("/",-1));
  client.publish(String(short_topic + "/status").c_str(), payload.c_str());
  
  StaticJsonDocument<330> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // State
  if (doc.containsKey("state")) {
    String state = doc["state"];

    if (state.equals("ON")) setState(true);
    else setState(false);
    Serial.println(state);
  }

  // Brightness
  if (doc.containsKey("brightness")) {
    int brightness = doc["brightness"];

    setBrightness(brightness);
    Serial.println(brightness);
  }

  // RGB color
  if (doc.containsKey("color")) {
    JsonObject color = doc["color"];
    uint8_t r = color["r"];
    uint8_t g = color["g"];
    uint8_t b = color["b"];

    staticColor(r,g,b);
    Serial.println(String(r)+", "+String(g)+", "+String(b));
  }

  // Effect
  if (doc.containsKey("effect")) {
    String effect = doc["effect"];

    if (effect.equals("rainbow"))
      setEffect(1);
    if (effect.equals("warm white"))
      setEffect(2);
    
    Serial.println(effect);
  }

  // Transition
  custom_TransitionTicks = -1;
  if (doc.containsKey("transition")) {
    int transition = doc["transition"];

    custom_TransitionTicks = 1.0 * (double)delay_loop / (double)(transition * 1000);
    Serial.println(String(transition));
  }
}
