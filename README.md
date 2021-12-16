# zero-box
Dual H-Bridge using TC4420's and MOSFETS with stereo control using RasPi Pico's as drivers and an Arduino Mega for the interface. 

I have included the UTFT libary as a zip folder, In Arduino go to 

SKETCH > INCLUDE LIBRARY > ADD ZIP LIBRARY... 

Find the UTFT.zip folder then click OK. The library will be installed and available.

Inside the zip you will also find a copy of the Creative Commons lisence. All credit for this library goes to   http://www.rinkydinkelectronics.com/library.php?id=51

Please also read their documentation for useage. You may need to change the model to fit your display. All modles can be found inside the UTFT.zip file by opening UTFT.h.

You can use your own display with an alternate libary aswell, as for now there is no support for additional displays not used in UTFT.h.

Currently there is no touch support implemented.

MORE TO COME:
I will include a  wiring diagram for everything including the H-Bridge circuit, wiring of the 3.2 inch 16 bit TFT display and the two RasPi Picos. 
