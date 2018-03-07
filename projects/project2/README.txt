#run `make` in this directory to compile the server and create a convenience script for the client

#start the server by running:
$	./srv <port>

#start the client by running: 
$	./client <host> <port> <command> [filename] <data_port>

# ./client is simply an alias for `python client.py`

# IMPORTANT: the server looks for files in a subdirectory "./files/"
# ALSO	   : make sure your directory and file has public file permissions


# TLDR:
# On the server host flip1
	$ make
	$ ./srv 5666

# On the client host flip2
	$ make
	$ ./client flip1 5666 -l 5777
	$ ./client flip1 5666 -g data.txt 5777
