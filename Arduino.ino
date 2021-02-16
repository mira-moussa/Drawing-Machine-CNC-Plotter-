
#include <Servo.h>

Servo myservo;  

String inputString = "";         // a String to hold incoming data
// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define slpD 4
#define dirPinUP 7
#define stepPinUP 6
#define slpUP 8
int delayMS = 1000;
#define stepsPerRevolution 200
int dirPins[] = {2 , 7};
int stepPins[] = {3 , 6};
int slpPins[] = {4 , 8};
int refX, refY , delX , delY = 0;
int Xaxis = 0;
int Yaxis = 1;
int Xfactor = 1;
int Yfactor = 1;
bool firstP = false;
void setup()
{
  Serial.begin(115200); // // opens serial port, sets data rate to 9600 bps
  Serial.setTimeout(50);
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(slpD, OUTPUT);
  pinMode(stepPinUP, OUTPUT);
  pinMode(dirPinUP , OUTPUT);
  pinMode(slpUP, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(slpD, LOW);
  digitalWrite(slpUP, LOW);
  inputString.reserve(200);
//myservo.attach(9);

}

void move(int motor, int dir, float stepsper){
   // myservo.write(0);
    digitalWrite(slpPins[motor], HIGH);
    digitalWrite(dirPins[motor], dir);
    for (int i = 0; i < stepsper; i++) {
      // These four lines result in 1 step:
      digitalWrite(stepPins[motor], HIGH);
      delayMicroseconds(delayMS);
      digitalWrite(stepPins[motor], LOW);
      delayMicroseconds(delayMS);
    }
    digitalWrite(slpPins[motor], LOW);
   // myservo.write(90);
  }
 String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

 
void loop()
{
  bool seriall;
  int incomX, incomY;
  if (Serial.available() > 0) {
    seriall = true;
           // get the new byte:
           char inChar = (char)Serial.read();
           
           inputString += inChar;
           if(inChar == ')' ) {
              String incomXX = getValue(inputString, ',', 0);
              String incomXXX = getValue(incomXX, '(', 1);
              String incomYY = getValue(inputString, ',', 1);
              String incomYYY = getValue(incomYY, ')', 0);
              int xx = incomXXX.toInt();
              int yy = incomYYY.toInt();
              if(!firstP) { refX = xx; refY = yy;}
              if(firstP){
                delX = (int)xx - (int)refX;
                delY = (int)yy - (int)refY;
                if(delX != 0){
                  if(delX > 0)
                    move(0, 1 , abs(delX) * Xfactor); 
                  if(delX < 0)
                    move(0, 0 , abs(delX) * Xfactor); 
                  }
                if(delY != 0){
                  if(delY > 0)
                    move(1, 1 , abs(delY) * Yfactor); 
                  if(delY < 0)
                    move(1, 0 , abs(delY) * Yfactor); 
                    }
                //delay(100); // put code here
                refX = xx; refY = yy;
                Serial.print("z");
                Serial.print(delX);
                Serial.print(",");
                Serial.print(delY);
                }else{
                Serial.print("z");
                firstP = true;
                }
               inputString.remove(0); 
           }
} else if (seriall == true){ 
 // String delimiter = ",";

  //inputString.remove(0);
  digitalWrite(LED_BUILTIN, LOW); //disable steppers
  }
}
