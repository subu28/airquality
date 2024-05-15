# Sanyaas Air Quality Monitor

> [!NOTE]
> This is a long article. Please make yourself comfortable before begining.

## Why
India is a country with pretty bad enforcement of environment protections. Almost anywhere is affected by one pollution or the other. I live in bangalore and my particular locality is quite noisy, close to a big road and has a lot of construction happening all around.

I also have some of my own behaviours to track like closing windows to block out noise which affects ventilation. I also needed to know if I should buy/build a air purifier vs keep windows open/closed.

To begin with I was looking for a few affordable monitors to purchase but as with every product on amazon I couldnt really get a listing that seemed reliable. Nor could I find a way to extend any to be able to record data and plot charts etc. Then I saw teardowns of very similar looking air quality monitors and very soon concluded that the best course of action was to build it myself.

## End Product
Image here

The monitor is Xcm * Ycm * Zcm in size. Runs on any 5V micro usb power.

Data is logged by calling a HTTP API. In my implementation its a call to a promethius gateway running on a Raspberry PI server in my home network. There is Promethius and Grafana running on the same PI server. I have a dashboard on grafana that looks like this.

Image here

## Buying Parts
Since this is the first air quality monitor I am building, I had to make a few costlier choices. All sensors I am buying are precalibrated. I can use this air quality monitor as referance for my next monitor and choose to buy uncalibrated and cheaper sensors. A few things I am skipping is the regular recalibration which would be needed if this was a scientific instrument.

#### Microprocessor
I am using esp32 devkit for this project. The board has a microUSB port for both programming and power. It has a programmer and a power regulator to output 3.3V. ESP32 chip uses 3.3V to run and all pins directly from the ESP32 chip are also running at 3.3V. On the board we have pins where we can access both 5V and 3.3V for power but all other pins for communication are 3.3V only. ESP32 also has a onboard wifi and bluetooth too. This chip and board has great support for Arduino which makes it possible to program very easy.

image here

#### Dust Sensor
I picked PMS7003 sensor for dust. Another large air quality monitoring project called airgradient uses PMS5003 which is a similar sensor from same munufacturer. It is compact and requires 5V power to operate. It has a 3.3V logic level though so good for use with the ESP32 board. There are libraries and examples available for use in arduino projects for both the sensors.

image here
datasheet here

#### CO2 Sensor
For CO2 sensor I bought SCD40. It is manufactured by Sensiron and available for me to order from amazon india. It is precalibrated and includes temperature and humidity sensors. A thing to keep in mind with temperature sensor is that the sensor has a bunch of heat producing components right next to it and we cannot expect the the values to be exact. A case design with ample air flow will help but still a temperature compensation will be required for acurate temperature readings. The sesnor has both automatic and manual recalibration options. It will be good to refer manufacturer manual to understand the sensor capabilities and usage in depth.

Image here
datasheet here

#### TVOC Sensor
TVOC refers to Total Volatile Organic Compounds. This should give an idea of paint fumes and other solvents in the air. I bought AGS02MA for this. It uses low bitrate communication which could be a problem if the controller chosen is not compatible. ESP32 has good library support and supports the required bitrates. This sensor needs to warm up before the readings stabilize. Typically after a long idle it will take about 15~20 minutes to become stable. I observed that it is sensitive to voltage fluctuations and sometimes it would not start after a quick power cycle. I usually end up switching off the monitor for 5~10 minutes before switching back on.

Image here
datasheet here

#### Display
After initial trials of whole setup, I decided to add a display since its much simpler to just look at the current values on the monitor instead of having to log in to the dashboard. I had a small OLED display purchased from before. It is 128x32 pixels display and has good library support. The thing to keep in mind with OLED displays is the burn in when displaying mostly static content but for this case it is cheap enough that I did not mind running it fulltime with some burn in mitigation. It should last a couple years without any problems.

Image here.

#### Overall Cost
This is my cost to build the first reference monitor with precalibrated parts/sensors.

| Item | Cost (INR) |
|------|---:|
| esp32 devkit | 489|
| PMS7003 | 2,599 |
| SCD40 | 1,979 |
| AGS02MA | 380 |
| OLED | 325 |
| **Total** | **5,772** |

## Integration

The program follows standard arduino model of a setup and loop function. In the setup, we start all the sensors, display and logging to serial port.

Logging to serial port helps with debugging. Once the setup is completed, the loop is started which runs once every 1 monute. This frequency helps run all sensors in passive/energy saving mode and use low power.

The overall circuit looks like this

image

#### Dust Sensor
Connect the dust sensor power and ground to 5v and ground respectively. Connect the Rx and Tx lines to pin 16 and pin 17. Then we will use the PMS7003.h library to communicate with the sensor using Serial communication. Code for this is available in src/dust.h

#### CO2 Sensor
Connect the vcc and ground to 5v and ground respectively and the sda and scl to pin 21 and pin 22 respectively. We will use the SensirionI2CScd4x.h library to connect to the sensor over I2C. Code for this is available in src/co2.h

#### TVOC Sensor
Connect the sensor power and ground to 3.3v and ground respectively and the sda and scl to pin 33 and pin 32 respectively. We will use the Adafruit_AGS02MA.h library to connect to the sensor over I2C. Code for this is available in src/tvoc.h

#### Display
Connect the vcc and ground to 5v and ground respectively and the sda and scl to pin 21 and pin 22 respectively. We will use the Adafruit_SSD1306.h library to connect to the sensor over I2C. Code for this is available in src/screen.h

#### WiFi and dashboards
Code for this is available in src/upload.h. For my monitor I have added username and password for couple of my available hotspots. A WiFi client is created which attempts to connect to the hotspots if not already connected before logging data. The data is sent to a promethius push gateway on because I want to be able to push the data instead of pulling from promethius. Then promethius ingests the data and finally grafana connects to the promethius and generates the graphs. Setting up the promethius and grafana are not part of this repo. You can modify the code to send your data to other locations as per your requirement.