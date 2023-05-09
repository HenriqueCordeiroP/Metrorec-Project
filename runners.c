// eu
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "metrorec.c"

void *passageiros_thread(void *arg)
{
    struct estacao *estacao = (struct estacao *) arg;
    estacao_espera_pelo_vagao(estacao);
    return NULL;
}

struct vagao_args {
    struct estacao *estacao;
    int assentos_livres;
};

void *vagao_thread(void *args)
{
    struct vagao_args *vargs = (struct vagao_args *) args;
    estacao_preencher_vagao(vargs->estacao, vargs->assentos_livres);
    return NULL;
}

int main(void)
{

            // criação da estação
            struct estacao estacao;
            estacao_init(&estacao);
            printf("Bem vindo ao METROREC.\n");

            // definição da quantidade de passageiros
            int numPassageiros = 12;
            pthread_t threadsPassageiros[numPassageiros];
            printf("Há %d passageiros na plataforma.\n", numPassageiros);

            for (int i = 0 ; i < numPassageiros ; i++) {
                estacao.id = i+1;
                pthread_create(&threadsPassageiros[i], NULL, passageiros_thread, &estacao);
                printf("Passageiro [%d] criado.\n", estacao.id);
            }
        
            // variáveis usadas no loop
            pthread_t vagao;
            struct vagao_args vargs;
            int restante = numPassageiros;
            int reap;
            int contVagao = 0;
            // definição da struct

            while(restante != 0){
                vargs.estacao = &estacao;
                vargs.assentos_livres = 5;
                // lançamento do vagão
                pthread_create(&vagao, NULL, vagao_thread, &vargs);
                printf("Um vagão está a caminho =D\n");
                contVagao++;
                // definição de quantos entram no vagão
                if(restante > vargs.assentos_livres){
                    reap = vargs.assentos_livres;
                } else{
                    reap = restante;
                }

                // atualização dos restantes na plataforma
                restante -= reap;

                for(int j = 0 ; j < reap ; j ++){
                    estacao_embarque(&estacao);
                }
            }
            printf("Todos os passageiros foram embarcados! (%d)\n", restante);
            printf("Vagões utilizados: %d\n", contVagao);
        return 0;
}

// falk
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <pthread.h>

// #include "metrorec.c"

// void *passageiros_thread(void *arg)
// {
//     struct estacao *estacao = (struct estacao *) arg;
//     estacao_espera_pelo_vagao(estacao);
//     printf("Passageiro embarcou\n");
//     return NULL;
// }

// struct vagao_args {
//     struct estacao *estacao;
//     int assentos_livres;
// };

// void *vagao_thread(void *args)
// {
//     struct vagao_args *vargs = (struct vagao_args *) args;
//     estacao_preecher_vagao(vargs->estacao, vargs->assentos_livres);
//     printf("Vagão preenchido\n");
//     return NULL;
// }

// int main(void)
// {
//     struct estacao estacao;
//     estacao_init(&estacao);

//     int num_passageiros = 11;
//     pthread_t passageiros[num_passageiros];

//     for (int i = 0; i < num_passageiros; i++) {
//         pthread_create(&passageiros[i], NULL, passageiros_thread, &estacao);
//         printf("Passageiro %d criado\n", i);
//     }

//     int num_assentos = 3;
//     int num_passageiros_embarcados = 0;
//     int cont=0;

//     while (num_passageiros_embarcados < num_passageiros) {
//         struct vagao_args vargs;
//         vargs.estacao = &estacao;
//         vargs.assentos_livres = num_assentos;

//         pthread_t vagao;
//         pthread_create(&vagao, NULL, vagao_thread, &vargs);
//         printf("Vagão criado com %d assentos livres\n", num_assentos);
//         cont++;

//         int num_passageiros_a_reap = num_passageiros - num_passageiros_embarcados;
//         if (num_passageiros_a_reap > num_assentos) {
//             num_passageiros_a_reap = num_assentos;
//         }

//         for (int i = 0; i < num_passageiros_a_reap; i++) {
//             estacao_embarque(&estacao);
//             num_passageiros_embarcados++;
//             printf("Passageiro embarcou no vagão\n");
//         }

//         printf("Todos os passageiros embarcados no vagão\n");
//     }
//     printf("total de vagoes: %d\n",cont);
//     printf("Todos os passageiros foram transportados\n");

//     return 0;
// }

// fonta
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <pthread.h>

// #include "metrorec.c"

// void *passageiros_thread(void *arg)
// {
//     struct estacao *estacao = (struct estacao *)arg;
//     estacao_espera_pelo_vagao(estacao);
//     return NULL;
// }

// struct vagao_args
// {
//     struct estacao *estacao;
//     int assentos_livres;
// };

// void *vagao_thread(void *args)
// {
//     struct vagao_args *vargs = (struct vagao_args *)args;
//     estacao_preecher_vagao(vargs->estacao, vargs->assentos_livres);
//     return NULL;
// }

// int main(void)
// {

//     // create one station
//     struct estacao station;
//     estacao_init(&station);

//     //
//     // create a number of passengers
//     // each passenger is a thread
//     int numPassengers = 10;
//     pthread_t passengerThreads[numPassengers];
//     for (int i = 0; i < numPassengers; i++)
//     {
//         pthread_create(&passengerThreads[i], NULL, passageiros_thread, (void *)&station);
//         printf("Creating thread %d!\n", i + 1);
//     }

//     int seatsPerCar = 3, cont = 0;
//     pthread_t carThread;

//     // loop to create as many car as necessary to board all passengers
//     while (numPassengers > 0)
//     {
//         printf("1\n");
//         // Criar uma nova estrutura carArgs para cada iteração do loop
//         struct vagao_args *carArgs = malloc(sizeof(struct vagao_args));
//         carArgs->estacao = &station;
//         carArgs->assentos_livres = seatsPerCar;

//         printf("2\n");
//         // create only one car with a number of free seats,this car is associated to a thread
//         pthread_create(&carThread, NULL, vagao_thread, (void *)carArgs);

//         /* if (numPassengers == 0)
//         {
//             break;
//         } */

//         printf("3\n");
//         // define the number of passenger to reap
//         int numPassengersToReap = (numPassengers > seatsPerCar) ? seatsPerCar : numPassengers;
//         numPassengers -= numPassengersToReap;
//         // for each thread associated to a passenger that finished

//         printf("4\n");
//         for (int i = 0; i < numPassengersToReap; i++)
//         {
//             // call estacao_embarque function to let the car know that the passenger is on board
//             estacao_embarque(&station);
//         }
//         printf("5\n");

//         if (numPassengers == 0)
//         {
//             for (int i = 0; i < cont; i++)
//             {
//                 free(carArgs);
//             }
//         }

//         printf("6\n");
//     }
//     printf("7\n");
//     pthread_mutex_destroy(&station.mutex);
//     /*     pthread_cond_destroy(&station.condVagao);
//         pthread_cond_destroy(&station.condChegou);
//         pthread_cond_destroy(&station.condEmbarque);
//         pthread_cond_destroy(&station.condPegouLock); */
//     printf("8\n");
// }