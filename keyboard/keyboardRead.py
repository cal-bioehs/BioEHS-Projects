#!/usr/bin/env python
 
from threading import Thread
import serial
import time
import collections
import struct
import pyautogui

class serialPlot:
    def __init__(self, serialPort='COM6', serialBaud=38400, dataNumBytes=2, windowLength=100):
        self.port = serialPort
        self.baud = serialBaud
        self.dataNumBytes = dataNumBytes
        self.rawData = bytearray(dataNumBytes)
        self.dataType = None
        if dataNumBytes == 2:
            self.dataType = 'h'     # 2 byte integer
        elif dataNumBytes == 4:
            self.dataType = 'f'     # 4 byte float
        self.data = []
        self.data.append(collections.deque([0] * windowLength, maxlen=windowLength))
        self.isRun = True	
        self.isReceiving = False
        self.thread = None
 
        print('Trying to connect to: ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        try:
            self.serialConnection = serial.Serial(serialPort, serialBaud, timeout=4)
            print('Connected to ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        except:
            print("Failed to connect with " + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
 
    def readSerialStart(self):
        if self.thread == None:
            self.thread = Thread(target=self.backgroundThread)
            self.thread.start()
            # Block till we start receiving values
            while self.isReceiving != True:
                time.sleep(0.1)

    def getSerialData(self):
    	data = self.rawData[0:self.dataNumBytes]
    	value, = struct.unpack(self.dataType, data)
    	return value
 
    def backgroundThread(self):    # retrieve data
        time.sleep(1.0)  # give some buffer time for retrieving data
        self.serialConnection.reset_input_buffer()
        while (self.isRun):
            self.serialConnection.readinto(self.rawData)
            self.isReceiving = True
 
    def close(self):
        self.isRun = False
        self.thread.join()
        self.serialConnection.close()
        print('Disconnected...')

 
 
def main():
    portName = 'COM6'
    # portName = '/dev/ttyUSB0'
    baudRate = 9600
    window = 20
    dataNumBytes = 2        # number of bytes of 1 data point
    s = serialPlot(portName, baudRate, dataNumBytes, window)   # initializes all required variables
    s.readSerialStart()                                               # starts background thread

    try:
    	while True:
    		item = s.getSerialData()
    		print(item)
    except KeyboardInterrupt:
    	s.close()
    	print('\nDone.')
 
if __name__ == '__main__':
    main()	