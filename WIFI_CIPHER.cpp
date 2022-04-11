#include <WiFi.h>
#include <PubSubClient.h>

//BLINKING LED//
#define ONBOARD_LED  2

//CONFIGURAMOS ACCESO A WIFI//
const char* ssid     = "virus peligroso";
const char* password = "nomelase"; 
const char *mqtt_server = "52.23.115.184";
const int mqtt_port = 1883;
const char *mqtt_user = "web_client";
const char *mqtt_pass = "121212";
//CONFIGURAMOS MQTT LIBRERIA//
WiFiClient espClient;
PubSubClient client(espClient);
//ASIGNAMOS VARIABLES
long lastMsg = 0;
char msg[25];
//*** DECLARACION FUNCIONES ***
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
String createJsonString();
//*** DECLARACION FUNCIONES ***

float h;
float t;

void setup() {
    pinMode(ONBOARD_LED, OUTPUT);
    Serial.begin(115200);
    randomSeed(micros());
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    }

void loop() {
    if (!client.connected()) {
        reconnect();
    }

    client.loop();
    h = random(30, 70);
    t = random(10, 40);

    long now = millis();
    if (now - lastMsg > 500){
        lastMsg = now;
        String to_send = String(h) + "," + String(t);
        to_send.toCharArray(msg, 25);
        Serial.print("Publicamos mensaje -> ");
        Serial.println(msg);
        client.publish("values", msg);
        long rssi = WiFi.RSSI();
        Serial.print("Received Signal Strength Indicator =");
        Serial.println(rssi);
    }

    delay(1000);
    String jsonToSend = createJsonString();
    Serial.println(jsonToSend);

}



//***    CONEXION WIFI      ***
void setup_wifi(){
    delay(10);
    // Nos conectamos a nuestra red Wifi
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Conectado a red WiFi!");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length){
    String incoming = "";
    Serial.print("Mensaje recibido desde -> ");
    Serial.print(topic);
    Serial.println("");
    for (int i = 0; i < length; i++) {
        incoming += (char)payload[i];
    }
    incoming.trim();
    Serial.println("Mensaje -> " + incoming);

    if ( incoming == "on") {
        digitalWrite(ONBOARD_LED, HIGH);
    } else {
        digitalWrite(ONBOARD_LED, LOW);
    }
}

void reconnect() {

    while (!client.connected()) {
        Serial.print("Intentando conexión Mqtt...");
        // Creamos un cliente ID
        String clientId = "esp32_";
        clientId += String(random(0xffff), HEX);
        // Intentamos conectar
        if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
            Serial.println("Conectado!");
            // Nos suscribimos
            client.subscribe("led1");
            client.subscribe("led2");
        } else {
            Serial.print("falló :( con error -> ");
            Serial.print(client.state());
            Serial.println(" Intentamos de nuevo en 5 segundos");

            delay(5000);
        }
    }
}

String createJsonString()
{
    String data = "{";
    data+="\"humidity\":";
    data+=String(h, 2);
    data+=",";
    data+="\"temperature\":";
    data+=String(t, 2);
    data+="}";

  return data;
}