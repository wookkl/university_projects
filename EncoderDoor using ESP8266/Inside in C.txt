#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
Servo myservo;
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
#define DOOR 14
#define TRIG_PIN 15
#define ECHO_PIN 16 
#define BUZZER 12

/////////////////
void Closethedoor();
void Openthedoor();
void BuzzerOn();
void BuzzerOff();
void PushBotton();
int ultra();
long duration;
int distance;
bool ultraflag = false;
bool flag = false;
int val_ultra;
int pos;

WiFiClient espClient;
PubSubClient client(espClient);
void setup() 
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  pinMode(BUTTON,INPUT);
  pinMode(DOOR, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER,OUTPUT);
  myservo.attach(DOOR);
  void Closethedoor();
}
void loop() 
{
  if (!client.connected())
  {
    client.connect("ESP8266Client", mqtt_user, mqtt_password);
    client.subscribe(mqtt_topic);
  }  
  client.loop();
  if(!ultraflag)
  {
    val_ultra = ultra();
    if(val_ultra<4)
    {
      BuzzerOn();
    }
    else
    {
      BuzzerOff();
    }
  }
  PushBotton();
}

void callback(char* topic, byte* payload, unsigned int length)
{

  String Msg = "";
  int i=0;

  while (i<length) Msg += (char)payload[i++];


  if (Msg == "Allow")
  {
    Openthedoor();
    ultraflag = true;
  }
  else if (Msg == "Ultraoff")
  {
    ultraflag = true;
  }
  else if (Msg == "Close")
  {
    Closethedoor();
    ultraflag = false;
  }

}
void PushBotton()
{
  if(digitalRead(BUTTON) == LOW)
  {
      if(!flag)
      {
          flag= true;
          
          if(ultraflag == false)
          {
             ultraflag = true;  
             Openthedoor();         
          }
          else if (ultraflag == true)
          {
            ultraflag = false;        
            Closethedoor();
          }
          
      }
  }
  else flag = false;
}
void Closethedoor()
{
   for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);             
    Serial.println("door Closed");
    delay(5);                    
  }

}
void Openthedoor()
{
   for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);            
    Serial.println("door Open");
    delay(5);            
  }
}
int ultra()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance= duration*0.034/2;
  return distance;
}
void BuzzerOn()
{
    delay(200);
    digitalWrite(BUZZER,LOW);
    delay(200);
    digitalWrite(BUZZER,HIGH);
}
void BuzzerOff()
{
    digitalWrite(BUZZER,LOW);
}

