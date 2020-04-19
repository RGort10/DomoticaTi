#include <SPI.h>
#include <Ethernet.h>
#include <SharpDistSensor.h>

const byte sensorPin0 = A0;
const byte sensorPin1 = A1;

const byte medianFilterWindowSize = 5;

SharpDistSensor sensor0(sensorPin0, medianFilterWindowSize);
SharpDistSensor sensor1(sensorPin1, medianFilterWindowSize);

int count_in = 0;
int count_out = 0;
int first_detected = 2;
int last_detected = 2;
int reset = 0;
int current_amount = 0;
int last_current_amount = 0;

int detect_distance = 500;

int motorPin =  9;

unsigned int nextKeyTime = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };

IPAddress serverip(192, 168, 0, 105);
IPAddress ip(192, 168, 0, 103);

EthernetClient server_client;
EthernetServer server(80);

void serverRequest(char* data, char* url);
void dump_byte_array(byte *buffer, byte bufferSize);
void getSPDValue();
void succesResponse(EthernetClient client);
void setSPDValue(char*);

void setup() {
    pinMode(motorPin, OUTPUT);
    
    Serial.begin(9600);
    while (!Serial);
    Ethernet.begin(mac, ip);
    //Ethernet.begin(mac);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        
        while (true) {
            delay(1);
        }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Ethernet cable is not connected.");
    }

    server.begin();
    Serial.print("Server started on: ");
    Serial.println(Ethernet.localIP());
    delay(500);
}

void loop() {
    EthernetClient client = server.available();
    if(client) {
        
        //Serial.println("New Client");
        boolean currentLineIsBlank = true;
        char buffer[100] = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if(strlen(buffer) < 99) { 
                    sprintf(buffer, "%s%c", buffer, c);
                }
                //Serial.write(c);
                
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
                                if(strcmp(url, "SPD/ping") == 0) {
                                    getSPDValue();
                                    succesResponse(client);
                                    break;
                                } else if (strcmp(url, "SPD/duration") == 0) {
                                    //setSPDDuration();
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
                        //Serial.print("Buffer: ");
                        //Serial.println(buffer);
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
        delay(100);
        // close the connection:
        client.stop();
        //Serial.println("client disconnected");
        //Serial.println();
    }

    int len = server_client.available();
    if (len > 0) {
        char response[50] = "";
        char buffer[250];
        int i;
        if (len > 250) len = 250;        
        server_client.read(buffer, len);
        buffer[len] = '\0';
        while (i <len) {
            if(buffer[i] == '\n' && buffer[i+1] == '\r' && buffer[i+2] =='\n') {
                i+=6;
                while(buffer[i] != '\n' || i <= len) {
                    sprintf(response, "%s%c", response, buffer[i]);
                    i++;
                }
                if(strncmp(response, "SPD", 3) == 0) {
                    setSPDValue(response);
                } else {
                    //Serial.print("NOT THE DATA: ");
                }
                break;
            }
            i++;
        }
        //Serial.print(buffer); // show in the serial monitor (slows some boards)
    }

    /*if(nextKeyTime < (unsigned int)millis()) {
        nextKeyTime = 0;
    }*/

    if((unsigned int) millis() > nextKeyTime) {   
        nextKeyTime += 50;
        //Serial.println(nextKeyTime);
        
        unsigned int distance0 = sensor0.getDist();
        unsigned int distance1 = sensor1.getDist();
      
        //Detect first and last sensor.
        if (distance0 <= detect_distance && first_detected == 2) 
        {
          first_detected = 0;
        } 
        else if (distance0 < detect_distance && last_detected == 2) 
        {
          last_detected = 0;
        }
      
        if (distance1 <= detect_distance && first_detected == 2) 
        {
          first_detected = 1;
        } 
        else if (distance1 <= detect_distance && last_detected == 2) 
        {
          last_detected = 1;
        }
      
        //Counting of people walking past. Adds when people walk past, not when standing in front.
        if (distance0 <= detect_distance && distance1 > detect_distance && first_detected == 1 && reset == 0) 
        {
          count_in +=1;
          reset = 1;
        }
      
        if (distance0 > detect_distance && distance1 <= detect_distance && first_detected == 0 && reset == 0) 
        {
          if (count_in > count_out) 
          {
            count_out +=1;
          }
          reset = 1;
        }
      
        //Printing values and resetting first and last.
        if (distance0 > detect_distance && distance1 > detect_distance && first_detected != 2 && last_detected != 2) 
        {
          first_detected = 2;
          last_detected = 2;
          reset = 0;
      
          current_amount = count_in - count_out;
      
          if (current_amount != last_current_amount)
          {
              last_current_amount = current_amount;
  
              //SEND CURRENT_AMOUNT
              char c_a[50];
              sprintf(c_a, " %d", current_amount);
              serverRequest(c_a, "POST /cgi-bin/arduino/sensor.cgi?PPL");
          
              //Print status over serial
              Serial.println("IN:");
              Serial.println(count_in);
              Serial.println("OUT:");
              Serial.println(count_out);
              Serial.println("Currently inside:");
              Serial.println(current_amount);
              Serial.println();
          }
       }
    }
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
        //Serial.println("Connecting....");
        server_client.print(url);
        server_client.println(" HTTP/1.1");
        server_client.print("HOST: ");
        server_client.print(serverip);
        server_client.println(":8081");
        server_client.println("User-Agent: arduino-ethernet");
        server_client.println("Content-Type: text/html");
        server_client.println("Connection: close");
        server_client.print("Content-Length: ");
        server_client.println(strlen(data)+1);
        server_client.println();
        server_client.println(data);
    } else {
        //Serial.println("Server request: Not Connected");
    }
}

void setSPDValue(char* response) {
    //Serial.print("Data: ");
    int value = atoi(response+3);//response:  SPD233  READS: 233
    //Serial.println(value);
    //Serial.println();
    if(value > 255) value = 255;
    analogWrite(motorPin, value);
}

void getSPDValue() {
    serverRequest("SPD", "GET /cgi-bin/arduino/actuator.cgi?SPD");
}
