from time import sleep

import serial

ser = serial.Serial('/dev/ttyUSB0', 9600) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish


while True:
    counter +=1
#    recv=ser.readline() # Read the newest output from the Arduino
 #   print recv # Read the newest output from the Arduino
    #ser.write(str(chr(recv))) # Convert the decimal number to ASCII then send it to the Arduino
    ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
    sleep(0.1) # Delay for one tenth of a second
    if counter == 255:
        counter = 32
