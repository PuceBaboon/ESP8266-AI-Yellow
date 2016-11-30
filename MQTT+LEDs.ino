/*
 * $Id: MQTT+LEDs.ino,v 1.2 2016/11/21 00:14:28 anoncvs Exp $
 *
 * ESP8266 doorbell.  MQTT required.
 *
 * NOTE - All user-specific settings (SSID/Passwd, etc)
 *        are in user_config.h.  These settings -MUST-
 *        be customized for -your- network.
 *
 * Current version includes code for fading RGB LED with a
 * look-up table to correct for human vision (to make the
 * fade appear linear to our eyes).  Google "LED gamma
 * correction" for more info).  The base code for the random
 * colour fading was submitted to the Arduino Forum by user
 * "Mike Mc" (Mike McRoberts, author of "Beginning Arduino") 
 * way back in November of 2008 and still works well with 
 * the ESP8266.
 * See:- http://forum.arduino.cc/index.php/topic,11293.0.html
 * See;- http://thearduinoguy.org/
 */
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <pgmspace.h>
#include "user_config.h"

void callback(char *intopic, byte * inpayload, unsigned int length);
long prevMsrTm = 0, prevMQTTmsg = 0, ldr_val = 0;
char clidbuff[STRBUFF_SIZE];


/*
 * Create a sequential mapping for the 
 * red-LED driver GPIO pins.
 */
// #define RLPINS       6               // Number of addressible red LEDs available.
// int pinmap[RLPINS] = { 16, 14, 4, 5, 0, 2 };
#define RLPINS	5               // Number of addressible red LEDs available.
int pinmap[RLPINS] = { 14, 4, 5, 0, 2 };

int BLED = BLED_PIN;            // Blue debug LED (set in user_config.h).
int GLED = GLED_PIN;            // Green debug LED (set in user_config.h).
int RLED = RLED_PIN;            // Red debug LED (set in user_config.h).
int PROG_SW = 0;                // The program switch is on GPIO0 (always).

//--------------- Random RGB -----------------------
float RGB1[3];
float RGB2[3];
float INC[3];

int red, green, blue;

int RedPin = RLED_PIN;
int GreenPin = GLED_PIN;
int BluePin = BLED_PIN;
//--------------------------------------------------

/* NOTE:- All of these values are configured in user_config.h.  */
const byte ip[] = WIFI_IPADDR;
const byte gateway[] = WIFI_GATEWY;
const byte netmask[] = WIFI_NETMSK;
const byte dnssrv[] = WIFI_DNSSRV;


/* ----------------- MQTT ---------------- */
/*
 * Change the defined MQTT packet size and keepalive
 * to larger values, as recommended by Theo.
 *
 * Note that the PubSubClient library should be included
 * -after- these redefines, not before.
 */
#ifdef MQTT_MAX_PACKET_SIZE
#undef MQTT_MAX_PACKET_SIZE
#endif
#define MQTT_MAX_PACKET_SIZE 1024
#ifdef MQTT_KEEPALIVE
#undef MQTT_KEEPALIVE
#endif
#define MQTT_KEEPALIVE 120
#include <PubSubClient.h>

String payload;
char buff[MQ_TOPIC_MAX];

unsigned long startMillis = 0;

WiFiClient wifiClient;
PubSubClient client(MQTT_HOST, MQTT_PORT, callback, wifiClient);


/*
 * MQTT callback routine for handling incoming messages from server.
 */
void callback(char *intopic, byte * inpayload, unsigned int length) {
    String receivedpayload;

    digitalWrite(GreenPin, HIGH);
    for (int i = 0; i < length; i++) {
        receivedpayload += (char) inpayload[i];
    }
#ifdef DEBUG
    Serial.print("MQTT topic: ");
    Serial.print(intopic);
    Serial.print(" => ");
    Serial.println(receivedpayload);
#endif
}


/*
 * MQTT reconnect loop for those occasions when the client has
 * been unexpectedly disconnected.
 */
boolean mqtt_reconnect() {
    if (client.connect((char *) clidbuff)) {
        client.subscribe(TOPIC1);       /* Defined in user_config.h. */
#ifdef DEBUG
        Serial.println("MQTT connected.");
#endif
    } else {
#ifdef DEBUG
        Serial.print("MQTT failed, rc=");
        Serial.println(client.state());
#endif
    }
    return client.connected();
}


/*
 * MQTT publish function.
 */
boolean sendmqttMsg(char *topictosend, String payload) {
    if (client.connected()) {
        unsigned int msg_length = payload.length();
#ifdef DEBUG
        Serial.print("Publishing topic + payload:  ");
        Serial.print(topictosend);
        Serial.print(" + ");
        Serial.print(payload);
        Serial.print(" (Length: ");
        Serial.print(msg_length);
        Serial.println(")");
#endif
        byte *p = (byte *) malloc(msg_length);
        memcpy(p, (char *) payload.c_str(), msg_length);

        if (client.publish(topictosend, p, msg_length, 1)) {
#ifdef DEBUG
            Serial.println("Published Successfully.");
#endif
            free(p);
            return 1;
        } else {
#ifdef DEBUG
            Serial.print("Publish Failed.");
#endif
            free(p);
            return 0;
        }
    }
}


/*
 * PWM table for producing a linear fade effect with LEDs.
 */
/* *INDENT-OFF* */
uint8_t LED_LookUp[256] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
    4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8,
    8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14,
    14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22,
    22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32,
    33, 33, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45,
    46, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78,
    80, 81, 82, 83, 85, 86, 87, 89, 90, 91, 93, 94, 95, 97, 98, 99,
    101, 102, 104, 105, 107, 108, 110, 111, 113, 114, 116, 117, 119, 121, 122, 124,
    125, 127, 129, 130, 132, 134, 135, 137, 139, 141, 142, 144, 146, 148, 150, 151,
    153, 155, 157, 159, 161, 163, 165, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 189, 191, 193, 195, 197, 199, 201, 204, 206, 208, 210, 212, 215, 217,
    219, 221, 224, 226, 228, 231, 233, 235, 238, 240, 243, 245, 248, 250, 253, 255
};
/* *INDENT-ON* */


/* 
 * Add a "yield()" into non-critical delay loops to
 * give the ESP some time for housekeeping operations
 * (just use the normal, unmodified delay() call when
 *  you have specific timing requirements).
 */
void Ydelay(ulong dcount) {
    yield();
    delay(dcount);
}


/*
 * We latch our own power on (and off) using an MOSFET pair
 * connected in parallel with the physical (momentary contact)
 * power switch. When the power switch is pressed, one of the 
 * first things the ESP8266 needs to do is latch the power on.
 * When the current iteration of this programm is done, it
 * simply turns it's own power off again.
 *
 * ChkAutoOff() will check the run time since start-up and
 * automatically power the unit off if RUNT_MAX millis is
 * exceeded.
 */
void LatchOn() {
    pinMode(POWER_SW, OUTPUT);  /* Don't forget! */
    digitalWrite(POWER_SW, HIGH);
#ifdef DEBUG
    Serial.println("Power switch latched on.");
#endif
}

void LatchOff() {
    digitalWrite(POWER_SW, LOW);
#ifdef DEBUG
    Serial.println("Power switch latched off.");
#endif
}

void ChkAutoOff() {
    unsigned long tm_now = millis();
    if (tm_now >= RUNT_MAX) {
        LatchOff();
    }
}

//--------------- Random RGB -----------------------
void RGB_loop() {
    randomSeed(analogRead(0));

    for (int x = 0; x < 3; x++) {
        INC[x] = (RGB1[x] - RGB2[x]) / 256;
    }

    for (int x = 0; x < 256; x++) {

        red = (LED_LookUp[int (RGB1[0])] * 4) - 1023;
        green = (LED_LookUp[int (RGB1[1])] * 4) - 1023;
        blue = (LED_LookUp[int (RGB1[2])] * 4) - 1023;

        analogWrite(RedPin, red);
        analogWrite(GreenPin, green);
        analogWrite(BluePin, blue);
        Ydelay(10);

        RGB1[0] -= INC[0];
        RGB1[1] -= INC[1];
        RGB1[2] -= INC[2];
    }

    for (int x = 0; x < 3; x++) {
        RGB2[x] = random(556) - 300;
        //RGB2[x] = random(756)-500;
        RGB2[x] = constrain(RGB2[x], 0, 255);

        Ydelay(500);
    }
}


void setup() {
    int conn_tries = 0;

    /* Defines for our random RGB LED display. */
    randomSeed(analogRead(0));
    RGB1[0] = 0;
    RGB1[1] = 0;
    RGB1[2] = 0;
    RGB2[0] = random(256);
    RGB2[1] = random(256);
    RGB2[2] = random(256);


    /* Before we do anything else, latch the power on. */
    LatchOn();

    /* Initialize the ADC pin as an input. */
    pinMode(A0, INPUT);

    /* Initialize all red LED pins as outputs. */
    int pmi;
    for (pmi = 0; pmi < RLPINS; pmi++) {
        pinMode(pinmap[pmi], OUTPUT);

        /* Quick dash-flash to show we're alive, then all set to off. */
        digitalWrite(pinmap[pmi], LOW);
        Ydelay(30);
        digitalWrite(pinmap[pmi], HIGH);

    }
    for (pmi = (RLPINS - 1); pmi >= 0; pmi--) {
        digitalWrite(pinmap[pmi], LOW);
        Ydelay(30);
        digitalWrite(pinmap[pmi], HIGH);
    }

    /* Initialize the RGB drive pins. */
    pinMode(RedPin, OUTPUT);
    pinMode(GreenPin, OUTPUT);
    pinMode(BluePin, OUTPUT);

    /* No network connection - RED */
    digitalWrite(RedPin, LOW);


    Serial.begin(115200);
    Ydelay(500);
    Serial.println(F(""));      // <CR> past boot messages.

#ifdef DEBUG
    Serial.print(F("Reset Info: "));
    Serial.println(ESP.getResetInfo());
#endif


    /* Get a unique client-ID for this ESP8266. */
    sprintf(clidbuff, MQTT_CLIENT_ID, ESP.getChipId());

#ifdef DEBUG
    Serial.print(F("   ClientID: "));
    Serial.println(clidbuff);
    Serial.println();
    Serial.print(F("Connecting to "));
    Serial.println(STA_SSID);
#endif


    /*
     * An open-ended loop  here will flatten the battery on
     * the sensor if, for instance, the access point is 
     * down, so limit our connection attempts.
     */
    Serial.print("WiFi Stat: ");
    Serial.println(WiFi.status());      // Reputed to fix slow-start problem.
    WiFi.mode(WIFI_STA);
    WiFi.config(IPAddress(ip), IPAddress(gateway),
                IPAddress(netmask), IPAddress(dnssrv));
    WiFi.begin(STA_SSID, STA_PASS, WIFI_CHANNEL, NULL);
    yield();

    while ((WiFi.status() != WL_CONNECTED)
           && (conn_tries++ < WIFI_RETRIES)) {
        digitalWrite(GreenPin, LOW);
        Ydelay(100);
        digitalWrite(GreenPin, HIGH);
#ifdef DEBUG
        Serial.print(".");
#endif
    }
    if (conn_tries == WIFI_RETRIES) {
#ifdef DEBUG
        Serial.println(F("No WiFi connection ...sleeping."));
#endif
        ESP.reset();
    }

    /* Network connection established - GREEN */
    digitalWrite(GreenPin, LOW);
    digitalWrite(RedPin, HIGH);

#ifdef DEBUG
    Serial.println();
    WiFi.printDiag(Serial);
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());
#endif


    /* MQTT initialization - YELLOW */
    digitalWrite(GreenPin, LOW);
    digitalWrite(RedPin, LOW);
    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(callback);



    /* OTA (Over The Air) code.  DO NOT DELETE!! */
/* *INDENT-OFF* */
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    digitalWrite(GreenPin, LOW);
    digitalWrite(RedPin, LOW);
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    digitalWrite(GreenPin, HIGH);
    digitalWrite(RedPin, HIGH);
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
/* *INDENT-ON* */
    /* End of OTA code. */
}


void loop() {
    ArduinoOTA.handle();
    yield();
    RGB_loop();
    yield();

    /* Read the LDR attached to the ADC pin. */
    long now = millis();
    if (now - prevMsrTm > MSR_TM_DLY) {
        prevMsrTm = now;
        ldr_val = analogRead(A0);
#ifdef DEBUG
        Serial.print("LDR value: ");
        Serial.println(ldr_val);
#endif
    }
    if (now - prevMQTTmsg > MQTT_TM_DELAY) {
        prevMQTTmsg = now;
        snprintf(buff, MQ_TOPIC_MAX, "Yellow %s in %s. LDR reads: %d", clidbuff, LOCATION, ldr_val);
#ifdef DEBUG
        Serial.println(buff);
#endif
        sendmqttMsg((char *) TOPIC2, buff);
    }

    ChkAutoOff();
    yield();

    if ((WiFi.status() == WL_CONNECTED) && (!client.connected())) {
        mqtt_reconnect();
    }
    client.loop();
    yield();
}
