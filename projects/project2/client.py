import socket
import sys
import time

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
  payload = [command, filename, data_port]

  for i in payload: 
    csock.send(i)
    print "Reply from server: " + str(csock.recv(256))
  
  # send termination code
  csock.send("0x0");

  time.sleep(0.1)
  
  # open new connection to download data
  dsock = socket.socket()
  dsock.connect(('', int(data_port)))
  # no payload this time

  while (1):
    msg = dsock.recv(1024)
    if (msg):
      print("%s " % msg)
    else:
      break


  csock.close()

main()
