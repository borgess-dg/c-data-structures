#include <stdio.h>
#include <stdlib.h>
//Tamanho padrão da fila.
unsigned int Size;

typedef struct {
	int front;
	int rear;
    void (*dequeue)();
    void (*enqueue)();
    void (*length)();
    void (*foreach)();
	int* itens;
}Queue;

//Adiciona dado à fila.
void enqueue(Queue* Data, int Value){
    Data->rear++;
    if((Data->rear < Size)){
        if(Data->rear >= Data->front){
            Data->itens[Data->rear - Data->front] = Value;
        }
        else{
            Data->itens[Data->rear] = Value;
        }
    }
    else{
        Data->rear--;
        fprintf(stderr, "%s", "The operation cannot be completed because a overflow exists!");
        //exit(-1);
    }
}

//Remove dado da fila.
void dequeue(Queue* Data){
    int tmp = 0;
    Data->front++;
    if((Data->front <= (Data->rear) + 1) && (Data->rear != -1)){
        tmp = Data->itens[Data->front];
    }
    else{
        Data->front--;
        fprintf(stderr, "%s", "The operation cannot be completed because a underflow exists!");
    }
}

//Exibe tamanho da pilha.
void length(Queue* Data){
    printf("%s%d", "THE QUEUE SIZE IS ", (Data->rear - Data->front) + 1);
}

//Exibe cada elemento da fila.
void foreach(Queue* Data){
    printf("%s", "[");
    for(int i = Data->front; i <= Data->rear; i++){
        if(i != Data->rear){
            printf("%s%d%s"," ", Data->itens[i], ",");
        }
        else{
            printf("%s%d"," ", Data->itens[i]);
        }
    }
    printf("%s", " ]");
}

//Inicial todos os elementos da pilha.
Queue NewQueue(size_t StackSize){
    //Variável auxiliar para configuração da Queue.
    Queue tmp;
    //Preenche as variáveis auxiliares de posição.
    tmp.front = 0;
    tmp.rear = -1;
    //Liga elementos do Struct até suas respectivas funções.
    tmp.enqueue = &enqueue;
    tmp.dequeue = &dequeue;
    tmp.length = &length;
    tmp.foreach = &foreach;
    //Iguala variável global size ao tamanho desejado para comparação em caso de over ou underflow.
    Size = StackSize;
    //Determina a quantidade máxima de itens na fila.
    tmp.itens = calloc(StackSize, sizeof(int));
    //Retorna a Queue para uso.
    return tmp;
}

int main()
{
    int opcao=-1;
    int valor=0;
    Queue Data = NewQueue(3);
    system("cls");
    
    while(opcao!=0){
        valor=0;
        printf("\n\nDIGITE 0 PARA SAIR\n");
        printf("Escolha uma opcao\n");
        printf("1: Enfileirar\n");
        printf("2: Desenfileirar\n");
        printf("3: Tamanho\n");
        printf("4 - Ver Fila\n");
       
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                //system("cls");
                printf("Digite um valor para enfileirar\n");
                scanf("%d",&valor);
                Data.enqueue(&Data, valor);
                break;

            case 2:
                Data.dequeue(&Data);
                break;
            case 3:
                Data.length(&Data);
                break;
            case 4:
                Data.foreach(&Data);
                break;
            case 0:
                printf("Tchau! Ate a proxima\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
            }
            //system("cls");
        }
    
    free(Data.itens);
    return 0;
}