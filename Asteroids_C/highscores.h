struct HighscoreStruct {
	int score;
	struct HighscoreStruct* next;
};

typedef struct HighscoreStruct element;

// Inserts score with name to sorted linked list
void insertToList(int score, char name[20]);

// Prints all list contents
void printList();

//Load highscores from file and add to linked list
void load_highscores();