#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define WHITE	0 // unvisited node
#define BLACK	1 // node is part of a negative cycl
#define GREY	2 // temporary for algorith iterations
#define INF 	2147000000
#define BIG		2000000000
#define NONE	-1

typedef struct vertex{
	int value, dist, prev, color;
	list_t *adj;
} vertex_t;


void BBFS(vertex_t *vertices, int source){
	list_t *queue = list_init();
	list_node_t *node;
	int v, w;

	list_push(queue, source, 0);
	vertices[source].color = GREY;
	
	while (!list_empty(queue)){
		v = list_peek(queue)->value;
		list_pop(queue);
		for (node=vertices[v].adj->head; node!=NULL ; node=node->next){
			w = node->value;
			if (!vertices[w].color){
				vertices[w].color = GREY;
				list_push(queue, w, 0);
			}
		}
		vertices[v].color = BLACK;
	}
	
	list_destroy(queue);
}

void BBellmanFord(vertex_t *vertices, int source, int N){
	list_node_t *node;
	int i, j, changed;
	
	vertices[source].dist = vertices[source].prev = 0;
	
	for (i=0; i<N-1; i++){ // relaxation iterations
		changed = 0;
		for (j=0; j<N; j++) for (node=vertices[j].adj->head; node!=NULL; node=node->next)
			if (vertices[j].dist+node->weight < vertices[node->value].dist){
				vertices[node->value].dist = vertices[j].dist+node->weight;
				vertices[node->value].prev = j;
				changed = 1;
			}
		if (!changed) break;
	}

	for (j=0; j<N; j++) // negative cycles iteration, finds cycle root
		for (node=vertices[j].adj->head; node!=NULL; node=node->next)
			if (vertices[j].dist+node->weight < vertices[node->value].dist)
				BBFS(vertices, j);
}

int minOf(int *vect, int N){
	int j=0;

	for (int i=0; i<N; i++)
		if (vect[i] < vect[j])
			j = i;

	return j;
}


int main(int argc, char const *argv[]){
	int N=2, F=2, C=1;	// Locals, Branches, Connections
	int i, j, u, v, w;

	/* INPUTSES */
	scanf("%d %d %d", &N, &F, &C);

	/* BRANCHES */
	int **costs = (int**) malloc((F+1)*sizeof(int));
	int branches[F];
	for (i=0; i<F; i++){
		scanf("%d", &branches[i]);
		costs[i] = malloc(N*sizeof(int));
	} costs[F] = malloc(N*sizeof(int));

	vertex_t *locals = (vertex_t*) malloc(N*sizeof(vertex_t));
	for (i=0; i<N; i++) locals[i].adj = list_init();
	
	for (i=0; i<C; i++){ // spot, spot, loss
		scanf("%d %d %d", &u, &v, &w);
		list_push(locals[u-1].adj, v-1, w);
	}

	for (i=0; i<N; i++){ // vertices init
		costs[F][i] = 0;
		locals[i].value = i;
		locals[i].dist = INF;
		locals[i].prev = NONE;
		locals[i].color = WHITE;
	}


	for (i=0; i<F; i++){
		BBellmanFord(locals, branches[i]-1, N);
		for (j=0; j<N; j++){
			costs[i][j] = locals[j].dist;
			costs[F][j] = (costs[i][j]<BIG) ? costs[F][j]+costs[i][j] : INF ; // meh
			locals[j].dist = INF;
		}
	}

	/* OUTPUTS */
	int best = minOf(costs[F], N);
	if (costs[F][best]<BIG){
		printf("%d %d\n", best+1, costs[F][best]);
		for (i=0; i<F; i++) printf("%d ", costs[i][best]);
	} else{
		printf("N");
	}
	printf("\n");


	/* FREE MEMORY */
	/*for(i=0; i<N; i++) list_destroy(locals[i].adj);
	for(i=0; i<=F; i++) free(costs[i]);
	free(locals);
	free(costs);
	free(branches);*/

	return 0;
}