# zero-box
Dual H-Bridge using TC4420's and MOSFETS with stereo control using RasPi Pico's as drivers and an Arduino Mega for the interface. 

I am hoping this tool to be useful, cheap and easy to use for anyone looking for an adjustable high power frequency stereo output device. Some useages include driving speakers, pizeo electric devices or coils of wire. 


Version 1.0.0

Materials:  

            4x PNP IRF9530    MOSFET
            4x NPN IRF530     MOSFET
            4x TC4420         DRIVER
            8x 1N4004         DIODE
            1x L7805          5 VOLT VOLTAGE REGULATOR
            4x 0.1uF          CAPACITOR
            20x 10K ohm       RESISTOR
            3x 3.3K           RESISTOR
            3x 2.2K           RESISTOR
            2x 2N3904         TRANSISTOR
            28AWG FLAT CABLE  WIRE
            SOLID 22 AWG      WIRE
            STRANDED 24AWG    WIRE
            SPEAKERS          TESTING
            13x PUSH BUTTONS
            1x PROTO BOARD
            2x Raspberry Pi Pico
            1x Arduino Mega 2650
            1x TFT_320QVT_9341
            
            Added in version 1.0.0:
            
            2x 1M ohm   RESISTOR
            2x 102 pF   CAPACITOR
            2x BOOST/BUCK CONVERTER
            2x 15SQ045  DIODE
            2x 100V 10A Digital Voltmeter Ammeter
            
            
            
            


I have included the UTFT libary as a zip folder, In Arduino go to 

SKETCH > INCLUDE LIBRARY > ADD ZIP LIBRARY... 

Find the UTFT.zip folder then click OK. The library will be installed and available.

Inside the zip you will also find a copy of the Creative Commons lisence. All credit for this library goes to   http://www.rinkydinkelectronics.com/library.php?id=51

Please also read their documentation for useage. You may need to change the model to fit your display. All modles can be found inside the UTFT.zip file by opening UTFT.h.

You can use your own display with an alternate libary aswell, as for now there is no support for additional displays not used in UTFT.h.

Currently there is no touch support implemented.

MORE TO COME:

I will include a  wiring diagram for everything including the H-Bridge circuit, wiring of the 3.2 inch 16 bit TFT display and the two RasPi Picos. 

USEAGE:

First menu asks to select constant, either sound or light. After it brings you to the main user interface displaying time of frequency cycle, frequency, wavelength and phase of each output. THE DEFAULT VALUES ARE SET TO 1.0. To enter a new frequency just start typing in on the keypad the desired frequency. Then it will ask which board/ output you would like to send the data to. After it will update the user interface to display the new values. To use the phase shift, press the phase shift button and it will ask for a value between 0 and 360, values over 360 can be entered. After it will ask which board/ output you would like to send the phase shift to. Then it will update the userinterface with the new values.  
