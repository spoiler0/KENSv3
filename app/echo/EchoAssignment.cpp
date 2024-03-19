#include "EchoAssignment.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <arpa/inet.h>

// !IMPORTANT: allowed system calls.
// !DO NOT USE OTHER NETWORK SYSCALLS (send, recv, select, poll, epoll, fork
// etc.)
//  * socket
//  * bind
//  * listen
//  * accept
//  * read
//  * write
//  * close
//  * getsockname
//  * getpeername
// See below for their usage.
// https://github.com/ANLAB-KAIST/KENSv3/wiki/Misc:-External-Resources#linux-manuals

int EchoAssignment::serverMain(const char *bind_ip, int port,const char *server_hello) {
  // Your server code
  // !IMPORTANT: do not use global variables and do not define/use functions
  // !IMPORTANT: for all system calls, when an error happens, your program must
  // return. e.g., if an read() call return -1, return -1 for serverMain.
  struct sockaddr_in serv_addr = {};
  struct sockaddr_in client_addr = {};
  const struct sockaddr clent_addr1 = (struct sockaddr) client_addr;
  socklen_t client_addr_len = (socklen_t)sizeof(client_addr);
  char buff[256];
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(inet_addr(bind_ip));
  serv_addr.sin_port = htons(port);
  int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(serv_sock == -1) return -1;
  if(bind(serv_sock,&serv_addr,sizeof(serv_addr))==-1) return -1;
  if(listen(serv_sock,3)==-1) return -1;
  int client_sock = accept(serv_sock, &client_addr1, client_addr_len);
  if(client_sock == -1) return -1;

  while(1) {
    memset(buff,0,256);
    int rd = read(client_sock, buff, sizeof(buff));
    if(rd == -1) break;
    submitAnswer(inet_ntop());
    int wr = write(client_sock, buff, sizeof(buff));
    if(wr == -1) break;
  }
  close(serv_sock);
  return 0;
}

int EchoAssignment::clientMain(const char *server_ip, int port,const char *command) {
  // Your client code
  // !IMPORTANT: do not use global variables and do not define/use functions
  // !IMPORTANT: for all system calls, when an error happens, your program must
  // return. e.g., if an read() call return -1, return -1 for clientMain.
  struct sockaddr_in serv_addr = {};
  char buff[256];
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(server_ip);
  serv_addr.sin_port = htons(port);
  int client_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(client_sock == -1) return -1;
  if(connect(client_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) return -1;
  while(1) {
    memset(buff,0,256);
    scanf("%255[^\n]", buff);
    int wr = write(client_sock, buff, strlen(buff));
    if(wr == -1) break;
    memset(buff,0,256);
    int rd = read(client_sock, buff, sizeof(buff));
    if(rd == -1) break;

  }
  close(client_sock);
  return 0;
}

static void print_usage(const char *program) {
  printf("Usage: %s <mode> <ip-address> <port-number> <command/server-hello>\n"
         "Modes:\n  c: client\n  s: server\n"
         "Client commands:\n"
         "  hello : server returns <server-hello>\n"
         "  whoami: server returns <client-ip>\n"
         "  whoru : server returns <server-ip>\n"
         "  others: server echos\n"
         "Note: each command is terminated by newline character (\\n)\n"
         "Examples:\n"
         "  server: %s s 0.0.0.0 9000 hello-client\n"
         "  client: %s c 127.0.0.1 9000 whoami\n",
         program, program, program);
}

int EchoAssignment::Main(int argc, char *argv[]) {

  if (argc == 0)
    return 1;

  if (argc != 5) {
    print_usage(argv[0]);
    return 1;
  }

  int port = atoi(argv[3]);
  if (port == 0) {
    printf("Wrong port number\n");
    print_usage(argv[0]);
  }

  switch (*argv[1]) {
  case 'c':
    return clientMain(argv[2], port, argv[4]);
  case 's':
    return serverMain(argv[2], port, argv[4]);
  default:
    print_usage(argv[0]);
    return 1;
  }
}
