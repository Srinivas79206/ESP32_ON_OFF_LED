#include <WiFi.h>

const char* ssid = "Pass";
const char* password = "12345677";

WiFiServer server(80);

String header;

// LED pin (GPIO 2 is commonly used)
const int ledPin = 19;
String ledState = "OFF";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {

            // LED ON
            if (header.indexOf("GET /on") >= 0) {
              digitalWrite(ledPin, HIGH);
              ledState = "ON";
            }

            // LED OFF
            if (header.indexOf("GET /off") >= 0) {
              digitalWrite(ledPin, LOW);
              ledState = "OFF";
            }

            // Web Page HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>");
            client.println("body { text-align: center; font-family: Arial; }");
            client.println(".btn { padding: 16px 40px; font-size: 20px; margin: 10px; cursor: pointer; }");
            client.println(".on { background-color: green; color: white; }");
            client.println(".off { background-color: red; color: white; }");
            client.println("</style></head>");

            client.println("<body>");
            client.println("<h1>ESP32 LED Control</h1>");
            client.println("<p>LED is " + ledState + "</p>");

            client.println("<a href=\"/on\"><button class=\"btn on\">ON</button></a>");
            client.println("<a href=\"/off\"><button class=\"btn off\">OFF</button></a>");

            client.println("</body></html>");
            client.println();

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}