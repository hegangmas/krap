##from time import sleep
##
##import serial
##
##ser = serial.Serial('COM8', 9600) # Establish the connection on a specific port
##
##counter = 32 # Below 32 everything in ASCII is gibberish
##
##while True:
##     counter +=1
##     ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
##     print ser.readline() # Read the newest output from the Arduino
##     sleep(.1) # Delay for one tenth of a second
##     if counter == 255:
##         counter = 32
##
##
##import serial
##
##i=0
##for i in rang(0:1000):
##    try:
##        ser = serial.Serial("COM8", 9600, timeout=1)
##        ser.close()
##        ser.open()
##        time.sleep(3)
##        read_val = ser.read(size=64)
##        print read_val
##        if read_val is not '':
##            print port
##    except serial.SerialException:
##        continiue
##        
from time import sleep

import serial

ser = serial.Serial('COM8', 9600) # Establish the connection on a specific port
#counter = 32 # Below 32 everything in ASCII is gibberish

#ser.write(str(chr(counter)))
while True:
    #counter +=1
    recv=ser.readline() # Read the newest output from the Arduino
    print recv # Read the newest output from the Arduino
    #ser.write(str(chr(recv))) # Convert the decimal number to ASCII then send it to the Arduino
    #ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
    sleep(0.1) # Delay for one tenth of a second
    #if counter == 255:
    #    counter = 32

