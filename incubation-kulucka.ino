#include "Si7021.h" // sıcaklık ve nem modülü / tempature and humidity sensor si07021
Si7021 si7021;
int role=11; // bu röle ısıtıcıyı kontrol ediyor / this relay for heater
int role_nem=10; // bu röle nemlendiriciyi kontrol ediyor / this relay for humidity machine
int sayac=0; // viyolun dönmesi için zaman sayacı// this veriable for tmimer
int ekran_sayac=0; //this for lcd timer 
 

double enyukseksicak=0; // this is for the most temp
double enyukseknem=0; //this is for the most humidity
//ekran lcd
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <Servo.h>
Servo myservo;
int sbayrak=0; //servo için bir sağa bir sola dönmesini sağladığımız değişken // i use this for servo direction
String viyol_yon="ort"; //egg viyol direction log
int donme_sayisi=0; // turn log
//int i;
int degree; //for servo degree // servo dönüş açısı için
 

void setup() {
pinMode(13, OUTPUT); //BUZZER
  myservo.attach(9); //yumurta viyol servosunun bağlı olduğu pin // egg viyol servo's pin
  

//ekran //lcd
lcd.init();
lcd.backlight();

  pinMode(role,OUTPUT);
  digitalWrite(role,HIGH);
  pinMode(role_nem,OUTPUT);
  digitalWrite(role_nem,HIGH);
  uint64_t serialNumber = 0ULL;
 
  Serial.begin(9600);
 
  //setting si7021 // si7021 için ayar
  si7021.begin();
  serialNumber = si7021.getSerialNumber();
  //arduino lib doesn't natively support printing 64bit numbers on the serial port
  //so it's done in 2 times
  Serial.print("Si7021 serial number: ");
  Serial.print((uint32_t)(serialNumber >> 32), HEX);
  Serial.println((uint32_t)(serialNumber), HEX);
}

void loop(){

 delay(500);
 sayac=sayac+1;  // this veriable will incrase every 500 miliseconds. it's mean sayac=2=(1 second)

            // this is log for higest temp and humidity  // ekrana yazdığırdıgımız en yüksek sıcak ve nem için kontrol ve güncelleme
            if(si7021.getTemperatureFromPreviousHumidityMeasurement()>enyukseksicak) 
            {enyukseksicak=si7021.getTemperatureFromPreviousHumidityMeasurement();}
            if(si7021.measureHumidity()>enyukseknem) 
            {enyukseknem=si7021.measureHumidity();}
            // this is log for higest temp and humidity  // ekrana yazdığırdıgımız en yüksek sıcak ve nem için kontrol ve güncelleme

                               // you need to set max min temp and humidity //burayı kendi şartlarınıza göre ayarlamalısınız
                              if(si7021.getTemperatureFromPreviousHumidityMeasurement()<37) //min temp sıcaklık
                              {
                                digitalWrite(role, LOW);
                                 Serial.println("isitma // heating");
                            
                                 
                                 //delay(5000);
                                }
                                else if(si7021.getTemperatureFromPreviousHumidityMeasurement()>37.3) //max temp sıcaklık
                                {
                                  digitalWrite(role, HIGH);
                                  Serial.println("isitmiyor //not heating");
                                 }
  

                            if(si7021.measureHumidity()<67) //min humidify / nem
                            
                             {
                             digitalWrite(role_nem, LOW);
                              Serial.println("nemlendiriyor // humidify "); 
                          
                              
                             }
                              else  if(si7021.measureHumidity()>69)  //max humidify nem
                                   { digitalWrite(role_nem, HIGH);
                                    Serial.println("nem-durdu // not humidify");
                                   }
                            /// you need to set max min temp and humidity //burayı kendi şartlarınıza göre ayarlamalısınız



Serial.print("   Enyuksek sicaklık max temp"); 
Serial.print(enyukseksicak);
Serial.print("  En yuksek nem / Max humidity ");
Serial.println(enyukseknem);

//
timerr(); // viyol servo turn fonksiyon // servo için dönüş fonksiyonu
ekran(); //lcd print information // ekrana bilgi yazdıran fonskiyon
alarm(); //warning alarm // tehlikeli sıcak ve nem ayarı
}
//loop end


void ekran(){ //lcd print information
  lcd.setCursor(0,0);
  lcd.print("S:");
  lcd.setCursor(2,0);
  lcd.print(si7021.getTemperatureFromPreviousHumidityMeasurement());
  lcd.setCursor(8,0);
  lcd.print("N:");
  lcd.setCursor(10,0);
  lcd.print(si7021.measureHumidity());
String A = "n";
  A=A+enyukseknem;
ekran_sayac=ekran_sayac+1;

if (ekran_sayac>40) ekran_sayac=0 ;

if(ekran_sayac<30) //ekran alt satır 15 sn yede bir değişecek
{
 
  lcd.setCursor(0,1);
  lcd.print("c");
  lcd.setCursor(1,1);
  lcd.print(enyukseksicak);
   lcd.setCursor(5,1);
  
   lcd.setCursor(7,1);
  lcd.print("n");
   lcd.setCursor(8,1);
  lcd.print(enyukseknem);
  lcd.setCursor(14,1);
  lcd.print(viyol_yon);
  //lcd.setCursor(15,1);
  //lcd.print(sbayrak);
  
} 
if (ekran_sayac==30) lcd.clear();

if (ekran_sayac>30)


{lcd.setCursor(0,1);
 lcd.print(sayac);
lcd.setCursor(6,1);
 lcd.print(viyol_yon);
 lcd.setCursor(11,1);
 lcd.print(donme_sayisi);
  
  }
  
  }
void don_1() {  //this fonksiyon use in timerr() bu fonksiyon timerr içerinde döndürme için kullanılıyor
          
               for(degree=75;degree<=105;degree+=5)
               {
                        myservo.write(degree);
                        delay(1000);
                        beep(10);
                        Serial.print("servo ac");
                        Serial.println(degree);
                        viyol_yon="sol";
             }

  }

  void don_2() {  //this fonksiyon use in timerr() bu fonksiyon timerr içerinde döndürme için kullanılıyor
    
            for(degree=105;degree>=75;degree-=5)
             { 
                      myservo.write(degree);
                      delay(1000);
                      beep(10);
                      Serial.print("servo kapat");
                      Serial.println(degree);
                       viyol_yon="sag";
           }
           
   
  }

  
  void timerr() // viyol servo turn fonksiyon // servo için dönüş fonksiyonu
  {


    if(1==1) //nem durdu
    {
      
    sayac=0;
    donme_sayisi=donme_sayisi+1;
      Serial.println(sbayrak);
      Serial.println("dakika geldi");
                beep(50);
                if(sbayrak==0)
                {
                        don_1();
                        sbayrak=1;
                        beep(100);
                        
                }
                else if(sbayrak==1)
                {
                         beep(100);
                        don_2();
                        sbayrak=0;     
                }
      
    
      } 
    }

    //buzzer
void beep(unsigned char delayms) //alarm beeg // buzzer alarmı
  {
  digitalWrite(13, HIGH);      
  delay(delayms);          
  digitalWrite(13, LOW);       
  delay(delayms);          
  } 
void alarm() //warning /uyarı
{
   if(si7021.getTemperatureFromPreviousHumidityMeasurement()<35 || si7021.getTemperatureFromPreviousHumidityMeasurement()>40 || si7021.measureHumidity()>75 || si7021.measureHumidity()<35)
         {
          beep(300);
         }
  
 }
