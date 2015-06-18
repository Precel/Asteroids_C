#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

node * head;

node* insert(int num) {
	node *temp, *prev, *next;
	temp = (node*)malloc(sizeof(node));
	temp->data = num;
	temp->ptr = NULL;
	if (!head){
		head = temp;
	}
	else{
		prev = NULL;
		next = head;
		while (next && next->data > num){
			prev = next;
			next = next->ptr;
		}
		if (!next){
			prev->ptr = temp;
		}
		else{
			if (prev) {
				temp->ptr = prev->ptr;
				prev->ptr = temp;
			}
			else {
				temp->ptr = head;
				head = temp;
			}
		}
	}
	return head;
}

void free_list(node *head) {
	node *prev = head;
	node *cur = head;
	while (cur) {
		prev = cur;
		cur = prev->ptr;
		free(prev);
	}
}

node* get_head() {
	return head;
}

void print_list() {
	node *p;
	p = head;
	while (p) {
		printf("%d\n", p->data);
		p = p->ptr;
	}
}