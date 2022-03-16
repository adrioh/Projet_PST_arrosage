#include <BME280I2C.h>  //bme
#include <Wire.h>  /bme

//http-wifi
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
const char* ssid = "secret";
const char* password = "secret";
AsyncWebServer server(80);

#define SERIAL_BAUD 115200

//bme
BME280I2C bme;
//gt110
int sensorPin = 35; // Déclaration de la broche du capteur
int sensorValue = 0; // Déclaration d’une variable
//relais
const int RelayPin = 18;

//led à la place de l'electrovanne
int LED_BUILTIN = 2;
int LED_SYS_STATU_OK = 33; 
int LED_SYS_STATU_NOK = 25;
int LED_VALVE_STATU_OPEN = 14;
int LED_VALVE_STATU_CLOSED = 26;
int LED_MOD_AUTO = 27; 
int LED_MOD_MANUAL = 32;

// 0 auto - 1 manuel
int modeEtat = 0;

//0 close - 1 open
int vanneEtat = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (LED_SYS_STATU_OK, OUTPUT);
  pinMode (LED_SYS_STATU_NOK, OUTPUT);
  pinMode (LED_VALVE_STATU_OPEN, OUTPUT);
  pinMode (LED_VALVE_STATU_CLOSED, OUTPUT);
  pinMode (LED_MOD_AUTO, OUTPUT);
  pinMode (LED_MOD_MANUAL, OUTPUT);

  //sYSTEM STATUS
  digitalWrite(LED_SYS_STATU_OK, LOW);
  digitalWrite(LED_SYS_STATU_NOK, HIGH);

  //Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  //get ip adress of esp
  Serial.println(WiFi.localIP());

  //  ip/temperature
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getTemperature().c_str());
  });

  //  ip/humidity/sol
  server.on("/humidity/sol", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getHumidityS().c_str());
  });

  //  ip/humidity/air
  server.on("/humidity/air", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getHumidityA().c_str());
  });

  //  ip/pression
  server.on("/pression", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getPression().c_str());
  });


  //  ip/auto
  server.on("/auto", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", setAutoMode().c_str());
  });

  //  ip/manuel
  server.on("/manuel", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", setManuelMode().c_str());
  });

  //  ip/open
  server.on("/open", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", openVanne().c_str());
  });

  //  ip/close
  server.on("/close", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", closeVanne().c_str());
  });

  //  ip/etat
  server.on("/etat", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getEtat().c_str());
  });




  // Start server
  server.begin();

  //relais
  pinMode(RelayPin, OUTPUT);



  while (!Serial) {} // Wait

  Wire.begin();

  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch (bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }
}

void loop() {
   digitalWrite(LED_SYS_STATU_OK, HIGH);
  digitalWrite(LED_SYS_STATU_NOK, LOW);
  /*
    //Partie capteur et affichage dans le serial
    Serial.println("___________________________________" );
    printBME280Data(&Serial);
    delay(500);

    sensorValue = analogRead(sensorPin);

    Serial.print("Humidity (soil): " );
    Serial.print(map(sensorValue,0,1024,0,100));
    Serial.println("% RH");

    delay(5000);
    //FIN*/

  commandElectro();
  ledMod();

  /*
    digitalWrite(RelayPin, HIGH);   //Turn on relay
    //delay(60000);
    //digitalWrite(RelayPin, LOW);   //Turn off relay
    //delay(10000);
  */
}


void printBME280Data (Stream* client) {

  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  client->print("Temp: ");
  client->print(temp);
  client->print("°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->println("Pa");

  delay(1000);
}

//Obtenir la temperature
String getTemperature() {
  float temp(NAN), pres(NAN), hum(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);
  return String(temp);
}


//Obtenir la pression
String getPression() {
  float temp(NAN), pres(NAN), hum(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);
  return String(pres);
}

//Obtenir l'humidité de l'air
String getHumidityA() {
  float temp(NAN), pres(NAN), hum(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);
  return String(hum);

}

//Obtenir l'humidité du sol
String getHumidityS() {
  sensorValue = analogRead(sensorPin);

  return String(map(sensorValue, 0, 1024, 0, 100));

}

String openVanne() {
  if (modeEtat == 1) { //si mode manuel
    //digitalWrite(LED_BUILTIN, HIGH);
    vanneEtat = 1; //open
  } else
    return "nok (a supp --> impossible en mode auto = passer en mode manuel)";

  return "ok (a supp --> electrovanne ouverte)";
}

String closeVanne() {
  if (modeEtat == 1) { //si mode manuel
    digitalWrite(LED_BUILTIN, LOW);
    vanneEtat = 0; //close
  } else
    return "nok (a supp --> impossible en mode auto = passer en mode manuel)";

  return "ok (a supp --> electrovanne fermée)";
}

String setAutoMode() {
  if (modeEtat == 0)
    return "already (a supp --> déjà en mode auto)";
  else {
    modeEtat = 0; //mode auto
    //digitalWrite(LED_BUILTIN, LOW);
    vanneEtat = 0; //close
  }

  return "ok (a supp --> passage en mode auto)";
}

String setManuelMode() {
  if (modeEtat == 1)
    return "already (a supp --> déjà en mode manuel)";
  else
    modeEtat = 1;

  return "ok (a supp --> passage en mode manuel)";
}

String getEtat() {
  String tempMode;
  String tempEtat;

  if (modeEtat == 0)
    tempMode = "auto";
  else
    tempMode = "manuel";

  if (vanneEtat == 0)
    tempEtat = "fermée";
  else
    tempEtat = "ouvert";

  return "mode: " + String(tempMode) + " etat vanne: " + String(tempEtat);
}


void commandElectro() {
  if (vanneEtat == 0) {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RelayPin, LOW);//Turn off relay
    digitalWrite(LED_VALVE_STATU_OPEN, LOW);
    digitalWrite(LED_VALVE_STATU_CLOSED, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(RelayPin, HIGH);   //Turn on relay
    digitalWrite(LED_VALVE_STATU_OPEN, HIGH);
    digitalWrite(LED_VALVE_STATU_CLOSED, LOW);
  }
}

void ledMod() {
  if (modeEtat == 0) { //auto
    digitalWrite(LED_MOD_AUTO, HIGH);
    digitalWrite(LED_MOD_MANUAL, LOW);
  } else {  //manuel
    digitalWrite(LED_MOD_AUTO, LOW);
    digitalWrite(LED_MOD_MANUAL, HIGH);
  }
}
