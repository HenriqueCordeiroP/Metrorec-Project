#include <pthread.h>

struct estacao {
    int contPassageiros;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int assentosLivres;
    pthread_cond_t condVagao;
    int id; 
};
void estacao_init(struct estacao *estacao) {
    pthread_mutex_init(&estacao->mutex, NULL);
    pthread_cond_init(&estacao->cond, NULL);
    pthread_cond_init(&estacao->condVagao, NULL);
    estacao->contPassageiros = 0;
}

void estacao_preecher_vagao(struct estacao * estacao, int assentos) { // 1 thread
    pthread_mutex_lock(&estacao->mutex);
    estacao->assentosLivres = assentos;
    pthread_cond_broadcast(&estacao->cond);
    pthread_cond_wait(&estacao->condVagao, &estacao->mutex);
    printf("Vagão indo embora\n");
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    estacao->contPassageiros++;
    printf("Passageiros: %d \n", estacao->contPassageiros);
    pthread_cond_wait(&estacao->cond, &estacao->mutex);
    while(estacao->contPassageiros > 0){
        if(estacao->assentosLivres > 0 && estacao->contPassageiros > 0){
            estacao->assentosLivres--;
            estacao->contPassageiros--;
        } else{
                pthread_cond_wait(&estacao->cond, &estacao->mutex);
        }
    }
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_embarque(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    printf("Embarcado: %d\n", estacao->id);
    if(estacao->assentosLivres == 0){
        pthread_cond_signal(&estacao->condVagao);
    }
    pthread_mutex_unlock(&estacao->mutex);
}

// usar array pra ocupar lugares? 
    // melhor do que variável contadora de assentos?
// resolver vagão indo embora ou chegando antes
