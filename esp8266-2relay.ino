#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Cambia estos valores según tu configuración de red
const char* ssid = "doorLS";
const char* password = "LSDoor2023*";

// Pines de los Reles
const int relayPin1 = D3;
const int relayPin2 = D4;

bool relay1Enabled = true; // Variable para habilitar/deshabilitar el botón de Relay 1
unsigned long lastActionTime = 0; // Variable para guardar el tiempo del último accionamiento

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Inicializar los pines de los Reles
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  // Conectar a la red Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("");

  // Esperar a que se conecte
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la red Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/relay1on", relay1On);
  server.on("/relay2on", relay2On);

  // Iniciar el servidor web
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  // Manejar las solicitudes del servidor web
  server.handleClient();
  
  // Verificar si ha pasado el tiempo de inactividad del Relay 1
  if (!relay1Enabled && (millis() - lastActionTime >= 500)) {
    relay1Enabled = true; // Habilitar el botón de Relay 1
    handleRoot(); // Actualizar la página para mostrar el botón habilitado
  }
}

void handleRoot() {
  // Página principal con botones para controlar los Reles
  String html = "<html><head><title>Control de Reles</title>";
  html += "<script>";
  html += "function accionarRelay1() {";
  html += "if (!document.getElementById('btnRelay1').disabled) {"; // Verificar si el botón está habilitado
  html += "document.getElementById('btnRelay1').disabled = true;"; // Deshabilitar el botón
  html += "document.getElementById('btnRelay1').innerText = 'Accionando...';"; // Cambiar el texto del botón
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/relay1on', true);";
  html += "xhr.send();";
  html += "}";
  html += "}";
  html += "</script></head><body>";
  html += "<h1>Control de Porton</h1>";
  html += "<h2>Rele 1</h2>";
  html += "<button id='btnRelay1' onclick='accionarRelay1()'>Accionar</button>";
  html += "<h1>Control de Puerta</h1>";
  html += "<h2>Rele 2</h2>";
  html += "<form action='/relay2on'><button>Accionar</button></form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void relay1On() {
  // Encender el Rele 1
  digitalWrite(relayPin1, LOW);
  delay(200); // Esperar 200 ms
  digitalWrite(relayPin1, HIGH); // Apagar el Rele 1
  lastActionTime = millis(); // Guardar el tiempo del último accionamiento
  relay1Enabled = false; // Deshabilitar el botón de Relay 1
}
void relay2On() {
  // Encender el Rele 2
  digitalWrite(relayPin2, LOW);
  delay(200); // Esperar 200 ms
  digitalWrite(relayPin2, HIGH); // Apagar el Rele 2
}
