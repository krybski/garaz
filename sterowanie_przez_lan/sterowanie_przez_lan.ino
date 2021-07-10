/*
  Web Server

  A simple web server that shows the value of the analog input pins.
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13
   Analog inputs attached to pins A0 through A5 (optional)

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi

*/

#include <SPI.h>
#include <Ethernet.h>

#define wlacznik 5
#define swiatlo_1 6
#define swiatlo_2 7

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x7A, 0x70, 0x25
};
IPAddress ip(192, 168, 100, 230);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  pinMode(wlacznik, INPUT_PULLUP); //Przycisk jako wejście
  pinMode(swiatlo_1, OUTPUT);
  pinMode(swiatlo_2, OUTPUT);
  digitalWrite(swiatlo_2, LOW);
  digitalWrite(swiatlo_1, LOW);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // start the server
  server.begin();

}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        if (request.indexOf("/LED=ON") != -1)  {
          digitalWrite(swiatlo_1, HIGH);
        }
        if (request.indexOf("/LED=OFF") != -1)  {
          digitalWrite(swiatlo_1, LOW);
        }
        if (request.indexOf("/LED2=ON") != -1)  {
          digitalWrite(swiatlo_2, HIGH);
        }
        if (request.indexOf("/LED2=OFF") != -1)  {
          digitalWrite(swiatlo_2, LOW);
        }
        char c = client.read();
 
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          // output the value of each analog input pin
          if (digitalRead(swiatlo_1) == LOW) {
            client.println("<h1><a href=\"/LED=ON\">LED GARAZ ON</a></h1><br>");
          }
          if (digitalRead(swiatlo_1) == HIGH) {
            client.println("<h1><a href=\"/LED=OFF\">LED GARAZ OFF</a></h1><br>");
          }
          client.println("<HR>");
          if (digitalRead(swiatlo_2) == LOW) {
            client.println("<h1><a href=\"/LED2=ON\">LED ORZECH ON</a></h1><br>");
          }
          if (digitalRead(swiatlo_2) == HIGH) {
            client.println("<h1><a href=\"/LED2=OFF\">LED ORZECH OFF</a></h1><br>");
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
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
  }
  if (digitalRead(wlacznik) == LOW) { //Jeśli przycisk wciśnięty
    if (digitalRead(swiatlo_1) == LOW) {
      digitalWrite(swiatlo_1, HIGH);
      digitalWrite(swiatlo_2, HIGH);
    }
    if (digitalRead(swiatlo_1) == HIGH) {
      digitalWrite(swiatlo_1, LOW);
      digitalWrite(swiatlo_2, LOW);
    }
    delay(1000);
  }
}
