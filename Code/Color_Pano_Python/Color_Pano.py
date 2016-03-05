import serial as ser

arduino = ser.Serial('COM6', 9600)
print(arduino.name)

while(True):
    print arduino.readline()
