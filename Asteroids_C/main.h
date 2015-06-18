#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>

struct Bullet {
	float position_x;
	float position_y;
	float rotation;
	float speed;
};

struct Asteroid {
	float position_x;
	float position_y;
	float rotation;
	float rotation_speed;
	float bitmap_rotation_angle;
	float speed;
	float size;
	ALLEGRO_BITMAP * bitmap;
};

// Main function, core of game, renders and controlls everything
int main();

// Function called from config.c used for showing read errors
void show_config_read_error();