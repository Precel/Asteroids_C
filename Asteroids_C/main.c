#pragma once
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include <time.h>

#include "config.h"
#include "main.h"
#include "highscores.h"
#include "linked_list.h"

int main()
{
	char str[100];
	ALLEGRO_DISPLAY *Screen = NULL;
	ALLEGRO_EVENT_QUEUE *EventQueue = NULL;
	ALLEGRO_EVENT Event;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *Background = NULL, *KeyDownImage = NULL, *Ship = NULL, *BulletBitmap, *AsteroidBitmap_1, *AsteroidBitmap_2, *AsteroidBitmap_3;
	bool Exit = false, leftMovement = false, rightMovement = false, redraw = true, isgameover = false;

	int bullet_pool_iterator = 0, asteroid_pool_iterator = 0, asteroids_count = 0, FPS = 60, asteroids_max_count = 15, score = 0, min_bullet_index = 0, min_asteroid_index = 0;
	struct Bullet ** bullet_pool = (struct Bullet**) malloc(10000 * sizeof(struct Bullet*));
	struct Asteroid ** asteroid_pool = (struct Asteroid**) malloc(10000 * sizeof(struct Asteroid*));

	int ScreenWidth = 800, ScreenHeight = 600;
	float playerPositionX = ScreenWidth / 2, playerPositionY = ScreenHeight / 2, playerAngle = 0, rotatingSpeed = 1, bulletSpeed = 0.1, asteroid_speed = 0.006;

	al_init_font_addon();
	al_init_ttf_addon();

	if (!al_init()) {
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	struct Configuration * cfg = load_config_file();
	printf("Config file loaded!\n");
	printf("Resolution: %d x %d\n", cfg->screen_width, cfg->screen_height);
	asteroid_speed = cfg->asteroid_speed;
	bulletSpeed = cfg->bullet_speed;
	asteroids_max_count = cfg->asteroids_max_count;

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	if (!al_init_image_addon()) {
		al_show_native_message_box(NULL, "Error!", "Image addon has failed to initialize.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	Screen = al_create_display(cfg->screen_width, cfg->screen_height);
	if (Screen == NULL) {
		al_show_native_message_box(Screen, "Error!", "Failed to create the display.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	EventQueue = al_create_event_queue();
	if (EventQueue == NULL) {
		al_show_native_message_box(Screen, "Error!", "Failed to create the event queue.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if (!al_install_keyboard()) {
		al_show_native_message_box(NULL, "Error!", "Failed to install keyboard.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_register_event_source(EventQueue, al_get_display_event_source(Screen));
	al_register_event_source(EventQueue, al_get_timer_event_source(timer));
	al_register_event_source(EventQueue, al_get_keyboard_event_source()); ///Tell allegro to get events from the keyboard

	Ship = al_load_bitmap("ship.png");
	BulletBitmap = al_load_bitmap("bullet.png");
	AsteroidBitmap_1 = al_load_bitmap("asteroid_1.png");
	AsteroidBitmap_2 = al_load_bitmap("asteroid_2.png");
	AsteroidBitmap_3 = al_load_bitmap("asteroid_3.png");
	if (Ship == NULL)
	{
		al_show_native_message_box(Screen, "Error!", "Failed to load -ship.png-", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	printf("Bitmaps loaded!\n");

	ALLEGRO_FONT *font = al_load_font("comic.ttf", 30, 0);
	if (font == NULL) {
		al_show_native_message_box(Screen, "Error!", "Failed to get font!.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	int rand_seed = time(NULL);
	srand(rand_seed);

	float w = al_get_bitmap_width(Ship);
	float h = al_get_bitmap_height(Ship);

	float asteroid_width = al_get_bitmap_width(AsteroidBitmap_1);
	float asteroid_height = al_get_bitmap_height(AsteroidBitmap_1);

	al_start_timer(timer);

	load_highscores();

	while (Exit == false)
	{
		if (!isgameover) {
			al_flip_display();
			if (al_get_next_event(EventQueue, &Event)) {

				if (Event.type == ALLEGRO_EVENT_TIMER) {
					redraw = true;
				}
				else redraw = false;

				if (Event.type == ALLEGRO_EVENT_KEY_DOWN) {
					if (Event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
						struct Bullet *tmp = (struct Bullet*) malloc(sizeof(struct Bullet));
						tmp->position_x = ScreenWidth / 2;
						tmp->position_y = ScreenHeight / 2;
						tmp->speed = bulletSpeed;
						tmp->rotation = playerAngle;

						bullet_pool[bullet_pool_iterator] = tmp;
						bullet_pool_iterator++;
					}

					else if (Event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
						leftMovement = true;
						rightMovement = false;
					}

					else if (Event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
						rightMovement = true;
						leftMovement = false;
					}

					else if (Event.keyboard.keycode == ALLEGRO_KEY_1) {
						isgameover = true;
						printf("Game over!");

						al_flip_display();
						al_clear_to_color(al_map_rgb(0, 0, 0));

						sprintf_s(str, 12, "Score: %d", score);
						al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth / 2, ScreenHeight / 2 + 40, 0, "GAME OVER ;]");
						al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth / 2, ScreenHeight / 2, 0, str);
				
						al_draw_text(font, al_map_rgb(255, 255, 255), 100, 90, 0, "Highscores: ");


						leftMovement = false;
						rightMovement = false;

						insertToList(score, "Rafal");

						node * p = get_head();
						int iter = 1;
						while (p && iter < 11) {
							printf("%d\n", p->data);
							sprintf_s(str, 12, "%d: %d", iter, p->data);
							al_draw_text(font, al_map_rgb(255, 255, 255), 100, 100 + iter * 30, 0, str);
							p = p->ptr;
							iter++;
						}

						al_flip_display();
					}

					else if (Event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
						printf("Exiting...\n");
						Exit = true;
					}
				}
			}

			if (Event.type == ALLEGRO_EVENT_KEY_UP) {
				if (Event.keyboard.keycode != ALLEGRO_KEY_SPACE) {
					//printf("Key up!\n");
					leftMovement = false;
					rightMovement = false;
				}
			}

			if (redraw) {
				if (leftMovement) {
					playerAngle -= 0.001 * rotatingSpeed;
				}
				else if (rightMovement) {
					playerAngle += 0.001 * rotatingSpeed;

				}

				al_clear_to_color(al_map_rgb(0, 0, 0));

				al_draw_rotated_bitmap(Ship,
					w / 2, h / 2, playerPositionX, playerPositionY, playerAngle, 0);


				int i = 0, j = 0;
				int old_min_bullet_index = min_bullet_index;

				for (i = old_min_bullet_index; i < bullet_pool_iterator; i++) {
					if (bullet_pool[i] != 0) {
						al_draw_rotated_bitmap(BulletBitmap,
							w / 2, h / 2, bullet_pool[i]->position_x, bullet_pool[i]->position_y, playerAngle, 0);
						bullet_pool[i]->position_x += cos(bullet_pool[i]->rotation - ALLEGRO_PI / 2) * bulletSpeed;
						bullet_pool[i]->position_y += sin(bullet_pool[i]->rotation - ALLEGRO_PI / 2) * bulletSpeed;

						if (sqrt(pow(bullet_pool[i]->position_x - ScreenWidth / 2, 2) + pow(bullet_pool[i]->position_y - ScreenHeight / 2, 2)) > cfg->screen_height) {
							printf("Bullet out of game bounds, freeing memory!\n");
							free(bullet_pool[i]);
							bullet_pool[i] = 0;
						}

						if (i < min_bullet_index) min_bullet_index = i;
					}
				}

				int old_min_asteroid_index = min_asteroid_index;


				for (i = old_min_asteroid_index; i < asteroid_pool_iterator; i++) {
					if (asteroid_pool[i] != 0) {
						al_draw_rotated_bitmap(asteroid_pool[i]->bitmap,
							asteroid_width / 2, asteroid_height / 2, asteroid_pool[i]->position_x, asteroid_pool[i]->position_y, asteroid_pool[i]->bitmap_rotation_angle, 0);
						asteroid_pool[i]->position_x += cos(asteroid_pool[i]->rotation - ALLEGRO_PI / 2) * asteroid_speed * asteroid_pool[i]->speed;
						asteroid_pool[i]->position_y += sin(asteroid_pool[i]->rotation - ALLEGRO_PI / 2) * asteroid_speed * asteroid_pool[i]->speed;

						asteroid_pool[i]->bitmap_rotation_angle += asteroid_pool[i]->rotation_speed;

						if (min_asteroid_index > i) min_asteroid_index = i;

						if (sqrt(pow(asteroid_pool[i]->position_x - ScreenWidth / 2, 2) + pow(asteroid_pool[i]->position_y - ScreenHeight / 2, 2)) > cfg->screen_height) {
							printf("Asteroid out of game bounds, freeing memory!\n");
							free(asteroid_pool[i]);
							asteroid_pool[i] = 0;
							asteroids_count--;
						}
						else if( sqrt(pow(asteroid_pool[i]->position_x - ScreenWidth / 2, 2) + pow(asteroid_pool[i]->position_y - ScreenHeight / 2, 2)) < 80) {
							isgameover = true;
							printf("Game over!");

							al_flip_display();
							al_clear_to_color(al_map_rgb(0, 0, 0));

							sprintf_s(str, 12, "Score: %d", score);
							al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth / 2, ScreenHeight / 2 + 40, 0, "GAME OVER ;]");
							al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth /2, ScreenHeight / 2, 0, str);
							
							leftMovement = false;
							rightMovement = false;

							
							insertToList(score, "Rafal");

							node * p = get_head();
							int iter = 1;
							while (p && iter < 1) {
								printf("%d\n", p->data);
								sprintf_s(str, 12, "%d: %d", iter, p->data);
								al_draw_text(font, al_map_rgb(255, 255, 255), 100, 100 + iter * 30, 0, str);
								p = p->ptr;
								iter++;
							}

							al_flip_display();
						}

						for (j = 0; j < bullet_pool_iterator; j++) {
							if (asteroid_pool[i] != 0 && bullet_pool[j] != 0) {
								if (sqrt(pow(asteroid_pool[i]->position_x - bullet_pool[j]->position_x, 2) + pow(asteroid_pool[i]->position_y - bullet_pool[j]->position_y, 2)) <= asteroid_pool[i]->size) {
									printf("Destroying asteroid!");
									free(asteroid_pool[i]);
									asteroid_pool[i] = 0;
									asteroids_count--;

									free(bullet_pool[j]);
									bullet_pool[j] = 0;

									score++;

									if (score % 4 == 0) {
										asteroid_speed += 0.0005;
										asteroids_max_count++;
									}
								}
							}
						}
					}
				}

				while (asteroids_max_count > asteroids_count) {
					struct Asteroid* tmp_asteroid = (struct Asteroid*) malloc(sizeof(struct Asteroid));
					float randomAngle = rand() % 360;
					tmp_asteroid->position_x = ScreenWidth / 2 + cos(randomAngle) * cfg->screen_height * 0.9;
					tmp_asteroid->position_y = ScreenHeight / 2 + sin(randomAngle) * cfg->screen_height * 0.9;
					tmp_asteroid->rotation = randomAngle + ALLEGRO_PI / 2 + rand();
					tmp_asteroid->bitmap_rotation_angle = randomAngle + ALLEGRO_PI / 2;
					tmp_asteroid->speed = (1 / ((rand() % 5) + 1)) + 1;
					tmp_asteroid->rotation_speed = (rand() % 100) / 945535.4;
					tmp_asteroid->size = 60;
					tmp_asteroid->bitmap = (rand() % 100 < 30 ? AsteroidBitmap_1 : (rand() % 100 < 50 ? AsteroidBitmap_2 : AsteroidBitmap_3));
					asteroid_pool[asteroid_pool_iterator] = tmp_asteroid;
					asteroids_count++;
					asteroid_pool_iterator++;

					printf("Adding new asteroid! Rotation speed: %f, Base Rotation Angle: %f\n", tmp_asteroid->rotation_speed, tmp_asteroid->bitmap_rotation_angle);
				}

				sprintf_s(str, 12, "Score: %d", score);
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth - 180, 30, 0, str);
			}

			if (Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				Exit = true;
			}
		}
		else {
			if (al_get_next_event(EventQueue, &Event)) {
				if (Event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
					printf("Space");
					score = 0;
					int i = 0;
					playerAngle = 0;
					isgameover = false;
					asteroids_max_count = 12;
					asteroid_speed = 0.006;
					min_bullet_index = 0;
					min_asteroid_index = 0;
					for (i = 0; i < asteroid_pool_iterator; i++) {
						asteroid_pool[i] = 0;
					}

					for (i = 0; i < bullet_pool_iterator; i++) {
						bullet_pool[i] = 0;
					}

					asteroids_count = 0;
					asteroid_pool_iterator = 0;
					bullet_pool_iterator = 0;
				}
			}
		}
	}

	al_destroy_event_queue(EventQueue);
	al_destroy_display(Screen);
	al_destroy_timer(timer);

	free_list(get_head());

	return 0;
}

void show_config_read_error() {
	al_show_native_message_box(NULL, "Error!", "Failed to load configuration file", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
}
