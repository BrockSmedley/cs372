import socket
import sys


def main():
  args = len(sys.argv)
  #print args

  argnames = ["server_host", "server_port", "command", "filename", "data_port"]

  if (args < 6):
    print "invalid arguments:\nhave %s\nmissing %s" % (argnames[:args-1], argnames[(args-1):])
    exit(420)

  server_host 	= sys.argv[1]
  server_port 	= sys.argv[2]
  command	= sys.argv[3]
  filename	= sys.argv[4]
  data_port	= sys.argv[5]

  #host = socket.gethostname()
  host = "client"
  print "%s opening control connection on port %s" % (host, server_port)

  csock = socket.socket()
  csock.connect(('', int(server_port)))

  csock.send("Howdy, server.");
  print "Reply from server: ", csock.recv(1024)


main()
