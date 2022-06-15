#include <WiFi.h>
#include <FirebaseESP32.h>
#include <GravityTDS.h>
#include <EEPROM.h>

 
#define FIREBASE_HOST "https://hy-ponics-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "4uf7trG1VaWpdByprGSIxSOCpWK9KWM4xJhLp9E1"
#define WIFI_SSID "KUDA PUTIH"
#define WIFI_PASSWORD "2566625666"

 
//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

#define sensorTDS 34
float temperature = 25;
float tdsValue = 0;
float ec = 0;
float Aref = 3.3;
GravityTDS gravityTds; 

#define sensorPH 35
int samples = 10;
float adc_resolution = 4096.0;
float ph (float voltage) {
  return 7 + ((3.56 - voltage) / -0.25);
}

 
void setup()
{
  Serial.begin(115200);
  EEPROM.begin(32);
  
  pinMode(sensorTDS, INPUT);
  gravityTds.setPin(sensorTDS);
  gravityTds.setAref(3.3);
  gravityTds.setAdcRange(4096);
  gravityTds.begin();

  pinMode(sensorPH, INPUT);
  
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
 
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  
  Serial.println("------------------------------------");
  Serial.println("Connected..."); 
}

void loop()
{
  readSensor();
  delay(3000);
}

void readSensor()
{
  //Fungsi EC Sensor
  float ECValue = analogRead(sensorTDS) * Aref / 4096.0;
  Serial.print("EC Value: ");
  Serial.println(ECValue, 2);
  json.set("/data", ECValue);
  Firebase.updateNode(firebaseData,"/Sensor/tdsSensor",json);

  //Fungsi TDS Sensor
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();
  Serial.print("TDS Value: ");
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  json.set("/data", tdsValue);
  Firebase.updateNode(firebaseData,"/Sensor/ecSensor",json);

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
}
