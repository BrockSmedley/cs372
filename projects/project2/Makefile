all: srv client dir

srv: server.cpp
	g++ -o srv server.cpp

dir:
	mkdir -p ./files
	echo "lorem ipsum potato potahto" > ./files/data.txt

client: 
	echo 'python client.py $$@' > client
	chmod +x ./client

clean:
	rm -f srv client
