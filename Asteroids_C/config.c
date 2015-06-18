#include "config.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>

struct Configuration * load_config_file() {
	char name[20];
	float value = 0.0;

	errno_t err;

	FILE *fp = NULL;
	//Windows
	err = fopen_s(&fp, "conffig.txt", "r");

	//Unix
	//fp = fopen("conffig.txt", "r");

	printf("err: %d", err);

	if (err == 0) {
		printf("File opened!");

		struct Configuration * cfg = (struct Configuration*) malloc(sizeof(struct Configuration));

		//Windows
		fscanf_s(fp, "%d", &cfg->asteroids_max_count);
		fscanf_s(fp, "%f", &cfg->asteroid_speed);
		fscanf_s(fp, "%f", &cfg->bullet_speed);
		fscanf_s(fp, "%d", &cfg->screen_width);
		fscanf_s(fp, "%d", &cfg->screen_height);

		//UNIX
		/*
		fscanf(fp, "%d", &cfg->asteroids_max_count);
		fscanf(fp, "%f", &cfg->asteroid_speed);
		fscanf(fp, "%f", &cfg->bullet_speed);
		fscanf(fp, "%d", &cfg->screen_width);
		fscanf(fp, "%d", &cfg->screen_height);
		*/

		fclose(fp);

		return cfg;
	}
	else {
		show_config_read_error();
		exit(1);
	}

	// Linux
	// fopen(fp, "config.txt", "r");

	if (fp == NULL) {
		fprintf(stderr, "Can't open input file!\n");
		show_config_read_error();
		exit(1);
	}
	else {
		struct Configuration * cfg = (struct Configuration*) malloc(sizeof(struct Configuration));
		printf("File opened!");
		// Linux - while (fscanf(fp, "%s %d", &name, &value) != EOF) {
		while (fscanf_s(fp, "%s %d", &name, &value) != EOF) {
			printf("%s = %d", name, value);

			if(name == "asteroids_max_count") cfg->asteroids_max_count = (int) value;
			else if (name == "asteroid_speed") cfg->asteroid_speed = value;
			else if (name == "bullet_speed") cfg->bullet_speed = value;
			else if (name == "screen_height") cfg->screen_height = (int) value;
			else if (name == "screen_width") cfg->screen_width = (int) value;
		}

		return cfg;
	}
}
