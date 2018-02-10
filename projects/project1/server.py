import SocketServer
import sys

class ChatServerHandler(SocketServer.BaseRequestHandler):
	def __init__(self, request, clientAddr, server):
		SocketServer.BaseRequestHandler.__init__(self, request, clientAddr, server)
		return

	def setup(self):
		return SocketServer.BaseRequestHandler.setup(self)


	def handle(self):
		data = self.request.recv(512)
		print data
		return

	def finish(self):
		return SocketServer.BaseRequestHandler.finish(self)


class ChatServer(SocketServer.TCPServer):
	def __init__(self, serverAddr, handlerClass=ChatServerHandler):
		SocketServer.TCPServer.__init__(self, serverAddr, handlerClass)
		return

	def activate(self):
		SocketServer.TCPServer.server_activate(self)
		return

	def serve(self):
		while True:
			self.handle_request()
		return

	def handle_request(self):
		return SocketServer.TCPServer.handle_request(self)

	def verify_request(self, request, clientAddr):
		return SocketServer.TCPServer.verify_request(self, request, clientAddr)

	def process_request(self, request, clientAddr):
		return SocketServer.TCPServer.process_request(self, request, clientAddr)

	def close(self):
		return SocketServer.TCPServer.server_close(self)

	def finish_request(self, request, clientAddr):
		return SocketServer.TCPServer.finish_request(self, request, clientAddr)

	def close_request(self, requestAddr):
		return SocketServer.TCPServer.close_request(self, requestAddr)

if __name__ == '__main__':
	import socket
	import threading
	import readPort

	# get port from kernel
	address = ('localhost', readPort.getPort(True))
	server = ChatServer(address, ChatServerHandler)
	ip, port = server.server_address
	print ip
	print port

	t = threading.Thread(target=server.serve)
	t.setDaemon(True)
	t.start()

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((ip, port))

	while True:
		response = s.recv(512)
		print response
