/*
 * $Id: user_config.h,v 1.7 2016/12/01 10:28:14 anoncvs Exp $
 *
 * Configuration for local set-up (ie:- Access-point ID & PWD).
 */


/*
 * GENERAL SETUP
 */
#define DEBUG		1		// Verbose output. "#undef" to disable.
#define RUNT_MAX        3 * 60 * 1000   // Max run time (3-minutes) before auto power-off.
#define MSR_TM_DLY	5 * 1000	// Delay time (5-seconds) between LDR measurements.
#define MQTT_TM_DELAY	30 * 1000	// Delay between MQTT publishings (30-seconds).
#define LOCATION	"Nagano, JAPAN"	// Your specific location here (will be published).


/*
 * ESP8266 SETUP  --  Most of this is specific to the AI-Thinker
 *                    "Yellow Development Board".
 */
#define	POWER_SW	16		// Power-on latch MOSFET.
#define	BLED_PIN	13		// Blue-LED on ESP pin GPIO5.
#define	GLED_PIN	12		// Green-LED on ESP pin GPIO14.
#define	RLED_PIN	15		// Red-LED on ESP pin GPIO12.
/*
 * The following LEDs are in a row of six across the
 * bottom of the board (and all are red).
 */
//#define RLED_1		16	// L/H side of board, next to RGB LED. /* Aka POWER */
#define RLED_2		14
#define RLED_3		4
#define RLED_4		5
#define RLED_5		0
#define RLED_6		2		// R/H side of board, next to prog jumper.


/*
 * WIFI SETUP
 */

#define STA_SSID	"YOUR_ACCESS_POINT"	// !!You MUST change this!!
#define STA_PASS	"YOUR_AP_PASSWORD"	// !!You MUST change this!!
#define STA_TYPE	AUTH_WPA2_PSK

/* Assign a static IP address to your ESP8266 */
#define WIFI_IPADDR	{ 192, 168, 170, 2 }	// !!You SHOULD change this to match your local network!!
#define WIFI_NETMSK	{ 255, 255, 255, 0 }	// !!You SHOULD change this to match your local network!!
#define WIFI_GATEWY	{ 192, 168, 170, 1 }	// !!You SHOULD change this to match your local network!!
#define WIFI_DNSSRV	{ 192, 168, 170, 1 }	// !!You SHOULD change this to match your local network!!
#define WIFI_CHANNEL	6		// !!You MUST change this!! -- Restart might be unreliable without this set.

/*
 * Number of times to test WiFi connection status before 
 * giving up and going back into deep-sleep (this is to
 * prevent the battery being flattened by an unavailable
 * access-point, or other, external problems.
 */
#define WIFI_RETRIES	20


/*
 * MQTT SETUP
 */
/*
 * The "broker.hivemq.com" server is a free, publicly accessible MQTT
 * service with a "dashboard" web monitor available at:-
 *               http://www.mqtt-dashboard.com/
 * If you don't have your own MQTT server, you can use this service to
 * subscribe and publish.  However, please do notethat HiveMQ gets
 * very, very busy at times (it's not uncommon to see in excess of 1,000
 * active clients at certain times of the day), so it can actually be quite
 * difficult to spot your topics popping up on the dashboard.
 */
// #define MQTT_HOST	"YOUR.MQTT.SERVER	// !!You MUST change this!!"
#define MQTT_HOST	"broker.hivemq.com"	/* Publicly accessible server. */
#define MQTT_PORT	1883
#define MQTT_BUF_SIZE	2048
#define MQTT_KEEPALIVE	120	/* In seconds. */
#define MQCONDEL	150	/* MQTT connection wait delay. */
#define MQ_TOPIC_MAX	256	/* Maximum size in characters. */
#define	TEMPR_SIZE	8	/* Temperature string conv temp storage. */
#define STRBUFF_SIZE	15	/* String buffer size. */

#define MQTT_CLIENT_ID	"YDB_%08X"	/* Yellow Dev Board + Unique ID */
// #define MQTT_USER	"ESP8266_USER"	/* Unused in this version. */
// #define MQTT_PASS	"ESP8266_PASS"	/* Unused in this version. */

/*
 * The following topics, "timestamp" and "Yellow/LDR" are suggested 
 * for demo use with the free, "broker.hivemq.com", public server. 
 * Change them to whatever you want if you're using your own, local
 * broker.
 */
// #define TOPIC1		"YOUR/FIRST/TOPIC"	// !!You MUST change this!!"
// #define TOPIC2		"YOUR/SECOND_TOPIC"	// !!You MUST change this!!"
// #define TOPIC3		"YOUR_THIRD_TOPIC"	// !!You MUST change this!!"
#define TOPIC1		"timestamp"
#define TOPIC2		"Yellow/LDR"

#define MQTT_RECONNECT_TIMEOUT 	5	/* In Seconds. */

#define CLIENT_SSL_ENABLE
#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE 	1024
