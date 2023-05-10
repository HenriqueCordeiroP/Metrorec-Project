#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "metrorec.c"

void *passageiros_thread(void *arg)
{
    struct estacao *estacao = (struct estacao *)arg;
    estacao_espera_pelo_vagao(estacao);
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

int main(void)
{

    // create one station
    struct estacao station;
    estacao_init(&station);

    //
    // create a number of passengers
    // each passenger is a thread
    int numPassengers = 10;
    pthread_t passengerThreads[numPassengers];
    for (int i = 0; i < numPassengers; i++)
    {
        pthread_create(&passengerThreads[i], NULL, passageiros_thread, (void *)&station);
        printf("Creating thread %d!\n", i + 1);
    }

    int seatsPerCar = 3, cont = 0;
    pthread_t carThread;

    // loop to create as many car as necessary to board all passengers
    while (numPassengers > 0)
    {
        printf("1\n");
        // Criar uma nova estrutura carArgs para cada iteração do loop
        struct vagao_args *carArgs = malloc(sizeof(struct vagao_args));
        carArgs->estacao = &station;
        carArgs->assentos_livres = seatsPerCar;

        printf("2\n");
        // create only one car with a number of free seats,this car is associated to a thread
        pthread_create(&carThread, NULL, vagao_thread, (void *)carArgs);

        /* if (numPassengers == 0)
        {
            break;
        } */

        printf("3\n");
        // define the number of passenger to reap
        int numPassengersToReap = (numPassengers > seatsPerCar) ? seatsPerCar : numPassengers;
        numPassengers -= numPassengersToReap;
        // for each thread associated to a passenger that finished

        printf("4\n");
        for (int i = 0; i < numPassengersToReap; i++)
        {
            // call estacao_embarque function to let the car know that the passenger is on board
            estacao_embarque(&station);
        }
        printf("5\n");

        if (numPassengers == 0)
        {
            for (int i = 0; i < cont; i++)
            {
                free(carArgs);
            }
        }

        printf("6\n");
    }
    printf("7\n");
    pthread_mutex_destroy(&station.mutex);
    /*     pthread_cond_destroy(&station.condVagao);
        pthread_cond_destroy(&station.condChegou);
        pthread_cond_destroy(&station.condEmbarque);
        pthread_cond_destroy(&station.condPegouLock); */
    printf("8\n");
}