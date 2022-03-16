int sensorPin = 35; // Déclaration de la broche du capteur
int sensorValue = 0; // Déclaration d’une variable
void setup() {
Serial.begin(115200);
}
void loop() {
sensorValue = analogRead(sensorPin);
delay(1000);
Serial.print("sensor = " );
Serial.println(sensorValue);
}
