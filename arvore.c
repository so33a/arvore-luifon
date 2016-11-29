#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "fila.h"

ARVORE novaArvore() {
  ARVORE a = malloc(sizeof(struct arvore));
  a->z = malloc(sizeof(struct node));
  a->raiz = a->z;
  a->raiz->left = a->raiz->right = a->z;
  a->z->left = a->z->right = a->z;
  return a;
}

void imprimePreOrdemR(ARVORE a, link h) {
  if (h != a->z) {
    printf ("%d ", h->key);
    imprimePreOrdemR(a, h->left);
    imprimePreOrdemR(a, h->right);
  }
}

int contaNosR (ARVORE a, link h) {
  if(h == a->z) return 0;
  return 1 + contaNosR(a, h->left) + contaNosR(a, h->right);
}

int contaParesR (ARVORE a, link h) {
  if(h == a->z)
    return 0;
  if (h->key %2 == 0)
    return 1 + contaParesR(a, h->left) + contaParesR(a, h->right);
  else
    return  contaParesR(a, h->left) + contaParesR(a, h->right);
}

void imprimePreOrdem (ARVORE a) {
  imprimePreOrdemR(a, a->raiz);
}
void imprimePosOrdemR(ARVORE a, link h) {
  if (h != a->z) {
    imprimePosOrdemR(a, h->left);
    imprimePosOrdemR(a, h->right);
    printf ("%d ", h->key);
  }
}

void imprimePosOrdem (ARVORE a) {
  imprimePosOrdemR(a, a->raiz);
}


void imprimeEmOrdemR (ARVORE a, link h) {
   if (h != a->z) {
     imprimeEmOrdemR(a, h->left);
     printf("%d ", h->key);
     imprimeEmOrdemR(a, h->right);
   }
}

void imprimeEmOrdem (ARVORE a) {
   imprimeEmOrdemR(a, a->raiz);
}

link buscaR (ARVORE a, link h, int key) {
  if(h == a->z) return NULL;
  if(h->key == key) return h;
  if( h->key < key)
    return buscaR(a, h->right, key);
  return buscaR(a, h->left, key);
}


link busca (ARVORE a, int key){
  return buscaR(a, a->raiz, key);
}

link novoNo(int key, link l, link r) {
  link x = malloc(sizeof(struct node));
  x->key = key;
  x->left = l;
  x->right = r;
  return x;
}
link inserirR (ARVORE a, link h, int key) {
  if(h == a->z)
    return novoNo(key, a->z, a->z);
  if(h->key == key) return h;
  if(h->key < key)
    h->right = inserirR(a, h->right, key);
  else
    h->left = inserirR(a, h->left, key);
  return h;
}
link inserirT (ARVORE a, link h, int key) {
  if(h == a->z)
    return novoNo(key, a->z, a->z);
  if(h->key == key) return h;
  if(h->key < key)  {
    h->right = inserirT(a, h->right, key);
    h = rotL(a, h);
  }
  else {
    h->left = inserirT(a, h->left, key);
    h = rotR(a, h);
  }
  return h;
}

void inserirNaRaiz(ARVORE a, int key) {
  a->raiz = inserirT(a, a->raiz, key);
}

void inserir (ARVORE a, int key){
  a->raiz = inserirR(a, a->raiz, key);
}

void imprimeEmLargura (ARVORE a) {
  link aux;
  FILA f = novaFila();
  enfilar(f, a->raiz);
  while (f->primeiro != NULL) {
    aux = desenfilar(f);
    printf ("%d ", aux->key);
    if (aux->left != a->z)  {
      enfilar (f,aux->left);
    }
    if (aux->right != a->z) {
      enfilar (f,aux->right);
    }
  }
  destroiFila(f);
}

link rotL(ARVORE a, link h) {
  link x = h->right;
  h->right = x->left;
  x->left = h;
  return x;
}
link rotR(ARVORE a, link h) {
  link x = h->left;
  h->left = x->right;
  x->right = h;
  return x;
}

link bpai(FILA temp, ARVORE a, link h, int key){
	if(h == a->z) return NULL;
	if(h->key == key){ 
		return h;
	}
	if( h->key < key){
		desenfilar(temp);
		enfilar(temp, h);
		return bpai(temp, a, h->right, key);
	}
	desenfilar(temp);
  	enfilar(temp, h);
  	return bpai(temp, a, h->left, key);
}

link ElementoE(ARVORE a, link buscaE){
	if(buscaE->left == a->z) return buscaE;
	ElementoE(a, buscaE->left);
}

void remover (ARVORE a, int key){
	link temp = busca(a, key);
	FILA t;
	link pai;
	t = novaFila();
	bpai(t, a, a->raiz, key);
	if(t->primeiro != NULL)
		pai = t->primeiro->noArvore;
	else
		pai = NULL;
	
	
	/*
	não tem filhos 
	*/
	if((temp->left == a->z) && (temp->right == a->z)){
		if(pai->right == temp){
			pai->right = a->z;
		}

		if(pai->left == temp){
			pai->left = a->z;
		}
		if(t->primeiro == NULL){
			a->raiz = a->z;
		}
		free(temp);
	}
	/*
	tem dois filhos
	*/
	else if(temp->left != a->z && temp->right != a->z ){
		link substituto = ElementoE(a, temp->right);
		FILA filho = novaFila();
	    bpai(filho, a, a->raiz, substituto->key);
		link paisub;
		
		if(filho->primeiro != NULL)
			paisub = filho->primeiro->noArvore;
		else
			paisub = NULL;
			if(paisub != NULL){
				if(substituto->right != a->z && paisub != NULL){			
				paisub->left = substituto->right;
			}
		if(paisub->left == substituto)
			paisub->left = a->z;
			}
		
		if(pai != NULL){
			if(pai->right == temp){
				pai->right = substituto;	
				if(substituto != temp->left)
					substituto->left = temp->left;
				else
					substituto->right = a->z;
				if(substituto != temp->right)
					substituto->right = temp->right;
				else
					substituto->right = a->z;
			}
			if(pai->left == temp){
				pai->left = substituto;
				if(substituto != temp->left)
					substituto->left = temp->left;
				else
					substituto->right = a->z;
				if(substituto != temp->right)
					substituto->right = temp->right;
				else
					substituto->right = a->z;
			}
		}
		else{
			a->raiz = substituto;
			substituto->left = temp->left;
			substituto->right = temp->right;
			free(temp);
		}
		free(temp);
	}	

	
	/*
		tem 1 filho
	*/
	else{
		//filho na esquerda
		if(temp->left != a->z ){
			if(pai->left == temp){
				pai->left = temp->left; 
			}
			if(pai->right == temp ){
				pai->right = temp->left;
			}
		}
		//filho na direita
		else{
			if(pai->left == temp){
				pai->left = temp->right;
			}
			if(pai->right == temp ){
				pai->right = temp->right;
			}
		}
			free(temp);
	}
}

void removerNo (ARVORE a, link node){
	
}

void destroiArvore(ARVORE a){
	
}
