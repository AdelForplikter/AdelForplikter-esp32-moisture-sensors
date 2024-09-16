# ESP32 Moisture Sensors

## Overview
This Arduino sketch reads 1 moisture sensor, then POST the data to a REST api with JSON. 
Current sketch is 2024-09-16_esp32-1sens-oled-sleep.ino

### Features
  - Support for OLED Display. Display is connected with SCK to GPIO22 (pin36) and SDA to GPIO21 (pin33). It supports 5V
  - Capasitive Soil Sensor datapin is connected to GPIO36 or pin3. It's also connected to 5V
  - Remember to edit the variables: ssid/password/server/endpoint/tokenauth/shortName/longName/groupID
  - The sketch does 200 readings. Then sort the readings, cut away top20 and bottom20 and then averages the 160 remaining. 
  - Then it converts everything to JSON and POSTs it to an API endpoint. 
  - Then it goes to sleep for 120 seconds before doing it all again. 

### Power Consumption
The microController uses about 6mA when in sleep mode and 100mA when reading and sending data. Without the OLED it uses around 3mA when sleeping.


### Notes:
The CA-certificate is the root certificate for the api-server I'm using. 
  My Let's encrypt-certificate is valid for 3 months. The next one (E5) is valid for 3 years. 
  The ISRG Root X1 certificate is valid until 2035. The certificate is harvested with 
  Google Chrome -> Certificate Viewer -> <Top> Certificate Hierarchy -> Export. 
  Then the quotation marks and line breaks "\n" is search/replace-added afterwards

Personal credentials are saved in .credentials