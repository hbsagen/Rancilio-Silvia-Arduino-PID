# Rancilio-Silvia-Arduino-PID
A hidden/internal PID for Rancilio Silvia based on Arduino.

The original temperature control in the Rancilio Silvia is not the best ever made. It fluctuates from 80s to the 110s C.

![alt text](https://github.com/hbsagen/Rancilio-Silvia-Arduino-PID/blob/main/Silvia%20PID.png)

As seen in this image, an Arduino with a SSR, and a basic PID, tuned for the Rancilio Silvia does a far better job of holding the temperature at 100 C. This project seeks to optimize this simple setup for stable temperatures and fast recovery after steaming.

Key features of the Arduino PID for Rancilio Silvia:

1 - Completely hidden, no displays, no buttons. My goal is to keep the stock look of the Silvia.
2 - Easy to build
3 - Fast recovery from both cold starts and steaming.
4 - Custom algorithm for the "Ready light", it can't be based on the boiler as the SSR use the boiler more frequently than the stock setup.
5 - Original buttons work as normal
6 - No limits on how long it can stay powered on.
7 - No additional procedures

Features that I might add:

1 - Log to SD card (already implemented, but I will remove it for simplicity and upload this version as "custom")
2 - Display for the temperature.
3 - Boiling point based on actual atmospheric pressure (I don't know yet if this is of any use for making better espresso)
4 - USB port for adjusting/tuning and logging

I will upload the Arduino file, schematics and more information as I complete them.
