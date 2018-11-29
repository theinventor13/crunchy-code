#include <stdio.h>
#include <stdbool.h>



struct node{
	unsigned char * line;	
	struct node * next;
};

typedef struct node node;

typedef struct{
	node * head;
	node * tail;		
}ll;



void init_ll(ll * list, size_t linelength){
	list->tail = (ll *)malloc(sizeof(ll));
	list->head = (ll *)malloc(sizeof(ll));
	list->head->line = (char *)malloc(sizeof(char) * (linelength + 1));
	list->head->next = list->tail;
	list->tail->next = NULL;
}

void set_ll_node(ll * list, char * string, size_t which){
	node * temp;
	temp = list->head;
	while(which > 0){
		temp = temp->next;
	}
	strcpy(temp->line, string);
}

void insert_ll_node(ll * list, char * string){
	node * temp;
	temp = (node *)malloc(sizeof(node));
	ll->tail->next = temp;
	ll->tail->line = (char *)malloc(sizeof(char) * (linelength + 1));
	strcpy(ll->tail->line, string);
	temp->next = NULL;
	ll->tail = temp;
	
}



int main(int argc, char ** argv){
	char string[] = "abcdefghdnsuwhed";
	ll * list = (ll *)malloc(sizeof(ll)); 
	init_ll(list, 16);
	set_ll_node(list, string, 0);
	printf("%s", list->head->line);
	
	
	


	return 0;
}

