#include <pthread.h>

struct estacao {
};

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_cond_t condVagao;
int i = 0;
int num_passageiros = 0;
int nAssentos;
void estacao_init(struct estacao *estacao) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&condVagao, NULL);
}

void estacao_preecher_vagao(struct estacao * estacao, int assentos) {
    pthread_mutex_lock(&mutex);
    nAssentos = assentos;
    printf("Liberou\n");
    pthread_cond_broadcast(&cond);
    pthread_cond_wait(&condVagao, &mutex);
    printf("Solta mutex\n");
    pthread_mutex_unlock(&mutex);
}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    pthread_mutex_lock(&mutex);
    num_passageiros++;
    printf("Num: %d \n", num_passageiros);
    pthread_cond_wait(&cond, &mutex);
    while(num_passageiros > 0){
        if(nAssentos > 0 && num_passageiros > 0){
            nAssentos--;
            num_passageiros--;
        } else{
                pthread_cond_wait(&cond, &mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void estacao_embarque(struct estacao * estacao) {
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&condVagao);
    printf("Emb %d\n", num_passageiros);
    pthread_mutex_unlock(&mutex);
}