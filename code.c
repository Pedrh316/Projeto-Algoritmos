#include <stdio.h>
#include <string.h>
#include <math.h>
#define LIMITE_CLIENTES 100
#define LIMITE_INVESTIMENTOS 30


/*
    Augusto Kenji Hirata Saraiva    RA: 2564335
    Pedro Henrique da Costa Silva   RA: 2575400
    Matheus Imazu Yoshida
*/

//structs
typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Telefone {
    int ddd;
    long int num;
} Telefone;

typedef struct Cliente {
    char nome[50];
    char cpf[11];
    Telefone telefone;
    Data data;
} Cliente;

typedef struct Investimento {
        int tipoAplicacao;
        char emissor[100];
        float taxa;
        char ativo;
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
Cliente listaClientes[LIMITE_CLIENTES];

//protótipo das funções
int validarData(int dia, int mes, int ano);
int validarTelefone(int ddd, long int num);
int calcularDiferencaDeDatas(Data dataPriori, Data dataPosteriori);
int calcularComprimento(long int valor);
int calcularPrimeiroNumero(int num);
float calcularIR(float lucro, Transacao transacao);
float calcularValorDePorcentagem(float valor, float porcentagem);
float somarImpostos(float imposto, float impostos[10]);
float calcularValorDeResgate(float valorAplicado, float juros, float imposto);
void registrarTransacao(Transacao transacao);
int encontrarCliente(Cliente cliente);
void imprimirExtrato(int index, char cpf[11]);
void gerarExtrato(Cliente cliente);
void cadastrarCliente();
Investimento cadastrarInvestimento();


//funções de cadastro, registros e extratos
void cadastrarCliente(){
    if(qntdClientes < LIMITE_CLIENTES){
        getchar();
        printf("Insira o seu nome.................................:");
        fgets(listaClientes[qntdClientes].nome, 50, stdin);
        printf("Insira o seu CPF (Insira apenas os números).......:");
        fgets(listaClientes[qntdClientes].cpf, 11, stdin);
        printf("Insira o DDD do seu número de telefone............:");
        scanf("%d", &listaClientes[qntdClientes].telefone.ddd);
        printf("Insira o seu número de telefone...................:");
        scanf("%ld", &listaClientes[qntdClientes].telefone.num);
        printf("Insira o dia que você nasceu......................:");
        scanf("%d", &listaClientes[qntdClientes].data.dia);
        printf("Insira o mês que você nasceu......................:");
        scanf("%d", &listaClientes[qntdClientes].data.mes);
        printf("Insira o ano que você nasceu......................:");
        scanf("%d", &listaClientes[qntdClientes].data.ano);
    }
    qntdClientes++;
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

int encontrarCliente(Cliente cliente){
    for(int i = 0; i < 100; i++){
        if(strcmp(clientes[i].cpf, cliente.cpf) == 0){
            return i;
        }
    }
}

void imprimirExtrato(int index, char cpf[11]){
    printf("Extrato do cliente %s\n", cpf);
    for(int j = 0; transacoes[index][j].idTransacao != NULL; j++){
        Transacao t = transacoes[index][j];
        Cliente c = t.cliente;
        Investimento i = t.investimento;
        printf("ID da Transação: %d\n", t.idTransacao);
        printf("Cliente:           Nome:%40s | CPF:%20s | Tel:%d %12ld | Data Nasc:%02d %02d %02d\n", 
            c.nome, c.cpf, c.telefone.ddd, c.telefone.num, c.data.dia, c.data.mes, c.data.ano
        );
        printf("Investimento:      Tipo:%40d | Emissor:%16s | Taxa:%14f | Ativo: %c\n",
            i.tipoAplicacao, i.emissor, i.taxa, i.ativo
        );
        printf("Data de aplicação: %02d/%02d/%d \nData de resgate:  %02d/%02d/%d \nValor da aplicação:%f \nValor de resgate: %f \n\n\n",
            t.dataAplicacao.dia, t.dataAplicacao.mes, t.dataAplicacao.ano,        
            t.dataResgate.dia, t.dataResgate.mes, t.dataResgate.ano,
            t.valorAplicacao, t.valorResgate
        );
    }
}

void gerarExtrato(Cliente cliente){    
    imprimirExtrato(encontrarCliente(cliente), cliente.cpf);
}


// funções de validação
int validarData(int dia, int mes, int ano){
    return (dia > 1 && dia < 30) && (mes > 1 || mes < 12) && (ano > 1900 || ano < 2023);
}
int validarTelefone(int ddd, long int num){
    return ((calcularComprimento(num) == 9 && calcularPrimeiroNumero(num) == 9) || (calcularComprimento(num) == 8)) && (ddd > 11 && ddd < 91);
}
int validarCPF (char CPF[11]){
int i=0, soma=0,resto=0;
    for(i=0;i<9;i++){
    soma += ((CPF[i])*(10-i));
    } 
    resto = (soma % 11); 
    if (resto < 2){
        CPF[10] == 0;
    } else {
        CPF[10] == (11-resto);
    }
    for(i=0;i<10;i++){
        soma += (CPF[i]*(11-i));
    }
    resto = (soma % 11);
    if (resto < 2){
        CPF[11] == 0;
    } else {
    CPF[11] == (11-resto);
    }
    return 0;
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

int calcularComprimento(long int valor){
    int length = 0;
    for(int i = 1; i <= valor; i*=10){
        length++;
    }
    return length;
}

int calcularPrimeiroNumero(int num){
    int length = calcularComprimento(num);
    int potencia10 = pow(10, length - 1);
    for(int i = 0; i <= 10; i++){
        if((potencia10 * i) > num && (potencia10 * i - num <= potencia10)){
            return i - 1;
        };
    }
    return potencia10;
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
