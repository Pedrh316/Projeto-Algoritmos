#include <stdio.h>
#include <string.h>


/*
    Augusto Kenji Hirata Saraiva    RA: 2564335
    Pedro Henrique da Costa Silva
    Matheus Imazu Yoshida
*/

//variáveis
int qntdClientes = 0, qntdMaximaClientes = 100;
int qntdInvestimentos = 0, qntdMaximaInvestimentos = 30;
//structs

typedef struct Data {
    int dia[2];
    int mes[2];
    int ano[4];
} Data;

typedef struct Telefone {
    char ddd[2];
    char num[9];
} Telefone;

typedef struct Cliente {
    char Nome[50];
    char CPF[11];
    char Telefone[9];
    char Data[10];
} Cliente;

typedef struct Investimento {
        int tipoAplicacao;
        char emissor[100];
        float taxa;
        char ativo[5];

} Investimento;

typedef struct Transacao{
    int idTransacao;
    Cliente cliente;
    Investimento investimento;
    Data dataAplicacao;
    float valorAplicacao;
    Data dataResgate;
    float valorResgate;
} Transacao;

//protótipo das funções
int validarData(int dia, int mes, int ano);

int validarTelefone(int ddd, char num[9]);

Cliente cadastrarCliente();

Investimento cadastrarInvestimento();

//funções

Cliente cadastrarCliente(){
    Cliente cliente;
    if(qntdClientes < qntdMaximaClientes){
        printf("Insira o seu nome.................................:");
        fgets(cliente.Nome, 50, stdin);
        printf("Insira o seu CPF (Insira apenas os números).......:");
        fgets(cliente.CPF, 11, stdin);
        printf("Insira o seu telefone (Com o seu DDD).............:");
        fgets(cliente.Telefone, 9, stdin);
        printf("Insira a sua data de nascimento...................:");
        getchar();
        fgets(cliente.Data, 10, stdin);
        return cliente;
    }else{
        printf("Não é mais possível cadastrar clientes, capacidade máxima de clientes atingida");
    }
}

Investimento cadastrarInvestimento(){
    Investimento investimento;
    int validador = 0;
    if(qntdInvestimentos < qntdMaximaInvestimentos){
        printf("Qual tipo de aplicação você deseja fazer?\nLCI / LCA (1)\nCDB (2)\nFundos (3)\n");
        scanf("%d", &investimento.tipoAplicacao);
        while(validador == 0){
            scanf("%d", &investimento.tipoAplicacao); 
            if(investimento.tipoAplicacao > 0 && investimento.tipoAplicacao <=3){
                validador++;
            }    
        }
        printf("Qual o banco que você deseja realizar essa aplicação?...:");
        fgets(investimento.emissor, 100, stdin);
        //Precisa ser finalizado taxa e ativo
    }else{
        printf("Não é mais possível cadastrar investimentos, capacidade máxima de investimentos atingida");
    }
}

int validarData(int dia, int mes, int ano){
    return (dia > 1 && dia < 30) && (mes > 1 || mes < 12) && (ano > 1900 || ano < 2023);
}

int validarTelefone(int ddd, char num[9]){
    return ((strlen(num) == 9 && num[0] == '9') || (strlen(num) == 8)) && (ddd > 11 && ddd < 91);
}

int tipoDeAplicacao () {
    int validador = 0;

    printf("Qual tipo de aplicação você deseja fazer?\nLCI / LCA (1)\nCDB (2)\nFundos (3)\n");
    while(validador == 0){
        scanf("%d", &escolherAplicacao); 
        if(escolherAplicacao > 0 && escolherAplicacao <=3){
            validador++;
        }
    }
    return(escolherAplicacao);
}


int main {
    return 0;
}

