# Smartch
A repository about building an air quality monitor with a SPS30 particulate matter sensor and 4-digit 7-segment display.

# Components Used
- Seeed Studio XIAO ESP32C3
- SPS30 PM2.5 Sensor
- 4-digit 7-segment Displa
- 74HC595 Shift Register
- - RGB LED
- Push Button
- - 4 x 330 ohms Resistors for 4-digit 7-segment Display
- 2 x 220 ohms Resistors for RGB LED
- 10k Resistor for Pull-Up Resistor in Button
- Perfboard to Solder Onto

# Circuit Diagram
Note that SPS30 is not on the schematic, but you should wire them appropriately to pins GND, 5V, SDA and SCL.

<img src="https://github.com/alexew0923/Air_Quality_Monitor/blob/main/Circuit_Diagram.png" width=50% height=50%>

# Functions
It measures concentration and number of particulate matter of size PM1.0, PM2.5, PM4.0, and PM10.0 and typical size of the particles. (for reference, PM2.5 is equal to 2.5 micrometers)
Displays the readings on to the 4-digit 7-segment display and lights up appropriate LED color based on an appropriate range (line 284 & 293 in .ino file) referenced to the chart below.

<img src="https://github.com/alexew0923/Air_Quality_Monitor/blob/main/PM_Standard.jpeg" width=50% height=50%>
Credit: ATMO https://atmotube.com/blog/particulate-matter-pm-levels-and-aqi

# Final Product
I cut out a case with cardboard to cover the wires and glued four magnets on both the case and SPS30 to make the sensor detachable from the case.

<img src="https://github.com/alexew0923/Air_Quality_Monitor/blob/main/Air_Quality_Monitor.jpeg" width=50% height=50%>
