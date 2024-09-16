// #include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiClientSecure client;

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

const int moistureSensor01 = 36;
int readFrequency = 0;  // seconds between readings. Using deep sleep now instead
String jsonString;
String shortName = "soil04";
String longName = "Soil Moisture Sensor 04 Stue";
String groupID = "Soilsensors";

bool displayUpdate = false;

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 120         /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(200);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Failed to initialize SSD1306."));
    for (;;)
      ;  // Loop forever
  }

  // Test display
  display.clearDisplay();
  display.setTextSize(1);  // 1-8. 1=4 lines, 2=2 lines.
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Aperture");
  display.setCursor(6, 18);
  display.print("Science");
  display.display();

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
}


const long timeBetweenMoistReads = 10;  // in milliseconds. With 25 milliseconds, 200 reads = 5 seconds
const int moistReadTotal = 200;         // How many reads should be averaged.
const int moistReadShaved = 20;         // After the array is sorted. This amount of readings are removed from each end. If this
                                        // number is 20 and readCount = 200. 40 readings are removed and the remaining 160 are averaged
                                        // Requirement:  (moistReadShaved * 2) < moistReadTotal
long moistLastRead = millis();
int currentMoistCount = 0;
int moisture01[moistReadTotal];
long moistureReading;

long nextHttpPost = millis() + (readFrequency * 1000);

bool timeToSleep = false;
bool firstReading = false;

void loop() {
  if (millis() >= (moistLastRead + timeBetweenMoistReads)) {  // Non Blocking reads
    moisture01[currentMoistCount] = analogRead(moistureSensor01);
    currentMoistCount++;
    moistLastRead = millis();

    if (currentMoistCount == moistReadTotal) {

      // Sort array
      int lt_length = sizeof(moisture01) / sizeof(moisture01[0]);
      qsort(moisture01, lt_length, sizeof(moisture01[0]), sort_desc);
      moistureReading = 0;
      long moistureReading01 = 0;
      for (int j = moistReadShaved; j < (moistReadTotal - moistReadShaved); j++) {
        moistureReading01 += moisture01[j];  //add readings, remove poles
      }
      moistureReading = (moistureReading01 / (moistReadTotal - (moistReadShaved * 2)));  // average

      currentMoistCount = 0;
    }
  }

  if (firstReading) {
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

    jsonString += "\"groupID\"";
    jsonString += ":";
    jsonString += "\"";
    jsonString += groupID;
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

    // Serial.print("Waiting ");
    // Serial.print(readFrequency);
    // Serial.println("s before next reading/posting");

    nextHttpPost = millis() + (readFrequency * 1000);
    timeToSleep = true;
  }

  if (currentMoistCount == 0) {  //200 readings are just done. Print it to OLED
    display.clearDisplay();
    display.setTextSize(1);  // 1-8. 1=4 lines, 2=2 lines.
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Sensor Reading:");
    display.setCursor(6, 18);
    display.print(moistureReading);
    display.display();

    firstReading = true;
  }

  if (timeToSleep) {
    Serial.println("Going to sleep now");
    Serial.flush();
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
  }
}

int sort_desc(const void* cmp1, const void* cmp2) {
  // Need to cast the void * to int *
  int a = *((int*)cmp1);
  int b = *((int*)cmp2);
  // The comparison
  // return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  return b - a;
}
