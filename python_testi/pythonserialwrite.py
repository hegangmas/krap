import serial # if you have not already done so
import time
ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)
ser.write('5')
ser.write('1')
ser.write('3')
ser.write('3')
ser.write('9')