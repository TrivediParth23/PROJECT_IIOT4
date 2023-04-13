#include <Adafruit_MAX31865.h>
#include <ESP8266WiFi.h>
String apiKey = "M1PSCDFTTUQBWW15"; // Enter your Write API key from ThingSpeak
const char *ssid = "Shaifan"; // replace with your wifi ssid and wpa2 key
const char *pass = "9173875572";
const char* server = "api.thingspeak.com";
Adafruit_MAX31865 thermo = Adafruit_MAX31865(D0, D1 , D2, D3);//(16, 5 , 4, 0)
#define RREF      430.0
#define RNOMINAL  100.0
WiFiClient client;
void setup()
{
Serial.begin(115200);
delay(10);
thermo.begin(MAX31865_3WIRE);
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
}
void loop()
{
 uint16_t rtd=thermo.readRTD();
    Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio,8);
  Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
  Serial.print("Temperature = "); Serial.println(thermo.temperature(RNOMINAL, RREF));
if (isnan(rtd))
{
Serial.println("Failed to read from rtd sensor!");
return;
}
if (client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
{
String postStr = apiKey;
postStr +="&field1=";
postStr += String(rtd);
postStr += "\r\n\r\n";
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
Serial.print("Temperature: ");
Serial.print(rtd);
Serial.println("%. Send to Thingspeak.");
}
client.stop();
Serial.println("Waiting...");
// thingspeak needs minimum 15 sec delay between updates
delay(15000);
}
