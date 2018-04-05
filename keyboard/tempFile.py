#!/usr/bin/env python
 
from threading import Thread
import serial
import time
import collections
import struct
import pyautogui

class serialPlot:
    def __init__(self, serialPort='COM6', serialBaud=38400, dataNumBytes=2, windowLength=100, threshold=150):
        self.port = serialPort
        self.baud = serialBaud
        self.dataNumBytes = dataNumBytes
        self.rawData = bytearray(dataNumBytes)
        self.dataType = None
        self.threshold = threshold
        if dataNumBytes == 2:
            self.dataType = 'h'     # 2 byte integer
        elif dataNumBytes == 4:
            self.dataType = 'f'     # 4 byte float
        self.data = []
        self.data.append(collections.deque([0] * windowLength, maxlen=windowLength))
        self.isRun = True	
        self.isReceiving = False
        self.thread = None
        self.thread1 = None
        self.high = False
 
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
        if self.thread1 == None:
            self.thread1 = Thread(target=self.processSerialData)
            self.thread1.start()

    def shouldClick(self):
        return self.high

    def tick(self):
        self.high = False

    def processSerialData(self):
        while self.isRun:
            time.sleep(0.1)
            while self.isReceiving and self.isRun:
                data = self.rawData[0:self.dataNumBytes]
                value, = struct.unpack(self.dataType, data)
                if value > self.threshold:
                    self.high = True

    def backgroundThread(self):    # retrieve data
        time.sleep(1.0)  # give some buffer time for retrieving data
        self.serialConnection.reset_input_buffer()
        while self.isRun:
            self.serialConnection.readinto(self.rawData)
            self.isReceiving = True
 
    def close(self):
        self.isRun = False
        self.thread.join()
        self.thread1.join()
        self.serialConnection.close()
        print('Disconnected...')

 
def cycle(x, y, s, x_increment = 105, count=10):
    pyautogui.moveTo(x, y, duration=0.5)
    s.tick()
    time.sleep(0.5)
    if s.shouldClick():
        pyautogui.click()
    for i in range(10):
        s.tick()
        pyautogui.moveRel(x_increment, 0, duration=0.25)
        time.sleep(0.5) # replace this with a while loop to keep checking
        if s.shouldClick():  # ToDo: Check if it's still over the right color
            pyautogui.click()

 
def main():
    portName = 'COM6'
    # portName = '/dev/ttyUSB0'
    baudRate = 9600
    windowLength = 20
    dataNumBytes = 2        # number of bytes of 1 data point
    threshold = 50
    s = serialPlot(portName, baudRate, dataNumBytes, windowLength, threshold)   # initializes all required variables
    s.readSerialStart()     # starts background thread

    x_increment = 105
    y_increment = 95
    
    pyautogui.click(1700, 1050)

    time.sleep(3.0)

    x0, y0, h, w = pyautogui.locateOnScreen('q.png') # ToDo: Check for capital letter if lowercase not found

    try:
        while True:
            # first row
            x = x0 + h/2
            y = y0 + w/2
            cycle(x, y, s)

            # second row
            x = x0 + h/2 + 45
            y = y0 + w/2 + y_increment
            cycle(x, y, s)

            # third row
            x = x0 + h/2
            y = y0 + w/2 + 2 * y_increment
            cycle(x, y, s)

            # fourth row
            x = x0 + h/2 + 5 * x_increment
            y = y0 + w/2 + 3 * y_increment
            s.tick()
            pyautogui.moveTo(x, y, duration=0.35)
            time.sleep(0.5)
            if s.shouldClick():
                pyautogui.click()
    except KeyboardInterrupt:
        s.close()
        print('Done.\n')
    except TypeError:
        print('Could not locate keyboard letter "q".\n')
        s.close()
        print('Done.\n')

 
if __name__ == '__main__':
    main()  