# ESP8266-AI-Yellow
Repository of ESP8266 demo programmes specific to the AI-Thinker "Yellow Development Board"

There are currently two different demos in this repository.  Both use the same basic hardware, the AI-Thinker produced "Yellow Development Board", which has been widely available on Ebay and Aliexpress for almost two years now.

The first demo is named:- `Initial_Power+LEDs.ino`

`Initial_Power+LEDs.ino` will work, even without a valid access-point and will quite happily flash the lights on your "Yellow" board and update the serial console every couple of seconds withe thhe ADC reading from the on-board LDR.  If you have a latching MOSFET power switch attached (GPIO-16 is the default drive pin), then it will switch itself off after three minutes of operation.  If you don't have the latching power-switch circuitry, it will quite happily continue running until it has exhausted the batteries.


Note that even this simple version has the Arduino OTA (Over The Air) bootloader enabled, so you should only need to flash the device via the serial connections once.  After that initial serial flash cycle you can flash via WiFi (much faster!).  Under Platformio (the build environment I currently prefer and recommend), you simply need to add one line to the bottom of your platformio.ini file, reading `upload_port = 192.168.172.236`, where the IP address is the one for you specific device on your local network.


