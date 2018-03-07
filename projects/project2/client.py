# Brock Smedley 2018

import os
import socket
import sys
import time

def writefile(filename, data):
  fp = open(filename, "w+")
  fp.write(data)
  fp.close()

def main():
  args = len(sys.argv)
  #print args

  argnames = ["server_host", "server_port", "command", "filename", "data_port"]

  if (args < 5):
    print "invalid arguments:\nhave %s\nmissing %s" % (argnames[:args-1], argnames[(args-1):])
    exit(420)

  server_host 	= sys.argv[1]
  server_port 	= sys.argv[2]
  command	= sys.argv[3]
  if command == "-l":
    filename  = "00"
    data_port = sys.argv[4]
  else:
    filename	= sys.argv[4]
    data_port	= sys.argv[5]
    if (os.path.exists(filename)):
      print "File with that name already exists in this directory..."
      exit(69)
      

  #host = socket.gethostname()
  print "opening control connection on port %s" % (server_port)

  csock = socket.socket()
  csock.connect(('', int(server_port)))
  payload = [command, filename, data_port]

  for i in payload: 
    csock.send(i)
    print "Reply from server: " + str(csock.recv(256))
  
  # send termination code
  csock.send("0x0");

  # hax
  time.sleep(0.1)
  
  # open new connection to download data
  dsock = socket.socket()
  dsock.connect(('', int(data_port)))
  # no payload this time

  while (1):
    msg = dsock.recv(1024)
    if (msg):
      if (msg == "0xERR"):
	print("ERROR: File does not exist on server.\n")
      elif (not os.path.exists(filename)):
	writefile(filename, msg)
    else:
      break


  csock.close()

main()

# thanks to https://stackoverflow.com/questions/27241804/sending-a-file-over-tcp-sockets-in-python
# for the boilerplate code
