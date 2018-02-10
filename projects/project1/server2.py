# server2.py
# another more simple implementation

import readPort
import socket

def main():
	while(True):
		conn = setup()
		print conn
		chat(conn)

def chat(conn):
	while 1:
		
		data = conn.recv(512)
		
		
		# decode data
		rcv = repr(data)

		# check for terminated connection
		if (len(rcv) == 2): # empty string
			return

		# print message from client
		print rcv
		message = raw_input("enter a message: ")
		
		# handle server self-quit
		if (message == "\\quit"):
			conn.sendall("Connection terminated")
			conn.shutdown(socket.SHUT_RDWR)
			quit()

		# send server's message
		conn.sendall("server> " + message)
	conn.close()

def setup():
	host = 'localhost'
	port = readPort.getPort(True)
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((host, port))
	s.listen(1)
	print "Waiting for connection..."
	conn, addr = s.accept()
	print 'connection from ', addr
	return conn

main()