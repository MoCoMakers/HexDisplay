For use with an ESP32, and an individually addressable rgb led strip.
<br><br>
For questions please reach out at mocomakers.com

# Getting Started
Wire the a 5V power supply to the 5V pin of the ESP32, and GND. Also wire the 5V and GND to the LED strip. Attach the data line to a pin defined in your code as `PIN`.

Count the length of your strip and update `N_LEDS` to match that.

Control the power draw by setting a `MAX_BRIGHTNESS` value between 0 and 255.
