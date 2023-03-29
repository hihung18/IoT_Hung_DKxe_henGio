#define BLYNK_TEMPLATE_ID "TMPLwIWOOcfW"
#define BLYNK_DEVICE_NAME "IoT HungHaoHongNgocAn"
#define BLYNK_AUTH_TOKEN "IHtgva5wyu9XJbbUSfxxqiFDK6BfgaBu"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

char auth[] = "IHtgva5wyu9XJbbUSfxxqiFDK6BfgaBu";
char ssid[] = "ABCXYZwifi";
char pass[] = "11223344";

WiFiUDP u;
NTPClient n(u,"2.asia.pool.ntp.org",7*3600);//lấy giờ Châu Á, múi 7

BlynkTimer timer;
#define led D0
#define analogPin A0
#define relay D1

#define ENA  D2
#define IN1 D3
#define IN2 D4

#define IN3 D6
#define IN4 D7
#define ENB  D8


int value,analogVal,button,Ref1,Ref2,phantram;
int up, down,left,right, hethong;
int HenGio, H11, M12, H13, M14,HOUR, MINUTE;
WidgetLED appled(V0);

void setup()
{

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(led,OUTPUT);
  pinMode(relay,OUTPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  

  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
// xe
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);

  Blynk.syncVirtual(V21);
  Blynk.syncVirtual(V22);
  Blynk.syncVirtual(V23);
  Blynk.syncVirtual(V24);
  digitalWrite(relay,LOW);
//hen go
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V11);
  Blynk.syncVirtual(V12);
  Blynk.syncVirtual(V13);
  Blynk.syncVirtual(V14);

}
BLYNK_WRITE(V2) { // điều chỉnh chế độ
 value = param.asInt();
}
BLYNK_WRITE(V3) { // Nut nhan bat tat relay
 button=param.asInt();
}
BLYNK_WRITE(V4) { // Lấy giá trị ngưỡng trên
 Ref1=param.asInt();
}
BLYNK_WRITE(V5) { // Lấy giá trị ngưỡng duoi
 Ref2=param.asInt();
}
BLYNK_WRITE(V6) { // gia tri he thong
 hethong = param.asInt();
}
BLYNK_WRITE(V21) { //  Lấy giá trị Tien
 up=param.asInt();
}
BLYNK_WRITE(V22) { // Lấy giá trị RePhai
 right=param.asInt();
}
BLYNK_WRITE(V23) { // Lấy giá trị ReTrai
 left=param.asInt();
}
BLYNK_WRITE(V24) { // Lấy giá trị Lui
 down=param.asInt();
}
BLYNK_WRITE(V10) { // Cong tac hen gio
 HenGio=param.asInt();
}
BLYNK_WRITE(V11) { // Lấy hour 1
 H11=param.asInt();
}
BLYNK_WRITE(V12) { // lấy minute 1
 M12=param.asInt();
}
BLYNK_WRITE(V13) { // lấy hour 2
 H13=param.asInt();
}
BLYNK_WRITE(V14) { // lấy minute 2
 M14=param.asInt();
}
int SoSanhTgian(int h1, int m1, int h2, int m2) // chỉ so sánh trong cùng 1 ngày
{
	if (h1 > h2 ) return 1;
	else if (h1 < h2) return -1;
	else
	{	
		if (m1 > m2) return 1;
		else if (m1 < m2) return -1;
		else return 0;
	} 
}
void NgungBom(){
    appled.off();
    digitalWrite(led,LOW);
    digitalWrite(relay,LOW);
    Serial.println("Do Am Cao - Ngung Bom");
}
void BomNuoc(){
    appled.on();
    digitalWrite(led,HIGH);
    digitalWrite(relay,HIGH);
    Serial.println("Do Am Thap - Bom");
}
void TurnLeft() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  Serial.println("Xe re trai");
}
 
void TurnRight() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Xe re phai");
}
void Backward() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("Xe chay lui");
}
void Forward() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Xe chay toi");
}
void stop() {
 digitalWrite(IN1,LOW);
 digitalWrite(IN2,LOW);
 digitalWrite(IN3,LOW);
 digitalWrite(IN4,LOW);
 Serial.println("Xe dung lai!");
}
void loop() {
  Blynk.run();

  n.update(); // cập nhật thời gian
  HOUR  = n.getHours(); // giờ hiện tại
  MINUTE = n.getMinutes(); // phút hiện tại
  // hien thi gio ra serial
  Serial.println();
  Serial.print(HOUR);
  Serial.print(":");
  Serial.print(MINUTE);
  Serial.print("  status:");
  Serial.println(HenGio);

  analogVal = analogRead(analogPin); // 0 --> 1023
  phantram=map(analogVal, 0, 1023, 100, 0); // chuyen sang phantram
  // Hien thi do am sang serial
  Serial.print("Do am: ");
  Serial.print(phantram);
  Serial.println(" %");

  Blynk.virtualWrite(V1,phantram); // gửi dữ liệu lên V1 trên Blynk IoT
  if (hethong == 0) { // Dung cam bien
    if (value == 1){ // che do Auto
      if ( HenGio == 0 || (HenGio == 1 && SoSanhTgian(H11,M12,HOUR,MINUTE) <= 0 && SoSanhTgian(H13,M14,HOUR,MINUTE) >= 0))
        if (phantram > Ref1){
          NgungBom();
        } 
        else if (phantram < Ref2){
          BomNuoc();
        }
        else {
          Serial.println("Do Am Binh Thuong");
        }
    }
    else  // che do MANUAL
    {
      if (button == 1) // CongTac -> ON
      {
        if (phantram > Ref1){
          digitalWrite(led,HIGH);
          appled.on();
          delay(20);
          digitalWrite(led,LOW);
          appled.off();
          delay(20);
        }
        appled.on();
        digitalWrite(relay,HIGH);
      }
      else { // CongTac -> OFF
        digitalWrite(relay,LOW);
        appled.off();
        digitalWrite(led,LOW);
      }
    }
  }
  else { // dieu khien xe
    NgungBom();
    if (up == 1 ) {
      Forward();
    } 
    else if (down == 1) {
      Backward(); 
    } 
    else if (left == 1) {
      TurnLeft();
    } 
    else if (right == 1) {
      TurnRight();
    } 
    else {
      stop();
    }
  }
  delay(5);
}

