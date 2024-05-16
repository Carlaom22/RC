#include "header.h"

void refresh(market *market){
    for(int i=0;i<MAX_MARKETS;i++){
        for(int j=0;j<MAX_SHARES;j++){
            if((rand() % 2 + 1 )== 1){
                market[i].market_shares[j].quantidade_compra+=10;
                market[i].market_shares[j].quantidade_venda+=10;
            }else{
                market[i].market_shares[j].quantidade_compra-=10;
                market[i].market_shares[j].quantidade_venda-=10;
            }
            if( market[i].market_shares[j].quantidade_compra == 0){
                market[i].market_shares[j].quantidade_compra+=10;
            }
            if(market[i].market_shares[j].quantidade_venda==0){
                market[i].market_shares[j].quantidade_venda+=10;
            }
            if((rand() % 2 + 1)==1){
                market[i].market_shares[j].preco_venda+=(float)0.01;
                market[i].market_shares[j].preco_compra+=(float)0.01;
            }else{
                market[i].market_shares[j].preco_venda-=(float)0.01;
                market[i].market_shares[j].preco_compra-=(float)0.01;
            }
            if(market[i].market_shares[j].preco_compra<0.01){
                market[i].market_shares[j].preco_compra +=(float)0.01;
            }
            if(market[i].market_shares[j].preco_venda<0.01){
                market[i].market_shares[j].preco_venda+=(float)0.01;
            }
        }
    }
}


int main() {

    shmid = shmget(IPC_PRIVATE, sizeof(client) * MAX_USERS, IPC_CREAT | 0777);
    listaclientes = shmat(shmid, NULL, 0);
    shmidd = shmget(IPC_PRIVATE, sizeof(market) * MAX_MARKETS, IPC_CREAT | 0777);
    mercados = shmat(shmidd,NULL,0);
    shmrf = shmget(IPC_PRIVATE, 2, IPC_CREAT | 0777);
    refresh_value = (int *)shmat(shmrf, NULL, 0);

    struct sockaddr_in si_minha, si_outra;
    socklen_t slen = sizeof(si_outra);
    int s, recv_len;
    char buf[BUFLEN];
    char *token;
    char sep[5] = " \r\n";
    char user[512];


    if(fork()==0){
        while(stop){
            refresh(mercados);
            sleep(*refresh_value);
        }
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        printf("Erro na criação do socket\n");

    si_minha.sin_family = AF_INET;
    si_minha.sin_port = htons(PORT);
    si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *) &si_minha, sizeof(si_outra)) == -1) {
        erro("Erro no bind");
    }

    readusers();

    while (1) {
        time_t times;
        struct tm *timeinfo;
        time(&times);
        timeinfo = localtime(&times);
        printf("Server inicializado - %s\n", asctime(timeinfo));
        char valor[512];


        printf("Admin or Client?\n");

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t * ) & slen)) == -1) {
            erro("Erro no recvfrom");
        }

        buf[recv_len] = '\0';

        token = strtok(buf, sep);

        while (token != NULL) {
            strcpy(valor, token);
            token = strtok(NULL, sep);
        }

        if(strcmp(valor, "admin") == 0){
            while (1) {
                int i = 0;
                char array[BUFLEN][512];

                printf("\tLogin: \n\n<username> <password>\n <QUIT_SERVER para fechar app.\n");

                if(sendto(s, "\nInsira username e password!\n\n", 27, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                    printf("Erro no sendto");
                }

                if(sendto(s, "\nQUIT_SERVER to close the server!\n\n", 35, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                    printf("Erro no sendto");
                }

                if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t * ) & slen)) == -1) {
                    erro("Erro no recvfrom");
                }

                buf[recv_len] = '\0';

                token = strtok(buf, sep);

                while (token != NULL) {
                    strcpy(array[i++], token);
                    token = strtok(NULL, sep);
                }

                if (strcmp(array[0], "QUIT_SERVER") == 0) {
                    if(sendto(s, "\nServidor a fechar!\n", 18, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    close(s);
                    return 0;
                } else if ((strcmp(array[0], administrador.login) == 0) && (strcmp(array[1], administrador.password) == 0)) {
                    printf("Login efetuado com sucesso!\n");
                    if(sendto(s, "\nLogin efetuado com sucesso!\n", 28, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    break;
                } else {
                    if (sendto(s, "\nComando enviado nao reconhecido!\n", 32, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    printf("Erro.\n");
                }
            }
            while (1) {
                int i = 0;
                char array[BUFLEN][512];

                if(sendto(s, "\n\n\tMENU\nADD_USER <name> <password> <markets> <balance> to insert new user\nREFRESH <value> to update refresh\nQUIT_SERVER to close the server\nLIST to show all the users\nDEL <user> to delete an user\nQUIT to go back\n\n", 212, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                    printf("Erro no sendto");
                }

                recv_len = 0;
                strcpy(buf, "");
                buf[0]=0;

                do {
                    strcpy(buf, "");
                    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t * ) & slen)) == -1) {
                        erro("Erro no recvfrom");
                    }
                    buf[recv_len - 1] = 0;
                } while (recv_len <= 1);


                token = strtok(buf, sep);
                while (token != NULL) {
                    strcpy(array[i++], token);
                    token = strtok(NULL, sep);
                }

                if (strcmp(array[0], "QUIT_SERVER") == 0) {
                    if(sendto(s, "\nServer a fechar!\n\n", 20, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    printf("Programa encerrado\n");
                    close(s);
                    return 0;
                } else if (strcmp(array[0], "ADD_USER") == 0) {
                    insert(array[1], array[2], array[3], atof(array[4]));
                    if(sendto(s, "\nNovo utilizador inserido!\n\n", 30, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                } else if (strcmp(array[0], "REFRESH") == 0) {
                    *refresh_value = atoi(array[1]);
                    printf("Refresh atualizado para %d !\n", *refresh_value);
                    if(sendto(s, "\nRefresh atualizado!\n\n", 24, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                } else if (strcmp(array[0], "LIST") == 0) {
                    show();
                } else if (strcmp(array[0], "DEL") == 0) {
                    delete(array[1]);
                    if(sendto(s, "\nUser eliminado com sucesso!\n\n", 32, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    number_of_users--;
                }else if (strcmp(array[0], "QUIT") == 0) {
                    printf("\nSessão terminada.\n\n");
                    if(sendto(s, "\nSessao terminada!\n\n", 22, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    break;
                } else printf("\nErro!\n\n");
            }
        } //Done
        bool continua=true;
        if (strcmp(valor, "client")==0) {
            while (continua) {
                int i = 0;
                char array[BUFLEN][512];

                printf("\tLogin: \n\n<username> <password>\n");

                if(sendto(s, "\nInsira username e password!\n\n", 27, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                    printf("Erro no sendto");
                }

                if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t * ) & slen)) ==
                    -1) {
                    erro("Erro no recvfrom");
                }

                buf[recv_len] = '\0';

                token = strtok(buf, sep);

                while (token != NULL) {
                    strcpy(array[i++], token);
                    token = strtok(NULL, sep);
                }

                for (int j = 0; j < MAX_USERS; ++j) {
                    if ((strcmp(array[0], listaclientes[j].nome) == 0) && (strcmp(array[1], listaclientes[j].pass) == 0)) {
                        printf("Login efetuado com sucesso!\n");
                        if(sendto(s, "\nLogin efetuado com sucesso!\n", 28, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                            printf("Erro no sendto");
                        }
                        continua=false;
                        break;
                    } else
                    if (sendto(s, "\nComando enviado nao reconhecido!\n", 32, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                        printf("Erro.\n");
                }
            }

            while (1){
                int i = 0;
                numOfAc = 6;
                char array[BUFLEN][512];

                recv_len = 0;
                strcpy(buf, "");
                buf[0]=0;

                do {
                    strcpy(buf, "");
                    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t * ) & slen)) == -1) {
                        erro("Erro no recvfrom");
                    }
                    buf[recv_len - 1] = 0;
                } while (recv_len <= 1);

                if(sendto(s, "\n\n\tMENU\nCHECK <user> to see what markets does a user have access!\nBUY <market> <name> <user> <quantity> <price> to place an offer to buy\nSELL <name> <acao> <price> <quantity> to place a sell\nQUIT to go back\n\n", 154, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                    printf("Erro no sendto");
                }

                token = strtok(buf, sep);
                while (token != NULL) {
                    strcpy(array[i++], token);
                    token = strtok(NULL, sep);
                }


                if (strcmp(array[0], "QUIT") == 0) {
                    printf("\nSessão terminada.\n\n");
                    if(sendto(s, "\nSessao terminada!\n\n", 22, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    break;
                    return 0;
                } else if (strcmp(array[0], "CHECK") == 0) {
                    showPrices(array[1]);

                }else if (strcmp(array[0], "BUY") == 0) {
                    buy(array[1], array[2], array[3], atoi(array[4]), atof(array[5]));
                    if(sendto(s, "\nOferta colocada!\n\n", 20, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    numOfAc--;
                }else if (strcmp(array[0], "SELL") == 0) {
                    sell(array[1], array[2], atof(array[3]), atoi(array[4]));
                    if(sendto(s, "\nAcao colocada no mercado!\n\n", 31, 0, (struct sockaddr *) &si_outra, slen) == -1) {
                        printf("Erro no sendto");
                    }
                    numOfAc++;
                }
                else printf("\nErro!\n\n");
            }
        }
    }
}