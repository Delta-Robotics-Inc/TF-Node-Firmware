  const int curr_pin = A0;
  
  float getMuscleAmps() {
      float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,raw=0.0;
    
      for (int x = 0; x < 10; x++){       // Get 10 samples
        AcsValue = analogRead(curr_pin);  // Read current sensor values   
        Samples = Samples + AcsValue;     // Add samples together
      }
      raw = Samples / 10.0;  // Taking Average of Samples
    
      float amps = ((raw * (5.0 / 1024.0)) - 2.5)/0.100; //5.0/1024 is conversion ratio of volts/native unit. 2.5 v is 0 A due to positive and negative ability
      
      return amps;
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(getMuscleAmps());
}
