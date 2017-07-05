
import RPi.GPIO as GPIO
import time
pin = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(pin,GPIO.OUT)
p = GPIO.PWM(pin,50)
p.start(7.5)
try:
    while True :
        p.ChangeDutyCycle(7.5)
        time.sleep(1)
        p.ChangeDutyCycle(12.5)
        time.sleep(1)
        p.ChangeDutyCycle(2.5)
        time.sleep(3)
except keyboardIntertput:
        p.stop()
GPIO.cleanup()