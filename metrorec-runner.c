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
	estacao_preecher_vagao(vargs->estacao, vargs->assentos_livres);
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
                // pthread_cond_wait(&estacao.condVagao, &estacao.mutex);
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
                    printf("Embarcado com sucesso :)\n");
                }
            }
            printf("Todos os passageiros foram embarcados! (%d)\n", restante);
            printf("Vagões utilizados: %d\n", contVagao);
        return 0;
}

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
