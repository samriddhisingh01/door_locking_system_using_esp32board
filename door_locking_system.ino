#define BLYNK_TEMPLATE_ID "TMPL3W78uNfxN"
#define BLYNK_TEMPLATE_NAME "new"
#define BLYNK_AUTH_TOKEN "FPvPa68VlvBms8W9ZyXi4jVpuIOcsfbo"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>


//----------------------------------------


BlynkTimer timer;
  

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "---";
char pass[] = "----";
// Defines SS/SDA PIN and Reset PIN for RFID-RC522.
#define SS_PIN  5  
#define RST_PIN 4

// Defines the button PIN.
#define BTN_PIN 15

// Variables for the number of columns and rows on the LCD.
int lcdColumns = 20;
int lcdRows = 4;

// Variable to read data from RFID-RC522.
int readsuccess;
char str[32] = "";
String UID_Result = "--------";

// Create LiquidCrystal_I2C object as "lcd" and set the LCD I2C address to 0x27 and set the LCD configuration to 20x4.
// In general, the address of a 20x4 I2C LCD is "0x27".
// However, if the address "0x27" doesn't work, you can find out the address with "i2c_scanner". Look here : https://playground.arduino.cc/Main/I2cScanner/
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  // (lcd_address, lcd_Columns, lcd_Rows)

// Create MFRC522 object as "mfrc522" and set SS/SDA PIN and Reset PIN.
MFRC522 mfrc522(SS_PIN, RST_PIN);

//__getUID()
// Subroutine to obtain UID/ID when RFID card or RFID keychain is tapped to RFID-RC522 module.
int getUID() {  
  if(!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  
  byteArray_to_string(mfrc522.uid.uidByte, mfrc522.uid.size, str);
  UID_Result = str;
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  return 1;
}
//________________________________________________________________________________

//__byteArray_to_string()
void byteArray_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len*2] = '\0';
}
//________________________________________________________________________________

//__VOID SETUP()
void setup(){
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // put your setup code here, to run once:

  pinMode(BTN_PIN, INPUT_PULLUP);
  
  // Initialize LCD.
  lcd.init();
  // turn on LCD backlight.
  lcd.backlight();

  // Init SPI bus.
  SPI.begin();      
  // Init MFRC522.
  mfrc522.PCD_Init(); 

  delay(1000);

  lcd.clear();
    

}
//________________________________________________________________________________

//__VOID LOOP()
void loop(){
  // put your main code here, to run repeatedly:
   String a="permitted            ";
  String b="not permitted         ";
  String c="UNKNOWN";
  String d="rollno:"+UID_Result;
  lcd.setCursor(0,0);
  
  delay(1000);

  if(UID_Result=="462E4273")
  {
      Blynk.virtualWrite(V0, "PERSON 1");
      lcd.print(d);
      delay(1000);
     lcd.setCursor(0,0);
     lcd.print(a);
     
      delay(1000);
      

  }
  else if(UID_Result=="2195732E")
  {
    Blynk.virtualWrite(V0, "PERSON 2");
    lcd.print(d);
      delay(1000);
     lcd.setCursor(0,0);
     lcd.print(a);
     
     delay(1000);

  }
  else
  {
    Blynk.virtualWrite(V0, "UNKNOWN");
     lcd.setCursor(0,0);
     lcd.print(b);
     
     delay(1000);
  }
  lcd.setCursor(0,1);
  lcd.print("TAP YOUR ID !!! ");  
  
  readsuccess = getUID();

 
  if(readsuccess){
    lcd.setCursor(6, 2);
    lcd.print("              ");
    delay(500);
  }
  delay(10);
  Blynk.run();
}
//________________________________________________________________________________
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
