#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Cambia estos valores según tu configuración de red
const char* ssid = "doorLS";
const char* password = "LSDoor2023*";

// Pines de los Reles
const int relayPin1 = D3;
const int relayPin2 = D4;

ESP8266WebServer server(80);

// Variables para controlar el estado del botón
bool btn1Enabled = true;
bool btn2Enabled = true;

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
}

void handleRoot() {
  // Página principal con botones para controlar los Reles
  String html = "<html><head><title>Control de Reles</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Calibri, sans-serif; text-align: center; background-color: #014a99; color: #f8f9fa; }";
  html += "h1 { margin-top: 100px; }";
  html += "h1 { margin-bottom: 50px; }";
  html += "h3 { color: #f29419; }";
  html += "h3 { margin-top: 50px; }";
  html += "button { padding: 20px 40px; font-size: 20px; margin: 5px; background-color: #ffc107; border: none; border-radius: 8px; color: #f8f9fa; font-weight: bold; }";
  html += "button:disabled { background-color: #6c757d; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Acceso Yrigoyen</h1>";
  html += "<h3>Porton</h3>";
  html += "<button id='btn1' onclick='accionarRelay1()' " + String(btn1Enabled ? "" : "disabled") + ">Accionar</button>";
  html += "<h3>Puerta</h3>";
  html += "<button id='btn2' onclick='accionarRelay2()' " + String(btn2Enabled ? "" : "disabled") + ">Accionar</button>";
  html += "<script>";
  html += "function accionarRelay1() {";
  html += "if (" + String(btn1Enabled) + ") {";
  html += "var btn1 = document.getElementById('btn1');";
  html += "btn1.innerHTML = 'Accionando...';";
  html += "btn1.disabled = true;";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/relay1on', true);";
  html += "xhr.send();";
  html += "setTimeout(function() {";
  html += "btn1.innerHTML = 'Accionar';";
  html += "btn1.disabled = false;";
  html += "}, 600);"; // Después de 600 ms, volver a habilitar el botón y cambiar el texto a "Accionar"
  html += "}";
  html += "}";
  html += "function accionarRelay2() {";
  html += "if (" + String(btn2Enabled) + ") {";
  html += "var btn2 = document.getElementById('btn2');";
  html += "btn2.innerHTML = 'Accionando...';";
  html += "btn2.disabled = true;";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/relay2on', true);";
  html += "xhr.send();";
  html += "setTimeout(function() {";
  html += "btn2.innerHTML = 'Accionar';";
  html += "btn2.disabled = false;";
  html += "}, 600);"; // Después de 600 ms, volver a habilitar el botón y cambiar el texto a "Accionar"
  html += "}";
  html += "}";
  html += "</script></body></html>";
  server.send(200, "text/html", html);
}

void relay1On() {
  // Encender el Rele 1
  digitalWrite(relayPin1, LOW);
  delay(200); // Esperar 200 ms
  digitalWrite(relayPin1, HIGH); // Apagar el Rele 1
}

void relay2On() {
  // Encender el Rele 2
  digitalWrite(relayPin2, LOW);
  delay(200); // Esperar 200 ms
  digitalWrite(relayPin2, HIGH); // Apagar el Rele 2
}
