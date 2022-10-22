# CameraShutterTimer
Enhanced Camera Shutter Timer Routine for @cameradatyl Arduino Timer 
Author Ian Wallace - Instagram @ian_onahillroad

This project has two versions V1 is a rewrite of the Arduino Sketch for the Cameradactyl Arduino Shutter Timer. 
V2,3,4 are my fully re-written shutter timer, You can read about the Cameradactyl 
Arduino shutter timer hardware set up on YouTube at https://youtu.be/UwOh3da_Y8s

Vesrion 2 
Described on my youToube here https://youtu.be/clALye887X4
This is my complete re-write and redesign which requires additional components

Version 3 Oct 2022
Improved Laser Saftey Switch handling

Version 4 Oct 2022
No longer has max tests limit due to Ardino memory as uses SD card ram for session data store

The timer uses a laser to sense the opening and closing of the Analog camera shutter and provide details of the time including statistics such as the average and standard deviation on the LCD panel, the test results are logged to the SD card.  The software automatically guesses the shutter speed being tested and shows the error from the expected time.

This design is a stand alone device that can be run on a power bank or small PSU as well as attached to a PC.  Safety was an important part of this design and the laser can be  switched on an off with an over-ride switch.  Laser light is dangerous to eyesight and lasers must be used with care. 

Added an indicative parts and tools list.  Please make your own check of items needed.

Added Basic hardware test program Oct 2022
