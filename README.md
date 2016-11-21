# ESP8266-AI-Yellow
Repository of ESP8266 demo programs specific to the AI-Thinker "Yellow Development Board"

![yellow-dev-board-iii-thumb](https://cloud.githubusercontent.com/assets/10677593/20459431/3681a340-af04-11e6-9c0b-52d5cf59e60a.jpg)

The hardware information related to these demo programs is available at <https://esp8266hints.wordpress.com/2016/11/15/yellow-doorbelldash-project/>.  Note that these programs will work on an unmodified "Yellow Development Board", but obviously the auto power-off will not function. The Eagle board files for the auto-latching switch are available in the `Eagle` sub-directory.  The board itself is also available from OSH Park's shared projects page <https://oshpark.com/shared_projects/iFWjTLa5>

![auto-power_latch_brd-small](https://cloud.githubusercontent.com/assets/10677593/20459406/88b76ee8-af03-11e6-97fc-fa292a8005d8.png)


## Demo Programs

There are currently two different demos in this repository.  Both use the same basic hardware, the AI-Thinker produced "Yellow Development Board", which has been widely available on Ebay and Aliexpress for almost two years now.

The first demo is named:- `Initial_Power+LEDs.ino`

`Initial_Power+LEDs.ino` will work, even without a valid access-point and will quite happily flash the lights on your "Yellow" board and update the serial console every couple of seconds withe the ADC reading from the on-board LDR.  If you have a latching MOSFET power switch attached (GPIO-16 is the default drive pin), then it will switch itself off after three minutes of operation.  If you don't have the latching power-switch circuitry, it will quite happily continue running until it has exhausted the batteries.


Note that even this simple version has the Arduino OTA (Over The Air) bootloader enabled, so you should only need to flash the device via the serial connections once.  After that initial serial flash cycle you can flash via WiFi (much faster!).  Under Platformio (the build environment I currently prefer and recommend), you simply need to add one line to the bottom of your platformio.ini file, reading `upload_port = 192.168.172.236`, where the IP address is the one for your specific device on your local network.

The second demo is named:- `MQTT+LEDs.ino`

`MQTT+LEDs.ino`, as the name suggests, **must** have an accessible local network to function.  **!!IMPORTANT!!** You **must** update the settings for IP addresses and Access-Point SSID and password in the `user_config.h` file before attempting to run this demo.  The `user_config.h` file also has a default setting for an MQTT broker (server) of "broker.hivemq.com".  This is a publicly accessible MQTT service running out of Landshut, near Munich in Germany.  HiveMQ provide tutorials and support for MQTT, as well as providing this free service to allow users like you and me to experiment with the system.  Please be nice if you use this service and keep your publications down to a reasonable number per hour (also remember that anyone, anywhere can read and potentially overwrite your topics, so be circumspect in what you publish.  The web dashboard to see the data being handled by the HiveMQ server is available at ![](http://www.mqtt-dashboard.com/).  Note that the server is handling many millions of requests per day and it's not unusual to see more than 1,000 active clients at any one time, so it can be quite difficult to spot your particular data popping up on the dashboard.  A better option is probably to install a version of the Mosquitto broker on your local machine.  The Mosquitto package comes with two command-line utilities, mosquitto_pub and mosquitto_sub, which allow you to publish or subscribe to topics from your desktop (for example, `mosquitto_sub -h broker.hivemq.com -t Yellow/LDR` will subscribe to the "Yellow/LDR" topic from the HiveMQ public server.  You will see your own data as it is published, as well as anyone else's data being published to that same topic).
