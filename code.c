#include <stdio.h>
#include <string.h>
#define LIMITE_CLIENTES 100
#define LIMITE_INVESTIMENTOS 30


/*
    Augusto Kenji Hirata Saraiva    RA: 2564335
    Pedro Henrique da Costa Silva   RA: 2575400
    Matheus Imazu Yoshida
*/

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
    char nome[50];
    char cpf[11];
    char telefone[9];
    char data[10];
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

//variáveis
int qntdClientes = 0;
int qntdInvestimentos = 0;
Cliente clientes[LIMITE_CLIENTES] = {};
Transacao transacoes[LIMITE_CLIENTES][1000] = {};
int qtdTransacoes = 0;

//protótipo das funções
int validarData(int dia, int mes, int ano);
int validarTelefone(int ddd, char num[9]);
int calcularDiferencaDeDatas(Data dataPriori, Data dataPosteriori);
float calcularIR(float lucro, Transacao transacao);
float calcularValorDePorcentagem(float valor, float porcentagem);
float somarImpostos(float imposto, float impostos[10]);
float calcularValorDeResgate(float valorAplicado, float juros, float imposto);
void registrarTransacao(Transacao transacao);
Cliente cadastrarCliente();
Investimento cadastrarInvestimento();


//funções de cadastro e registros
Cliente cadastrarCliente(){
    Cliente cliente;
    if(qntdClientes < LIMITE_CLIENTES){
        printf("Insira o seu nome.................................:");
        fgets(cliente.nome, 50, stdin);
        printf("Insira o seu CPF (Insira apenas os números).......:");
        fgets(cliente.cpf, 11, stdin);
        printf("Insira o seu telefone (Com o seu DDD).............:");
        fgets(cliente.telefone, 9, stdin);
        printf("Insira a sua data de nascimento...................:");
        getchar();
        fgets(cliente.data, 10, stdin);
        return cliente;
    }else{
        printf("Não é mais possível cadastrar clientes, capacidade máxima de clientes atingida");
    }
}

Investimento cadastrarInvestimento(){
    Investimento investimento;
    int validador = 0;
    if(qntdInvestimentos < LIMITE_INVESTIMENTOS){
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

void registrarTransacao(Transacao transacao){
    for(int i = 0; i < LIMITE_CLIENTES; i++){
        int isCurrentClient = strcmp(clientes[i].nome, transacao.cliente.nome) == 0 && strcmp(clientes[i].cpf, transacao.cliente.cpf) == 0;
        if(isCurrentClient){
            for(int j = 0; j < 1000; j++){
                // corrigir o null.
                if(transacoes[i][j].idTransacao == NULL){
                    transacoes[i][j] = transacao;
                    break;
                }
            }
            break;
        }
    }
}

// funções de validação
int validarData(int dia, int mes, int ano){
    return (dia > 1 && dia < 30) && (mes > 1 || mes < 12) && (ano > 1900 || ano < 2023);
}
int validarTelefone(int ddd, char num[9]){
    return ((strlen(num) == 9 && num[0] == '9') || (strlen(num) == 8)) && (ddd > 11 && ddd < 91);
}


// funções auxiliares
int calcularDiferencaDeDatas(Data dataPriori, Data dataPosteriori){
    int dia = dataPosteriori.dia - dataPriori.dia, 
        mes = dataPosteriori.mes - dataPriori.mes,
        ano = dataPosteriori.ano - dataPriori.ano;
    return dia + (30 * mes) + (360 * ano);
}

float calcularValorDePorcentagem(float valor, float porcentagem){
    return (valor * (porcentagem / 100));
}

float calcularValorDeResgate(float valorAplicado, float juros, float imposto){
    float total = 0;
    float lucro = calcularValorDePorcentagem(valorAplicado, juros);
    total += valorAplicado + lucro - calcularValorDePorcentagem(lucro, imposto);
    return total;
}

float calcularIR(float lucro, Transacao transacao){
    int dias = calcularDiferencaDeDatas(transacao.dataAplicacao, transacao.dataResgate);
    if(dias <= 180){
        return calcularValorDePorcentagem(lucro, 22.5);
    } else if(dias <= 360){
        return calcularValorDePorcentagem(lucro, 20);
    } else if(dias <= 720){
        return calcularValorDePorcentagem(lucro, 17.5);
    } else{
        return calcularValorDePorcentagem(lucro, 15);
    }
}

float somarImpostos(float imposto, float impostos[10]){
    int total = imposto;
    for(int i = 0; i < 10; i++){
        if(imposto == 0) break;
        total += impostos[i];
    }
    return total;
}
//


int tipoDeAplicacao () {
    int aplicacao = 0;
    printf("Qual tipo de aplicação você deseja fazer?\nLCI / LCA (1)\nCDB (2)\nFundos (3)\n");
    while(aplicacao == 0 || aplicacao > 3){
        scanf("%d", &aplicacao); 
    }
    return(aplicacao);
}


int main() {
    return 0;
}
