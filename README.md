# CloudComputing
<h1 align="center">
  <img align="center" src="/img/hyponics.png"  width="230"></img>
<br>
HY.PONICS
</h1>

## Library for IoT Device
**Code**
```
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <GravityTDS.h>
#include <EEPROM.h>
```

## Define Constanta for Firebase and Wifi Authentication
**Code**
```
#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_AUTH "FIREBASE_AUTH"
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "12345678"
```

## Define FirebaseESP32 Data Object
**Code**
```
FirebaseData firebaseData;
FirebaseJson json;
GravityTDS gravityTds;
#define sensorTDS 34
#define sensorPH 35
int samples = 10;
float Aref = 3.3;
float tdsValue = 0;
float temperature = 25;
float adc_resolution = 4096.0;
float ph (float voltage) {
  return 7 + ((3.56 - voltage) / -0.25);
}
```

## Setup Initial Condition
**Code**
```
void setup()
{
  Serial.begin(115200);
  EEPROM.begin(32);
  
  pinMode(sensorTDS, INPUT);
  pinMode(sensorPH, INPUT);
  gravityTds.setPin(sensorTDS);
  gravityTds.setAref(3.3);
  gravityTds.setAdcRange(4096);
  gravityTds.begin();
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
```

## Set Database Read Timeout to 1 Minute (Max 15 Minutes)
**Code**
```
Firebase.setReadTimeout(firebaseData, 1000 * 60);
//Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
Firebase.setwriteSizeLimit(firebaseData, "tiny");
Serial.println("------------------------------------");
Serial.println("Connected..."); 
```

## Set Looping Condition
**Code**
```
void loop()
{
  readSensor();
  delay(3000);
}
```

## Sensor Reading Function
**Code**
```
  //Fungsi EC Sensor
  float ECValue = analogRead(sensorTDS) * Aref / 4096.0;
  Serial.print("EC Value: ");
  Serial.println(ECValue, 2);
  json.set("/data", ECValue);
  Firebase.updateNode(firebaseData,"/Sensor/ecSensor",json);

  //Fungsi TDS Sensor
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();
  Serial.print("TDS Value: ");
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  json.set("/data", tdsValue);
  Firebase.updateNode(firebaseData,"/Sensor/tdsSensor",json);

  //Fungsi pH Sensor
  int measurings=0;
  for(int i = 0; i<samples; i++)
  {
    measurings += analogRead(sensorPH);
    delay(10);
  }
  float voltage = 5 / adc_resolution * measurings/samples;
  Serial.print("pH Value= ");
  Serial.println(ph(voltage));
  json.set("/data", ph(voltage));
  Firebase.updateNode(firebaseData,"/Sensor/phSensor",json);
```
