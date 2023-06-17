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
int qtdClientes = 0;
Investimento investimentos[3][5];
Investimento investimentosCadastrados[3][5];

//protótipo das funções
int validarData(int dia, int mes);
int validarDataNascimento(int dia, int mes, int ano);
int validarTelefone(int ddd, long int num);
int calcularDiferencaDeDatas(Data dataPriori, Data dataPosteriori);
int calcularComprimento(long int valor);
int calcularPrimeiroNumero(int num);
float calcularIR(float lucro, float porcentagem);
float calcularValorDePorcentagem(float valor, float porcentagem);
float somarImpostos(float imposto, float impostos[10]);
float calcularValorDeResgate(float valorAplicado, Investimento investimento, int dias);
void registrarTransacao(Transacao transacao);
int encontrarCliente(char cpf[11]);
void imprimirExtrato(int index, char cpf[11]);
void gerarExtrato(Cliente cliente);
void cadastrarCliente();
void definirOperacao();
Transacao obterDadosTransacao();
Investimento cadastrarInvestimento();
float calcularImpostos(int tipo, int dias);


//funções de cadastro, registros e extratos

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
    int v = encontrarCliente(cliente.cpf);
    printf("\n%d\n", v);
    imprimirExtrato(encontrarCliente(cliente.cpf), cliente.cpf);
}


// funções de validação
int validarDataNascimento(int dia, int mes, int ano){
    return validarData(dia, mes) && (ano > 1900 || ano < 2023);
}
int validarData(int dia, int mes){
    return (dia > 1 && dia < 30) && (mes > 1 || mes < 12);
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
int validarInvestimento(int tipo){
    return tipo == 1 || tipo == 2 || tipo == 3;
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

float calcularValorDeResgate(float valorAplicado, Investimento investimento, int dias){    
    float total = 0,
          jurosDataResgate = (investimento.taxa / 360 * dias),
          lucro = calcularValorDePorcentagem(valorAplicado, jurosDataResgate),
          imposto = calcularImpostos(investimento.tipoAplicacao, dias);
    total += valorAplicado + lucro - calcularValorDePorcentagem(lucro, imposto);
    return total;
}

float calcularIR(float lucro, float porcentagem){
    return calcularValorDePorcentagem(lucro, porcentagem);
}

float calcularPorcentagemIR(int dias){
    if(dias <= 180){
        return 22.5;
    } else if(dias <= 360){
        return 20;
    } else if(dias <= 720){
        return 17.5;
    } else{
        return 15;
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

Investimento obterInvestimento(){
    int tipo = 0, opcao = 0;    
    printf("Digite o tipo de aplicação que deseja fazer:\n");
    imprimirTiposDeInvestimento();
    scanf("%d", &tipo);
    if(validarInvestimento(tipo) == 0) return obterInvestimento();
    imprimirInvestimentos(tipo, 'C');
    printf("Escolha uma das opções de investimento cadastrados acima:");
    scanf("%d", &opcao);
    return opcao >= 1 && opcao <= 5 ? investimentosCadastrados[tipo][opcao] : obterInvestimento();
}

void imprimirTiposDeInvestimento(){
    printf("(1)LCI/LCA\n(2)CDB\n(3)Fundos\n:");
}

void imprimirInvestimento(Investimento investimento){
    printf("Tipo:%d\nEmissor:%s\nTaxa:%f\nAtivo:%c\n\n -------------------\n", 
        investimento.tipoAplicacao, investimento.emissor,
        investimento.taxa, investimento.ativo
    );
}

void imprimirInvestimentos(int tipo, char criterio){
    if(criterio == 'C') {
        for(int i = 0; i < 5; i++){
            imprimirInvestimento(investimentosCadastrados[tipo][i]);
        }
    } else{
        for(int i = 0; i < 5; i++){
            imprimirInvestimento(investimentos[tipo][i]);
        }
    };
}

int encontrarCliente(char cpf[11]){
    printf("Arm:%s\n%sArm:%s\n", clientes[0].nome, clientes[0].cpf);
    printf("%s\n", cpf);
    for(int i = 0; i < 100; i++){
        if(strcmp(clientes[i].cpf, cpf) == 0){
            return i;
        }
    }
    return -1;
}
//


int tipoDeAplicacao () {
    int aplicacao = 0;
    printf("Qual tipo de aplicação você deseja fazer?\n(1)LCI / LCA\n(2)CDB\n(3)Fundos\n");
    while(aplicacao == 0 || aplicacao > 3){
        scanf("%d", &aplicacao); 
    }
    return(aplicacao);
}

Transacao obterDadosTransacao(){
    Transacao transacao;
    int indiceCliente = 0, dias = 0;
    char cpf[11];

    printf("Digite o CPF do cliente:");
    scanf("%s", cpf);    
    indiceCliente = encontrarCliente(cpf);
    if(indiceCliente == -1) return obterDadosTransacao();
    transacao.cliente = clientes[indiceCliente];
    transacao.investimento = obterInvestimento();
    printf("Digite a data de aplicação no modelo DIA MÊS ANO: ");
    scanf("%d %d %d", &transacao.dataAplicacao.dia, &transacao.dataAplicacao.mes, &transacao.dataAplicacao.ano);
    printf("Digite o valor da aplicação: ");
    scanf("%f", &transacao.valorAplicacao);
    printf("Digite a data de resgate no modelo DIA MÊS ANO: ");
    scanf("%d %d %d", &transacao.dataResgate.dia, &transacao.dataResgate.mes, &transacao.dataResgate.ano);
    dias = calcularDiferencaDeDatas(transacao.dataAplicacao, transacao.dataResgate);
    transacao.valorResgate = calcularValorDeResgate(transacao.valorAplicacao, transacao.investimento, dias);
    qtdTransacoes++;
    transacao.idTransacao = qtdTransacoes;
    return transacao;
}

float calcularImpostos(int tipo, int dias){
    if(tipo == 1){
        return 0;
    }
    if(tipo == 2){
        return calcularPorcentagemIR(dias);
    }
    if(tipo == 3){
        float impostos[10] = {calcularPorcentagemIR(dias), 0};
        return somarImpostos(1, impostos);
    }
}

// funções de serviços
void cadastrarCliente(){
    if(qntdClientes < LIMITE_CLIENTES){
        getchar();
        printf("Insira o seu nome:");
        fgets(clientes[qntdClientes].nome, 51, stdin);
        printf("Insira o seu CPF (Insira apenas os números):");
        fgets(clientes[qntdClientes].cpf, 12, stdin);        
        printf("Digite seu número de celular ou fixo no formato DDD NUMERO:");
        scanf("%d %ld", &clientes[qntdClientes].telefone.ddd, &clientes[qntdClientes].telefone.num);
        printf("Digite sua data de nascimento no formato DIA MES ANO:");
        scanf("%d %d %d", &clientes[qntdClientes].data.dia, 
            &clientes[qntdClientes].data.mes, 
            &clientes[qntdClientes].data.ano);
    }
    qntdClientes++;
}

Investimento cadastrarInvestimento(){
    Investimento investimento;
    int validador = 0;
    if(qntdInvestimentos < LIMITE_INVESTIMENTOS){
        printf("Qual tipo de aplicação você deseja fazer?\n(1)LCI / LCA\n(2)CDB\n(3)Fundos\n");
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


void definirOperacao(){
    int operacao = 0;
    printf("Qual operação você deseja fazer?\n(1)Cadastro de cliente\n(2)Cadastro de investimento\n(3)Transação\n");
    scanf("%d", &operacao);
    if(operacao == 1){
        cadastrarCliente();
    } else if(operacao == 2){
        cadastrarInvestimento();
    } else if(operacao == 3){
        int obterExtrato = 0;
        Transacao dadosTransacao = obterDadosTransacao();
        registrarTransacao(dadosTransacao);
        printf("Deseja obter o extrato? (1)Sim (0)Nao:");
        scanf("%d", &obterExtrato);    
        getchar();    
        if(obterExtrato) gerarExtrato(dadosTransacao.cliente);
    } else{
        printf("Essa operação não existe, tente novamente.\n");
    }
}

int main() {
    while(1){
        definirOperacao();
    }
    return 0;
}
