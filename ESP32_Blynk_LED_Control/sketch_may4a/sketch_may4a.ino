#define BLYNK_TEMPLATE_ID "TMPL3iSzQ1B8_"
#define BLYNK_TEMPLATE_NAME "ESP32 LED"
#define BLYNK_AUTH_TOKEN "ZM1h1eRVVzPZ9BDGqaybZI8dDZjPF2MY"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "esp32";   
char pass[] = "12345678";

int ledPin = 2;

BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(ledPin, value);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to Blynk...");
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

void loop() {
  Blynk.run();
}