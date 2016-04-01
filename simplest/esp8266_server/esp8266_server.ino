// Import required libraries
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// Constants
const int LISTENPORT = 80;
const char* BAGHHOST = "86.119.29.77";
const int BAGHPORT = 80;

// Wifi constants
const char* ssid = "net";
const char* PASSWORD = "password";

// Wifi client and server
WiFiClient client;
WiFiServer server(LISTENPORT);
DynamicJsonBuffer jsonBuffer;

// Leds constants
const int LEDPIN = 5;
const int NLEDS = 24;
const int BRIGHTNESS = 10;

// Led index
int ledIndex = 0;
// Leds strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NLEDS, LEDPIN, NEO_GRB + NEO_KHZ800);

void debug(String message){
  if(Serial) Serial.print(message);
}

void debugln(String message){
  if(Serial)Serial.println(message);
}

void activateNextLed(){
  strip.setPixelColor(ledIndex, strip.Color(255, 0, 255));
  ledIndex = ledIndex + 1;
  strip.show();
  debugln(String("") + ledIndex);
}

JsonArray& getRequestBody(){
  String payload = "";
  bool emptyLine = true;
  
  // Skip headers
  while(client.available()){
    char c = client.read();
    if(c == '\r') continue;
    else if(c == '\n'){
      if(emptyLine) break;
      emptyLine = true;
    }
    else emptyLine = false;
  }
  
  // Capture Json payload
  while(client.available()){
    char c = client.read();
    payload += c;
  }
  debug("Payload is: ");
  debugln(payload);
  return jsonBuffer.parseArray(payload);
}

void processEvent(String event){
  Serial.println(event);
}

void handleHTTPRequest(){
  debugln("Received http request");
  JsonArray& root = getRequestBody();
  if(root.success()){
    debugln("Payload processed");
    String type = root[0]["type"];
    if(type != ""){
      debugln("Request is valid.");
      // Send response
      client.print(String("HTTP/1.1 200 OK\r\n\r\n"));
      activateNextLed();
      return;
    }
  }
  client.print("HTTP/1.1 400 Bad Request\r\n\r\n");
}

void showIP(){
  // Transmit IP address
  client.connect(BAGHHOST, BAGHPORT);
  String body = "{\"ip\": \"" + WiFi.localIP().toString() + "\"}\r\n";
  String request = String("POST ") + "/iflux/esp" + " HTTP/1.1\r\n" + 
                "Host: " + BAGHHOST + "\r\n" + 
                "Connection: close\r\n" +
                "Content-Type: Application/json\r\n" + 
                "Content-Length: " + body.length() + "\r\n" + 
                "\r\n" + 
                body;
  client.print(request);
  delay(500);
  debugln(request);
  while(client.available()){
    client.read();
  }
}

void setup(void)
{
  pinMode(5, OUTPUT);
  // Start Serial
  Serial.begin(115200);
  delay(500);
  
  // Start leds
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  
  // Connect to WiFi
  WiFi.begin(ssid, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    debug(".");
  }
  debugln("");
  debugln("WiFi connected");

  // Start the server
  server.begin();
  debugln("Server started");

  // Print the IP address
  debugln(WiFi.localIP().toString());
  // Announce IP to server
  showIP();

}


void loop() {

  // Handle REST calls
  client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  handleHTTPRequest();
  
}

