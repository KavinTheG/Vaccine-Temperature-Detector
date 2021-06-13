#include <ESP8266WiFi.h>
#include "NodeMcu_ESP8266_Gmail_Sender_by_FMT.h"  

#pragma region Globals
//replace the below two lines with your information
const char* ssid = "BELL117";                      // WIFI network name
const char* password = "6737F54C9D4E";              // WIFI network password
/*--------------------------------------------------*/
uint8_t connection_state = 0;                    
uint16_t reconnect_interval = 10000;             
#pragma endregion Globals

String recipientID;
String subject;
String body;

void InputInfo(){ // function to input recipient's email ID, subject and body of the email through the serial monitor
  Serial.println("\n");
  Serial.print("Recipient's Email ID : ");
  while(Serial.available() == 0);
  recipientID = Serial.readString(); // reads from the serial monitor
  recipientID.replace("\n", "");
  Serial.println(recipientID);
  
  Serial.print("Subject              : ");
  while(Serial.available() == 0);
  subject = Serial.readString(); // reads from the serial monitor
  subject.replace("\n", "");
  Serial.println(subject);

  Serial.print("Body                 : ");
  while(Serial.available() == 0);
  body = Serial.readString(); // reads from the serial monitor
  for(int i = 0; i < body.length(); i++) {
    Serial.print((char)body.charAt(i));
    if (i % 65 == 0 && i != 0) {
      if(body.charAt(i) == ' '){
        Serial.println();
        Serial.print("                       ");
      }
      else{
        Serial.print("-\n");
        Serial.print("                       ");
      }
    }
  }
}

void CustomInfo(){ // function to set custom settings
  //replace the below three lines with your information
  recipientID = (String)"kavin.balag@gmail.com"; // enter the email ID of the recipient
  subject = (String)"this is the subject of the email"; // enter the subject of the email
  body = (String)"this is the body of the email"; // enter the body of the email
  /*--------------------------------------------------------------*/
}

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
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
    Serial.print("Connected to ");
    Serial.print(ssid);
    Serial.println(" successfully!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
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


void setup()
{
    Serial.begin(115200);
    
    
    connection_state = WiFiConnect();
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    Gsender *gsender = Gsender::Instance();
    
    InputInfo(); //inputs receiver's gmail id, subject and body of the email
    //CustomInfo(); // uncomment this line if you want to send the email using the custom information which are already set in the code; and comment the above line.
    
    if(gsender->Subject(subject)->Send(recipientID, body)) { // sends the email using a single line function
        Serial.println("Email sent"); // message confirmation
        pinMode(LED_BUILTIN, OUTPUT);
        //Led confirmation that the email has been sent; the builtin led on the board will blink if everything done successfully
          digitalWrite(LED_BUILTIN, LOW);
        
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
          delay(200);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);

          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
          delay(200);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
          delay(1000);

          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
          delay(200);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
          digitalWrite(LED_BUILTIN, LOW);
          

    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError()); // prints out the exact error if the email wasn't successfully sent
    }
}
    

void loop(){}
