#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char nomes[60][20] = {"Abner","Ana","Anderson","Antonio","Arthur","Augusto","Beatriz","Brendo","Caio","Carla","Carlos","Clara","Claudia","Claudio","Enzo","Ernani","Fabiana","Felipe","Fernanda","Fernando","Flavia","Flavio","Frederico","Gabriel","Geraldo","Geraldo","Giovanna","Graziela","Guilherme","Gustavo","Henrique","Hugo","Humberto","Joao","Jose","Julia","Laylla","Lucas","Luisa","Luiz","Luiza","Manoela","Marcela","Marcelo","Marcus","Maria","Marques","Matheus","Pacheco","Pedro","Rafael","Renata","Thiago","Valter","Victor","Vinicius","Vinicius","Vitor","Wallace","Yuri"};
char sobrenomes[60][20] = {"Almeida","Alves","Amorim","Andrade","Araujo","Borges","Chagas","Corsi","Cunha","Custodio","Domingues","Duarte","Faria","Farias","Ferreira","Florencio","Fontes","Franco","Freitas","Gastao","Godoi","Gomes","Goulart","Gouveia","Graciano","Guimaraes","Ignacio","Lazzarini","Lima","Macarini","Macedo","Matos","Martins","Matias","Melo","Meres","Nunes","Oliveira","Otoni","Paula","Pereira","Pinheiro","Pinho","Pio","Pontes","Prado","Reis","Ribeiro","Rocha","Rodrigues","Royer","Silva","Siqueira","Soares","Souza","Sticca","Ventino","Ventura","Vezono"};

void novo_nome(char *str) {
   int posicao = rand() % 60;
   strcpy(str,nomes[posicao]); // primeiro nome
   strcat(str," ");
   int segundonome = rand() % 2;
   if (segundonome == 1) {
       posicao = rand() % 60;
       strcat(str,nomes[posicao]); // segundo nome
       strcat(str," ");
   }
   int quantidade = rand() % 3 + 1;
   for (int i = 0; i < quantidade; i++) {
       posicao = rand() % 60;
       strcat(str,sobrenomes[posicao]); // sobrenomes
       if (i < quantidade-1)
           strcat(str," ");
   }
}

void novo_nascimento(time_t *data) {
    time(data); // data corrente
    int dia = rand() % 18250; // um dia nos últimos 50 anos
    *data -= (dia * 24 * 60 * 60);
}

typedef struct registro { unsigned long chave; char nome[100]; time_t nascimento; } registro;

int main() {
    srand(time(NULL));

    int numregistros = 10000000;
    printf("Tamanho do registro: %d bytes\n", sizeof(registro));
    printf("Numero de registros: %d\n", numregistros);
    printf("Tamanho do arquivo: %d bytes (%.1f Mbytes)\n", sizeof(registro) * numregistros, (sizeof(registro) * numregistros) / (float)(1024*1024));

    registro r;
    FILE *f = fopen("dados.bin","wb+");
    for (int i = 1; i <= numregistros; i++) {
        r.chave = i;
        novo_nome(r.nome);
        novo_nascimento(&r.nascimento);
        fwrite(&r, sizeof(r), 1, f);
        if ((i + 1) % (numregistros/10) == 0) printf("\n%.0f%%",((i + 1)/(float)numregistros)*100.0);
        //struct tm * t = localtime(&r.nascimento);
        //printf("%s %d/%d/%d %s", r.nome, t->tm_mday, t->tm_mon+1, t->tm_year+1900, ctime(&r.nascimento));
    }
    fclose(f);

    return 0;
}
