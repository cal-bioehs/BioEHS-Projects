import pyautogui, time


x_increment = 105
y_increment = 95


pyautogui.click(1700, 1050)

time.sleep(3.0)

x0, y0, h, w = pyautogui.locateOnScreen('q.png')

def cycle(x, y, count=10):
	pyautogui.moveTo(x, y, duration=0.5)
	time.sleep(0.5)
	for i in range(10):
		pyautogui.moveRel(x_increment, 0, duration=0.25)
		time.sleep(0.5)

try:
	while True:
		# first row
		x = x0 + h/2
		y = y0 + w/2
		cycle(x, y)

		# second row
		x = x0 + h/2 + 45
		y = y0 + w/2 + y_increment
		cycle(x, y)

		# third row
		x = x0 + h/2
		y = y0 + w/2 + 2 * y_increment
		cycle(x, y)

		# fourth row
		x = x0 + h/2 + 5 * x_increment
		y = y0 + w/2 + 3 * y_increment
		pyautogui.moveTo(x, y, duration=0.35)
		time.sleep(0.5)

except KeyboardInterrupt:
	print('\nDone.')
