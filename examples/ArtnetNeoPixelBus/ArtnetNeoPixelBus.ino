/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <Artnet.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NeoPixelBus.h>

#define PIN_NEO 2
#define NEO_PIXEL_COUNT 16

NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(NEO_PIXEL_COUNT, PIN_NEO);

const char* ssid = "your-ssid";
const char* password = "your-wifi-password";

Artnet artnet;

void setColor(RgbColor color) {
  int i;
  for (i=0; i<NEO_PIXEL_COUNT; i++) {
    strip.SetPixelColor(i, color);
  }
  strip.Show();
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  Serial.println("ArtNet -------------");
  Serial.println("Universe: " + universe);
  Serial.println("Length: " + length);
  Serial.println("Sequence: " + sequence);
  
  if (universe == 1) {
    Serial.println("data for universe 1 :)");
    int i;
    for (i = 0; i < NEO_PIXEL_COUNT; i++) {
      strip.SetPixelColor(i, RgbColor(data[i*3 + 0], data[i*3 + 1], data[i*3 + 2]));
    }
    strip.Show();
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(PIN_NEO, OUTPUT);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
  artnet.begin();
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop() {
  artnet.read();
}

