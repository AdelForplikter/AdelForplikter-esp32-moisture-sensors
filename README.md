# ESP32 Moisture Sensors

## Overview:
This Arduino sketch reads 3 moisture sensors. Then POST the data to a REST api with JSON. 

### Notes:
The CA-certificate is the root certificate for the api-server I'm using. This is due to validity date. 
  My Let's encrypt-certificate is valid for 3 months. The next one (E5) is valid for 3 years. 
  The ISRG Root X1 certificate is valid until 2035. The certificate is harvested with 
  Google Chrome -> Certificate Viewer -> <Top> Certificate Hierarchy -> Export. 
  Then the quotation marks and line breaks "\n" is search-replace-added afterwards

Personal credentials are saved in .credentials