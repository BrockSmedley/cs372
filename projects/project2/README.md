Run `make` in this directory to compile the server and create a convenience script for the client

Start the server by running:

`./srv <port>`

Start the client by running: 

`./client <host> <port> <command> [filename] <data_port>`

>Note: `./client` is simply an alias for `python client.py`

**IMPORTANT**: the server looks for files in a subdirectory *`./files/`*.
Make sure your directory and file has public file permissions (`chmod -R 744`).


# TLDR:
On the server host (e.g. flip1)
```
make
./srv 5666
```
On the client host (e.g. flip2)
```
make
./client flip1 5666 -l 5777
./client flip1 5666 -g data.txt 5777
```
