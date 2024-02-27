#include <WiFi.h>

const char* ssid     = "google_fiber"; // Change this to your WiFi SSID
const char* password = "justdoit"; // Change this to your WiFi password

const char* ssid1     = "Adamantium"; // Change this to your WiFi SSID
const char* password1 = "usainbolt"; // Change this to your WiFi password

const char* host = "192.168.1.9"; // This should not be changed
const int httpPort = 9091; // This should not be changed

void readResponse(WiFiClient *client){
  unsigned long timeout = millis();
  while(client->available() == 0){
    if(millis() - timeout > 5000){
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client->available()) {
    String line = client->readStringUntil('\r');
    // Serial.print(line);
  }
}

int option = 0;

void connect() {
  WiFi.disconnect();

  option = 0;
  WiFi.begin(ssid, password);
  int wait = 0;
  while (WiFi.status() != WL_CONNECTED && wait < 20) {
    wait++;
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    delay(500);

    option = 1;
    wait = 0;
    WiFi.begin(ssid1, password1);
    while (WiFi.status() != WL_CONNECTED && wait < 20) {
      wait++;
      delay(500);
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    option = 2;
  }
}

char upload (float temperature, float humidity, uint16_t co2, int PM1, int PM2_5, int PM10, uint32_t tvoc) {
  if (WiFi.status() != WL_CONNECTED) {
    connect();
  }
  if (option == 2) {
    return 'D';
  }

  WiFiClient client;

  String body = "temp " + String(temperature) + "\nco2 " + String(co2) + "\nhumidity " + String(humidity) + "\nPM1 " + String(PM1) + "\nPM2_5 " + String(PM2_5) + "\nPM10 " + String(PM10) + "\ntvoc " + String(tvoc) + '\n';

  if (client.connect(host, httpPort)) {
    client.println("POST /metrics/job/airquality/instances/monitor1 HTTP/1.1");
    client.println(String("Host: ") + host);
    client.println("content-length:"+ String(body.length()));
    client.println("Connection: close");
    client.println();
    client.println(body);
    readResponse(&client);
    client.stop();
  } else {
    return 'E' ;
  }

  return option == 0 ? 'G' : 'A';
}