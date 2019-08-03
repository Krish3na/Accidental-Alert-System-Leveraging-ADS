#include <math.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "SIM900.h"
#include "sms.h"
SMSGSM sms;

const int rs = 1, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

char sms_position;
char phone_number[20];
char sms_text[100];
int i;
const int x= A0;
const int y= A1;
const int z= A2; 
int xr, yr, zr,xr1, yr1, zr1;  
double xout, yout, zout,xout1, yout1, zout1,a,b,c;

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  ss.begin(GPSBaud);
  lcd.setCursor(1,0);
  lcd.print("Accident Alert");
  lcd.setCursor(5,1);
  lcd.print("System");
    delay(2000);
  pinMode(x,INPUT);
  pinMode(y,INPUT);
  pinMode(z,INPUT);
  xr = analogRead(x); 
  yr = analogRead(y); 
  zr = analogRead(z);
  xout = ( ( ( (double)(xr * 5)/1024) - 1.65 ) / 0.330 );
  yout = ( ( ( (double)(yr * 5)/1024) - 1.65 ) / 0.330 );
  zout = ( ( ( (double)(zr * 5)/1024) - 1.80 ) / 0.330 );
Serial.println(xout);
Serial.println(yout);
Serial.println(zout);
}


void loop() {
 
    while (ss.available() > 0)
{
  gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
  xr1 = analogRead(x); 
  yr1 = analogRead(y); 
  zr1 = analogRead(z);
  xout1 = ( ( ( (double)(xr1 * 5)/1024) - 1.65 ) / 0.330 );
  yout1 = ( ( ( (double)(yr1 * 5)/1024) - 1.65 ) / 0.330 );
  zout1 = ( ( ( (double)(zr1 * 5)/1024) - 1.80 ) / 0.330 );
Serial.println(xout1);
Serial.println(yout1);
Serial.println(zout1); 
   a=xout1-xout;
   b=yout1-yout;
   c=zout1-zout;
   Serial.println(a);
   Serial.println(b);
   Serial.println(c);
   if(abs(a)>=0.5 || abs(b)>=0.5 || abs(c)>=0.5)
{Serial.println("Accident Detected");
  lcd.setCursor(1,0);
  lcd.print("Accident Detected");
  delay(3000);
  Sendsms();
}
    }
}
}

void Sendsms()
{if (gsm.begin(9600)) 
    {
        Serial.println("\nstatus=READY");
        started=true;
    } 
    else 
        Serial.println("\nstatus=IDLE");

    if(started) 
    {
        if (sms.SendSMS("+918639646769", "Accident Detected"))
        {sms.SendSMS("+918639646769", "gps.location.lat()");
         sms.SendSMS("+918639646769", "gps.location.lng()");
          Serial.println("\nSMS sent OK.");
          lcd.setCursor(0,0);
          lcd.print("Lat= "); 
          lcd.print(gps.location.lat(), 6);
          lcd.print(" Lng= "); 
          lcd.println(gps.location.lng(), 6);
        }
        else
        {
          Serial.println("\nError sending SMS.");
        }      
    }
    }
