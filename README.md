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

Data is logged by calling a HTTP API. In my implementation its a call to a promethius gateway running on a Raspberry PI server in my home network. There is Promethius and Grafana running on the same PI server. I have a dashboard on grafan that looks like this.

Image here

## Buying Parts
Since this is the first air quality monitor I am building, I had to make a few costlier choices. All sensors I am buying are precalibrated. I can use this air uality monitor as referance for my next monitor and choose to buy uncalibrated and cheaper sensors. A few things I am skipping is regular recalibration which would be needed if this was a scientific instrument.

#### Microprocessor
I am using esp32 devkit for this project. The board has a microUSB port for both programming and power. It has a programmer and a power regulator to output 3.3V. ESP32 chip uses 3.3V to run and all pins directly from the ESP#@ chip are also running at 3.3V. On the board we have pins where we can access both 5V and 3.3V for power but all other pins for communication are 3.3V only. ESP32 also has a onboard wifi and bluetooth too. This chip and board has great support for Arduino which makes it possible to program very easy.

image here

#### Dust Sensor
I picked PMS7003 sensor. It is compact and requires 5V power to operate. It has a 3.3V logic level though so good for use with the ESP32 board. Costs about INR2600 on amazon. While researching which dust sensor to use, I was checking airgradient project and it recomended PMS5003. After checkign sensors available to me for ordering i picked 7003 since it seemed to be equally good if not better. Same manufacturer and integration with esp32 is basically same.

image here

#### CO2 Sensor
For CO2 sensor I bought SCD40. Sensiron is the manufacturer and they seemed reputed enough and I was able to order one. It is precalibrated too and additionally contains temperature and humidity sensors. A thing to keep in mind with temperature sensor is that the sensor has a bunvh of heat producing components right next to it and we cannot expect the the values to be exact. Also our design for the case would influence overall temperature sensor accuracy. The sensor has options to compensate and recalibrate the temperature sensor. Automatic recalibration of CO2 sensor etc. It will be good to refer manufacturer manual to understand the sensor capabilities and usage in depth. Costs about INR2000.

Image here

#### TVOC Sensor
TVOC refers to Total Volatile Organic Compounds. This should give an Idea of paint fumes and other solvents in the air. I bought AGS02MA for this. It uses low bitrate communication which could be a problem if the chipset chosen is not compatible. ESP32 has good library support and supports the required bitrates. This sensor is also snsitive to voltage fluctuations and has peculiar startup behaviours. When switching on the reading from this sensor are too high and they need quite some time before they start getting close to real value. Sometimes it would not start after brief power cycling. I usually end up switching of the monitor for 5~10 minutes and then starting it up. Costs about INR400

Image here

#### Display
After initial trials of whole setup, I decided to add a display since its much simpler to just look at the current values on the monitor instead of having to log in to the dashboard. I had a small OLED display purchased from before. It is 128x32 pixels display and has good library support. Costs about INR300. The thing to keep in mind with OLED displays is the burn in when displaying mostly static content but for this case it is cheap enough that I did not mind running it fulltime with some burn in mitigation. It should last a couple years without any problems.

Image here.

## Integration

