#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>

#define RST_PIN 9
#define SS_PIN  8

MFRC522 mfrc522(SS_PIN, RST_PIN);

char previousKey[50];
unsigned int nextKeyTime;
bool switchedOFF = true;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress serverip(192, 168, 178, 131);
IPAddress ip(192, 168, 178, 14);

EthernetClient server_client;
EthernetServer server(80);

void serverRequest(char* data, char* url);
void dump_byte_array(byte *buffer, byte bufferSize);
void getLOCValue();
void setLOCDuration();
void succesResponse(EthernetClient client);

void setup() {
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);

    Serial.begin(9600);
    while (!Serial);

    Ethernet.begin(mac, ip);

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
        }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Ethernet cable is not connected.");
    }

    server.begin();
    Serial.print("Server started on: ");
    Serial.println(Ethernet.localIP());
    delay(500);

    mfrc522.PCD_Init();
}

void loop() {
    EthernetClient client = server.available();
    if(client) {
        
        Serial.println("New Client");
        boolean currentLineIsBlank = true;
        char buffer[100] = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if(strlen(buffer) < 99) { 
                    sprintf(buffer, "%s%c", buffer, c);
                }
                Serial.write(c);

                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    // output the value of each analog input pin
                    for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
                        int sensorReading = analogRead(analogChannel);
                        client.print("analog input ");
                        client.print(analogChannel);
                        client.print(" is ");
                        client.print(sensorReading);
                        client.println("<br />");
                    }
                    client.println("</html>");
                    break;
                }
                if (c == '\n') {
                    if(strncmp(buffer, "GET /", 5) == 0) {
                        char* urlStart = strstr(buffer, "GET ") + 5;
                        char* urlEnd   = strstr(buffer, " HTTP/1.1");
                        if(urlStart != nullptr && urlEnd != nullptr) {
                            int dataSize = urlEnd - urlStart;
                            char* url = malloc(dataSize+2); 
                            strncpy(url, urlStart, dataSize);
                            url[dataSize] = '\0';
                            if(strlen(url) == dataSize) {
                                if(strcmp(url, "LOC/ping") == 0) {
                                    getLOCValue();
                                    succesResponse(client);
                                    break;
                                } else if (strcmp(url, "LOC/duration") == 0) {
                                    setLOCDuration();
                                    succesResponse(client);
                                    break;
                                } else {
                                    client.println("HTTP/1.1 400 BAD REQUEST");
                                    client.println("Content-Type: text/plain");
                                    client.println("Connection: close");  // the connection will be closed after completion of the response
                                    client.println();
                                    client.println("Bad Request Please check url");
                                    client.println(url);
                                    break;
                                }
                            } else {
                                client.println("HTTP/1.1 400 BAD REQUEST");
                                client.println("Content-Type: text/plain");
                                client.println("Connection: close");  // the connection will be closed after completion of the response
                                client.println();
                                client.println("Bad Request Please check url");
                                client.println(url);
                            break;
                            }
                        } else {
                            client.println("HTTP/1.1 400 BAD REQUEST");
                            client.println("Content-Type: text/plain");
                            client.println("Connection: close");  // the connection will be closed after completion of the response
                            client.println();
                            client.println("Bad Request Please check url and HTTP VERSION");
                            break;
                        }
                    } else {
                        Serial.print("Buffer: ");
                        Serial.println(buffer);
                    }

                // you're starting a new line
                strcpy(buffer, "");
                currentLineIsBlank = true;
                } else if (c != '\r') {
                // you've gotten a character on the current line
                currentLineIsBlank = false;
                }
            }
        }
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }

    int len = server_client.available();
    if (len > 0) {
        byte buffer[80];
        if (len > 80) len = 80;
        server_client.read(buffer, len);
        Serial.write("data: ");
        Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }

    if(nextKeyTime < (unsigned int)millis()) {
        nextKeyTime = 0;
    }

    if((unsigned int) millis() > nextKeyTime && switchedOFF == false) {
        switchedOFF = true;
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
    }

    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

}

void dump_byte_array(byte *buffer, byte bufferSize) {
    char key[50] = "";
    for (byte i = 0; i < bufferSize; i++) {
        strcat(key, buffer[i] < 0x10 ? " 0" : " ");
        sprintf(key, "%s%X", key, buffer[i]);
    }
    if(strcmp(key, previousKey) != 0 || (unsigned int)millis() > nextKeyTime) {
        Serial.print("Card UID: ");
        Serial.println(key);
        serverRequest(key+1, "POST /cgi-bin/arduino/sensor.cgi");
        sprintf(previousKey, key);
        nextKeyTime= millis() + 5000;
        digitalWrite(6,HIGH);
    } else {
        Serial.println("Card already used in the last 5 seconds!");
        Serial.println(nextKeyTime);
        digitalWrite(7, HIGH);
    }
    switchedOFF = false;
    mfrc522.PICC_HaltA();       // Halt PICC

}

void succesResponse(EthernetClient client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();
    client.println("I Hope I did the Thing YOU asked.");
}

void serverRequest(char* data, char* url) {
    server_client.stop();
    if(server_client.connect(serverip, 8081)) {
        Serial.println("Connecting....");
        server_client.print(url);
        server_client.println(" HTTP/1.1");
        server_client.println("HOST: 192.168.178.131:8081");
        server_client.println("User-Agent: arduino-ethernet");
        server_client.println("Content-Type: text/html");
        server_client.println("Connection: close");
        server_client.print("Content-Length: ");
        server_client.println(strlen(data)+1);
        server_client.println();
        server_client.println(data);
    } else {
        Serial.println("Not Connected");
    }
}

void getLOCValue() {
    serverRequest("LOC", "GET /cgi-bin/api/arduino.cgi");
}

void setLOCDuration() {
    // Do nothing for now
}