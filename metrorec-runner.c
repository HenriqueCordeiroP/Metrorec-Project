// hendrich
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "metrorec.c"

void *passageiros_thread(void *arg) {
    struct estacao *estacao = (struct estacao *)arg;
    estacao_espera_pelo_vagao(estacao);
    estacao_embarque(estacao);
    return NULL;
}

struct vagao_args {
    struct estacao *estacao;
    int assentos_livres;
};

void *vagao_thread(void *args) {
    struct vagao_args *vargs = (struct vagao_args *)args;
    estacao_preencher_vagao(vargs->estacao, vargs->assentos_livres);
    return NULL;
}

void run_test(int num_passageiros, int num_assentos) {
    struct estacao estacao;
    estacao_init(&estacao);

    pthread_t passageiros[num_passageiros];

    for (int i = 0; i < num_passageiros; i++) {
        pthread_create(&passageiros[i], NULL, passageiros_thread, &estacao);
    }

    int num_passageiros_embarcados = 0;

    while (num_passageiros_embarcados < num_passageiros) {
        struct vagao_args vargs;
        vargs.estacao = &estacao;
        vargs.assentos_livres = num_assentos;

        pthread_t vagao;
        pthread_create(&vagao, NULL, vagao_thread, &vargs);
        pthread_join(vagao, NULL);

        num_passageiros_embarcados += num_assentos;
        if (num_passageiros_embarcados > num_passageiros) {
            num_passageiros_embarcados = num_passageiros;
        }
    }

    for (int i = 0; i < num_passageiros; i++) {
        printf("Pre Join");
        pthread_join(passageiros[i], NULL);
    }
}

int main(void) {
    // Execute a função 'run_test' com diferentes valores para os testes
    run_test(12, 5);
    // run_test(10, 3);
    // run_test(6, 4);
    // run_test(100, 25);

    return 0;
}