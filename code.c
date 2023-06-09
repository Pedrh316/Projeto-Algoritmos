#include <stdio.h>
#include <string.h>
/*
    Pedro Henrique da Costa Silva
*/

int validarData(int dia, int mes, int ano){
    return (dia > 1 && dia < 30) && (mes > 1 || mes < 12) && (ano > 1900 || ano < 2023);
}

int validarTelefone(int ddd, char num[9]){
    return ((strlen(num) == 9 && num[0] == '9') || (strlen(num) == 8)) && (ddd > 11 && ddd < 91);
}



struct Data {
    int dia[2];
    int mes[2];
    int ano[4];
}

struct Telefone {
    char ddd[2];
    char num[9];
}

int main(){


    printf("oi");

}