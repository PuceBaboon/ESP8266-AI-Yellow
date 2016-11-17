/*
 * $Id: user_config.h,v 1.2 2016/11/17 00:05:03 anoncvs Exp $
 *
 * Configuration for local set-up (ie:- Access-point ID & PWD).
 */


/*
 * GENERAL SETUP
 */
#define DEBUG		1		// Verbose output. "#undef" to disable.
#define RUNT_MAX        3 * 60 * 1000   // Max run time (3-minutes) before auto power-off.
#define MSR_TM_DLY      2 * 1000        // Delay time (2-seconds) between LDR measurements.


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
#define WIFI_IPADDR	{ 192, 168, 172, 236 }
#define WIFI_NETMSK	{ 255, 255, 255, 0 }
#define WIFI_GATEWY	{ 192, 168, 172, 51 }
#define WIFI_DNSSRV	{ 192, 168, 172, 39 }
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
#define MQTT_HOST	"hazeltonrig.pucebaboon.com"
#define MQTT_PORT	1883
#define MQTT_BUF_SIZE	2048
#define MQTT_KEEPALIVE	120	/* In seconds. */
#define MQCONDEL	150	/* MQTT connection wait delay. */
#define MQ_TOPIC_SIZE	256	/* Characters. */
#define	TEMPR_SIZE	8	/* Temperature string conv temp storage. */
#define STRBUFF_SIZE	15	/* String buffer size. */

#define MQTT_CLIENT_ID	"Puce_%08X"
#define MQTT_USER	"Puce_USER"
#define MQTT_PASS	"Puce_PASS"

#define TOPIC1		"TIME/Epoch"
#define TOPIC2		"tdata/raw"
#define TOPIC3		"temperature"

#define MQTT_RECONNECT_TIMEOUT 	5	/* In Seconds. */

#define CLIENT_SSL_ENABLE
#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE 	1024
