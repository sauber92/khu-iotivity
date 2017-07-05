import RPi.GPIO as GPIO
import time
pin=18
press_button=26

GPIO.setmode(GPIO.BCM)

GPIO.setup(press_button, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(pin,GPIO.OUT)
p = GPIO.PWM(pin,50)
p.start(0)
print("Please close the door and press the button")
try:
    GPIO.wait_for_edge(press_button, GPIO.FALLING)
    print("The door has been closed")
    p.start(0)
    p.ChangeDutyCycle(12.5)
    time.sleep(0.5)
    p.stop()
except:
    pass
GPIO.cleanup()
