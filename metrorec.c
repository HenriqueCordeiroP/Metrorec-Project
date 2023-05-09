#include <pthread.h>
#include <stdio.h>

struct estacao {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_cond_t condVagao;
    int contPassageiros;
    int assentosLivres;
    int alocando;
    int id; 
};
int i = 1;
void estacao_init(struct estacao *estacao) {
    pthread_mutex_init(&estacao->mutex, NULL);
    pthread_cond_init(&estacao->cond, NULL);
    pthread_cond_init(&estacao->condVagao, NULL);
    estacao->contPassageiros = 0;
}

void estacao_preencher_vagao(struct estacao * estacao, int assentos) { // 1 thread
    pthread_mutex_lock(&estacao->mutex);
    // printf("PV - Passageiros - %d\n", estacao->contPassageiros);
    estacao->assentosLivres = assentos;
    estacao->alocando = assentos;
    pthread_cond_broadcast(&estacao->cond);
    pthread_cond_wait(&estacao->condVagao, &estacao->mutex);
    printf("Vagão indo embora\n");
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_espera_pelo_vagao(struct estacao * estacao) { // passou dessa ta valendo
    pthread_mutex_lock(&estacao->mutex);
    estacao->contPassageiros++;
    // printf("Passageiros: %d \n", estacao->contPassageiros);
    pthread_cond_wait(&estacao->cond, &estacao->mutex); // ele respeita o mutex na volta?
    while(estacao->contPassageiros > 0){
        if(estacao->assentosLivres > 0 && estacao->contPassageiros > 0){
            printf("EPV - Passageiros - %d\n", estacao->contPassageiros);
            estacao->contPassageiros--; // erro
            estacao->assentosLivres--;
        } else{
            pthread_cond_wait(&estacao->cond, &estacao->mutex);
        }
    }
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_embarque(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    // printf("E - Passageiros %d\n",estacao->contPassageiros);
    // printf("E - Livres - %d\n", estacao->assentosLivres);
    // estacao->assentosLivres--; // faz sentido?
    estacao->alocando--;
    // printf("E - Embarcado: %d\n", i++);
    // printf("E - Passageiros - %d\n", estacao->contPassageiros);
    if(estacao->alocando ==  0 || estacao->contPassageiros == 0){
        pthread_cond_signal(&estacao->condVagao);   
    }
    pthread_mutex_unlock(&estacao->mutex);
}
// embarque so chama quando a thread do passageiro termina

// usar array pra ocupar lugares? 
    // melhor do que variável contadora de assentos?
// resolver vagão indo embora ou chegando antes
