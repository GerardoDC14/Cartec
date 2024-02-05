import smbus
import time
# Nvidia Jetson TX2 i2c Bus 1
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x1D

def writeNumber(value):
    bus.write_byte(address, value)
    # bus.write_byte_data(address, 0, value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    return number

while True:
    var = input("")
    if not var:
        continue

    writeNumber(var)
    number = readNumber()
