import serial

i=0
for modem in PortList:
    for port in modem:
        try:
            ser = serial.Serial(port, 9600, timeout=1)
            ser.close()
            ser.open()
            ser.write("ojkakkaj")
            time.sleep(3)
            read_val = ser.read(size=64)
            print read_val
            if read_val is not '':
                print port
        except serial.SerialException:
            continue
        i+=1
