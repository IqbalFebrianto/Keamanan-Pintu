//BUZZER
int BUZZER_PIN = 11 ;

//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//SENSOR SUHU
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float DataSuhu;

//SENSOR ULTRASONIK
#define trigPin 3 
#define echoPin 4 
long duration;
int distance;

//DOOR LOCK
const int Relay = 2;

void setup() {
  Serial.begin(9600);

  //BUZZER
  pinMode(BUZZER_PIN, OUTPUT);

  //LCD
  lcd.init();
  lcd.backlight();
  
  //SENSOR SUHU
  mlx.begin();

  //SENSOR ULTRASONIK
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //DOOR LOCK
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
}

void loop() {
  //SET BUZZER TIDAK BERSUARA
  noTone(BUZZER_PIN);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Silahkan Tap");
  lcd.setCursor(0,1);
  lcd.print("KTP Anda !");

  String SerialNodeMCU = "";
  while(Serial.available()>0)
  {
    SerialNodeMCU += char(Serial.read());
  }
  //Serial.println(SerialNodeMCU);

  int kondisi1 = SerialNodeMCU.indexOf("1");
  int kondisi2 = SerialNodeMCU.indexOf("2");
  int kondisi3 = SerialNodeMCU.indexOf("3");
  int kondisi4 = SerialNodeMCU.indexOf("4");
  int kondisi5 = SerialNodeMCU.indexOf("5");
  
  /*
  Serial.println(kondisi1);
  Serial.println(kondisi2);
  Serial.println(kondisi3);
  Serial.println(kondisi4);
  Serial.println(kondisi5);
  */
  
  //KONDISI JIKA ADA PERINTAH
  if(kondisi1==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Proses cek kartu...");
    //Serial.println("Proses cek kartu...");
    
  } else if(kondisi2==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connection Fail");
    //Serial.println("Connection Failed");
    
  } else if(kondisi3==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Client Timeout");
    //Serial.println("Client TimeOut");
    
  } else if(kondisi4==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("KTP Tidak");
    lcd.setCursor(0,1);
    lcd.print("Terdaftar");
    //Serial.println("KTP Tidak Terdaftar");
    //ALARM
    tone(BUZZER_PIN, 1000);
    delay(1000);
    tone(BUZZER_PIN, 2000);
    delay(1000);
    noTone(BUZZER_PIN);
    delay(2000);
    
  } else if(kondisi5==0){
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("KTP");
    lcd.setCursor(0,1);
    lcd.print("Terdaftar");

    delay(2000);
    
    //TAMPILKAN PERINTAH CEK SUHU KE LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Silhkan Cek Suhu");
    lcd.setCursor(0,1);
    lcd.print("Tubuh Anda !");
  
    delay(4000);
  
    //SENSOR ULTRASONIK
    digitalWrite(trigPin, LOW); //CLEAR TRIGPIN KONDISI
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); //Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW); // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; // KALKULASI JARAK KE CM Speed of sound wave divided by 2 (go and back)
    //Serial.println(distance);
  
    if(distance < 10)
    {
      //BACA DATA SUHU
      DataSuhu = mlx.readObjectTempC();
    
      //nyalakan buzzer BEEP
      tone(BUZZER_PIN, 5000);
      delay(200); 
      noTone(BUZZER_PIN);
      
      //KIRIM DATA SUHU KE SERIAL MONITOR
      //Serial.println("Suhu Anda : ");
      Serial.print(DataSuhu);
      //Serial.print("C");
    
      //TAMPILKAN DATA SUHU KE LCD
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Suhu Anda");
      lcd.setCursor(10,0);
      lcd.print(DataSuhu);
      lcd.setCursor(15,0);
      lcd.print("C");    
      lcd.setCursor(0,1);
      lcd.print("Silahkan Masuk !");
  
      delay(500);
      
      if(DataSuhu > 37){
        //Serial.print("Door Closed");
        //TAMPILKAN WARNING KE LCD
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Warning! Suhu");
        lcd.setCursor(0,1);
        lcd.print("Anda > 37C");
        //ALARM
        tone(BUZZER_PIN, 1000);
        delay(1000);
        tone(BUZZER_PIN, 2000);
        delay(1000);
        tone(BUZZER_PIN, 1000);
        delay(1000);
        tone(BUZZER_PIN, 2000);
        delay(1000);
        //TAMPILKAN PESAN KE LCD
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Silhkan Istirahat");
        lcd.setCursor(0,1);
        lcd.print("Jaga Kesehatan !");
        delay(2000);
      }else{
        digitalWrite(Relay, LOW);
        //Serial.print("Door Opened");
        delay(5000);
        digitalWrite(Relay, HIGH);
      }
   
  
    }

  } //END SerialNodeMCU
  
  delay(2000);

}
