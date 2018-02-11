First build the C client using make.
make

To start the server, run the following:
python server2.py 30005

In a seperate terminal, run the client
./client 30005

If the client terminates the connection while the server is still running, the server will open a new connection on the next port (incrementing by 1).

Chatting is very simple; just follow the prompts on the screen. To send a message, enter it and press the return key.
