from machine import Pin, UART, PWM
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
        self.f = 1.0
        self.duty = 50
        if cap:
            # print("Cap FULL")
            try:
                with open("vars.txt", "r") as vars:
                    k = vars.readlines()
                    g = float(k[0])
                    self.r = int(g)
                    f = float(k[1])
                    self.f = float(f)
                    d = float(k[2])
                    self.duty = int(d)
                    sleep(0.5)
            except:
                pass

        self.uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
        self.uart1 = UART(1, baudrate=9600, tx=Pin(8), rx=Pin(9))

        self.phase = 0
        self.msg = ""
        self.t = 0.0




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
                        if 'duty' in self.msg:
                            duty = self.msg.split('=')
                            self.duty = int(duty[1])
                            # print(f'Received duty {self.duty}')
                        self.f = float(self.msg)
                        self.t = 1 / self.f
                        # wavelength = 343 / self.f
                        # print("Time Period:", self.t, "Frequency:", self.f, "Wavelength:", wavelength, "Meters")

                        self.r = (self.t * 1000000) / 2.000000
                        # print(f'Delay value is: {self.r}')

                        with open("vars.txt", "w") as vars:
                            vars.write(str(self.r) + "\n")
                            vars.write(str(self.f) + "\n")
                            vars.write(str(self.duty) + "\n")
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
                    # print(f'MSG is: {self.msg}')
                    # try:
                    self.phase = round(float(self.msg))
                    # print(f'Got phase {self.phase}')
                    sleep(0.25)
                    # except ValueError:
                    #     pass
                        # print("Number Not Valid")
                    self.msg = ""
                else:
                    self.msg += d.decode()

    def run(self):
        while True:
            if int(self.f) >= 10:
                pwm = PWM(Pin(2))
                pwm.freq(int(self.f))
                pwm.duty_u16(int(self.duty/100 * 65535))
                if self.phase >= 1:
                    pwm.deinit()
                    sleep_us(self.phase)
                    # print(f'Phase shifted ABOVE 10 hz {self.phase}')
                    self.phase = 0

            else:
                sig = Pin(2, Pin.OUT)
                if self.phase >= 1:
                    # print(f'Phase shifted BELOW 10 hz {self.phase}')
                    # print(f'Frequency is: {self.f}')
                    sleep_us(self.phase)
                    self.phase = 0
                delay = int(self.r)
                sig.high()
                sleep_us(delay)
                sig.low()
                sleep_us(delay)






if __name__ == '__main__':
    m = main()
    _thread.start_new_thread(m.freq_main, ())
    m.run()


