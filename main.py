from machine import Pin, ADC, UART
from time import sleep
import _thread             
uart = UART(0, 9600)

led2 = Pin(2, Pin.OUT)
led3 = Pin(3, Pin.OUT)

on_LED = Pin(25, Pin.OUT)

on_LED.high()

print("PICO MAIN")

r = 1.0

def freq_main():
    global r
    while True:
        b = uart.readline()
        sleep(0.25)
        if b:
            try:
                c = b.decode()
                f = float(c)
                t = 1/f
                tL = t/2
                wavelength = 343/f
                print("Time Peroid:",t,"Frequency:",f,"Wavelength:",wavelength,"Meters")
                
                r = tL
                
                
                
            except ValueError:
                print("Number Not Valid")
                    
            
        
 
def run():
    global r
    while True:
        
        led2.high()
        led3.low()
                    
        sleep(r)
                    
        led2.low()
        led3.high()
                    
        sleep(r)
    

        
if __name__ == '__main__':
    _thread.start_new_thread(freq_main,())
    run()
    

    
    