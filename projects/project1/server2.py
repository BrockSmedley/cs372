# server2.py
# another more simple implementation

import socket
import sys

# driver function; initializes and starts chat
# increments port after client terminates
def main():
	port = int(sys.argv[1])
	while(True):
		conn = setup(port)
		#print conn
		chat(conn)
		port = port + 1;

# sends and receives data on a socket connection conn
def chat(conn):
	while 1:
		
		data = conn.recv(512)
		
		# decode data
		rcv = repr(data)
		#rcv = rcv.strip("'");

		# check for terminated connection
		if (len(rcv) == 2): # empty string
			return

		# print message from client
		print rcv.strip("\'");
		message = raw_input("enter a message: ")
		
		# handle server self-quit
		if (message == "\\quit"):
			conn.sendall("Connection terminated")
			quit()

		# send server's message
		conn.sendall("server> " + message)
	
	conn.shutdown(socket.SHUT_RDWR)
	conn.close()

# establishes a new TCP socket connection and returns the connection object conn
def setup(port):
	host = 'localhost'
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((host, port))
	s.listen(1)
	print "Waiting for connection on port %d..." % port;
	conn, addr = s.accept()
	print 'connection from ', addr
	return conn

main()
