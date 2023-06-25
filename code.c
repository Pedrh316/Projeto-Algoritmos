#include <stdio.h>
#include <string.h>
#include <math.h>
#define LIMITE_CLIENTES 100
#define LIMITE_INVESTIMENTOS 30
#define LIMITE_BANCOS 10


/*
    Augusto Kenji Hirata Saraiva    RA: 2564335
    Pedro Henrique da Costa Silva   RA: 2575400
    Matheus Imazu Yoshida           RA: 2575388
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

typedef struct IndiceInvestimento{
    int tipo;
    int banco;
} IndiceInvestimento;

//variáveis
int qntdClientes = 0;
int qntdInvestimentos = 0;
Cliente clientes[LIMITE_CLIENTES] = {};
Transacao transacoes[LIMITE_CLIENTES][1000] = {};
int qtdTransacoes = 0;
int qtdClientes = 0;
Investimento investimentos[3][LIMITE_BANCOS];
Investimento investimentosCadastrados[3][LIMITE_BANCOS];


//protótipo das funções
int validarData(Data d);
int validarDataNascimento(Data d);
int validarTelefone(Telefone t);
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
int procurarInvestimento(Investimento investimentos[3][LIMITE_BANCOS], int tipoAplicacao, char emissor[100]);
int validarCadastroCliente(Cliente c);
int validarCadastroInvestimento(int tipoAplicacao);
int continuarProcesso();
void realizarTransacao();
void cadastrarInvestimento();
float calcularImpostos(int tipo, int dias);
void mudarAtivo();
void imprimirClientes();
int encontrarIndiceVazio(Investimento investimentos[3][LIMITE_BANCOS], int tipoAplicacao);
void attInvestimentos();
void imprimirTiposDeInvestimento();
void cadastrarInvestimentoExistente(int tipoAplicacao);
void imprimirInvestimento(Investimento investimento);
void imprimirInvestimentos(int tipo, char criterio);


//atribuição de valores iniciais
void attInvestimentos(){
    char bancos[5][100] = {"Banco do Brasil","Caixa","Bradesco", "Itaú", "Santander"};
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 5; j++){
            investimentos[i][j].tipoAplicacao = i + 1;
            strcpy(investimentos[i][j].emissor,bancos[j]);
            investimentos[i][j].taxa = 10 + j;
            investimentos[i][j].ativo = 's';
        }
    }
}


//funções para teste
void imprimirClientes(){
    for(int i = 0; i < LIMITE_CLIENTES; i++){
        Cliente c = clientes[i];
        int ddd = c.telefone.ddd, dia = c.data.dia, mes = c.data.mes, ano = c.data.ano;
        long int num = c.telefone.num;
        printf("Nome:%s\nCPF:%s\nN° Telefone:%d %ld, Nascimento:%d %d %d", c.nome, c.cpf, ddd, num, dia, mes, ano);
    }
}

//funções de cadastro, registros e extratos
void imprimirExtrato(int index, char cpf[11]){
    printf("Extrato do cliente %s\n", cpf);
    for(int j = 0; transacoes[index][j].idTransacao != 0; j++){
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
int validarDataNascimento(Data d){
    return validarData(d) && (d.ano >= 1900 && d.ano <= 2023);
}
int validarData(Data d){
    return (d.dia >= 1 && d.dia <= 30) && (d.mes >= 1 || d.mes <= 12);
}
int validarTelefone(Telefone t){
    return ((calcularComprimento(t.num) == 9 && calcularPrimeiroNumero(t.num) == 9) || (calcularComprimento(t.num) == 8)) && (t.ddd > 11 && t.ddd < 91);
}
int validarCPF(char cpf[11]){
    int soma = 0, resto = 0, digito = 0;
    int saoIguais(){
        int v = 0, primeiroCaractere = cpf[0];
        for(int i = 0; i < 11; i++){
            v += primeiroCaractere == cpf[i];
        }
        return v == 11;
    }
    int digitoValido(int v){
        for(int i = v; i > 1; i--){
            soma += (cpf[v - i] - 48) * i;
        }
        resto = soma % 11;
        digito = ((resto*10) % 11) % 10;
        soma = 0;
        resto = 0;
        return cpf[v - 1] == digito + 48;
    }
    return digitoValido(10) && digitoValido(11) && saoIguais() == 0;
}
int validarInvestimento(int tipo){
    printf("oi %d\n", tipo);
    return tipo == 1 || tipo == 2 || tipo == 3;
}
int validarCadastroCliente(Cliente c){
    return (validarDataNascimento(c.data) && validarTelefone(c.telefone) && validarCPF(c.cpf));
}
int validarCadastroInvestimento(int tipoAplicacao){
    return (tipoAplicacao >= 1 && tipoAplicacao <= 3);
}

// funções auxiliares
int procurarInvestimento(Investimento investimentos[3][LIMITE_BANCOS], int tipoAplicacao, char emissor[100]){
    for(int i = 0; i < LIMITE_BANCOS; i++){
        if(strcmp(investimentos[tipoAplicacao - 1][i].emissor, emissor) == 0){
            return i;
        };
    }
    return -1;
}

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
          jurosDataResgate = (investimento.taxa / (360.0 / dias)),
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
        for(int i = 0; i < LIMITE_BANCOS; i++){
            if(investimentosCadastrados[tipo - 1][i].tipoAplicacao == 0) break;
            imprimirInvestimento(investimentosCadastrados[tipo - 1][i]);
        }
    } else{
        for(int i = 0; i < LIMITE_BANCOS; i++){
            if(investimentosCadastrados[tipo - 1][i].tipoAplicacao == 0) break;
            imprimirInvestimento(investimentos[tipo - 1][i]);
        }
    };
}

int encontrarCliente(char cpf[11]){
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


float calcularImpostos(int tipo, int dias){
    if(tipo == 1){
        return 0;
    }
    if(tipo == 2){
        return calcularPorcentagemIR(dias);
    }
    if(tipo == 3){
        float impostos[10] = {calcularPorcentagemIR(dias), 0};
        printf("\n\n oi %f\n\n", somarImpostos(1, impostos));
        return somarImpostos(1, impostos);
    }
}

// funções de serviços
void cadastrarCliente(){
    Cliente c;
    if(qntdClientes < LIMITE_CLIENTES){
        getchar();
        printf("Insira o seu nome:");
        fgets(c.nome, 50, stdin);
        printf("Insira o seu CPF (Insira apenas os números):");
        fgets(c.cpf, 12, stdin);        
        printf("Digite seu número de celular ou fixo no formato DDD NUMERO:");
        scanf("%d %ld", &c.telefone.ddd, &c.telefone.num);
        printf("Digite sua data de nascimento no formato DIA MES ANO:");
        scanf("%d %d %d", &c.data.dia, &c.data.mes, &c.data.ano);
        if(validarCadastroCliente(c)){   
            if(encontrarCliente(c.cpf) == -1){
                clientes[qntdClientes] = c;
                printf("Cliente cadastrado com sucesso.\n");
            } else{
                printf("CPF já cadastrado. Tente novamente\n:");
                cadastrarCliente();
            }
        } else{
            printf("Ops! Parece que uma informação não está correta, informe seus dados novamente:\n");
            cadastrarCliente();
        }
    } else{
        printf("Limite de usuários cadastrados atingida.");
    }
}

int continuarProcesso(){
    int continuar = 0;
    printf("(0)Não\n(1)Sim\n:");
    scanf("%d", &continuar);    
    if(continuar != 0 && continuar != 1){
        printf("Opção inválida... Digite novamente:\n");
        return continuarProcesso();
    }
    return continuar;
}

void cadastrarInvestimentoExistente(int tipoAplicacao){
    char emissor[100];
    int indiceInvestimento = 0;
    printf("Qual banco você deseja cadastrar nessa aplicação?...:\nBanco do Brasil\nCaixa\nBradesco\nItaú\nSantander\nOBS: se houver na sua opção, informe letra maiúscula e acentos.\n:");
    fgets(emissor, 100, stdin);
    emissor[strlen(emissor) - 1] = '\0';
    if(procurarInvestimento(investimentosCadastrados, tipoAplicacao, emissor) != -1){
        printf("Esse investimento já foi cadastrado, deseja fazer outro cadastro de investimento?\n");            
        return continuarProcesso() ? cadastrarInvestimento() : NULL;
    } 
    if(procurarInvestimento(investimentos, tipoAplicacao, emissor) == -1){
        printf("O emissor informado não está correto. Deseja tentar novamente?\n");
        return continuarProcesso() ? cadastrarInvestimento() : NULL;
    }
    int indiceVazio = encontrarIndiceVazio(investimentosCadastrados, tipoAplicacao);
    indiceInvestimento = procurarInvestimento(investimentos, tipoAplicacao, emissor);
    investimentosCadastrados[tipoAplicacao - 1][indiceVazio] = investimentos[tipoAplicacao - 1][indiceInvestimento];
    qntdInvestimentos++;
}

void cadastrarInvestimentoNovo(int tipoAplicacao){    
    int indiceVazio = 0;
    Investimento investimento;
    investimento.tipoAplicacao = tipoAplicacao;
    printf("Digite o nome do emissor\n:");
    fgets(investimento.emissor, 100, stdin);
    investimento.emissor[strlen(investimento.emissor) - 1] = '\0';
    printf("Informe a taxa do investimento\n:");
    scanf("%f", &investimento.taxa);
    investimento.ativo = 's';
    indiceVazio = encontrarIndiceVazio(investimentosCadastrados, tipoAplicacao);
    investimentosCadastrados[tipoAplicacao - 1][indiceVazio] = investimento;
    qntdInvestimentos++;
}

void cadastrarInvestimento(){
    int tipoAplicacao = 0, novoInvestimento;
    char emissor[100];
    if(qntdInvestimentos == LIMITE_INVESTIMENTOS){     
        printf("Não é mais possível cadastrar investimentos, capacidade máxima de investimentos atingida");
        return;
    }
    printf("Deseja cadastrar uma opção de investimento disponível ou cadastrar um novo investimento?\n(0)Investimento disponível\n(1)Novo investimento\n:");
    scanf("%d", &novoInvestimento);
    printf("Qual tipo de aplicação você deseja cadastrar?\n");
    imprimirTiposDeInvestimento();
    scanf("%d", &tipoAplicacao);
    getchar();
    if(validarCadastroInvestimento(tipoAplicacao) == 0){
        printf("Tipo de aplicação é inválido. Deseja tentar novamente?\n");
        return continuarProcesso() ? cadastrarInvestimento() : NULL;
    };
    if(novoInvestimento){
        cadastrarInvestimentoNovo(tipoAplicacao);
    } else{
        cadastrarInvestimentoExistente(tipoAplicacao);
    }
}

int encontrarIndiceVazio(Investimento investimentos[3][LIMITE_BANCOS], int tipoAplicacao){
    int i = 0;
    for(; i < LIMITE_BANCOS; i++){
        if(strcmp(investimentos[tipoAplicacao - 1][i].emissor,"") == 0){
            break;
        }
    }
    return i;
}

void registrarTransacao(Transacao transacao){
    for(int i = 0; i < LIMITE_CLIENTES; i++){
        int isCurrentClient = strcmp(clientes[i].nome, transacao.cliente.nome) == 0 && strcmp(clientes[i].cpf, transacao.cliente.cpf) == 0;
        if(isCurrentClient){
            for(int j = 0; j < 1000; j++){
                // corrigir o null.
                if(transacoes[i][j].idTransacao == 0){                    
                    transacoes[i][j] = transacao;
                    break;
                }
            }
            break;
        }
    }
}

void realizarTransacao(){
    Transacao transacao;
    int indiceCliente = 0, indiceInvestimento = 0, obterExtrato = 0, dias = 0, tipoAplicacao = 0;
    char cpf[11], emissor[100];    
    printf("Digite o CPF do cliente:");
    scanf("%s", cpf);    
    indiceCliente = encontrarCliente(cpf);
    if(indiceCliente == -1) {
        printf("Cliente não encontrado. Deseja tentar novamente?\n");
        if(continuarProcesso()){realizarTransacao();} else{return;}
    }
    transacao.cliente = clientes[indiceCliente];
    printf("Digite o tipo de aplicação para realizar o investimento\n");
    imprimirTiposDeInvestimento();
    scanf("%d", &tipoAplicacao);
    getchar();
    if(validarInvestimento(tipoAplicacao)){
        printf("Esse tipo de aplicação é inexistente. Deseja tentar novamente?\n");
        if(continuarProcesso()){realizarTransacao();} else{return;}
    };
    imprimirInvestimentos(tipoAplicacao, 'C');
    printf("Digite o emissor de algumas das opções listadas acima\n:");
    fgets(emissor, 100, stdin);
    emissor[strlen(emissor) - 1] = '\0';
    indiceInvestimento = procurarInvestimento(investimentosCadastrados, tipoAplicacao, emissor);
    if(indiceInvestimento == -1){
        printf("Esse investimento não está cadastrado. Deseja tentar novamente?\n");
        if(continuarProcesso()){realizarTransacao();} else{return;}
    }
    transacao.investimento = investimentosCadastrados[tipoAplicacao - 1][indiceInvestimento];
    printf("Digite a data de aplicação no modelo DIA MÊS ANO: ");
    scanf("%d %d %d", &transacao.dataAplicacao.dia, &transacao.dataAplicacao.mes, &transacao.dataAplicacao.ano);
    printf("Digite o valor da aplicação: ");
    scanf("%f", &transacao.valorAplicacao);
    printf("Digite a data de resgate no modelo DIA MÊS ANO: ");
    scanf("%d %d %d", &transacao.dataResgate.dia, &transacao.dataResgate.mes, &transacao.dataResgate.ano);
    dias = calcularDiferencaDeDatas(transacao.dataAplicacao, transacao.dataResgate);
    if(dias < 0){
        printf("Data de resgate inválida. Deseja tentar novamente?\n");
        if(continuarProcesso()){realizarTransacao();} else{return;}
    }
    transacao.valorResgate = calcularValorDeResgate(transacao.valorAplicacao, transacao.investimento, dias);
    qtdTransacoes++;
    transacao.idTransacao = qtdTransacoes;

    registrarTransacao(transacao);
    printf("Deseja obter o extrato? (1)Sim (0)Nao:");
    scanf("%d", &obterExtrato);    
    getchar();   
    if(obterExtrato) gerarExtrato(transacao.cliente);
}

void mudarAtivo(){
    char escolherAtivo;
    int validar = 0;
    int indiceBanco = 0;
    int tipoAplicacao;
    char emissor[100];
    
    printf("Digite o tipo de aplicação do Investimento que você deseja mudar o ativo\n");
    imprimirTiposDeInvestimento();
    scanf("%d", &tipoAplicacao);
    printf("Digite o emissor do Investimento que você deseja mudar o ativo\n:");
    getchar(); 
    fgets(emissor, 100, stdin);
    emissor[strlen(emissor) - 1] = '\0';
    indiceBanco = procurarInvestimento(investimentosCadastrados, tipoAplicacao, emissor);
    if(indiceBanco != -1 && tipoAplicacao > 0 && tipoAplicacao < 4){
        printf("Você deseja deixar o investimento como ativo ou não?\nDigite (s) para deixá-lo ativo e (n) para não deixá-lo ativo..:");
        while(validar == 0){
            scanf("%c", &escolherAtivo);
            getchar();
            if(escolherAtivo == 's' || escolherAtivo == 'n'){
                validar++;
            }else{
                printf("A opção escolhida não existe, tente novamente!!!\n\n");
                mudarAtivo();
            }
        }
    }else{
        printf("O investimento escolhido não existe, tente novamente!!!\n\n");
        mudarAtivo();
    }
    investimentosCadastrados[tipoAplicacao - 1][indiceBanco].ativo = escolherAtivo;
}


void definirOperacao(){
    int operacao = 0;
    printf("Qual operação você deseja fazer?\n(1)Cadastro de cliente\n(2)Cadastro de investimento\n(3)Transação\n(4)Mudar ativo\n");
    scanf("%d", &operacao);
    if(operacao == 1){
        cadastrarCliente();
    } else if(operacao == 2){
        cadastrarInvestimento();
    } else if(operacao == 3){
        realizarTransacao();
    } else if(operacao == 4){
        mudarAtivo();
    } else {
        printf("Essa operação não existe, tente novamente.\n");
    }
}

int main() {
    attInvestimentos();
    while(1){
        definirOperacao();
    }
    return 0;
}
