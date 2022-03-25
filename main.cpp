#define TINY_GSM_MODEM_SIM800 //Tipo de modem que estamos usando
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


#define TOPIC "emergencia"

// PINOUT UTILIZADOS //
#define ONBOARD_LED  2 // PINE DEL LED (no se usa)
  // PINES DEL GPS
#define GPS_RXPin 16
#define GPS_TXPin 17
  // PINES DEL GSM
#define GSM_RXPin 4
#define GSM_TXPin 2

//Intervalo entre os envios e refresh da tela
#define INTERVAL 5000

//CONFIGURAMOS MQTT LIBRERIA//
const char *mqtt_server = "52.23.115.184";
const int mqtt_port = 1883;
const char *mqtt_user = "web_client"; // Se utiloz?
const char *mqtt_pass = "121212"; // Se utilizo?


// OBJETOS //
//Canal serial que vamos usar para comunicarmos GSM
HardwareSerial SerialGSM(1);
TinyGsm modemGSM(SerialGSM);
TinyGsmClient gsmClient(modemGSM);
//Canal serial que vamos usar para comunicarmos Gps
HardwareSerial  neogps(2); // The serial connection to the GPS device
TinyGPSPlus gps; // The TinyGPS++ object
//CONFIGURAMOS MQTT
PubSubClient client(gsmClient);
//TIEMPO DE ULTIMO ENVIO
uint32_t lastTime = 0;

float latitude; //
float longitude; //Variável onde iremos armazenar o valor da temperatura
float speed;
float temperature;
float humidity;

// DECLARAR FUNCIONES A USAR
void setupGSM();
void publishMQTT();
String createJsonString();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void readGPS();


void setup()
{
	pinMode(ONBOARD_LED, OUTPUT);
	Serial.begin(115200);
  // ESTABLECER BAUD RATE DEL MODULO GPS
  neogps.begin(9600,SERIAL_8N1, GPS_RXPin,GPS_TXPin);
  Serial.println("neogps serial initialize");
  delay(10);
  // ESTABLECER BAUD RATE DEL MODULO GSM
  SerialGSM.begin(9600, SERIAL_8N1, GSM_RXPin, GSM_TXPin, false);
  Serial.println("SIM800L serial initialize");
  delay(3000); 

  Serial.println("Initializing modem...");
  Serial.println(modemGSM.getModemInfo());
  modemGSM.restart();
  Serial.println(modemGSM.getModemInfo());

	setupGSM(); //Inicializa e configura o modem GSM
	randomSeed(micros());
  // CONFIG MQTT
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(callback);
  delay(2000);

}

void loop()
{
  //Faz a leitura da umidade e temperatura
  readGPS();
  //Se desconectou do server MQTT
  if(!client.connected())
  {
	reconnect();
  }
  client.loop();
   //Tempo decorrido desde o boot em milissegundos
  long now = millis();
  if(now - lastTime > INTERVAL)
  {
  publishMQTT();
    lastTime = now;
	delay(10000);
  }
}

void setupGSM()
{
  Serial.println("Setup GSM...");//display.println("Setup GSM...");
  //Inicializamos a serial con SIM800L
  delay(3000);
  //Mostra informação sobre o modem
  Serial.println(modemGSM.getModemInfo());
  //Inicializa o modem
  if (!modemGSM.restart())
  {
    Serial.println("Restarting GSM Modem failed");//display.println("Restarting GSM Modem failed");
    delay(10000);
    ESP.restart();
    return;
  }

  //Espera pela rede
  if (!modemGSM.waitForNetwork())
  {
    Serial.println("Failed to connect to network");//display.println("Failed to connect to network");
    delay(10000);
    ESP.restart();
    return;
  }

  //Conecta à rede gprs (APN, usuário, senha)
  if (!modemGSM.gprsConnect("", "", "")) {
    Serial.println("GPRS Connection Failed");//display.println("GPRS Connection Failed");
    delay(10000);
    ESP.restart();
    return;
  }

  Serial.println("Setup GSM Success");//display.println("Setup GSM Success");
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


void publishMQTT()
{
  //Cria o json que iremos enviar para o server MQTT
  readGPS();
  String msg = createJsonString();
  Serial.print("Publish message: ");
  Serial.println(msg);
  //Publicamos no tópico
  int status = client.publish(TOPIC, msg.c_str());
  Serial.println("Status: " + String(status));//Status 1 se sucesso ou 0 se deu erro
  
}

String createJsonString()
{
  String data = "{";
    if(!isnan(humidity) && !isnan(temperature))
    {
      data+="\"humidity\":";
      data+=String(humidity, 2);
      data+=",";
      data+="\"temperature\":";
      data+=String(temperature, 2);
    }
  data+="}";
  return data;
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

void readGPS(){ // GPS displayInfo
Serial.print("HOLAAAAAAAAA!!!!");
  while(neogps.available())
  {
    if (gps.encode(neogps.read()))
    {

      if (gps.location.isValid() ){
        latitude = (gps.location.lat());
        longitude = (gps.location.lng());
        speed = gps.speed.kmph();
        Serial.print("Latitude:  ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
        Serial.print("Speed: ");
        Serial.println(speed, 6);
      }
        else {
        Serial.println(F("INVALID"));
        }
    }
  }

}
