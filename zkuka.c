//zkuka

#include <pthread.h>
#include <stdbool.h>

// Struct que representa a estação
struct estacao {
    pthread_mutex_t mutex; // Lock para sincronização
    pthread_cond_t carro_chegou; // Condição que sinaliza que um carro chegou
    pthread_cond_t passageiros_embarcaram; // Condição que sinaliza que todos os passageiros embarcaram
    int embarcando;
    int esperando;
    int assentos_livres; // Quantidade de assentos livres no carro
    bool carro_na_estacao; // Indica se o carro está na estação
};

// Inicializa a estação
void estacao_init(struct estacao *estacao) {
    pthread_mutex_init(&estacao->mutex, NULL);
    pthread_cond_init(&estacao->carro_chegou, NULL);
    pthread_cond_init(&estacao->passageiros_embarcaram, NULL);
    estacao->assentos_livres = 0;
    estacao->esperando = 0;
    estacao->embarcando = 0;
    estacao->carro_na_estacao = false;
    printf("Vagao inicializado\n");
}
// Chamada pelo vagão quando chega na estação
void estacao_preecher_vagao(struct estacao *estacao, int assentos) {
    pthread_mutex_lock(&estacao->mutex);

    if(assentos == 0){

        printf("Não há assentos disponiveis no vagão.\n");
        exit(1);
    }
    // Marca que o carro está na estação e define a quantidade de assentos livres
    estacao->carro_na_estacao = true;
    estacao->assentos_livres = assentos;

    // Sinaliza para os passageiros que o carro chegou na estação
    printf("Vagao disponivel\n");
    pthread_cond_broadcast(&estacao->carro_chegou);

    // Aguarda até que todos os passageiros embarquem
    while (estacao->assentos_livres > 0 || estacao->esperando > 0) {
        pthread_cond_wait(&estacao->passageiros_embarcaram, &estacao->mutex);
    }
    
    pthread_mutex_unlock(&estacao->mutex);
}

// Chamada pelos passageiros para esperar pelo vagão
void estacao_espera_pelo_vagao(struct estacao *estacao) {
    pthread_mutex_lock(&estacao->mutex);
    

    printf("Passageiro chegou\n");
    estacao->esperando++;
    // Enquanto não houver carro na estação ou não houver assentos livres, aguarda a chegada do carro
    while (!estacao->carro_na_estacao || estacao->assentos_livres == 0) {
        pthread_cond_wait(&estacao->carro_chegou, &estacao->mutex);
    }

    printf("Passageiro saiu de espera\n");

    // O vagão está na estação e há assentos livres, decrementa a quantidade de assentos livres
    estacao->esperando--;
    estacao->assentos_livres--;
    estacao->embarcando++;

    pthread_mutex_unlock(&estacao->mutex);
}

// Chamada pelos passageiros após embarcarem no vagão
void estacao_embarque(struct estacao *estacao) {
    pthread_mutex_lock(&estacao->mutex);
    // Se todos os passageiros já embarcaram, sinaliza para o carro que pode partir
    
    if (estacao->assentos_livres == 0 || estacao->esperando == 0) {
        estacao->carro_na_estacao = false;
        pthread_cond_signal(&estacao->passageiros_embarcaram);
    }

    estacao->embarcando--;
    printf("Passageiro Embarcou\n");

    pthread_mutex_unlock(&estacao->mutex);
}