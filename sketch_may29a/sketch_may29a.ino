#include<LiquidCrystal.h>              //loads LCD library

   LiquidCrystal screen(10,9,5,4,3,2);       //an LCD object named screen has been created and it uses the pin numbers of the arduino that are specified in the braces

   float emptylevel;               //variable for distance between ultrasonic sensor and water
   float upperlimit=5.0;           // max level of water in the tank
   float lowerlimit=25.0;          //min level of water in the tank
   int relaypin=7;                 //relay pin is connected to pin 7
   int trigpin=12;                 // transmitter pin of ultrasonic sensor is connected to pin 12
   int echopin=8;                 // receiving pin of ultrasonic sensor is connected to pin 8
   int buzzer=6;                   //buzzer is connected to pin 6
   int LCDdelay=1000;              //delay time for LCD display (screen)
   int buzzdelay=1000;              //delay time for buzzer


void setup()
{
  Serial.begin(9600);                   //starts serial communication
  screen.begin(16,2);                   //starts LCD(16 columns,2 rows)
  pinMode(relaypin,OUTPUT);             //initialises as output pin
  pinMode(trigpin,OUTPUT);              //initialises as output pin
  pinMode(echopin,INPUT);               //initialises as input pin
  pinMode(buzzer,OUTPUT);               // initialises as output pin
  screen.print("WELCOME");               //prints to LCD
 }


//user defined function 1 ......
 float determine()
 {
  float timetaken;                                       // variable for total time taken by sound signal to reach echopin from trigpin
  float speedofsound=343.0;                              //speed of sound in  m/s
  float distance;                                        //variable for total distance travelled by the sound signal between trigpin and echopin

  digitalWrite(trigpin,LOW);                             //outputs 0v to the trigpin
  delayMicroseconds(2000);                               //0v output is maintained for 2000 microseconds (2 milliseconds)
  digitalWrite(trigpin,HIGH);                            //outputs 5v to the trigpin
  delayMicroseconds(10);                                 //5v output is maintained for 10 microseconds
  digitalWrite(trigpin,LOW);                             //outputs 0v to the trigpin
 
  timetaken=pulseIn(echopin,HIGH);                     //receives the high sound signal and estimates the time in microseconds taken by it
  timetaken=timetaken/1000000.0;                       //converts time from microseconds to seconds
  Serial.print("total travel time is ");               //prints to serial monitor
  Serial.print(timetaken);                             //
  Serial.println(" seconds");                          //
  delay(500);
 
  distance=speedofsound*timetaken;                                          //determines total distance travelled in meters
  distance=distance/2.0;                                                    //as actual distance between sensor and water is half of the total distance travelled by the sound signal
  distance=distance*100.0;                                                  //converts from m into cm
  Serial.print("distance of water from sensor is ");                        //prints to serial monitor
  Serial.print(distance);                                                   //
  Serial.println(" cm");                                                    //
  delay(500);

  return(distance);
 }



//user defined function 2 .........
  void wipe()
  {
    screen.setCursor(0,1);                              //places cursor at column 0 and row 1
    screen.print("                ");                   //prints 16 blanks to LCD
    screen.setCursor(0,0);                              //places cursor at column 0 and row 0
    screen.print("                ");                   //prints 16 blanks to LCD
    screen.setCursor(0,0);                              //again places cursor at column 0 and row 0
  }




void loop()
{
  emptylevel=determine();                       //calls determine() function

  // first if statement.......  
  if(emptylevel<upperlimit)
   {
    wipe();                                     //calls wipe() function
    screen.print("EMPTY LEVEL IS ");            //prints to the LCD      
    screen.setCursor(0,1);                      //                
    screen.print(emptylevel);                   //
    screen.print(" cm");                        //
    delay(LCDdelay);                            //delay of 1000 ms
   
    if(relaypin==LOW
    )                            //if relay is on or motor is on
     {
      wipe();
      screen.print("TANK IS FULL");              //prints to the LCD
      screen.setCursor(0,1);                     //cursor moves
      screen.print("MOTOR WILL OFF");            //prints to LCD
      delay(LCDdelay);                           //delay of 1000 ms
      digitalWrite(relaypin,HIGH);               //turns off the motor
      digitalWrite(buzzer,HIGH);                 //buzzer sounds
      delay(buzzdelay);                                             //for 250 ms
      digitalWrite(buzzer,LOW);                  //buzzer turns off                      
     }
    else
     {                                               //if relay is off or motor is off
      wipe();
      screen.print("TANK IS FULL");             //prints to the LCD
      screen.setCursor(0,1);                    //
      screen.print("MOTOR IS OFF");             //
      delay(LCDdelay);                          //delay of 1000 ms      
     }
    }


   //second if statement starts.......
   if(emptylevel>lowerlimit)
    {
     wipe();
     screen.print("EMPTY LEVEL IS ");                 //prints to the LCD
     screen.setCursor(0,1);                           //
     screen.print(emptylevel);                        //
     screen.print(" cm");                             //
     delay(LCDdelay);                                 //delay of 1000 ms

     while(emptylevel>upperlimit)
      {
       if(relaypin==HIGH)                            //if relay is off or motor is off
        {
         wipe();
         screen.print("WATER IS LESS");              //prints to LCD
         screen.setCursor(0,1);                      //
         screen.print("MOTOR WILL ON ");             //
         delay(LCDdelay);                            //delay of 1000 ms
         digitalWrite(relaypin,LOW);                 //turns on the motor
         digitalWrite(buzzer,HIGH);                  //buzzer sounds
         delay(buzzdelay);                                          //for 250 ms
         digitalWrite(buzzer,LOW);                   //buzzer turns off  
        }
       else
        {                                             //if relay is on or motor is on
         wipe();
         screen.print("WATER IS RAISING");            //prints to LCD
         screen.setCursor(0,1);                       //
         screen.print("MOTOR IS ON");                 //
         delay(LCDdelay);                             //delay of 1000 ms  
        }
     
       emptylevel=determine();                      //calls determine() function
      }
    }


   //third if statement starts.......
   if(emptylevel>upperlimit && emptylevel<lowerlimit)
    {
     wipe();
     screen.print("WATER IS ");                         //prints to LCD
     screen.setCursor(0,1);                             //
     screen.print("SUFFICIENT");                        //
     delay(LCDdelay);                                  //delay of 1000 ms
    }                              
}
