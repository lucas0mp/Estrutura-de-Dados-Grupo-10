#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int carregar_dados(const char* nome_arquivo, Processo processos[], int max) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char linha[256];
    int i = 0;
    fgets(linha, sizeof(linha), arquivo); // Ignora o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) && i < max) {
        char* token = strtok(linha, ",");
        processos[i].id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(processos[i].numero, token);

        token = strtok(NULL, ",");
        strcpy(processos[i].data_ajuizamento, token);

        token = strtok(NULL, ",{}");
        processos[i].id_classe = atoi(token);

        processos[i].num_assuntos = 0;
        token = strtok(NULL, ",{}");
        while (token && processos[i].num_assuntos < 10) {
            processos[i].id_assunto[processos[i].num_assuntos++] = atoi(token);
            token = strtok(NULL, ",{}");
        }

        token = strtok(NULL, ",\n");
        processos[i].ano_eleicao = atoi(token);

        i++;
    }

    fclose(arquivo);
    return i;
}

void salvar_em_csv(const char* nome_arquivo, Processo processos[], int n) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) return;

    fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
    for (int i = 0; i < n; i++) {
        fprintf(arquivo, "%d,\"%s\",%s,{%d},{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento, processos[i].id_classe);
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(arquivo, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].num_assuntos - 1) fprintf(arquivo, ",");
        }
        fprintf(arquivo, "},%d\n", processos[i].ano_eleicao);
    }

    fclose(arquivo);
}

void ordenar_por_id(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processos[i].id > processos[j].id) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

int contar_por_classe(Processo processos[], int n, int classe) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (processos[i].id_classe == classe) {
            count++;
        }
    }
    return count;
}

int contar_assuntos_distintos(Processo processos[], int n) {
    int encontrados[10000] = {0};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            encontrados[processos[i].id_assunto[j]] = 1;
        }
    }

    int total = 0;
    for (int i = 0; i < 10000; i++) {
        if (encontrados[i]) total++;
    }

    return total;
}

void listar_multiplos_assuntos(Processo processos[], int n) {
    printf("\nProcessos com mais de um assunto:\n");
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("ID: %d, Numero: %s\n", processos[i].id, processos[i].numero);
        }
    }
}

int calcular_dias_em_tramitacao(const char* data_inicio) {
    struct tm tm_inicio = {0};

    // Converte a string manualmente com sscanf
    sscanf(data_inicio, "%d-%d-%d %d:%d:%d",
           &tm_inicio.tm_year, &tm_inicio.tm_mon, &tm_inicio.tm_mday,
           &tm_inicio.tm_hour, &tm_inicio.tm_min, &tm_inicio.tm_sec);

    // Ajustes necessários para o struct tm
    tm_inicio.tm_year -= 1900;
    tm_inicio.tm_mon -= 1;

    time_t t_inicio = mktime(&tm_inicio);
    time_t t_atual = time(NULL);

    double segundos = difftime(t_atual, t_inicio);
    return (int)(segundos / (60 * 60 * 24));
}
