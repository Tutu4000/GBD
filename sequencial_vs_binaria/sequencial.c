#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct registro
{
    unsigned long chave;
    char nome[100];
    time_t nascimento;
} registro;


void buscar_nome(char *nome_buscar, char *arquivo);
void buscar_posicao(int posicao, char *arquivo);

int main()
{
    clock_t start = 0, end = 0;
    time_t startsec = 0, endsec = 0;
    char nome_buscar[100] = {"Carlos Domingues"};
    registro r;
    FILE *f = fopen("dados.bin", "rb+");
    start = clock();
    //startsec = time(NULL);
    printf("Busca sequencial");
    buscar_nome(nome_buscar, "dados.bin");
    //endsec = time(NULL);
    end = clock();
    printf("Tempo em milisegundos: %f", (end-start)/ (double) CLOCKS_PER_SEC);
   // printf("\nTempo em segundos: %f", (startsec-endsec));

    printf("\n");

    start = clock();
    printf("\nBusca por posicao:\n");
    buscar_posicao( 3910, "dados.bin");
    end = clock();
    printf("Tempo em milisegundos: %f", (end-start)/ (double) CLOCKS_PER_SEC);

}

void buscar_nome(char *nome_buscar, char *arquivo) {
    registro r;
    FILE *f = fopen(arquivo, "rb");
    if (f == NULL) {
        printf("Arquivo nao pode ser aberto%s\n", arquivo);
        return;
    }

    while (fread(&r, sizeof(registro), 1, f)) {
        if (strcmp(r.nome, nome_buscar) == 0) {
            struct tm *t = localtime(&r.nascimento);
            printf("%s %d/%d/%d %s", r.nome, t->tm_mday, t->tm_mon+1, t->tm_year+1900, ctime(&r.nascimento));
            fclose(f);
            return;
        }
    }

    printf("Nome %s não encontrado no arquivo %s\n", nome_buscar, arquivo);
    fclose(f);
}

void buscar_posicao(int posicao, char *arquivo) {
    registro r;
    FILE *f = fopen(arquivo, "rb");
    if (f == NULL) {
        printf("Arquivo nao pode ser aberto %s\n", arquivo);
        return;
    }

    fseek(f, posicao * sizeof(registro), SEEK_SET);
    if (fread(&r, sizeof(registro), 1, f)) {
        struct tm *t = localtime(&r.nascimento);
        printf("%s %d/%d/%d %s", r.nome, t->tm_mday, t->tm_mon+1, t->tm_year+1900, ctime(&r.nascimento));
    } else {
        printf("Não foi possível ler o registro na posição %d com %d saltos no arquivo %s\n", posicao, arquivo);
    }

    fclose(f);
}