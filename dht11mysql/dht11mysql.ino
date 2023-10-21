#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#include <HTTPClient.h>

const char *ssid = "TP-Link_6F4A";
const char *password = "#@#@#@#@#@";
String serverURL = "http://192.168.0.110/aurdino/dht11/index.php"; 

WebServer server(80);
DHT dht(4, DHT11);

float readDHTTemperature(); // Function prototype
float readDHTHumidity();    // Function prototype

void sendSensorData(float temperature, float humidity) {
  HTTPClient http;
  
  // Prepare the data to send
  String data = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
  
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send the POST request
  int httpResponseCode = http.POST(data);
  
  http.end();
}

void setup(void) {
  Serial.begin(115200);
  dht.begin();

  // Manually set the static IP address, gateway, and subnet mask
  IPAddress staticIP(192, 168, 0, 100); // Set your desired static IP address
  IPAddress gateway(192, 168, 0, 1);   // Set your gateway IP address
  IPAddress subnet(255, 255, 255, 0);  // Set your subnet mask

  WiFi.config(staticIP, gateway, subnet);

  // Connect to your WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(10000); // Allow the CPU to switch to other tasks

  float temperature = readDHTTemperature();
  float humidity = readDHTHumidity();

  if (temperature != -1 && humidity != -1) {
    sendSensorData(temperature, humidity);
  }
}

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
           <head>\
             <meta http-equiv=\"refresh\" content=\"5\">\
           </head>\
           <body>\
           Temperature: %.2f &deg;C<br>\
           Humidity: %.2f %%</body></html>",
           readDHTTemperature(), readDHTHumidity()
          );

  server.send(200, "text/html", msg);
}

float readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}
