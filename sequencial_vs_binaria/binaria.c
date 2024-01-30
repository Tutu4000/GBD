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

typedef struct indice
{
    char nome[100];
    int posicao;
} indice;
//obs: Posição aqui não significa índice, o índice é definido pela posição do struct indice no arquivo de indices. Posição é a posição real de tal nome no struct original

int comparar(const void *a, const void *b) {
    return strcmp(((indice *)a)->nome, ((indice *)b)->nome); //qsort recebe ponteiros para void que precisam receber cast para o desejado
}

void criar_indice(char *arquivo_dados, char *arquivo_indices) {
    registro r;
    indice i;
    FILE *f_dados = fopen(arquivo_dados, "rb");
    FILE *f_indices = fopen(arquivo_indices, "wb");
    int posicao = 0;

    while (fread(&r, sizeof(registro), 1, f_dados)) {
        strcpy(i.nome, r.nome);
        i.posicao = posicao;
        fwrite(&i, sizeof(indice), 1, f_indices);
        posicao++;
    }

    fclose(f_dados);
    fclose(f_indices);


    //sorting dos indices
    FILE *f = fopen(arquivo_indices, "rb+");
    indice *indices = malloc(posicao * sizeof(indice));
    fread(indices, sizeof(indice), posicao, f);
    qsort(indices, posicao, sizeof(indice), comparar);
    rewind(f);
    fwrite(indices, sizeof(indice), posicao, f);
    free(indices);
    fclose(f);
}

void buscar_nome(char *nome_buscar, char *arquivo_dados, char *arquivo_indices) {
    registro r;
    indice i;
    FILE *f_dados = fopen(arquivo_dados, "rb");
    FILE *f_indices = fopen(arquivo_indices, "rb");
    int inicio = 0, fim, meio;

    fseek(f_indices, 0, SEEK_END);  //move o ponteiro para o final para então calcular o fim (necessario para busca binaria)
    fim = ftell(f_indices) / sizeof(indice) - 1;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        fseek(f_indices, meio * sizeof(indice), SEEK_SET);
        fread(&i, sizeof(indice), 1, f_indices);

        int cmp = strcmp(i.nome, nome_buscar);
        if (cmp == 0) {
            fseek(f_dados, i.posicao * sizeof(registro), SEEK_SET);
            fread(&r, sizeof(registro), 1, f_dados);
            printf("%s encontrado na posicao %d\n", r.nome, i.posicao);
            struct tm *t = localtime(&r.nascimento);
            printf("%s %d/%d/%d %s", r.nome, t->tm_mday, t->tm_mon+1, t->tm_year+1900, ctime(&r.nascimento));
            fclose(f_dados);
            fclose(f_indices);
            return;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    fclose(f_dados);
    fclose(f_indices);
    printf("\nNome não encontrado\n");
    return;
}

int main() {
    clock_t start = 0, end = 0;
    start = clock();
    criar_indice("dados.bin", "indices.bin");
    end = clock();
    printf("Tempo para criacao do arquivo indices: %f\n", (end-start)/ (double) CLOCKS_PER_SEC);

    start = clock();
    buscar_nome("Carlos Domingues", "dados.bin", "indices.bin");
    end = clock();
    printf("\nTempo para busca binaria no arquivo: %f\n", (end-start)/ (double) CLOCKS_PER_SEC);
    return 0;
}