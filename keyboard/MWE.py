import serial
import sys
import time

device = 'COM6'
baud = 9600
comms = serial.Serial(device, baud)
threshold = 100

class EMGStream(object):
	def sample(self):
		while True:
			val = self.try_float(comms.readline().decode("utf-8", "ignore").strip('\n').strip('\r'))
			print(str(val))
			
	def try_float(self, s):
		try:
			return float(s)
		except:
			return 0

if __name__ == '__main__':
	try:
		EMG_app = EMGStream()
		EMG_app.sample()
	except Exception as e:
		print(e)
		comms.close()
