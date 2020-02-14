#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress serverip(192, 168, 178, 131);
IPAddress ip(192, 168, 178, 14);

EthernetClient server_client;
EthernetServer server(80);

void setup() {

    Serial.begin(9600);
    while (!Serial);

    Ethernet.begin(mac, ip);

    delay(500);

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
}

void loop() {
    EthernetClient client = server.available();
    if(client) {
        
        Serial.println("New Client");
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
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
                // you're starting a new line
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
        Serial.println("client disconnected\nConnecting to server");
        connectToServer();
    }

    int len = server_client.available();
    if (len > 0) {
        byte buffer[80];
        if (len > 80) len = 80;
        server_client.read(buffer, len);
        Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }

}

void connectToServer() {
    server_client.stop();
    if(server_client.connect(serverip, 8081)) {
        Serial.println("Connecting....");
        server_client.println("POST /cgi-bin/arduino/sensor.cgi HTTP/1.1");
        server_client.println("HOST: 192.168.178.131:8081");
        server_client.println("User-Agent: arduino-ethernet");
        server_client.println("Content-Type: text/html");
        server_client.println("Connection: close");
        server_client.println("Content-Length: 11");
        server_client.println();
        server_client.println("A0 B4 F3 BB");
    } else {
        Serial.println("Not Connected");
    }
}