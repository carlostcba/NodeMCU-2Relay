// Incluir librerías necesarias
#include <Arduino.h>

// Definir los pines
#define BOTON_PIN 2    // Pin del botón
#define RELE_PIN  3    // Pin del relé

// Definir variables
bool estadoBotonAnterior = HIGH;  // Estado anterior del botón
unsigned long tiempoUltimaAccion = 0; // Tiempo de la última acción
const unsigned long tiempoDebounce = 50; // Tiempo de debounce del botón
const unsigned long tiempoReleOn = 200; // Tiempo que el relé permanece activo
const unsigned long tiempoDesactivacionBoton = 500; // Tiempo de desactivación del botón

// Configuración
void setup() {
  pinMode(BOTON_PIN, INPUT_PULLUP); // Configurar el pin del botón como entrada con pull-up
  pinMode(RELE_PIN, OUTPUT);         // Configurar el pin del relé como salida
}

// Bucle principal
void loop() {
  // Leer el estado actual del botón
  bool estadoBoton = digitalRead(BOTON_PIN);

  // Verificar si el botón ha sido presionado y ha pasado el tiempo de debounce
  if (estadoBoton != estadoBotonAnterior && millis() - tiempoUltimaAccion > tiempoDebounce) {
    // Actualizar el tiempo de la última acción
    tiempoUltimaAccion = millis();
    
    // Cambiar el estado del relé
    digitalWrite(RELE_PIN, HIGH); // Activar el relé
    delay(tiempoReleOn);          // Esperar el tiempo de activación del relé
    digitalWrite(RELE_PIN, LOW);  // Desactivar el relé
    
    // Desactivar el botón por un tiempo para evitar rebotes
    delay(tiempoDesactivacionBoton);
  }

  // Actualizar el estado anterior del botón
  estadoBotonAnterior = estadoBoton;
}
