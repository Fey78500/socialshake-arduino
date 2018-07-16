#include <SPI.h>
#include <MFRC522.h>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

// Set these to run example.
#define FIREBASE_HOST "socialshake-fc175.firebaseio.com"
#define FIREBASE_AUTH "l0lIzs1qXbtEh2iTAhkqlHdZLjEvHWNk2Nzmf3p9"
 
#define RST_PIN         D3         // Configurable, see typical pin layout above
#define SS_PIN          D8        // Configurable, see typical pin layout above

unsigned long USER_ID = 2848951907;
 
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

StaticJsonBuffer<200> jsonBuffer;

//PROJECT 5 RGB LED
//int redPin   = D5;
int greenPin = D4;//D4
int bluePin  = D2;

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
//pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  // connect to wifi.
  colorRGB(255,255,0);
  delay(1500);

  WiFiManager wifiManager;
  wifiManager.autoConnect("SocialShake");
  Serial.println("connected...yeey :)");
 
  colorRGB(255,0,255);
  delay(2000);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop() {
  colorRGB(255,255,255);
  // Look for new cards
  if (! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Look if the id of the card is the same as himself
  unsigned long id = getID();
  if(id == USER_ID){
    return;
  }

  //Create a object
  JsonObject& obj = jsonBuffer.createObject();
  obj["from"] = String(USER_ID);
  obj["to"] = String(id);
  obj["matchFrom"] = "";
  obj["matchTo"] = "";

  //Push to firebase
  Firebase.push("invitations", obj);
  // handle error
  if (Firebase.failed()) {
    Serial.print("pushing /invitations failed:");
    Serial.println(Firebase.error());  
    return;
  }else{
    colorRGB(255,0,255);
  }
  
  Serial.println(id);

  delay(150);
  //digitalWrite(greenPin,LOW);
}

unsigned long getID(){
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}
// 0 = On, 255 = off
void colorRGB(int red, int green, int blue){
  //analogWrite(redPin,constrain(red,0,255));
  digitalWrite(greenPin,constrain(green,0,255));
  digitalWrite(bluePin,constrain(blue,0,255));
}

/*
int n = 0;

void loop() {
  // set value
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}*/
