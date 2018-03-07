# Brock Smedley 2018

import os
import socket
import sys
import time

MSGSIZE = 512

def writefp(filename):
  fp = open(filename, "w+")
  return fp

def writefile(fp, data):
  fp.write(data)

def main():
  args = len(sys.argv)
  #print args

  argnames = ["server_host", "server_port", "command", "filename", "data_port"]
  filename = ""
  if (args < 5):
    print "invalid arguments:\nhave %s\nmissing %s" % (argnames[:args-1], argnames[(args-1):])
    exit(420)
  elif (args < 6 and args >= 5):
    if sys.argv[3] == "-l":
      filename  = "0"
      data_port = sys.argv[4]
    else:
      print "Invalid arguments"
      exit(420)
  elif sys.argv[3] == "-g":
    filename	= sys.argv[4]
    data_port	= sys.argv[5]
  else:
    print "Invalid arguments"
    exit(420)

  server_host 	= sys.argv[1]
  server_port 	= sys.argv[2]
  command	= sys.argv[3]
 
    
  if (filename != "" and filename != "0" and os.path.exists(filename)):
    print "File with that name already exists in this directory..."
    exit(69)
      

  host = socket.gethostname()
  print "opening control connection on port %s" % (server_port)

  csock = socket.socket()
  csock.connect((server_host, int(server_port)))
  payload = [command, filename, data_port, host]

  for i in payload: 
    csock.send(i)
    print "Reply from server: " + str(csock.recv(MSGSIZE))
  
  # send termination code
  csock.send("0x0");

  # hax
  time.sleep(0.15)
  
  # open new connection to download data
  dsock = socket.socket()
  dsock.connect((server_host, int(data_port)))
  # no payload this time

  if (not os.path.exists(filename)):
    fp = writefp(filename)
  
  while (1):
    msg = dsock.recv(MSGSIZE)
    if (msg):
      if (msg == "0xERR"):
	print "ERROR: File does not exist on server.\n"
      elif (command == "-g"):
	print "downloading %s..." % filename
	writefile(fp, msg)
      else:
	print msg
    else:
      break


  csock.close()
  print "Done!"

main()

# thanks to https://stackoverflow.com/questions/27241804/sending-a-file-over-tcp-sockets-in-python
# for the boilerplate code
