struct Configuration {
	int asteroids_max_count;
	float asteroid_speed;
	float bullet_speed;
	int screen_width;
	int screen_height;
};

// Loads configuration from conffig.txt file and return struct Configuration with game parameters 
struct Configuration * load_config_file();