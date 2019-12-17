/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

// NetPie Receive part
#include <AuthClient.h>
#include <MQTTClient.h>
#include <SHA1.h>
#include <Arduino.h>
#include <EEPROM.h>

// stm32 communication part
#include <SoftwareSerial.h>
SoftwareSerial s(3,1);
#define RcvCount  10
#define Tempo     1

String UARTrcvData;
char UARTrcvBuffer;
int UARTrcvCount;

String UARTtrnData;
char UARTtrnBuffer;
int UARTtrnCount;

#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = "zetar";
const char* password = "1234554321";

#define APPID   "Coolant"
#define KEY     "YhOtKikv34mAspG"
#define SECRET  "GeqBNs3BO9etwxAS9BATkFiwT"
#define ALIAS   "esp8266"

WiFiClient client;
// NetPie Receive part
AuthClient *authclient;

int timer = 0;
MicroGear microgear(client);


/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';

    // NetPie Receive Part
    char strState[msglen];
    for (int i = 0; i < msglen; i++)
    {
       strState[i] = (char)msg[i];
       Serial.print((char)msg[i]);
    }
    
    // comment out old code : Serial.println((char *)msg);
    Serial.println();

    String stateStr = String(strState).substring(0, msglen);

    // Processing Input//////////////////////////////////////////////////////
    Serial.println("stateStr ::==");
    Serial.println(stateStr);
    s.print(stateStr);

    //if (stateStr ==
    ///////////////////////////////////////////////////////////////////////////
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}


void setup() {
    /* Add Event listeners */
    /* Call onMsghandler() when new message arraives */
    microgear.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    microgear.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    microgear.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    microgear.on(CONNECTED,onConnected);

    Serial.begin(9600);
    s.begin(115200);
    Serial.println("Starting...");

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        // stm32 communication part ///////////////////////////////////////////////////////
        UARTrcvData = Serial.read();

      
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            /*Serial.println("Publish...");*/

            /* Chat with the microgear named ALIAS which is myself */
            /*/////////////////////////Here///////////////////////////////////////////////*/
            microgear.chat("freeboard","20.19.18.17.16");
            /////try publishing
//            microgear.publish("/esp8266/temp1", 15);
//            microgear.publish("/esp8266/temp2", 16);
//            microgear.publish("/esp8266/fan", 17);
//            microgear.publish("/esp8266/pump", 18);
            timer = 0;
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
