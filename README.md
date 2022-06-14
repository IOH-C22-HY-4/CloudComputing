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
int pinSensor = 34; 
int nilaiSensor = 0;
```

## Setup Initial Condition
**Code**
```
void setup()
{
 
  Serial.begin(115200);
 
 pinMode(pinSensor, INPUT);
 
 
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

## Set database read timeout to 1 minute (max 15 minutes)
**Code**
```
Firebase.setReadTimeout(firebaseData, 1000 * 60);
//Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
Firebase.setwriteSizeLimit(firebaseData, "tiny");
```

## Set Looping Condition
**Code**
```
void loop()
{
  nilaiSensor = analogRead(pinSensor);
  int Sdata = map(nilaiSensor,0,4095,0,1000);
  Serial.println(Sdata); 
  delay(100); 
  json.set("/data", Sdata);
  Firebase.updateNode(firebaseData,"/Sensor",json);
}
```
