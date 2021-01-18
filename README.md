# Rancilio-Silvia-Arduino-PID
A hidden/internal PID for Rancilio Silvia based on Arduino.

The original temperature control in the Rancilio Silvia is not the best ever made. It fluctates from 80s to the 110s C.

![alt text](https://github.com/hbsagen/Rancilio-Silvia-Arduino-PID/blob/main/Silvia%20PID.png)

As seen in this image, an Arduino with a SSR, nad a basic PID, tuned for the Rancilio Silvia does a far better job of holding the temperature at 100 C. This project seeks to optimize this simple setup for stable temperatures and fast recovery after steaming.
