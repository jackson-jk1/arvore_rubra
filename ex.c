#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct arvoreRB {
 int valor;
 int cor;
 struct arvoreRB *esq;
 struct arvoreRB *dir;
} ArvoreRB;

int is_red(ArvoreRB * no){
 if(!no)
    return BLACK;
 return(no->cor == RED);
}

ArvoreRB * rot_esq(ArvoreRB * no){
 ArvoreRB * x = no->dir;
 no->dir = x->esq;
 x->esq = no;
 x->cor = no->cor;
 no->cor = RED;
 return(x);
}

ArvoreRB * rot_dir(ArvoreRB * no){
 ArvoreRB * x =  no->esq;
 no->esq = x->dir;
 x->dir = no;
 x->cor = no->cor;
 no->cor = RED;
 return(x);
}

ArvoreRB * color_flip(ArvoreRB * no){
 no->cor = RED;
 no->esq->cor = BLACK;
 no->dir->cor = BLACK;
 return(no);
}

void in_order(ArvoreRB *a){
  if(!a)
    return;
  in_order(a->esq);
  printf("%d %d, ",a->valor, is_red(a));
  in_order(a->dir);
}

void print(ArvoreRB * a,int spaces){
  int i;
  for(i=0;i<spaces;i++) printf(" ");
  if(!a){
    printf("//\n");
    return;
  }

  printf("%d\n", a->valor);
  print(a->esq,spaces+2);
  print(a->dir,spaces+2);
}


ArvoreRB* inserir (ArvoreRB *no, int v) {
 if (no == NULL) {
     no = (ArvoreRB*)malloc(sizeof(ArvoreRB));
     no->valor = v;
     no->esq = no->dir = NULL;
     no->cor = RED;
 }
 else if (v < no->valor) {
    no->esq = inserir (no->esq, v);
 }
 else {
    no->dir = inserir (no->dir, v);
 }
 return no;
}

ArvoreRB* remover(ArvoreRB *a, int x){
  ArvoreRB * aux, * pai_aux;
  int filhos = 0,tmp;

  if(!a)
    return(NULL);

  if(a->valor < x)
    a->dir = remover(a->dir,x);
  else if(a->valor > x)
    a->esq = remover(a->esq,x);
  else{
    if(a->esq)
      filhos++;
    if(a->dir)
      filhos++;

    if(filhos == 0){
      free(a);
      return(NULL);
    }
    else if(filhos == 1){
      aux = a->esq ? a->esq : a->dir;
      free(a);
      return(aux);
    }
    else{
      aux = a->esq;
      pai_aux = a;
      while(aux->dir){ pai_aux = aux; aux = aux->dir; }
      tmp = a->valor;
      a->valor = aux->valor;
      aux->valor = tmp;
      pai_aux->dir = remover(aux,tmp);
      return(a);
    }
  }

  return(a);
}

ArvoreRB* consertar (ArvoreRB *no) {
 if (is_red(no->dir) && is_red(no->esq)){
    no = rot_esq(no);
 }
 if (is_red(no->esq) && is_red(no->esq->esq)){
    no = rot_dir(no);
 }
 if (is_red(no->esq) && is_red(no->dir)){
    no = color_flip(no);
 }
 return no;
}
ArvoreRB* chama_consertar (ArvoreRB *a) {
  if (!a) { return; }
  chama_consertar (a->esq);
  a = consertar(a);
  chama_consertar (a->dir);
  a = consertar(a);
  return a;
}

void main() {
    ArvoreRB *arvore;
    int escolha = 0, elemento1 = 0, elemento2 = 0,first = 1;

    while(escolha != 4){

      printf ("\nEscolha a operacao: \n");
      printf ("1 - Inserir\n");
		  printf ("2 - Remover\n");
	  	printf ("3 - Imprimir\n");
		  printf ("4 - Sair\n");
	  	scanf("%d", &escolha);

        switch (escolha){
            case 1:
                printf("Digite o numero que deseja inserir:\n");
                scanf("%d", &elemento1);
                if(first == 1){
                    arvore = inserir(NULL, elemento1);
                    first = 0;
                }
                else{
                    arvore = inserir(arvore, elemento1);
                }
                arvore = chama_consertar(arvore);
                break;
            case 2:
                printf("Digite o numero que deseja remover:\n");
                scanf("%d", &elemento2);

                arvore = remover(arvore, elemento2);
                arvore = chama_consertar(arvore);
                break;
            case 3:
                arvore = chama_consertar(arvore);
                arvore->cor = BLACK;
                in_order(arvore);
                break;
            case 4:
                break;
            default:
                printf ("Operacao Invalida: \n");
                break;
        }
    }
}
