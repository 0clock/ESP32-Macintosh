#include <WiFi.h>
#include <time.h>
#include "NTPClient.h"
#include <Ds1302.h>

const char* ssid     = "404";
const char* password = "@aA123456";

int CLK = 7;
int CS = 6;
int DIN = 4; //这里定义了那三个脚
int sw1=18;//拨轮按键
int sw2=19;
int sw3=10;

int sec;
 int n=10;
#define battery 0
WiFiUDP ntpUDP; 
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60*60*8, 30*60*1000);
  
void smgdispaly() {
  if(sec!=timeClient.getSeconds()){
  int h1,h2,m1,m2,s1,s2;
  h1=timeClient.getHours()/10;
  h2=timeClient.getHours()%10;
  m1=timeClient.getMinutes()/10;
  m2=timeClient.getMinutes()%10;
  s1=timeClient.getSeconds()/10;
  s2=timeClient.getSeconds()%10;
   Write_Max7219(0x0f, 0x00);       //显示测试：1；测试结束，正常显示：0
   Write_Max7219(1,h1);
   Write_Max7219(2,h2);
   Write_Max7219(4,m1); 
   Write_Max7219(5,m2);
   Write_Max7219(7,s1);
   Write_Max7219(8,s2);
   sec=timeClient.getSeconds();
    }
  }

void batterydisplay(){
    float analog_value = 0;
     analog_value = analogRead(battery);
     delay(1000);
     Serial.printf("Current Reading on Pin(%d)=%f\n",battery,analog_value*4.2/2910);
    
    }

void switchkey(){
  if(digitalRead(sw2)==LOW){
    delay(10);
    if(digitalRead(sw2)==LOW){
      Serial.println(n);
      Write_Max7219(0x09, 0xff); 
      Write_Max7219(0x0a, n++);
       if(n>=15) n=15;
     }
    }
    if(digitalRead(sw3)==LOW){
      delay(10);
        if(digitalRead(sw3)==LOW){
          Serial.println(n);
          Write_Max7219(0x09, 0xff); 
          Write_Max7219(0x0a, n--);
          if(n<=0) n=0;          
          }
      }
}
void setup() {  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(CLK,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(DIN,OUTPUT); //让三个脚都是输出状态
  
  pinMode(battery,INPUT);
  
  pinMode(sw1,INPUT_PULLUP);
  pinMode(sw2,INPUT_PULLUP);
  pinMode(sw3,INPUT_PULLUP);
  
  Init_MAX7219();
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  sec=timeClient.getSeconds();
   Serial.println(timeClient.getFormattedTime());
}


void loop() {
  // put your main code here, to run repeatedly:
   timeClient.update();
   smgdispaly();
   switchkey();
    delay(50);
}
void Delay_xms(unsigned int x)
{
  unsigned int i,j;
  for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//--------------------------------------------
//功能：向MAX7219写入字节
//入口参数：DATA 
//出口参数：无
//说明：
void Write_Max7219_byte(unsigned char DATA)         
{
    unsigned char i;    
    digitalWrite(CS,LOW);                
    for(i=8;i>=1;i--)
    {                  
      digitalWrite(CLK,LOW);    
      if(DATA&0X80)
           digitalWrite(DIN,HIGH);
      else
           digitalWrite(DIN,LOW);
      DATA<<=1;
      digitalWrite(CLK,HIGH);
     }                                 
}
//-------------------------------------------
//功能：向MAX7219写入数据
//入口参数：address、dat
//出口参数：无
//说明：
void Write_Max7219(unsigned char address,unsigned char dat)
{ 
   digitalWrite(CS,LOW);
   Write_Max7219_byte(address);           //写入地址，即数码管编号
   Write_Max7219_byte(dat);               //写入数据，即数码管显示数字 
   digitalWrite(CS,HIGH);                        
}

void Init_MAX7219(void)
{
   Write_Max7219(0x09, 0xff);       //译码方式：BCD码
   Write_Max7219(0x0a, 15);       //亮度
   Write_Max7219(0x0b, 0x07);       //扫描界限；4个数码管显示
   Write_Max7219(0x0c, 0x01);       //掉电模式：0，普通模式：1
   Write_Max7219(1,0xa);
   Write_Max7219(2,0xa);
   Write_Max7219(3,0xa);
   Write_Max7219(4,0xa);
   Write_Max7219(5,0xa);
   Write_Max7219(6,0xa);
   Write_Max7219(7,0xa);
   Write_Max7219(8,0xa);
   delay(100);
}
