#include <pthread.h>

struct estacao {
    int contPassageiros;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int assentosLivres;
    pthread_cond_t condVagao;
    int id; 
    int libera;
    int assentos;
};

void estacao_init(struct estacao *estacao) {
    pthread_mutex_init(&estacao->mutex, NULL);
    pthread_cond_init(&estacao->cond, NULL);
    pthread_cond_init(&estacao->condVagao, NULL);
    estacao->contPassageiros = 0;
}

void estacao_preencher_vagao(struct estacao * estacao, int assentos) {
    pthread_mutex_lock(&estacao->mutex);
    estacao->assentosLivres = assentos;
    estacao->assentos = assentos;
    estacao->libera = 0;
    printf("Vagão chegou\n");
    pthread_cond_broadcast(&estacao->cond);
    pthread_cond_wait(&estacao->condVagao, &estacao->mutex);
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    estacao->contPassageiros++;
    pthread_cond_wait(&estacao->cond, &estacao->mutex);
    while(estacao->contPassageiros > 0){
        if(estacao->assentosLivres >= estacao->contPassageiros){
            estacao->libera = estacao->assentos - estacao->contPassageiros;
        }
        if(estacao->assentosLivres > 0 && estacao->contPassageiros > 0){
            estacao->assentosLivres--;
            estacao->contPassageiros--;
            break;
        } else {
                pthread_cond_wait(&estacao->cond, &estacao->mutex);
        }
    }
    pthread_mutex_unlock(&estacao->mutex);
    // sleep(1);
}

void estacao_embarque(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    estacao->libera++;
    if(estacao->libera == estacao->assentos){
        // estacao->libera = 0;
        pthread_cond_signal(&estacao->condVagao);
    }
    pthread_mutex_unlock(&estacao->mutex);
}