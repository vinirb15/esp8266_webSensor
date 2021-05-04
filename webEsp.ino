#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h> 
#include <SPI.h>

const char* ssid = "wifi-id"; 
const char* password = "pass"; 
const int oneWireBus = 4;     
 
WiFiServer server(80); 
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(115200); 
    delay(10); 
    
    sensors.begin();

    Serial.println(""); 
    Serial.println(""); 
    Serial.print("Conectando a "); 
    Serial.print(ssid); 
    
    WiFi.begin(ssid, password); 
    
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500); 
        Serial.print("."); 
    }
    Serial.println(""); 
    Serial.print("Conectando "); 
    Serial.println(ssid); 
    server.begin(); 
    Serial.println("Servidor iniciado"); 
    
    Serial.print("IP: "); 
    Serial.print("http://");
    Serial.println(WiFi.localIP()); 
}
void loop() {
    sensors.requestTemperatures(); 
    float temperatureC1 = sensors.getTempCByIndex(0);
    float temperatureC2 = sensors.getTempCByIndex(1);

    WiFiClient client = server.available(); 
    if (!client) { 
        return; 
    }
    Serial.println("Um novo usuário conectou"); 
    while(!client.available()){ 
      delay(1); 
    }
    String request = client.readStringUntil('\r'); 
    Serial.println(request); 
    client.flush(); 
 
    client.println("HTTP/1.1 200 OK"); 
    client.println("Content-Type: text/html"); 
    client.println("");
    client.println("<!DOCTYPE HTML>"); 
    client.println("<html>"); 
    client.println("<h1><center>Web Sensor</center></h1>"); 
    client.println("<center>"); 
    client.println(temperatureC1); 
    client.println(" C"); 
    client.println("</center>");
    client.println("<center>"); 
    client.println(temperatureC2); 
    client.println(" C"); 
    client.println("</center>"); 
    client.println("</html>"); 
    delay(500); 
    Serial.println("Cliente desconectado"); 
    Serial.println(""); 
}
