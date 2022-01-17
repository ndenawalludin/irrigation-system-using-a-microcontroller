byte pump1 = 14;
byte pump2 = 10;
void setup() {
 Serial.begin(9600);
 while (!Serial);
 pinMode(pump1, OUTPUT); // variant low/high
 digitalWrite(pump2, LOW); // variant input/output
}
void loop() {
 digitalWrite(pump1, HIGH); // pump1 deactivated
 //pinMode(pump2, INPUT); // pump2 deactivated
 delay(10000);
 digitalWrite(pump1, LOW); // pump1 activated
 //pinMode(pump2, OUTPUT);  // pump2 activated
 delay(10000);
}
