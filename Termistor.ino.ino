
#include <Wire.h>
#include <math.h>


#define ADS1110_ADDRESS 0x48  // Adresa I2C a ADS1110

int ThermistorPin = 0;// Pinul analogic la care este conectat termistorul (A0)
float Rs1 = 10000.0; // Rezistența rezistorului de referință
float Rs2 = 4700.0; 
float Vout1, ADCvalue,  logR_TH1, R_TH1, T1, Tarduino; // Variabile pentru calcularea rezistenței și temperaturii
float B = 3950.0; // Coeficientul Beta
float T0 = 25 + 273.15; // Temperatura nominală în Kelvin
float R0 = 10000.0; // Rezistența la 25°C
float VCC1 = 5.0; // Tensiunea de alimentare
float VCC2 = 5.0; // Tensiunea de alimentare
float  Vout2, logR_TH2, R_TH2, T2, Tconvertor;
 

// citirea datelor de la convertor
int16_t readADS1110() {
  Wire.requestFrom(ADS1110_ADDRESS, 2);
  if (Wire.available() <= 2) {
    int16_t result = Wire.read() << 8 | Wire.read();
    return result;
  }
  return 0;
}
// Inițializeazăm comunicarea serială
void setup() {
   Wire.begin();        // Inițializează I2C
  Serial.begin(9600);  // Inițializează serial monitor
  
}

void loop() {

  ADCvalue = analogRead(ThermistorPin);  // valoarea digitală corespunzătoare tensiunii analogice de pe pinul A0.
  //Calculele pentru senzorul conectat direct la arduino.
  Vout1= (ADCvalue /1023.0) * 5.0;   // Calculează tensiunea 
  R_TH1 = Rs1 * ((VCC1 / Vout1) - 1.0); // Calculează rezistența NTC 
  T1 = 1 / ((1 / T0) + (log(R_TH1 / R0) / B)); // Utilizarea ecuației Beta pentru a afla temperatura  
  Tarduino = T1 - 273.15; // Conversie în grade Celsius

  int16_t ADCvalue = readADS1110();  // Valoarea digitală 
  //Calculele pentru senzorul conectat la arduino prin ADS1110.
  Vout2= (ADCvalue / 32767.0) * 2.048;  // Calculează tensiunea  
  R_TH2 = Rs2  * (VCC2/Vout2 - 1); // Calculează rezistența NTC 
  T2 = 1 / ((1 / T0) + (log(R_TH2 / R0) / B));// Calculează temperatura folosind ecuația beta
  Tconvertor = T2 - 273.15; // Conversie în grade Celsius    

  
  // Afișează valoarea temperaturi
  Serial.print("T1: ");
  Serial.print(Tarduino);
  Serial.println(" C");

  Serial.print("T2: ");
  Serial.print(Tconvertor);
  Serial.println(" C");
  
delay(300000);
  
}


