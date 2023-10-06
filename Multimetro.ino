/*
CENTRO EDUCATIVO TECNICO LABORAL KINAL 
Electronica dispositivos digitales
Taller de electronica 
NOMBRE:
*Diego Alejandro Culajay Gonzalez
Carnet
*2019485
*/

#include <SoftwareSerial.h>

const int medir1 = 2;         // Pin positivo
const int medir2 = 3;         // Pin negativo
int Valuable;                // Variable que almacena el valor raw (0 a 1023)
float vol;               // Variable que almacena el voltaje (0.0 a 25.0)

float Sensi = 0.185;

const int pinAnalogico = A2;    // Pin analógico para medir la tensión
const float resistenciaConocida = 1000.0; // Resistencia conocida en ohmios (1k ohm)

SoftwareSerial BLUEE(7, 6);

unsigned long tiempoAnterior = 0;
const unsigned long intervaloMedicion = 1000; // Intervalo de medición en milisegundos

void setup() {
  pinMode(MED1, OUTPUT);  // Configurar el pin positivo como salida
  pinMode(MED2, OUTPUT);  // Configurar el pin negativo como salida
  Serial.begin(9600);    // Iniciar la comunicación serial
  BLUEE.begin(9600);
}

char recep;

void loop() {
  // Verificar si hay datos disponibles desde la aplicación Bluetooth
  if (BLUEE.available()) {
    recep = BLUEE.read();
    Serial.println(recep);

    // Procesar comandos según corresponda
    if (recep == '1') {
      medirVoltaje();
    }
    else if (recep == '2') {
      medirResistencia();
    }
    else if (recep == '3') {
      medirCorriente();
    }
  }

  // Realizar mediciones continuas a intervalos regulares
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= intervaloMedicion) {
    tiempoAnterior = tiempoActual;

    // Realizar la medición que desees aquí
    // Por ejemplo: medirCorriente();
  }
}

void medirCorriente() {
  float Corriente=calculo(500);
      Serial.print("Corriente: ");
      Serial.println(Corriente,3);
      BLUEE.print(Corriente,3);
      BLUEE.print(";");
}
float calculo(int numeroMuestral)
{
float LeerSenso = 0;
float inten = 0;
for(int i=0;i<numeroMuestral;i++)
{
  LeerSenso= analogRead(A2) * (5.0 / 1023.0);
  inten=inten+(LeerSenso-2.5)/Sensibilidad;
}
inten=inten/numeroMuestral;
return(inten);
}

void medirVoltaje() {
  digitalWrite(medir1, HIGH);
  digitalWrite(medir2, HIGH);
  Valuable = analogRead(A2);               // Realizar la lectura
  VOLTAGE = map(Valuable, 0, 1023, 0, 250) / 10.0;  // Escalar a 0.0 - 25.0
  Serial.print("Voltaje: ");
  Serial.println(VOL);          // Mostrar el valor por serial
  BLUEE.print(VOLTAGE);
  BLUEE.print(";");
}

void medirResistencia() {
  digitalWrite(MED1, LOW);
  digitalWrite(MED2, LOW);
  int valorAnalogico = analogRead(pinAnalogico);

  // Convertir el valor analógico a tensión (en voltios)
  float tensionMedida = (float)valorAnalogico * (5.0 / 1023.0);

  // Calcular la corriente (I) usando la ley de Ohm: I = V / R
  float corriente = tensionMedida / resistenciaConocida;

  // Calcular la resistencia desconocida usando la ley de Ohm: R = V / I
  float resistenciaDesconocida = tensionMedida / corriente;

  Serial.print("Resistencia: ");
  Serial.println(resistenciaDesconocida);
  BLUEE.print(resistenciaDesconocida);
  BLUEE.print(";");
}
