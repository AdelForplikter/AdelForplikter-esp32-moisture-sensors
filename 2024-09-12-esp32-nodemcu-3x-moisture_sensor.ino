#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* rootCACertificate = "-----BEGIN CERTIFICATE-----\n"
                                "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
                                "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
                                "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
                                "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
                                "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
                                "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
                                "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
                                "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
                                "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
                                "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
                                "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
                                "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
                                "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
                                "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
                                "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
                                "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
                                "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
                                "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
                                "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
                                "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
                                "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
                                "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
                                "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
                                "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
                                "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
                                "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
                                "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
                                "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
                                "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
                                "-----END CERTIFICATE-----\n";

// Semi sensitive - START
const char* ssid = "";
const char* password = "";
const char* server = "";  // Server that accepts api POST-data in json format
const char* endpoint = "/api/sensors";
const char* tokenauth = "Authorization:authtoken";  //Token authorization header
// Semi sensitive - END


// GPIO PINS
//We can not use ADC on any of the ADC2 channels while
//WiFi is on: Excluded pins:GPIO4, GPIO0, GPIO2, GPIO15,
//GPIO13, GPIO12, GPIO14, GPIO27, GPIO25 and GPIO26.
//We can use ADC1, which uses pins GPIO36, GPIO37,
//GPIO38, GPIO39, GPIO32, GPIO33, GPIO34 and GPIO35.

const int moistureSensor01 = 36;  // GPIO for Sensor 01
const int moistureSensor02 = 39;  // GPIO for Sensor 02
const int moistureSensor03 = 34;  // GPIO for Sensor 03
int readFrequency = 60;           // seconds between readings

String jsonString;
String shortName;
String longName;
int moistureReading;

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(" / ");
  Serial.println(WiFi.localIP());
  client.setCACert(rootCACertificate);
  pinMode(moistureSensor01, INPUT);
  pinMode(moistureSensor02, INPUT);
  pinMode(moistureSensor03, INPUT);
}
void loop() {
  int moistureReading01 = analogRead(moistureSensor01);
  int moistureReading02 = analogRead(moistureSensor02);
  int moistureReading03 = analogRead(moistureSensor03);
  for (int i = 0; i <= 2; i++) {
    if (i == 0) {
      shortName = "soil01";
      longName = "Soil Sensor 01 Stue";
      moistureReading = moistureReading01;
    } else if (i == 1) {
      shortName = "soil02";
      longName = "Soil Sensor 02 Stue";
      moistureReading = moistureReading02;
    } else if (i == 2) {
      shortName = "soil03";
      longName = "Soil Sensor 03 Stue";
      moistureReading = moistureReading03;
    }

    // The only required value on <server>/api/sensors is "sensorUniqueShortName". The rest is optional.
    // 3x example JSON strings. Pass one at a time to <server>/api/sensors. If you pass JSON with the same sensorUniqueShortName, the last one will overwrite the previous ones
    // {"sensorUniqueShortName":"soil01","sensorLongName":"Soil Sensor 01 Stue","wifiSignal":-50,"readTime":"2024-09-12T10:27:12.664Z","readFrequency":"60s","sensorValue":"2534","sensorMinimum":"0","sensorMaximum":"4096","deviceIP":"192.168.0.72"}
    // {"sensorUniqueShortName":"soil02","sensorLongName":"Soil Sensor 02 Stue","wifiSignal":-52,"readTime":"2024-09-12T10:27:14.680Z","readFrequency":"60s","sensorValue":"2543","sensorMinimum":"0","sensorMaximum":"4096","deviceIP":"192.168.0.72"}
    // {"sensorUniqueShortName":"soil03","sensorLongName":"Soil Sensor 03 Stue","wifiSignal":-52,"readTime":"2024-09-12T10:27:16.709Z","readFrequency":"60s","sensorValue":"2602","sensorMinimum":"0","sensorMaximum":"4096","deviceIP":"192.168.0.72"}


    // Constructing JSON
    jsonString = "";
    jsonString += "{";

    jsonString += "\"sensorUniqueShortName\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += shortName;
    jsonString += "\"";

    jsonString += ",";

    jsonString += "\"sensorValue\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += moistureReading;
    jsonString += "\"";

    jsonString += ",";

    jsonString += "\"sensorLongName\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += longName;
    jsonString += "\"";

    jsonString += ",";

    jsonString += "\"wifiSignal\"";
    jsonString += ":";
    // jsonString += "\"";
    jsonString += WiFi.RSSI();
    // jsonString += "\"";

    jsonString += ",";

    jsonString += "\"readFrequency\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += readFrequency;
    jsonString += "s";
    jsonString += "\"";

    jsonString += ",";

    jsonString += "\"sensorMinimum\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += "0";
    jsonString += "\"";

    jsonString += ",";

    jsonString += "\"sensorMaximum\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += "4096";
    jsonString += "\"";

    jsonString += ",";

    jsonString += "\"deviceIP\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += WiFi.localIP().toString().c_str();
    jsonString += "\"";

    jsonString += "}";


    Serial.print("Server:");
    Serial.print("https://");
    Serial.print(server);
    Serial.print(":443");
    Serial.println(endpoint);

    client.connect(server, 443);
    int tries = 0;
    while (!client.connected()) {
      Serial.printf("*** Can't connect. ***\n-------\n");
      delay(500);
      Serial.print(".");
      client.connect(server, 443);
      tries++;
      if (tries > 10) {
        return;
      }
    }
    Serial.printf("Connected!\n");
    Serial.print("JSON: ");
    Serial.println(jsonString);
    client.print("POST ");
    client.print(endpoint);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(server);
    client.print("\r\n");
    client.print("User-Agent: NodeMCU32\r\n");  // make your own
    client.print("Content-Length: ");
    client.print(jsonString.length());
    client.print("\r\n");
    client.print(tokenauth);
    client.print("\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Accept-encoding: *\r\n");
    client.print("\r\n");
    client.print(jsonString);
    uint32_t to = millis() + 10000;
    if (client.connected()) {
      Serial.println("Reading/Posting...");
      do {  // wait until there is data
        int avail = client.available();
        if (avail > 0) {
          break;
        }
        Serial.print(".");
        delay(500);
      } while (millis() < to);
      Serial.println();
      to = millis() + 5000;
      do {
        char tmp[512];  // we might need more
        memset(tmp, 0, 512);
        int rlen = client.read((uint8_t*)tmp, sizeof(tmp) - 1);
        if (rlen < 0) {
          break;  // the end
        }
        // Serial.print(tmp);
      } while (millis() < to);
      Serial.println("Finished reading/posting");
    }
    client.stop();
    Serial.printf("-------\n\n");
  }
  Serial.print("Waiting ");
  Serial.print(readFrequency);
  Serial.println("s before next reading/posting");
  delay(readFrequency * 1000);  // Blocking for now. If microcontroller is used for something else, make it non blocking
}