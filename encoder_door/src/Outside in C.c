#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "OLED.h"
OLED display(4, 5);
const char* ssid = "wookkl"; // 와이파이 이름
const char* password = "03150329"; // 와이파이 비밀번호
//const char* ssid = "KPU_WiFi858"; // 와이파이 이름
//const char* password = ""; // 와이파이 비밀번호
#define mqtt_server "tailor.cloudmqtt.com" // MQTT server 주소
#define mqtt_port 16425 // port 번호
#define mqtt_topic "iot_test" // topic (자유롭게 작성}
#define mqtt_user "exwgdklw" // username
#define mqtt_password "5qPqz72yo-qP" // password
#define BUTTON 0

/////////////////
WiFiClient espClient;
PubSubClient client(espClient);
void Enter_Password();
bool flag = false;
bool flag2 = false;
bool pw_flag = false;
char num[10];
char usr_password[5];
char home_pw[5]={'1','2','3','4'};
int count=9;
int encoder0PinA = 12;
int encoder0PinB = 13;
int encoder0Pin_button = 2;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
int button = LOW;
int p;

void setup() 
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  pinMode(BUTTON,INPUT);
  display.begin();
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0Pin_button, INPUT);

}
void loop() 
{
  if (!client.connected())
  {
    client.connect("ESP8266Client2", mqtt_user, mqtt_password);
    client.subscribe(mqtt_topic);
  }  
  client.loop();
  PushBotton(); 
  if(pw_flag) Enter_Password();  
}

void callback(char* topic, byte* payload, unsigned int length)
{

  String Msg = "";
  int i=0;

  while (i<length) Msg += (char)payload[i++];

}
void PushBotton()
{
  if(digitalRead(BUTTON) == LOW)
  {
      if(!flag)
      {
          flag= true;
          pw_flag = !pw_flag;
          if(pw_flag)
          {
            display.print("EnterPassword!",0,0);
            display.print("PASSWORD:",1,0);
          }
          else
          display.clear();            
          
      }
  }
  else flag = false;
}
void Enter_Password()
{
  n = digitalRead(encoder0PinA);
  if((encoder0PinALast == LOW) && (n == HIGH))
  {
    if(digitalRead(encoder0PinB) == LOW)
    {
        encoder0Pos++;
    }
    elsef
    {
      if(encoder0Pos>0)
        encoder0Pos--;
    }
  }
  encoder0PinALast = n;
  button = digitalRead(encoder0Pin_button);

  itoa((encoder0Pos%100)/10,num,10);
  p = ((encoder0Pos%100)/10)+48;
  display.print(num,1,count);
  if(button == LOW)
  { 
    if(!flag2)
      {
          flag2= true;
          encoder0Pos = 0;
          usr_password[count-9] = p;
          count++;
          if(count>12)
          {
            usr_password[4]= '\0';
            home_pw[4]= '\0';
            count = 9;
            pw_flag = false;
           display.clear();
           if(strcmp(home_pw,usr_password)==0)
           {
            display.print("Correct!",0,0);
           client.publish(mqtt_topic,"Allow");
            delay(1000);
            display.clear();        
           }
           else
           {
            display.print("Incorrect!",0,0);
            delay(1000);
            display.clear(); 
           }
           return;
          }
      }
  }
  else flag2 = false;
}
