from machine import Pin, UART
from time import sleep_us, sleep
import _thread
import uos
uart = UART(0, 9600)

led2 = Pin(2, Pin.OUT)
led3 = Pin(3, Pin.OUT)

on_LED = Pin(25, Pin.OUT)

on_LED.high()


print("PICO MAIN")

r = 500

try:
    with open("vars.txt", "r") as vars:
        k = vars.readlines()
        g = float(k[0])
        r = int(g)
        print(type(g),g)
        sleep(0.5)
except:
    pass


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
                wavelength = 343/f
                print("Time Peroid:",t,"Frequency:",f,"Wavelength:",wavelength,"Meters")
                
                r = (t*1000000)/2
                print(r)
                with open("vars.txt", "w") as vars:
                    vars.write(str(r))
                    sleep(0.25)
            except ValueError:
                print("Number Not Valid")
                    
            
        
 
def run():
    global r
    while True:
        delay = r
        delay = int(delay)
        print(delay)
        led2.high()
        led3.low()
                    
        sleep_us(delay)
                    
        led2.low()
        led3.high()
                    
        sleep_us(delay)
    

        
if __name__ == '__main__':
    _thread.start_new_thread(freq_main,())
    run()
    

    
    