from machine import Pin, UART
from time import sleep_us, sleep
import _thread

print("PICO MAIN")
on_LED = Pin(25, Pin.OUT)
on_LED.high()


class main:
    def __init__(self):
        cap = Pin(22, Pin.IN)
        sleep(1)
        self.r = 500000
        if cap:
            # print("Cap FULL")
            try:
                with open("vars.txt", "r") as vars:
                    k = vars.readlines()
                    g = float(k[0])
                    self.r = int(g)
                    # print(type(g), g)
                    sleep(0.5)
            except:
                pass

        self.uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
        self.uart1 = UART(1, baudrate=9600, tx=Pin(8), rx=Pin(9))

        self.led2 = Pin(2, Pin.OUT)
        self.led3 = Pin(3, Pin.OUT)

        self.phase = 0
        self.msg = ""
        self.t = 0.0
        self.f = 0.0


    def freq_main(self):
        while True:
            if self.uart.any():
                # print("Received UART")
                b = self.uart.read(1)
                # print(f'Raw UART data is: {b}')
                if b == b'\n':
                    self.msg = self.msg.rstrip("\r")
                    # print(f'Full UART data is: {self.msg} ')
                    try:
                        self.f = float(self.msg)
                        self.t = 1 / self.f
                        # wavelength = 343 / self.f
                        # print("Time Period:", self.t, "Frequency:", self.f, "Wavelength:", wavelength, "Meters")

                        self.r = (self.t * 1000000) / 2.000000
                        # print(f'Delay value is: {self.r}')

                        with open("vars.txt", "w") as vars:
                            vars.write(str(self.r))
                            sleep(0.25)

                        # print("---" * 10)
                        # print(f'Delay is {int(self.r)}')
                        # print(f'Frequency is {self.f}')
                        # print(f'Time is {self.t}')

                    except ValueError:
                        pass
                        # print("Number Not Valid")
                    self.msg = ""
                else:
                    self.msg += b.decode()

            if self.uart1.any():
                d = self.uart1.read(1)
                if d == b'\n':
                    self.msg = self.msg.rstrip("\r")
                    try:
                        self.phase = float(self.msg) * 1000
                        # print(f'Got phase {self.phase}')
                        sleep(0.25)
                    except ValueError:
                        pass
                        # print("Number Not Valid")
                    self.msg = ""
                else:
                    self.msg += d.decode()

    def run(self):
        while True:
            ph = int(self.phase)
            if ph >= 1:
                print("PH:", ph, "Phase:", self.phase)
                sleep_us(ph)
                self.phase = 0

            delay = int(self.r)

            self.led2.high()
            self.led3.low()

            sleep_us(delay)

            self.led2.low()
            self.led3.high()

            sleep_us(delay)


if __name__ == '__main__':
    m = main()
    _thread.start_new_thread(m.freq_main, ())
    m.run()


