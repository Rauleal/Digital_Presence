/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266/ESP32

  Example for sending messages from the ESP8266/ESP32 to a remote computer
  The example is sending "hello, osc." to the address "/test".

  This example code is in the public domain.

  --------------------------------------------------------------------------------------------- */
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

//wifi configuration

//char ssid[] = "Ziggo3781173";          // your network SSID (name)
//char pass[] = "yhct4zrGztxg";         // your network password

//char ssid[] = "Hogwarts WiFi 24G";          // your network SSID (name)
//char pass[] = "68733421";                    // your network password// A UDP instance to

char ssid[] = "Raul_portableHotspot";          // your network SSID (name)
char pass[] = "1234567890";                    // your network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192, 168, 43, 204);     // remote IP of your computer
const unsigned int outPort = 8202;          // remote port to receive OSC
const unsigned int localPort = 9202;        // local port to listen for OSC packets (actually not used for sending)

//sensor configuration
int PulseSensor_0 = A0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int Signal_0;  // holds the incoming raw data. Signal value can range from 0-1024
int BPM_0;
int Threshold_0 = 550;            // Determine which Signal to "count as a beat", and which to ingore. usually 550

PulseSensorPlayground pulseSensor_0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);           // led to indicate wifi is active
  digitalWrite(LED_BUILTIN, HIGH);        // led start off

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);     //led is on when wifi is conected
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

  // set up hearth beat sensor

  pulseSensor_0.analogInput(PulseSensor_0);
  pulseSensor_0.setThreshold(Threshold_0);
  pulseSensor_0.begin();


}

void loop() {

  // read hearth beat sensor

  Signal_0 = analogRead(PulseSensor_0);
  //Serial.println(Signal_0);


  OSCMessage msg("/hearth_beat_signal");
  msg.add(Signal_0);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
  delay(50);
}
