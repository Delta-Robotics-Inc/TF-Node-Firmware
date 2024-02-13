//simple power sketch

int button_in = 4;
int M1 = 3;
int M2 = 5;
int M3 = 6;

selected_muscle = M1;
power = 0.5; //number between 0-1.0 (0% to 100%)

void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(button_in, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(button_in) {
    analogWrite(selected_muscle, power * 255);
    Serial.println("Button pressed!");
  }
  else {
    analogWrite(selected_muscle, 0); //turn muscle off if no button
    Serial.println("Button unpressed!");
  }
}
