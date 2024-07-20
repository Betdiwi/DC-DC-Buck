const int pinEntrada = 4; 
const int ledPin = 2;  
const int frecuencia = 20000;
const int canal = 0;
const int resolution = 8;
int dutyCycle = 127;
int currentDutyCycle = 0; // Duty cycle actual

void setup() {
  Serial.begin(115200);
  // Configuramos el PWM
  ledcSetup(canal, frecuencia, resolution);
  ledcAttachPin(ledPin, canal);
  pinMode(pinEntrada, INPUT);
}

void loop() {
  // Leer el valor analógico (0 - 4095)
  int valorADC = analogRead(pinEntrada);
  // Convertir el valor leído a voltaje (asumiendo que el rango de 0 a 4095 corresponde a 0V a 3.3V)
  float voltaje = valorADC * (3.3 / 4095.0);

  // Convertir el voltaje a un valor real según tu escala
  float voltaje_real = voltaje * 12.32;

  // Calcular el ciclo de trabajo en porcentaje
  if (voltaje_real != 0) {
    dutyCycle = 1800 / voltaje_real;
  } else {
    dutyCycle = 0; // Evitar división por cero
  }

  // Asegurarse de que el dutyCycle esté dentro de los límites válidos
  if (dutyCycle > 100) {
    dutyCycle = 100;
  } else if (dutyCycle < 0) {
    dutyCycle = 0;
  }

  // Ajustar el duty cycle actual gradualmente hacia el duty cycle deseado
  if (currentDutyCycle < dutyCycle) {
    currentDutyCycle++;
  } else if (currentDutyCycle > dutyCycle) {
    currentDutyCycle--;
  }

  // Convertir el ciclo de trabajo a bits (0 - 255 para 8 bits de resolución)
  int dutyCycle_bits = (currentDutyCycle * 255) / 100;

  // Aplicar el ciclo de trabajo al PWM
  ledcWrite(canal, dutyCycle_bits);

  // Mostrar el voltaje y el ciclo de trabajo en el monitor serie
  Serial.print("Voltaje: ");
  Serial.print(voltaje_real);
  Serial.print(" V   ");
  Serial.print("DCycle: ");
  Serial.print(currentDutyCycle);
  Serial.println(" %");

  // Esperar un breve periodo antes de la siguiente lectura para una transición suave
  delay(10); // Ajusta este valor según lo suave que desees la transición
}
