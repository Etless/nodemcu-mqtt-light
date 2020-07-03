<br/>

# nodemcu-mqtt-light

Arduino program to connect an NodeMCU to home assistant with the help of MQTT. 
The program is made to turn control an neopixel strip. It also accepts effects.

<br/>

### Installation:

Copy over all the files in `sketch_lightwall_V2` to your computer
and open it in arduino.

<br/>

### Usage

You have to fill in the wifi and MQTT settings in `sketch_lightwall_V2.ino`.

To connect home assistant to the arduino, you'll need home assistant to be connected to the MQTT network
and add the following in `configuration.yaml`.

```bash
light:
  - platform: mqtt
    schema: json
    name: "arduino_light"
    command_topic: "living_room/light/state" # subscription path
    state_topic: "living_room/light/status"  # subscription path with "/status" instead of "/state" at the end
    brightness: true
    rgb: true
    effect: true
    effect_list: 
      - static
      - rainbow 
      - flash
    optimistic: false
    qos: 0
```

<br/>
