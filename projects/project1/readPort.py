# readPort.py
# reads and auto-increments port from my text file so I can be lazy

# inc: Boolean; true to increment and write new port #; false to leave alone
def getPort(inc=False):
	fp = open("port.txt", "r")
	p = fp.readline()
	fp.close()
	port = int(p)

	if (inc):
		port += 1
		fp = open("port.txt", "w")
		fp.write(str(port))
		fp.close()

	return port

def test():
	print getPort(False)
