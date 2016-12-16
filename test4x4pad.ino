int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

int startpin = 2;
int endpin = 10;
String output, sensor;

void setup()
{
  Serial.begin(9600);  
  for (int i=startpin; i <= endpin; i++){
    pinMode(i, INPUT_PULLUP);
  }
  
}

void loop()
{
   //Serial.println("Start:");
   
   for (int i=startpin; i <= endpin; i++){
        if (!digitalRead(i) && i!=3){
          output = " Die Taste ";
          sensor = digitalRead(i);
          Serial.println (i + output + sensor);
          //delay(100);  
        }
        
   } 
   //Serial.println("....");
  // delay(1000);
}
