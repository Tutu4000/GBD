#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include<cstdio>

typedef struct registro
{
    unsigned long chave;
    char nome[100];
    time_t nascimento;
} registro;

bool sort_por_nome(const std::pair<std::string,int> & a, const std::pair<std::string,int> & b){
    return a.first < b.first;
}

void criar_indice(char *arquivo_dados, char *arquivo_indices){
    std::vector<registro> registros;
    std::vector<std::pair<std::string,int>> indices;
    registro r;


    FILE *f_dados = fopen(arquivo_dados, "rb");
    FILE *f_indice = fopen(arquivo_indices, "wb");

    int indice = 0;
    while(fread(&r, sizeof(registro),1,f_dados)){
        registros.push_back(r);
        indices.push_back(std::make_pair(std::string(r.nome), indice));
        indice++;
    }

    std::sort(indices.begin(),indices.end(), sort_por_nome);

    for(const auto& i: indices){
        fwrite(&i.second, sizeof(int), 1,f_indice);
    }

    fclose(f_indice);
    fclose(f_dados);
};

void ler_arquivo_indice(char *nome_arquivo){
    int i;
    FILE *f_arquivo_indices = fopen(nome_arquivo, "rb");
    while(fread(&i,sizeof(int),1,f_arquivo_indices)){
        std::cout << i << std::endl;
    }
}

void listar_nomes(char *nome_arquivo){
    registro r;
    FILE *f_dados = fopen(nome_arquivo, "rb");
    while(fread(&r, sizeof(registro),1,f_dados)){
        std::string nome(r.nome);
        struct tm *t = localtime(&r.nascimento);
        std::string data_nascimento = ctime(&r.nascimento);
        data_nascimento.erase(data_nascimento.end() - 1);
        std::cout << nome << " " << t->tm_mday << "/" << t->tm_mon+1 << "/" << t->tm_year+1900 << " " << data_nascimento << std::endl;
    }
    fclose(f_dados);
}

void buscar_nome(char *nome_buscar, char *arquivo_dados, char *arquivo_indices){
    registro r;
    int i;
    FILE *f_dados = fopen(arquivo_dados, "rb");
    FILE *f_indice = fopen(arquivo_indices,"rb");
    int inicio = 0, fim, meio;

    fseek(f_indice, 0,SEEK_END);
    fim = ftell(f_indice)/ sizeof(int) -1;

    while(inicio <= fim){
        meio = (inicio+fim)/2;
        fseek(f_indice, meio * sizeof(int), SEEK_SET);
        fread(&i, sizeof(int), 1,f_indice);
        fseek(f_dados, i * sizeof(registro), SEEK_SET);
        fread(&r, sizeof(registro), 1, f_dados );

        std::string nome_meio(r.nome);
        int cmp = nome_meio.compare(nome_buscar);
        if (cmp == 0){
            struct tm *t = localtime(&r.nascimento);
            std::string data_nascimento = ctime(&r.nascimento);
            data_nascimento.erase(data_nascimento.end() - 1);
            std::cout << r.nome << " " << t->tm_mday << "/" << t->tm_mon+1 << "/" << t->tm_year+1900 << " " << data_nascimento;
            fclose(f_dados);
            fclose(f_indice);
            return;
        }
        if (cmp < 0){
            inicio = meio+1;
        }
        else{
            fim = meio-1;
        }
    }
    fclose(f_dados);
    fclose(f_indice);
    std::cout << "Nome nao encontrado" << std::endl;
    return;
}

int main(){
    char* nome_buscar = "Frederico Souza Lazzarini";
    char* arquivo_dados = "dados.bin";
    char* arquivo_indices = "indices.bin";


    criar_indice(arquivo_dados,arquivo_indices);
    buscar_nome(nome_buscar, arquivo_dados, arquivo_indices);

    return 0;
}
