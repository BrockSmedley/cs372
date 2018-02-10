# client.py
import socket
import readPort

def send(message, socket):
        return socket.sendall(message)
        

def connect(ip, port):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if (s.connect_ex((ip, port)) != 0):
                print "ERROR: Could not connect"
                quit()

        return s

def main():
        ip='localhost'
        
        # get port that server's using
        port = readPort.getPort()
        print port

        handle = raw_input("handle: ")[:10]
        # create socket; connect to server
        s = connect(ip, port)

        while 1:
                # send message
                message = raw_input("enter a message: ")
                if (message == "\\quit"):
                        break
                send(handle+"> "+message, s)
                data = s.recv(512)
                print repr(data)


        s.close()
        

main()
