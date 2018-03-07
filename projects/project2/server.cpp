/* Brock Smedley 2018 */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#define FAILURE 1337


int initAddress(int port, sockaddr_in *address){

  address->sin_family = AF_INET;
  //address->sin_addr.s_addr = "localhost";
  address->sin_addr.s_addr = INADDR_ANY;
  address->sin_port = htons(port);

  //printf("address inited\n port %d\n", ntohs(address->sin_port));

  return 1;
}

// returns socket file descriptor
int initSock(int port, struct sockaddr_in *address){
  // AF_INET: 		IPv4
  // SOCK_STREAM: 	TCP
  // 0:			IP
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  
  int option = 1;

  if (sock <= 0){
    perror("socket failure");
    exit(FAILURE);
  }

  // setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
  // lets you use same port & avoid "address already in use" error
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))){
    perror("sockopt failure");
    exit(FAILURE);
  }

  initAddress(port, address);
    
  // bind sock to port
  if (bind(sock, (struct sockaddr *)address, sizeof(*address)) < 0){
    perror("binder failure");
    exit(FAILURE);
  }
  
  return sock;
}


int serve(int sock, struct sockaddr_in *address){
  int listensock;
  int l = sizeof(*address);
  printf("addrlen: %d\n", l);

  // listen(): waits for client to connect
  if (listen(sock, 1) < 0){
    perror("listen failure");
    exit(FAILURE);
  }
  else
    printf("socket %d listening.\n", (listensock));

  // accept(): accepts a valid connection
  if ((listensock = accept(sock, (struct sockaddr *)&(*address), (socklen_t*)&l)) < 0){
    perror("accept failure");
    exit(FAILURE);
  }
  else
    printf("accepting connection on socket %d\n", (listensock));


  
  return listensock;
}


void cleanup(int sock){
  // stop sending and receiving on this socket
  shutdown(sock, 2);
}


void erase(char* s, int size){
  printf ("erasing buff\n");
  memset(s, 0, size*sizeof(*s));
}


char* ls(){
  DIR *dirp;
  struct dirent *ep;
  static char out[1024];

  dirp = opendir("./files/");
  if (dirp){
    while (ep = readdir(dirp)){
      strcat(out, ep->d_name);
      strcat(out, "\n");
    }
    (void) closedir (dirp);
  }
  else
    perror("directory listing failed");

  return out;
}

// returns an open file pointer
FILE* filep(char* filename){
  char mfn[128];
  strcpy(mfn, "./files/");
  strcat(mfn, filename);
  //printf("%s\n", filename);
  FILE *fp = fopen(mfn, "r");

  return fp;
}

int main(int argc, char** const argv){
  // INITIAL SETUP---------------------------
  // will hold info about server/port
  struct sockaddr_in address;
  int msg;
  char buff[512] = {0};
  char *howdy = "Howdy, client.";
  static char lsl[1024] = {0};

  if (! argv[1]){
    printf("missing port argument\n");
    exit(FAILURE);
  }
  
  int port = atoi(argv[1]); 
  if (!port) { // first and only arg should be port
    perror("invalid port argument");
    exit(FAILURE);
  }
  else
    printf("Starting server on port %d\n", port);
 

  // MAIN SEND-IT LOOP ------------------------------
  while (1){
    int dport;
    char sport[16];
    char cmd[16];
    char filename[16];
    char out[1024];
    bool sendfile = false;
    
    // connect to socket and start server
    int sock = initSock(port, &address);
    int servesock = serve(sock, &address);
    int i = 0;
    do {
      // read data from accepted connection
      int n = strlen(buff);
      erase(buff, strlen(buff));      
      msg = read(servesock, buff, 64);
      printf("Reply from client: %s\n", buff);
      if (strcmp(buff, "0x0") == 0)
	break;
      // send confirmation reply
      send(servesock, "OK", strlen("OK"), 0);
      printf("sent OK to client\n");
      
      if (i == 0){ // command set
	strncpy(cmd, buff, strlen(buff));
	printf("command: %s\n", cmd);
	if (! strcmp(cmd, "-l")){
	  //erase(out, sizeof(out));
	  //strcpy(out, ls());
	  if (lsl[0] == 0){
	    strcpy(lsl, ls());
	  }
	}
	else if (! strcmp(cmd, "-g")){
	  sendfile = true;
	}
      }
      else if (i == 1){ // filename set
	strncpy(filename, buff, strlen(buff));
	
      }
      else if (i == 2){ // port set
	strncpy(sport, buff, strlen(buff));
      }

      i++;
    } while (true);
    dport = atoi(sport);

    printf("command: %s\n", cmd);
    printf("filename: %s \n", filename);
    printf("data port: %s \n", sport);
    

    // Connect to client on data port
    int datasock = initSock(dport, &address);
    int dssock = serve(datasock, &address);
    FILE* fp = filep(filename);
    int lsent;

    if (sendfile){
      if(!fp) {
	// file does not exist
	send(dssock, "0xERR", 5, 0);
      }
      else{
	// while we can read data from the file
	while ((lsent = fread(out, sizeof(char), 1024, fp)) > 0){
	  // send it
	  send(dssock, out, strlen(out), 0);
	}
      }
    }
    else{
      //strcpy(out, ls());
      // send ls
      printf("sending ls...\n %s", lsl);
      send(dssock, lsl, strlen(lsl), 0);
    }

    // empty output buffer
    erase(out, strlen(out));
    
    /// end of file transfer
    cleanup(sock);
    cleanup(servesock);
    cleanup(datasock);
    cleanup(dssock);
  }

  printf("Program \"%s\" finished successfully.\n", (argv[0]));
  return 0;
}

// thanks to http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
// and https://www.geeksforgeeks.org/socket-programming-cc/
// for the boilerplate code
