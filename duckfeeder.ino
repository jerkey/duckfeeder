#define DOORCLOSED 9 // angle when closed
#define DOOROPEN 180 // angle when open
#define DOORPIN 10 // pin for servo

#define SPK_L1 4 // speaker terminals, left and right
#define SPK_L2 5
#define SPK_R1 6
#define SPK_R2 7

#include <Servo.h>
Servo doorServo;  // create servo object to control a servo

int inByte = 0;         // incoming serial byte
int doorSpeed = 100;  // how many milliseconds delay between degrees

byte position,lastPosition = DOOROPEN;

void setup() {
  pinMode(DOORPIN,OUTPUT);
  doorServo.attach(DOORPIN);
  Serial.begin(9600);
  Serial.println("hello i'm a duck food bin");
  doorServo.write(DOOROPEN);
  pinMode(SPK_L1,OUTPUT);
  pinMode(SPK_L2,OUTPUT);
  pinMode(SPK_R1,OUTPUT);
  pinMode(SPK_R2,OUTPUT);
  randomSeed(analogRead(0));
}

void loop()
{
  if (Serial.available() > 0) {
    inByte = Serial.read();
    switch (inByte) {
    case 'o':
      Serial.println("open");
      position = DOOROPEN;
      Serial.println(position);
      door(position);
      break;
    case 'c':
      Serial.println("close");
      position = DOORCLOSED;
      Serial.println(position);
      door(position);
      break;
    case '+':
      position += 1;
      Serial.println(position);
      door(position);
      break;
    case '-':
      position -= 1;
      Serial.println(position);
      door(position);
      break;
    case 'q':
      Serial.println("quack");
      quack();
      break;
    default:
      Serial.print(inByte);
      Serial.println("o for open, c for close");
      break;
    }
  }
}

void stepper(boolean dir, unsigned int distance, int speed) {
  for(int i=0; i <distance ;  i++){
    delayMicroseconds(500);
    delayMicroseconds(speed);
  }
}

void door(int angle) {
  int step = 1; // must be 1 for "i != angle" to work
  if (angle < lastPosition) step *= -1; // count the right direction
  for(int i=lastPosition; i != angle;  i += step){
    doorServo.write(i);
    delay(doorSpeed);
  }
  doorServo.write(angle);
  lastPosition = angle;
}

void quack() {
  int squawkCount = 20;
  int squawkLength = 5;
  int squawkPitch;
  for (int i = 0; i < squawkCount; i++) {
    squawkPitch = random(1000); // gets added onto regular timing
    for (int j = 0; j < squawkLength; j++) {
      digitalWrite(SPK_L1,HIGH);
      digitalWrite(SPK_L2,LOW);
      digitalWrite(SPK_R1,HIGH);
      digitalWrite(SPK_R2,LOW);
      delayMicroseconds(80);
      digitalWrite(SPK_L1,LOW);
      digitalWrite(SPK_L2,HIGH);
      digitalWrite(SPK_R1,LOW);
      digitalWrite(SPK_R2,HIGH);
      delayMicroseconds(80);
      digitalWrite(SPK_L1,HIGH);
      digitalWrite(SPK_L2,LOW);
      digitalWrite(SPK_R1,HIGH);
      digitalWrite(SPK_R2,LOW);
      delayMicroseconds(2340);
      digitalWrite(SPK_L1,LOW);
      digitalWrite(SPK_L2,HIGH);
      digitalWrite(SPK_R1,LOW);
      digitalWrite(SPK_R2,HIGH);
      delayMicroseconds(2000+squawkPitch);
    }
    digitalWrite(SPK_L1,LOW);
    digitalWrite(SPK_L2,LOW);
    digitalWrite(SPK_R1,LOW);
    digitalWrite(SPK_R2,LOW);
    delay(50+random(100));
  }
}
