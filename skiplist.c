
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVEL 1024
//node struct 
typedef struct node { 
    int key; 
    int val;
    struct node **next; 
} node;

//list struct
typedef struct skiplist{
	int level;
	int size;
	struct node *first;
} skiplist;

//initialize method
skiplist* initalize(skiplist * list){
	node *first = (node *)malloc(sizeof(struct node));
	list->first = first;
	first->key = 0;
	first->next = (node **)malloc(sizeof(node*) * MAX_LEVEL + 1);

	for (int i = 0; i < MAX_LEVEL; i++){
		first->next[i] = list->first;
	}

	list->level = 1;
	list->size = 0;

	return list;
}

// random level
int get_level(){
	int level = 1;
	int random_max = 10;
	while (rand() < random_max /2 && level < MAX_LEVEL){
		level++;
	}
	return level;
}

void add(skiplist *list, int key, int val) {
    node *update[MAX_LEVEL + 1];
    node *x = list->first;
    int i, level;
    for (i = list->level; i >= 1; i--) {
        while (x->next[i]->key < key)
            x = x->next[i];
        update[i] = x;
    }
    x = x->next[1];
 
    if (key == x->key) {
        x->val = val;
        return;
    } else {
        level = get_level();
        if (level > list->level) {
            for (i = list->level + 1; i <= level; i++) {
                update[i] = list->first;
            }
            list->level = level;
        }
 
        x = (node *) malloc(sizeof(node));
        x->key = key;
        x->val = val;
        x->next = (node **) malloc(sizeof(node*) * (level + 1));
        for (i = 1; i <= level; i++) {
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
    }
    return;
}

int search(skiplist *list, int key){
	node *x = list->first;
	for (int i = 0; i < list->level; i++){
		while (x->next[i]->key < key){
			x = x->next[i];
		}
		if (x->next[1]->key == key){
			return x->next[1]->val;
		} else return 0;
	}
	return 0;
}

void skiplist_node_free(node *x) {
    if (x) {
        free(x->next);
        free(x);
    }
}

void delete(skiplist *list, int key) {
    int i;
    node *update[MAX_LEVEL + 1];
    node *x = list->first;
    for (i = list->level; i >= 1; i--) {
        while (x->next[i]->key < key)
            x = x->next[i];
        update[i] = x;
    }
 
    x = x->next[1];
    if (x->key == key) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->next[i] != x)
                break;
            update[i]->next[1] = x->next[i];
        }
        skiplist_node_free(x);
 
        while (list->level > 1 && list->first->next[list->level]
                == list->first)
            list->level--;
        return;
    }
    return;
}

void print(skiplist *list) {
    node *x = list->first;
    while (x && x->next[1] != list->first) {
        printf("%d[%d]->", x->next[1]->key, x->next[1]->val);
        x = x->next[1];
    }
}


int main(){
	skiplist list;
    initalize(&list);
    //char line[1024];

    char command[10];
    int key;
    int val;

	char *line = NULL;
	size_t  n = 1024;

    strcpy(command, "temp");//might not need

	while(strcmp(command, "quit") != 0){

		getline(&line, &n, stdin);

		int len = strlen(line);
		printf("last char is %c", line[len]);

		sscanf(line, " %s%d%d", command, &key, &val);

		if (strcmp(command, "add") == 0){
			printf("true");
			add(&list, key, val);
		} else if (strcmp(command, "search") == 0){
			printf("%d\n", search(&list, key));
		} else if (strcmp(command, "delete") == 0){
			delete(&list, key);
		} else if (strcmp(command, "print\n") == 0){
			print(&list);
		} else if (strcmp(command, "quit") == 0){
			exit(0);
		}
	}
	return 0;
}
