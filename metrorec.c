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

}

void estacao_preecher_vagao(struct estacao * estacao, int assentos) { // 1 thread
    pthread_mutex_lock(&estacao->mutex);
    estacao->assentosLivres = assentos;
    // printf("preencher_vagao - assentos: [%d]\n", assentos);
    pthread_cond_wait(&estacao->condVagao, &estacao->mutex);
    pthread_cond_broadcast(&estacao->cond); // vagão chegou -> broadcast
    pthread_mutex_unlock(&estacao->mutex);
    printf("Vagão indo embora\n");
    // pthread_exit(NULL);
}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    estacao->contPassageiros++;
    // printf("espera_pelo_vagao - Passageiro [%d] aguardando.\n", estacao->id);
    pthread_cond_wait(&estacao->cond, &estacao->mutex); // cond wait
    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_embarque(struct estacao * estacao) {
    pthread_mutex_lock(&estacao->mutex);
    printf("embarque - Passageiros: %d\n", estacao->contPassageiros);
        estacao->contPassageiros--;
    if(estacao->assentosLivres > 0){
        printf("embarque - Assentos: %d\n", estacao->assentosLivres);
        estacao->assentosLivres--;
        // printf("preencher_vagao - Assento tomado pelo passageiro [%d]", estacao->id);
    } 
    if(estacao->assentosLivres == 0){
        pthread_cond_signal(&estacao->condVagao);
        pthread_mutex_unlock(&estacao->mutex);
        pthread_cond_wait(&estacao->cond, &estacao->mutex);
    }else{
        pthread_mutex_unlock(&estacao->mutex);

    }
}

// tem que vfazer o vagao ir embora antes de vir outro
// o wait nao ta funcioando
// achar um modo de infiltrar o signal
// travar os passageiros se nao tiver assento
// procurar onde botar o assento de forma que funcione

// usar array?