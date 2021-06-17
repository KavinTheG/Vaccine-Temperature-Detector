#include <ESP8266WiFi.h>
#include "Gsender.h"
#include <NTPClient.h>
#include <DHT.h>
#include <WiFiUdp.h>


// UTC Offset, for canada eastern time its -5:00
const long countryTimeZoneOffset = -5.00;
const long utcOffsetInSeconds = countryTimeZoneOffset * 60 * 60;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// The pin used for the dht sensor 
#define dhtPin 5

// Initializes a new DHT variable called dht
// The sensor used is the DHT22 model
DHT dht(dhtPin, DHT22); 

// Store the sensor values in the variables blow
float humidity, temp;

#pragma region Globals
const char* ssid = "Jaffna";                           // WIFI network name
const char* password = "Apple2013";                       // WIFI network password
uint8_t connection_state = 0;                    // Connected to WIFI or not
uint16_t reconnect_interval = 10000;             // If not connected wait time to try again
#pragma endregion Globals
boolean sent = false;

String subject, body;
/*uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);  
        Serial.println(nSSID);
    } else {
        WiFi.begin(ssid, password);
        Serial.println(ssid);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    timeClient.begin();
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}
*/


void gmailSender(String subject, String message) {
    Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    if(gsender->Subject(subject)->Send("testingbottosendmail@gmail.com", message)) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
}


void setup()
{
  Serial.begin(115200);
  /*connection_state = WiFiConnect();
  if(!connection_state)  // if not connected to WIFI
      Awaits();          // constantly trying to connect
  */
  dht.begin();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
}

void loop(){
  timeClient.update();
  humidity = dht.readHumidity();

  // Read the temp values
  temp = dht.readTemperature();
  subject = "Temperature & Humidity Data";
  body = "Data readings from esp8266 #1";
  body += "<br>Temp: ";
  body += temp;
  body += "<br>Humidity: ";
  body += humidity;


  //Serial.println(timeClient.getMinutes());
  if (timeClient.getMinutes() % 3 == 0 && !sent){
    gmailSender(subject, body);
    sent = true;
  }
  if (timeClient.getMinutes() % 3 != 0) {
    sent = false;
  }
  
  
}
