#include <stdlib.h>
#include "highscores.h"
#include <stdio.h>
#include "linked_list.h"

element * head;
element * previous;

void add_highscore(int score, char * name) {
	FILE *fp = NULL;
	errno_t err;
	//Unix
	//fp = fopen("highscores.txt", "a+");

	//Windows
	err = fopen_s(fp, "highscores.txt", "a+");
	if (err == 0) {
		fprintf(fp, "%d %s", score, name);
		fclose(fp);
	}
	else {
		printf("Error while opening file, error code: %d", err);
	}
}


void insertToList(int score, char name[20]) {
	insert(score);

	char * nam = "rafal\n";
	
	// MVSC
	FILE *fp = NULL;
	errno_t err;
	err = fopen_s(&fp, "highscores.txt", "a+");
	if (err == 0) {
		fprintf(fp, "%d\n", score);
		fclose(fp);
	}
	else {
		printf("Error while opening file, error code: %d", err);
	}

	//Unix

	/*
	FILE *fp = fopen("highscores.txt", "a+");
	if(fp != NULL) {
		fprintf(fp, "%d\n", score);
		fclose(fp);
	}
	else {
		printf("Error while opening file, error code: %d", err);
	}
	*/
}

void load_highscores() {
	printf("Loading highscores...\n");
	FILE *fp = NULL;
	errno_t err;
	char name[20];
	int score = 0;
	err = fopen_s(&fp, "highscores.txt", "r");
	if (err == 0) {
		//Windows
		while (fscanf_s(fp, "%d", &score) != EOF) {
		
		//UNIX
		//while (fscanf(fp, "%d", &score) != EOF) {

			insert(score);
		}
		fclose(fp);
		printf("File closed!");
	}
	else {
		printf("Error while opening file, error code: %d", err);
	}

	print_list();
}

void printList() {
	element * temp = head;
	while (temp != 0) {
		printf("Element %d\n", temp->score);
		temp = temp->next;
	}
}
