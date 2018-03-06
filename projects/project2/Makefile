all: srv client

srv: server.cpp
	g++ -o srv server.cpp

client: 
	echo 'python client.py $$@' > client
	chmod +x ./client

clean:
	rm -f srv client
