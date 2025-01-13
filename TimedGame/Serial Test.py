import time

import serial
import serial.tools.list_ports as port_list

try:
    comPortNum = 10
    #comPortNum = input("Enter the number for your com port: ")
    serialComm = serial.Serial(f"COM{comPortNum}", 115200)
    connected = True
except Exception as e:
    print(f"An excetion occured: {e}")
    print(f"Could not connect to COM{comPortNum}")

def test_com():
    query = str("110111111111111001011111")
    query = str("000100000000000000001000")
    serialComm.write(query.encode())
    time.sleep(0.5)
    data_read = serialComm.read(serialComm.in_waiting)
    print(data_read.strip())


query = str("011,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
for x in range(1):
    test_com()
