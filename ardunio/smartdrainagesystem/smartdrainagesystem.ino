nvolatile int flow_frequency;
unsigned int l_hour;
unsigned char flowsensor = 2; 
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
   pinMode(flowsensor, INPUT);
   pinMode(3,OUTPUT);
   pinMode(4,INPUT);
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); 
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
}
void loop ()
{
      waterflow();
      ultra();
	  delay(100);
}
void waterflow()
{
      currentTime = millis();
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Count
   }
   if(l_hour>50){
   Serial.println("4");
   delay(100);
   }
   else
   {
    Serial.println("5");
    delay(1000);
   }
}
void ultra()
{
  digitalWrite(3,LOW);      //sending a pulse
  delayMicroseconds(2);
  digitalWrite(3,HIGH);
  delayMicroseconds(10);
  digitalWrite(3,LOW);
  float t=pulseIn(4,HIGH);
  t=t/2;
  int d=0.0343*t;       //After converting m into cms and s into ms
 // Serial.println("distance:");
  //Serial.println(d);
  if(d>20)
  {
    Serial.println("1");
  }
  else if(d>10 && d<20)
  {
     Serial.println("2");
  }
  else if(d<10)
  {
     Serial.println("3");
  }
  delay(3000);
}
