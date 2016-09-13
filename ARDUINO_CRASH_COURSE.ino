
int red = 4;
int yellow = 5;
int green = 6;

int milli = 50;


void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

}

void loop() {

  //
  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(milli);               // wait for a second
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW); // turn the LED off by making the voltage LOW
  delay(milli);               // wait for a second
  // wait for a second
}
