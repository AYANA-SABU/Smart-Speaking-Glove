// #include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h> 


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIN_MP3_TX 2
#define PIN_MP3_RX 3
#define GSM_RX 6
#define GSM_TX 7
#define GPS_RX 4
#define GPS_TX 5
#define BUTTON_PIN 8

SoftwareSerial mp3Serial(PIN_MP3_RX, PIN_MP3_TX);
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

DFRobotDFPlayerMini player;
TinyGPSPlus gps;

const int FLEX_PIN_1 = A1;
const int FLEX_PIN_2 = A2;
const int FLEX_PIN_3 = A3;
const int FLEX_PIN_4 = A6;
const int FLEX_PIN_5 = A7;

int prevFlexValue1 = 0;
int prevFlexValue2 = 0;
int prevFlexValue3 = 0;
int prevFlexValue4 = 0;
int prevFlexValue5 = 0;

  int diffFlex1 = 0;
  int diffFlex2 = 0;
  int diffFlex3 = 0;
  int diffFlex4 = 0;
  int diffFlex5 = 0;
  
bool buttonPressed = false;
double latitude,longitude;
 
void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);
  gsmSerial.begin(9600);
  gpsSerial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Halt program if OLED display initialization fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("OK"));
  display.display();
  delay(2000);

  if (player.begin(mp3Serial)) {
    Serial.println("DFPlayer Mini initialized");
    player.volume(30);
  } else {
    Serial.println("Failed to initialize DFPlayer Mini");
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  
 
  // Process GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      if (gps.location.isValid()) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("Ready"));
        display.display();
        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
      } else {
        Serial.println("Invalid GPS data");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("No GPS data"));
        display.display();
      }
    }
  }

  // Check button state
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;
    sendSMS();
  } else if (digitalRead(BUTTON_PIN) == HIGH && buttonPressed) {
    buttonPressed = false;
  }


   // Read flex sensor value
  int flexValue1 = analogRead(FLEX_PIN_1);
  int flexValue2 = analogRead(FLEX_PIN_2);
  int flexValue3 = analogRead(FLEX_PIN_3);
  int flexValue4 = analogRead(FLEX_PIN_4);
  int flexValue5 = analogRead(FLEX_PIN_5);
  
//  Serial.print(flexValue1);
//  Serial.print("  ");
//  Serial.print(flexValue2);
//  Serial.print("  ");
//  Serial.print(flexValue3);
//  Serial.print("  ");
//  Serial.print(flexValue4);
//  Serial.print("  ");
//  Serial.println(flexValue5);

    diffFlex1 = flexValue1 - prevFlexValue1;
    diffFlex2 = flexValue2 - prevFlexValue2;
    diffFlex3 = flexValue3 - prevFlexValue3;
    diffFlex4 = flexValue4 - prevFlexValue4;
    diffFlex5 = flexValue5 - prevFlexValue5;

    Serial.println();
    Serial.print("Flex 1: ");
    Serial.print(diffFlex1);
    Serial.print("  Flex 2: ");
    Serial.print(diffFlex2);
    Serial.print("  Flex 3: ");
    Serial.print(diffFlex3);
    Serial.print("  Flex 4: ");
    Serial.print(diffFlex4);
    Serial.print("  Flex 5: ");
    Serial.println(diffFlex5);
//    display.clearDisplay();
//    display.setCursor(0, 20);
//    display.println(diffFlex1);
//    display.setCursor(20, 20);
//    display.println(diffFlex2);
//    display.setCursor(40, 20);
//    display.println(diffFlex3);
//    display.setCursor(60, 20);
//    display.println(diffFlex4);
//    display.setCursor(80, 20);
//    display.println(diffFlex5);
//      display.display();
  // Check flex sensor value and play audio accordingly
  if (diffFlex1 > 10 ) {    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("What is your name"));
    display.display();
    player.play(1);
   
    printDetail(player.readType(), player.read()); //Print the detail message from DFPlayer to handle different errors and states.
  
    delay(2500);
  }
   if (diffFlex2 > 5) {
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("I want medicine"));
    display.display();
    player.play(2);
     printDetail(player.readType(), player.read());
    delay(2500);
  }
  
     if (diffFlex3 > 5) {
      
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Hello, How are you?"));
    display.display();
    player.play(3);
     printDetail(player.readType(), player.read());
    delay(2500);
  }
  
    else if (diffFlex4 > 5) {
      
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("I want water"));
    display.display();
    player.play(4);
     printDetail(player.readType(), player.read());
    delay(2500);
  }
  
    else if (diffFlex5 > 5) {
      
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("I want Food"));
    display.display();
    player.play(5);
     printDetail(player.readType(), player.read());
    delay(2500);
  }
  prevFlexValue1 = flexValue1;
  prevFlexValue2 = flexValue2;
  prevFlexValue3 = flexValue3;
  prevFlexValue4 = flexValue4;
  prevFlexValue5 = flexValue5;

  delay(100); // Adjust delay as needed
}

void sendSMS() {
  Serial.println("Sending GPS location via SMS...");
  display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("SMS sending...."));
        display.display();

  gsmSerial.println("AT");
  delay(500);
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(500);
  gsmSerial.println("AT+CMGS=\"+917034487669\""); // Replace with recipient's number
  delay(500);
  
  // Format GPS location as Google Maps link
  gsmSerial.print("I need help ");
  gsmSerial.print("http://maps.google.com/maps?q=");
//  gsmSerial.print("Lat:");
  gsmSerial.print(latitude,6);
  gsmSerial.print(",");
  gsmSerial.print(longitude,6);
//  gsmSerial.print(googleMapsLink);
  gsmSerial.write(26); // End SMS with Ctrl+Z
    display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("SMS sent...."));
        display.display();
        
  delay(1000); // Wait for SMS to be sent
  Serial.println("GPS location sent via SMS.");
} 


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
