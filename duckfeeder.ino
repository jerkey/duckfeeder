#define DOORCLOSED 9 // angle when closed
#define DOOROPEN 180 // angle when open
#define DOORPIN 10 // pin for servo

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
