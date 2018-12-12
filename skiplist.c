
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVEL 1024
//node struct 
typedef struct node { 
    int key; 
    int val;
    struct node **forward; 
} node;

//skiplist struct
typedef struct skiplist{
	int level;
	int size;
	struct node *header;
} skiplist;

//initialize method
skiplist* initalize(skiplist * list){
	node *header = (node *)malloc(sizeof(struct node));
	list->header = header;
	header->key = 0;
	header->forward = (node **)malloc(sizeof(node*) * MAX_LEVEL + 1);

	for (int i = 0; i < MAX_LEVEL; i++){
		header->forward[i] = list->header;
	}

	list->level = 1;
	list->size = 0;

	return list;
}

	// randomLevel()
	// newLevel := 1
	// -- random0 returns a random value in [O...l)
	// while random0 -Z p do
	// newLevel := newLevel + 1
	// return min(newLevel, MaxLevel) 
int random_level(){
	int new_level = 1;
	int random_max = 10;
	while (rand() < random_max /2 && new_level < MAX_LEVEL){
		new_level++;
	}
	return new_level;
}

	// Insert(list, searchKey, newvalue)
	// local update[l ..MaxLevel]
	// x := list-+header
	// for i := list+level downto 1 do
	// while x+forward[i]+key c searchKey do
	// x := x+forward[i]
	// -- x+key < searchKey I x+fonuard[i]+key
	// update[i] := x
	// x := x+forward[l]
	// if x+key = searchKey then x+value := newValue
	// else
	// newLevel := randomLevel()
	// if newLevel > list+level then
	// for i := lisblevel + 1 to newLevel do
	// update[i] := listjheader
	// list+level := newLevel
	// x := makeNode(newLevel, searchKey, value)
	// for i := 1 to newLevel do
	// x+forward[i] := update[i]-+forward[i]
	// update[i]-+forward[i] := x 
void add(skiplist *list, int key, int val) {
    node *update[MAX_LEVEL + 1];
    node *x = list->header;
    int i;
    for (i = list->level; i >= 1; i--) {
        while (x->forward[i]->key < key && x->forward[i]->key != 0){
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[1];
 
    if (x->key == key) {
        x->val = val;
    } else {
        int new_level = random_level();
        if (new_level > list->level) {
            for (i = list->level + 1; i <= new_level; i++) {
                update[i] = list->header;
            }
            list->level = new_level;
        }
 		
 		//make node
        x = (node *) malloc(sizeof(node));
        x->key = key;
        x->val = val;
        x->forward = (node **) malloc(sizeof(node*) * (new_level + 1));
        for (i = 1; i <= new_level; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
    return;
}

	// Search(list, searchKey)
	// x := lisbheader
	// -- loop invariant: x-+key < searchKey
	// for i := list+level downto 1 do
	// while x-+fotward[i]+key c searchKey do
	// x := x+forward[i]
	// -- x+key < searchKey I x+forward[ I] +key
	// x := x+forward[l]
	// if x-+key = searchKey then return x+value
	// else return failure 
int search(skiplist *list, int key){
	node *x = list->header;
	for (int i = list->level; i >= 1; i--) {
        while (x->forward[i]->key < key && x->forward[i]->key != 0){
            x = x->forward[i];
        }
    }
    x = x->forward[1];
    if (x->key == key){
    	return x->val;
    }
	return 0;
}

	// Delete(list, searchKey)
	// local update[l ..MaxLevel]
	// x := list-+header
	// for i := list-+level downto 1 do
	// while x+forward[i]+key c searchKey do
	// x := x+forward[i]
	// update[i] := x
	// x := x+forward[ I]
	// if x-+key = searchKey then
	// for i := 1 to lisblevel do
	// if update[i]-+forward[i] # x then break
	// update[i]+forward[i] := x-+forward[i]
	// free(x)
	// while list+level > 1 and
	// list+header-+forward[list+level] = NIL do
	// list+level := list+level - 1 
void delete(skiplist *list, int key) {
    int i;
    node *update[MAX_LEVEL + 1];
    node *x = list->header;
    for (i = list->level; i >= 1; i--) {
        while (x->forward[i]->key < key && x->forward[i]->key != 0){
            x = x->forward[i];
        }
        update[i] = x;
    }
 
    x = x->forward[1];
    if (x->key == key) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->forward[i] != x)
                break;
            update[i]->forward[1] = x->forward[i];
        }

        free(x->forward);
        free(x);
 
        while (list->level > 1 && list->header->forward[list->level]
                == list->header)
            list->level--;
    }
    return;
}

void print(skiplist *list) {
    node *x = list->header;
    printf("(");
    while (x && x->forward[1] != list->header) {
        printf("(%d,%d)", x->forward[1]->key, x->forward[1]->val);
        x = x->forward[1];
        if (x->forward[1]->key != 0){
        	printf(",");
        }
    }
    printf(")\n");
    return;
}

int main(){
	skiplist* list;
    list = initalize(list);

    //char line[1024];

    char command[10];
    int key;
    int val;

	char *line = NULL;
	size_t  n = 1024;
	ssize_t read = 0;

	while (1){
		read = getline(&line, &n, stdin);

		if(read == -1){
			printf("failure with getline()");
			exit(1);
		}
		if (read > 0 && line[read - 1] == '\n'){
			line[read - 1] = '\0';
		}

		sscanf(line, "%s %d %d", command, &key, &val);

		if (strcmp(command, "add") == 0){
			add(list, key, val);
		}
		if(strcmp(command, "search") == 0){
			printf("%d\n", search(list, key));
		}
		if(strcmp(command, "delete") == 0){
			delete(list, key);
		}
		if(strcmp(command, "print") == 0){
			print(list);
		}
		if(strncmp(command, "quit", 3)==0){
			exit(1);
		}

		free(line);
		line = NULL;
	}
	return 0;
}
