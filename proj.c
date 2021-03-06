#include <stdio.h>
#include <stdlib.h>

#define INF 	2147000000
#define BIG 	2000000000
#define NONE	-1

/* LIST STRUCTURE */

typedef struct list_node {
	int value, weight;
	struct list_node *next;
} list_node_t;

typedef struct {
	list_node_t *head, *tail;
} list_t;

list_t *list_init(){
	list_t *list = (list_t*) malloc(sizeof(list_t));
	list->head = (list->tail = NULL);
	return list;
}

int list_empty(list_t *list){
	return (list->head == NULL);
}

void list_push(list_t *list, int value, int weight){
	list_node_t *node;
	node = (list_node_t*) malloc(sizeof(list_node_t));
	node->value = value;
	node->weight = weight;
	node->next = NULL;

	if (list_empty(list)){
		list->head = (list->tail = node);
	} else{
		list->tail->next = node;
		list->tail = list->tail->next;
	}
}

void list_pop(list_t *list){
	list_node_t *temp = list->head;
	list->head = list->head->next;
	free(temp);
}

list_node_t *list_peek(list_t *list){
	return list->head;
}

void list_destroy(list_t *list){
	while(list->head != NULL)
		list_pop(list);
	free(list);
}

/* PROJ IMPLEMENTATION */ 

typedef struct vertex{
	int value, dist, prev;
	list_t *adj;
} vertex_t;

void BBellmanFord(vertex_t *vertices, int source, int N){
	list_node_t *node;
	int i, j, changed;
	
	vertices[source].dist = vertices[source].prev = 0;
	
	for (i=0; i<N-1; i++){ /* relaxation iterations */
		changed = 0;
		for (j=0; j<N; j++) for (node=vertices[j].adj->head; node!=NULL; node=node->next)
			if (vertices[j].dist+node->weight < vertices[node->value].dist){
				vertices[node->value].dist = vertices[j].dist+node->weight;
				vertices[node->value].prev = j;
				changed = 1;
			}
		if (!changed) break;
	}
}

int main(int argc, char const *argv[]){
	int N=2, F=2, C=1;	/* Locals, Branches, Connections */
	int i, j, u, v, w;

	/* INPUTSES */
	scanf("%d %d %d", &N, &F, &C);
	int branches[F], totals[N], costs[F][N];
	for (i=0; i<F; i++)	scanf("%d", &branches[i]);

	vertex_t locals[N];
	for (i=0; i<N; i++){ /* vertices init */
		totals[i] = 0;
		locals[i].value = i;
		locals[i].dist = INF;
		locals[i].prev = NONE;
		locals[i].adj = list_init();
	}

	for (i=0; i<C; i++){ /* local, local, cost */
		scanf("%d %d %d", &u, &v, &w);
		list_push(locals[u-1].adj, v-1, w);
	}

	for (i=0; i<F; i++){ /* BellmanFord calls + cost savings */
		BBellmanFord(locals, branches[i]-1, N);
		for (j=0; j<N; j++){
			costs[i][j] = locals[j].dist;
			totals[j] = (costs[i][j]<BIG) ? totals[j]+costs[i][j] : INF ;
			locals[j].dist = INF;
		}
	}

	/* OUTPUTSES */
	int best = 0;
	for (i=1; i<N; i++) if (totals[i] < totals[best]) best = i;
	if (totals[best]<INF){
		printf("%d %d\n", best+1, totals[best]);
		for (i=0; i<F; i++) printf("%d ", costs[i][best]);
		printf("\n");
	} else{
		printf("N\n");
	}

	/* FREE MEMORY */
	for(i=0; i<N; i++) list_destroy(locals[i].adj);

	return 0;
}