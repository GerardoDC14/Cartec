import serial, time, json

cad = 0
brake = 0

arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=1, write_timeout=1)


def showMenu():
    print('Select the light')


if __name__ == '__main__':
    while True:
        showMenu()
        option = input('Light detected: ').upper()
        if option == 'G':
           cad = '0'
           arduino.write(cad.encode('ascii'))
        elif option == 'R':
           brake = float(input('Select a value between .1 to 1: '))
           cad = str(brake)
           arduino.write(cad.encode('ascii'))
        elif option == 'X':
            print('Shutting down... \n')
            break
        else:
            print('Wrong input \n')
        # Write string data serial
        #Sarduino.write(cad.encode('ascii'))
        time.sleep(1)
        # Print serial message
        try:
            ## Read the encoded string
            raw_string_b = arduino.readline()
            ## Decode the string 
            raw_string_s = raw_string_b.decode('ascii')
            raw_string_j = json.loads(raw_string_s)
            print(raw_string_j)        
        except:
            print("Exception occurred, somthing wrong...")
    arduino.close()
