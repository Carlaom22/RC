#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <pthread.h>
#include <semaphore.h> // include POSIX semaphores
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#ifndef UDP_SERVER_C_FUNCS_H
#define UDP_SERVER_C_FUNCS_H

#endif //UDP_SERVER_C_FUNCS_H
#define MAX_CLIENTES 5
#define MAX_USERS 10
#define BUFLEN 512    // Tamanho do buffer
#define PORT 9876    // Porto para recepção das mensagens
#define MAX_MARKETS 2
#define MAX_SHARES 3

typedef struct admin{
    char login[512];
    char password[512];
}admin;

typedef struct acao{
    char nome[512];
    float preco_venda;
    float preco_compra;
    int quantidade_venda;
    int quantidade_compra;
}acao;

typedef struct wallet{
    int amount;
    acao *acoes;
}wallet;

typedef struct market {
    char market_name[512];
    acao market_shares[MAX_SHARES];
} market;

typedef struct client{
    char nome[512];
    char pass[512];
    float saldo;

    struct wallet wallets[MAX_MARKETS * MAX_SHARES];

    struct market *acesso[MAX_MARKETS];
}client;

int shmid;
int shmidd;
int shmrf;
int number_of_users;
bool stop;
int *refresh_value;
int numOfAc;

client *listaclientes;
market *mercados;
admin administrador;
acao share;


void insert(char *nome, char *pass, char *mercado, float saldo);
void delete(char *nome);
void show();
void inicialize_client_file(client *clnt, char *user, char *pd, float bal);
void readusers();
void inicialize_market(market *mkt, char *name);
void showPrices(char *user);
void buy(char *market, char *name, char *user, int quantidade, float preco);
void sell(char *nome, char *acao, float preco, int quantidade);
void erro(char *s);