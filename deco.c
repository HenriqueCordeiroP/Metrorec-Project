// fonta
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include "metrorec.c"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
atomic_int counter = ATOMIC_VAR_INIT(0);
void *passageiros_thread(void *arg)
{
    struct estacao *estacao = (struct estacao *)arg;
    estacao_espera_pelo_vagao(estacao);

    atomic_fetch_add(&counter, 1);

    return NULL;
}

struct vagao_args
{
    struct estacao *estacao;
    int assentos_livres;
};

void *vagao_thread(void *args)
{
    struct vagao_args *vargs = (struct vagao_args *)args;
    estacao_preecher_vagao(vargs->estacao, vargs->assentos_livres);
    return NULL;
}

void run_test(int num_passageiros, int num_assentos)
{
    struct estacao estacao;
    estacao_init(&estacao);

    pthread_t passageiros[num_passageiros];

    for (int i = 0; i < num_passageiros; i++)
    {
        pthread_create(&passageiros[i], NULL, passageiros_thread, &estacao);
    }

    while (num_passageiros > 0)
    {
        struct vagao_args vargs;
        vargs.estacao = &estacao;
        vargs.assentos_livres = num_assentos;

        pthread_t vagao;
        pthread_create(&vagao, NULL, vagao_thread, &vargs);

        int numPassengersReap = (num_passageiros > num_assentos) ? num_assentos : num_passageiros;
        num_passageiros -= numPassengersReap;

        for (int i = 0; i < numPassengersReap; i++)
        {
            while (counter != numPassengersReap)
                ;
            // aguarda o retorno das threads de passageiros
            estacao_embarque(&estacao);
        }
        atomic_store(&counter, 0);
    }
}

int main(void)
{
    run_test(112, 3);
    // passageiros, assentos
    return 0;
}