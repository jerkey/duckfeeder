#define STEPPIN 11     
#define DIRPIN 12      
#define STEPSPERMM 15.87 // 1000 steps = 63mm of wire.
#define WIRELENGTH 77 * STEPSPERMM // 77mm total pieces wanted.
#define STRIPLENGTH 4 * STEPSPERMM // 5mm stripped
#define TOOLOFFSET 9.5 * STEPSPERMM // 9.5mm is distance between stripper and chopper
#define FWD HIGH // which way is DIR wired?
#define BACK LOW
#define STRIPOPEN 20 // position of stripper servo
#define STRIPCLOSED 175
#define CHOPCLOSED 170
#define CHOPOPEN 10
#define STRIPPIN 9 // what pin is it on
#define CHOPPIN 10

#include <Servo.h> 
Servo chopServo;  // create servo object to control a servo 
Servo stripServo;  // create servo object to control a servo 

int inByte = 0;         // incoming serial byte
int speed = 10000;  // how many microseconds delay between steps

void setup() {
  pinMode(STEPPIN, OUTPUT);      
  pinMode(DIRPIN, OUTPUT);
  pinMode(CHOPPIN,OUTPUT);
  chopServo.attach(CHOPPIN);
  pinMode(STRIPPIN,OUTPUT);
  stripServo.attach(STRIPPIN);
  Serial.begin(9600);
  Serial.println("hello");
  chopServo.write(CHOPOPEN);
  stripper(STRIPOPEN);
}

void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    switch (inByte) {
    case 'w':
      Serial.println("wirefeed!");
      stepper(FWD,WIRELENGTH,speed);
      break;
    case 'f':
      Serial.println("forward");
      stepper(FWD,STRIPLENGTH,speed);
      break;
    case 'b':
      Serial.println("back");
      stepper(BACK,STRIPLENGTH,speed);
      break;
    case ']':
      Serial.println("] ooch forward");
      stepper(FWD,35,speed);
      break;
    case '[':
      Serial.println("[ ooch back");
      stepper(BACK,35,speed);
      break;
    case 'F':
      Serial.println("forward TOOLOFFSET");
      stepper(FWD,TOOLOFFSET,speed);
      break;
    case 'B':
      Serial.println("back TOOLOFFSET");
      stepper(BACK,TOOLOFFSET,speed);
      break;
    case 'o':
      Serial.println("open");
      stripper(STRIPOPEN);
      break;
    case 'c':
      Serial.println("close");
      stripper(STRIPCLOSED);
      break;
    case 'x':
      Serial.println("xchop!");
      stepper(FWD,TOOLOFFSET,speed);
      chop();
      stepper(BACK,TOOLOFFSET,speed);
      break;
    case 'j':
      Serial.println("just chop");
      chop();
      break;
    case 'G':
      Serial.println("Go fwd until any key");
      while (Serial.available() == 0) stepper(FWD,STRIPLENGTH,speed);
      inByte = Serial.read();
      break;
    case 'R':
      Serial.println("Reverse until any key");
      while (Serial.available() == 0) stepper(BACK,STRIPLENGTH,speed);
      inByte = Serial.read();
      break;
    case '=':
      Serial.print("speed ");
      speed += 100;
      Serial.println(speed);
      break;
    case '-':
      Serial.print("speed ");
      speed -= 100;
      Serial.println(speed);
      break;
    default:
      Serial.println("wirefeed forward back Forward(TOOLOFFSET) Back(TOOLOFFSET) open close xchop justchop Go Reverse");
      break;
    }
  }
}

void stepper(boolean dir, unsigned int distance, int speed) {
  digitalWrite(DIRPIN, dir);
  for(int i=0; i <distance ;  i++){
    digitalWrite(STEPPIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEPPIN, LOW);
    delayMicroseconds(speed);
  }
}

void stripper(int angle) {
  stripServo.write(angle);              // tell servo to go to position in variable 'angle'
  delay(750);                       // waits one second for the servo to reach the position 
}

void chop() {
  chopServo.write(CHOPCLOSED);
  delay(900);
  chopServo.write(CHOPOPEN);
  delay(700);
}
