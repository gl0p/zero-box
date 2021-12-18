from machine import Pin, UART
from time import sleep_us, sleep, sleep_ms
import _thread
import uos
uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
uart1 = UART(1, baudrate=9600, tx=Pin(8), rx=Pin(9))

led2 = Pin(2, Pin.OUT)
led3 = Pin(3, Pin.OUT)

on_LED = Pin(25, Pin.OUT)

on_LED.high()


print("PICO MAIN")

r = 1000000

phase = 0

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
    global phase
    while True:
        b = uart.readline()
        sleep(0.25)
        d = uart1.readline()
        sleep(0.25)
        if d:
            e = d.decode()
            phase = float(e)*1000
            sleep(0.25)
            
            
        if b:
            try:
                c = b.decode()
                print(type(c), c)
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
    global phase
    while True:
        ph = int(phase)
        print("PH:", ph, "Phase:", phase)
        if ph >= 1:
            sleep_us(ph)
            ph = 0
            phase = 0
            
        delay = r
        delay = int(delay)
        
        led2.high()
        led3.low()
                    
        sleep_us(delay)
                    
        led2.low()
        led3.high()
                    
        sleep_us(delay)
        
if __name__ == '__main__':
    _thread.start_new_thread(freq_main,())
    run()
    

    
    