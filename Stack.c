#include <stdio.h>
#include <stdlib.h>

unsigned int StackSize;

typedef struct{
    int top;
    void (*push)();
    int (*pop)();
    void (*traverse)();
    int (*size)();
    int* elements;
}Stack;

void push(Stack* Data, int Value){
    Data->top++;
    if(Data->top > StackSize){
        Data->elements = realloc(Data->elements, ((Data->top + 1) * sizeof(int)));
    }
    Data->elements[Data->top] = Value;
}

int pop(Stack* Data){
    int i = Data->top;
    Data->top--;
    return Data->elements[i];
}

void traverse(Stack* Data){
    printf("%s", "[");
    for(int i = 0; i < Data->top; i++){
        if(i != (Data->top - 1))
            printf("%s%d%s", " ", Data->elements[i], ",");
        else
            printf("%s%d", " ", Data->elements[i]);
    }
    printf("%s", "]");
}

int size(Stack* Data){
    return Data->top;
}

Stack NewStack(int ArraySize){
    Stack tmp;
    //Set the top index.
    tmp.top = -1;
    //Link functions to Struct elements.
    tmp.push = &push;
    tmp.pop = &pop;
    tmp.traverse = &traverse;
    tmp.size = &size;
    //Compare.
    StackSize = ArraySize;
    //Set array with the initial number of elements.
    tmp.elements = calloc(ArraySize, sizeof(int));
    if(tmp.elements == NULL){
        fprintf(stderr, "%s", "Momery allocation erro!");
        exit(-1);
    }
}

int main(void) {
  int op;
  int valor;
  int i;
  Stack Data = NewStack(5);

  printf("\n------ PILHA ESTATICA -------");

  do{
      printf("\n 1 - Empilhar");
      printf("\n 2 - Desempilhar");
      printf("\n 3 - Tamanho da pilha");
      printf("\n 4 - Sair");
      printf("\n Digite a opcao: ");
      scanf("%d", &op);

      switch(op){
            case 1:
                printf("\n Digite o valor: ");
                scanf("%d", &valor);
                Data.push(&Data, valor);
                break;
            case 2:
                printf("\n Valor: %d \n", Data.pop(&Data));
                break;
            case 3:
                printf("\n Tamanho: %d\n", Data.size);
                break;
            case 4:
                Data.traverse(&Data);
      }

  } while(op != 4);

  free(Data.elements);

  return 0;
}