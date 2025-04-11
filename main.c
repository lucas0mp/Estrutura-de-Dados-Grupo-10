#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

#define MAX_PROCESSOS 1000

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = carregar_dados("processo_043_202409032338.csv", processos, MAX_PROCESSOS);
    if (total == 0) {
        printf("Erro ao carregar os dados.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1. Ordenar por ID (crescente)\n");
        printf("2. Ordenar por data de ajuizamento (decrescente)\n");
        printf("3. Contar processos por id_classe\n");
        printf("4. Contar id_assuntos distintos\n");
        printf("5. Listar processos com mais de um assunto\n");
        printf("6. Dias de tramitacao de um processo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            ordenar_por_id(processos, total);
            salvar_em_csv("ordenado_por_id.csv", processos, total);
            printf("Dados ordenados por ID salvos em 'ordenado_por_id.csv'.\n");
        } else if (opcao == 2) {
            ordenar_por_data(processos, total);
            salvar_em_csv("ordenado_por_data.csv", processos, total);
            printf("Dados ordenados por data salvos em 'ordenado_por_data.csv'.\n");
        } else if (opcao == 3) {
            int classe;
            printf("Digite o id_classe: ");
            scanf("%d", &classe);
            int contagem = contar_por_classe(processos, total, classe);
            printf("Existem %d processos com id_classe %d.\n", contagem, classe);
        } else if (opcao == 4) {
            int distintos = contar_assuntos_distintos(processos, total);
            printf("Existem %d id_assuntos distintos.\n", distintos);
        } else if (opcao == 5) {
            listar_multiplos_assuntos(processos, total);
        } else if (opcao == 6) {
            int i;
            printf("Digite o Indice do processo (0 a %d): ", total - 1);
            scanf("%d", &i);
            if (i >= 0 && i < total) {
                int dias = calcular_dias_em_tramitacao(processos[i].data_ajuizamento);
                printf("O processo está em tramitação há %d dias.\n", dias);
            } else {
                printf("Indice invalido.\n");
            }
        }

    } while (opcao != 0);

    return 0;
}
