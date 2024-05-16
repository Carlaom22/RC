#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_CLIENTES 5
#define BUFLEN 512    // Tamanho do buffer
#define PORT 9876    // Porto para recepção das mensagens


typedef struct client{
    char *nome;
    char *pass;
    char shares[2];
    char *saldo;
}Client;

typedef struct acao{
    char *nome;
    char *preco;
}acao;

typedef struct mercado{
    char*nome;
    acao acoes[3];
}market;

typedef struct bolsa{
    market mercados[2];
}bolsa;



void start_client(char *nome, char *pass, const char acoes[2],char *saldo, Client *c){
        c->nome=nome;
        c->pass=pass;
        c->saldo=saldo;
        for(int i = 0; i<2;i++){
            c->shares[i]=acoes[i];
        }
}

void start_market(char *nome, char *preco,char*nm, market *m){
    int i=0,j=0;
    while(strcmp(m[i].nome,NULL)==0)
        i++;
    strcpy(m[i].nome,nm);
    while(strcmp(m[i].acoes[j].nome,NULL)==0)
        j++;
    strcpy(m[i].acoes[j].nome,nome);
    strcpy(m[i].acoes[j].preco,preco);

}

void start_bolsa(market *m, bolsa*b){
    int i=0;
    while(strcmp(b->mercados[i].nome,NULL)==0)
        i++;
    b->mercados[i]=*m;
}

void readusers(char *admin, char*pass)
{
    FILE *userfile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *token;
    Client j;
    bolsa b;
    market m;
    int c= 0;
    long users;
    if ((userfile = fopen("dados.txt", "r")) == NULL)
    {
        printf("Failed to open user file");
        exit(1);
    }
    while ((read = getline(&line, &len, userfile)) != -1)
    {
        if(c==0){
            c++;
            token=strtok(line,"/");
            strcpy(admin,token);
            while( token != NULL ) {
                token = strtok(NULL, "/");
                strcpy(pass,token);
            }
        }
        if(c==1){
            c++;
            char *ptr;
            users = strtol(line, &ptr, 10);
        }
        if(c>1 || c<=1+users){
            char *p = strtok (line, " ");
            char *array[3];
            int cont=0;
            while (p != NULL){
                array[cont++] = p;
                p = strtok (NULL, " ");
            }
            start_client(array[0],array[1],NULL,array[2],&j);
            c++;
        }
        else{
            char *p = strtok (line, ";");
            char *array[3];
            int cont=0;
            while (p != NULL){
                array[cont++] = p;
                p = strtok (NULL, ";");
            }
            start_market(array[1],array[2],array[0],&m);
            start_bolsa(&m,&b);
        }

    }
    fclose(userfile);
    if (line)
        free(line);
}

void erro(char *s) {
    perror(s);
    exit(1);
}

int main() {
    struct sockaddr_in si_minha, si_outra;
    socklen_t slen = sizeof(si_outra);
    int s, recv_len;
    char buf[BUFLEN];

    int controler = 0;
    Client c;
    bool first = true;
    Client clients[MAX_CLIENTES];

    char *admin = "admin";
    char *pass = "admin_password";

    // Cria um socket para recepção de pacotes UDP
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        erro("Erro na criação do socket");


    si_minha.sin_family = AF_INET;
    si_minha.sin_port = htons(PORT);
    si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

    // Associa o socket à informação de endereço
    if (bind(s, (struct sockaddr *) &si_minha, sizeof(si_outra)) == -1) {
        erro("Erro no bind");
    }

    while (1) {
        printf("\t( Menu )\nEscolha uma das seguintes opções: \n( 1 ) - Login.\n( 2 ) - Sair\n");
        //enviamos a ultima resposta ao cliente udp
        if ((recv_len = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr *) &si_outra, &slen)) == -1)
            erro("Erro a receber mensagem.");
        // Para ignorar o restante conteúdo (anterior do buffer)
        buf[recv_len - 1] = '\0';

        if (strcmp(buf, "1") == 0) {
            printf("Introduza o nome de utilizador:\n");
            if ((recv_len = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr *) &si_outra, &slen)) == -1)
                erro("Erro a receber mensagem.");
            // Para ignorar o restante conteúdo (anterior do buffer)
            buf[recv_len - 1] = '\0';
            while (strcmp(buf, admin) != 0) {
                printf("Nome errado. Introduza o nome de utilizador:\n");
                if ((recv_len = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr *) &si_outra, &slen)) == -1)
                    erro("Erro a receber mensagem.");
                // Para ignorar o restante conteúdo (anterior do buffer)
                buf[recv_len - 1] = '\0';
            }
            printf("Introduza a pass:\n");
            if ((recv_len = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr *) &si_outra, &slen)) == -1)
                erro("Erro a receber mensagem.");
            // Para ignorar o restante conteúdo (anterior do buffer)
            buf[recv_len - 1] = '\0';
            while (strcmp(buf, pass) != 0) {
                printf("Pass errada. Introduza a pass:\n");
                if ((recv_len = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr *) &si_outra, &slen)) == -1)
                    erro("Erro a receber mensagem.");
                // Para ignorar o restante conteúdo (anterior do buffer)
                buf[recv_len - 1] = '\0';
            }
            while (1) {
                printf("%d\n",controler);
                if ((recv_len = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr *) &si_outra, &slen)) == -1)
                    erro("Erro a receber mensagem.");
                // Para ignorar o restante conteúdo (anterior do buffer)
                buf[recv_len - 1] = '\0';


                char *p = strtok (buf, " ");
                char *array[5];
                int cont=0;
                while (p != NULL){
                    array[cont++] = p;
                    p = strtok (NULL, " ");
                }
                if (strcmp(array[0], "ADD_USER") == 0) {
                    if (controler > 0) {
                        bool v=true;
                        for (int i = 0; i < controler; i++) {
                            if (strcmp(clients[i].nome, array[1]) == 0) {
                                strcpy(clients[i].shares, array[3]);
                                strcpy(clients[i].saldo, array[4]);
                                v=false;
                            }
                        }
                        if(v==true) {
                            controler++;
                            start_client(array[1], array[2], array[3], array[4], &c);
                        }
                    } else {
                        controler++;
                        start_client(array[1], array[2], array[3], array[4], &c);
                        printf("%s\n",c.nome);
                    }
                }


                if (strcmp(array[0], "DEL") == 0) {
                    for (int i = 0; i < controler; i++) {
                        if (strcmp(clients[i].nome, array[1]) == 0) {
                            for (int y = i; i < controler - 1; i++) {
                                strcpy(clients[y].nome, clients[y + 1].nome);
                                strcpy(clients[y].pass, clients[y + 1].pass);
                                strcpy(clients[y].shares, clients[y + 1].shares);
                                strcpy(clients[y].saldo, clients[y + 1].saldo);
                            }
                            controler--;
                        }

                    }
                }
                if (strcmp(array[0], "LIST") == 0) {
                    printf("hello\n");
                    for (int i = 0; i < controler; ++i)
                        printf("%s\n", clients[0].nome);
                    printf("hello\n");
                }
                if (strcmp(array[0], "QUIT") == 0) {
                    close(s);
                }
                if (strcmp(array[0], "QUIT_SERVER") == 0) {
                    close(s);
                    exit(0);
                }
            }
            // Fecha socket e termina programa
        }
        if (strcmp(buf, "2") == 0) {
            close(s);
            exit(0);
        }
        close(s);

        return 0;
    }
}
