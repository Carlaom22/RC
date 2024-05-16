#include "header.h"


void erro(char *s) {
    perror(s);
    exit(1);
}

void insert(char *nome, char *pass, char *mercado, float saldo){

    if(number_of_users==MAX_USERS) {
        printf("Número máximo de clientes atingido!\n");
    }
    for (int i = 0; i < (number_of_users); i++) {
        if (strcmp(nome, listaclientes[i].nome) == 0) {
            printf("Nome de utilizador ja existente, tente outro nome.\n");
            break;
        }
        if((strcmp(nome, listaclientes[i].nome) != 0) && (strcmp(mercado, mercados[i].market_name) == 0)){
            strcpy(listaclientes[number_of_users].nome, nome);
            strcpy(listaclientes[number_of_users].pass, pass);
            listaclientes[number_of_users].saldo = saldo;
            strcpy((char *) listaclientes[number_of_users].wallets, mercado);
            for (int j = 0; j < MAX_MARKETS; j++){
                if (strcmp(mercado, mercados[i].market_name)==0){
                    listaclientes[number_of_users].acesso[0] = &mercados[i];
                }
                else
                    printf("Mercado nao disponivel.\n");
            }
            printf("Novo utilizador inserido!\n");
            number_of_users++;
            break;
        }
    }
}

void inicialize_market(market *mkt, char *name){
    strcpy(mkt->market_name, name);
}

void readusers(){

    FILE *fp;
    char array[3][512];
    fp = fopen("/RC/dados.txt", "r");
    if (NULL == fp) {
        printf("File can't be opened \n");
    }
    fscanf(fp, "%[^/]%*c%s", array[0], array[1]);
    strcpy(administrador.login, array[0]);
    strcpy(administrador.password, array[1]);


    fscanf(fp, " %d", &number_of_users);


    for (int i = 0; i < number_of_users; i++) {
        fscanf(fp, " %[^;]%*c%[^;]%*c%s", array[0], array[1], array[2]);
        inicialize_client_file(&listaclientes[i], array[0], array[1], atof(array[2]));
    }

    for (int i = 0; i < MAX_MARKETS; i++) {
        for (int f = 0; f < MAX_SHARES; f++) {
            fscanf(fp, " %[^;]%*c%[^;]%*c%s", array[0], array[1], array[2]);
            if (f == 0) {
                inicialize_market(&mercados[i], array[0]);
            }
            strcpy(share.nome,array[1]);
            share.preco_compra = atof(array[2]);
            mercados[i].market_shares[f] = share;
        }
    }
    printf("read ok\n");
}

void show() {
    if (number_of_users > 0) {
        for (int i = 0; i < number_of_users; i++) {
            printf("Username: %s\n", listaclientes[i].nome);
            printf("Password: %s\n", listaclientes[i].pass);
            printf("Bolsas: %s\n", listaclientes[i].wallets);
            printf("saldo: %f\n", listaclientes[i].saldo);
            printf("--------------------\n");
        }
    } else
        printf("Não existem cientes para mostrar!\n");
}

void showPrices(char *user){
    for (int f = 0;  f < MAX_USERS; ++f) {
        if (strcmp(user, listaclientes[f].nome)==0) {
            printf("Nome do mercado: %s\n", listaclientes[f].acesso[0]->market_name);
            for(int j=0;j<MAX_SHARES;++j){
                printf("Acoes do mercado: %s\n", listaclientes[f].acesso[0]->market_shares[j].nome);
                printf("Preco de compra: %f\n", listaclientes[f].acesso[0]->market_shares[j].preco_compra);
            }

        }
    }
}


void buy(char *market, char *name, char *user, int quantidade, float preco){
    float custo;
    int z = 0;
    for (int i = 0; i < MAX_USERS; ++i){
        if(strcmp(user, listaclientes[i].nome)==0){
            for (int j = 0; j < MAX_SHARES; ++j) {
                if (strcmp(market,listaclientes[i].acesso[0]->market_name)==0){
                    if(strcmp(name, listaclientes[i].acesso[0]->market_shares[j].nome) == 0) {
                        custo = (float) quantidade * listaclientes[i].acesso[0]->market_shares[j].preco_compra;
                        if(preco <= listaclientes[i].acesso[0]->market_shares[j].preco_venda) {
                            if (custo < listaclientes[i].saldo) {
                                listaclientes[i].wallets[z].amount = quantidade;
                                listaclientes[i].wallets[z].acoes = name;
                                listaclientes[i].saldo -= custo;
                                z++;
                            }
                        }
                    }
                }
            }
        }
    }
}


void delete(char *nome) {
    int n;
    for (int i = 0; i < (number_of_users); i++) {
        if(strcmp(nome, listaclientes[i].nome)==0){
            n = i;
            break;
        }
    }
    for (int i = n ; i < (number_of_users) ; i++) {
        listaclientes[i] = listaclientes[i + 1];
    }
}

void inicialize_client_file(client *clnt, char *user, char *pd, float bal) {
    strcpy(clnt->nome, user);
    strcpy(clnt->pass, pd);
    clnt->saldo = bal;
    for (int i = 0; i < MAX_USERS; i++) {
        clnt->acesso[i] = NULL;
    }
    int pos = 0;

    for (int i = 0; i < MAX_MARKETS; i++) {
        for (int j = 0; j < MAX_SHARES; j++) {
            clnt->wallets[pos + j].acoes = &mercados[i].market_shares[j];
            clnt->wallets[pos + j].amount = 0;
        }
        pos += 3;
    }
}

void sell(char *nome, char *acao, float preco, int quantidade){
    for (int i = 0; i < MAX_USERS; ++i) {
        if (strcmp(nome, listaclientes[i].nome)==0){
            for (int j = 0; j < MAX_SHARES; ++j) {
                if(strcmp(acao, listaclientes[i].acesso[0]->market_shares[j].nome) == 0) {
                    mercados[numOfAc+1].market_shares[j].quantidade_venda+=quantidade;
                    mercados[numOfAc+1].market_shares[j].preco_venda = preco;
                }
            }
        }
    }
}