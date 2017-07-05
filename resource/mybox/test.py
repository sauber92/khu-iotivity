'''
import time
print "TEST PYTHON"
print "SLEEP.............."
time.sleep(3)
print "WAKE UP!!"
print "TEST PYTHON END"
'''
import RPi.GPIO as GPIO
import time
pin = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(pin,GPIO.OUT)
p = GPIO.PWM(pin,50)
p.start(0)

p.ChangeDutyCycle(7.5)
time.sleep(0.5)
      
p.stop()
GPIO.cleanup()
