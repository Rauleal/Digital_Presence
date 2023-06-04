/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266/ESP32

  Example for receiving open sound control (OSC) messages on the ESP8266/ESP32
  Send integers '0' or '1' to the address "/led" to turn on/off the built-in LED of the esp8266.

  This example code is in the public domain.

  This scripts reuses the to activate a motor using pwm

  --------------------------------------------------------------------------------------------- */
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

//char ssid[] = "iPhone van Raul";          // your network SSID (name)
//char pass[] = "wtc79wagfu2u";             // your network password

char ssid[] = "Raul_portableHotspot";          // your network SSID (name)
char pass[] = "1234567890";                    // your network password

//char ssid[] = "Hogwarts WiFi 24G";          // your network SSID (name)
//char pass[] = "68733421";                    // your network password// A UDP instance to let us send and receive packets over UDP

WiFiUDP Udp;
//const IPAddress outIp(192, 168, 43, 204);     // remote IP (not needed for receive)
//const unsigned int outPort = 8001;          // remote port (not needed for receive)
const unsigned int localPort = 9001;        // local port to listen for UDP packets (here's where we send the packets)




OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

#ifndef BUILTIN_LED
#ifdef LED_BUILTIN
#define BUILTIN_LED LED_BUILTIN
#else
//#define BUILTIN_LED 13
#endif
#endif

int MOTOR_PIN = D2; //pin connected to pwm for motor

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  digitalWrite(BUILTIN_LED, ledState);    // turn *on* led
  digitalWrite(MOTOR_PIN, ledState);

  Serial.begin(115200);



  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Configures static IP address
  
//  WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
//  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
//    Serial.println("STA Failed to configure");
//  }
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
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

}



void loop() {
  OSCMessage msg;
  int size = Udp.parsePacket();


  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      ledState = msg.getInt(0);
      //Serial.println(msg.getInt(0));

      //msg.dispatch("/1/toggle1", led);
      // figure out why dispatch does not work

    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }

  trigger_Motor(ledState);
}

void trigger_Motor(int ledState) {

  if (ledState > 255) {
    ledState = 255;
  } else if (ledState < 0) {
    ledState = 0;
  }

  analogWrite(LED_BUILTIN, ledState);
  analogWrite(MOTOR_PIN, ledState);

}
