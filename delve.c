# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <stdbool.h>
# include <string.h>

typedef struct {
	int str;
	int spd;
	int wis;
} StatModifiers;

typedef struct {
	char name[100];
	char description[255];
	int quantity;
	StatModifiers modifiers;
} Item;

typedef struct {
	Item items[28];
	int inventoryCount;
} Inventory;

typedef struct {
	char* type;
} Room;

typedef struct {
	char name[50];
	int str;
	int spd;
	int wis;
	int maxHp;
	int currHp;
	Room currRoom;
	Inventory inventory;
} Player;

typedef struct {
	int roomCount;
} Map;

typedef struct {
	char name[50];
	int str;
	int spd;
	int wis;
	int currHp;
	int maxHp;
	Inventory inventory;
} Enemy;

Player player;
Map map;
bool inCombat;

void createCharacter();
void printInventory();
void printStats();
void startGame();
void nextRoom();
void startBattle();

int main() {
	srand(time(NULL));
	createCharacter();
	bool playing = true;

	startGame();

	while (playing == true) {
		if (inCombat) startBattle();
		printf("What would you like to do now?\n");
		// game loop here
		char command[50];
		scanf("%s", command);

		if (strcmp(command, "i") == 0) {
			printInventory();
		} else if (strcmp(command, "s") == 0) {
			printStats();
		} else if (strcmp(command, "q") == 0) {
			playing = false;
		} else if (strcmp(command, "m") == 0) {
			nextRoom();
		}
	}

	return 0;
}

void createCharacter() {
	system("clear");
	printf("You wake up in a strange place... You don't remember much\n");
	printf("What's your name? ");
	fgets(player.name, sizeof(player.name), stdin);
	printf("Welcome to the world of Delve, %s\n", player.name);
	player.maxHp = 10;
	player.currHp = 10;
	player.str = 1;
	player.spd = 1;
	player.wis = 1;
	Inventory inventory;
	player.inventory = inventory;
	player.inventory.inventoryCount = 0;
}

void printInventory() {
	if (player.inventory.inventoryCount == 0) printf("Your inventory is empty\n");
	else {
		for (int i = 0; i < player.inventory.inventoryCount; i++){
			printf("%s\n", player.inventory.items[i]);
		}
	}
}

void printStats() {
	printf("HP: %d / %d\n", player.currHp, player.maxHp);
	printf("STR %d\n", player.str);
	printf("SPD %d\n", player.spd);
	printf("WIS %d\n", player.wis);
}

void startGame() {
	// create a room
	Room startingRoom = {"start"};
	map.roomCount = 1;
	player.currRoom = startingRoom;
	// check room type
	if (strcmp(player.currRoom.type, "start") == 0) printf("This is the starting room\n");
	// either combat / shop / rest
}

void nextRoom() {
	//check how to create an array of strings
	char possibleRooms[4][25] = {"fight", "hard fight", "boss fight", "shop"};

	char* roomChoice = possibleRooms[(rand() % 4)];
	Room newRoom = {roomChoice};
	player.currRoom = newRoom;
	printf("You walked into a %s\n", roomChoice);
	if (strcmp(player.currRoom.type, "shop") != 0) {
		inCombat = true;
	}
}

void startBattle() {
	char command[50];
	StatModifiers daggerStats = {1, 1, 0};
	Item dagger = {"Dagger", "Allows for fast strikes, but doesn't deal much damage", 1, daggerStats};
	Inventory goblinInventory;
	goblinInventory.items[0] = dagger;
	Enemy newEnemy = {"Goblin", 1, 2, 1, 6, 6, goblinInventory};
	printf("You have engaged with %s\n", newEnemy.name);
	while (inCombat) {
		printf("Its your turn, choose your next action carefully\n");
		scanf("%s", command);
		printf("%s\n", command);
		inCombat = false;
	}
};
