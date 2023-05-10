#include <pthread.h>

struct estacao {
    int contPassageiros;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int assentosLivres;
    pthread_cond_t condVagao;
    int id; 
    int libera;
};

int i = 1;
void estacao_init(struct estacao *estacao) {
    pthread_mutex_init(&estacao->mutex, NULL);
    pthread_cond_init(&estacao->cond, NULL);
    pthread_cond_init(&estacao->condVagao, NULL);
    estacao->contPassageiros = 0;
}

void estacao_preecher_vagao(struct estacao * estacao, int assentos) { // 1 thread
    pthread_mutex_lock(&estacao->mutex);
    estacao->assentosLivres = assentos;
    estacao->libera = 1;
    printf("Vagão chegando\n");
    pthread_cond_broadcast(&estacao->cond);
    pthread_cond_wait(&estacao->condVagao, &estacao->mutex);
    printf("Vagão indo embora\n");
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    estacao->contPassageiros++;
    // printf("Passageiros a: %d \n", estacao->contPassageiros);
    pthread_cond_wait(&estacao->cond, &estacao->mutex);
    while(estacao->contPassageiros > 0){
        if(estacao->assentosLivres > 0 && estacao->contPassageiros > 0){
            estacao->assentosLivres--;
            // printf("Ass %d\n", estacao->assentosLivres);
            estacao->contPassageiros--;
            // printf("Passageiros: %d\n", estacao->contPassageiros);
            printf("Id: %ld\n", pthread_self());
            // printf("Embarcado: %d\n", estacao->id);
            break;
        } else {
                // printf("Wait\n");
                pthread_cond_wait(&estacao->cond, &estacao->mutex);
        }
    }
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_embarque(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    // if(estacao->assentosLivres == 0 || estacao->contPassageiros == 0){ // quando é par não libera  
        // estacao->libera--;
        // if(estacao->contPassageiros == 0){
        //     estacao->libera = 0;
        // }      
        // if(estacao->libera == 0){
    if(estacao->libera){
        estacao->libera = 0;
        pthread_cond_signal(&estacao->condVagao);
    }
    // printf("Libera: %d", estacao->libera);
    pthread_mutex_unlock(&estacao->mutex);
}

// ta dando segmentation fault as vezes