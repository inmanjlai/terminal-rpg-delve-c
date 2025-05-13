# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <stdbool.h>
# include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////
//                          DEFINING FUNDAMENTAL GAME STRUCTURES                         //
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	int playerTurn;
	int inCombat;
} CombatManager;

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

///////////////////////////////////////////////////////////////////////////////////////////
//                             INTIALIZING GAME RESOURCES                                //
///////////////////////////////////////////////////////////////////////////////////////////

Player player;
Map map;
CombatManager combatManager;

void createCharacter();
void printInventory();
void printStats();
void startGame();
void nextRoom();
void startBattle();
void playerTurn();
void enemyTurn();
Enemy generateEnemy();

///////////////////////////////////////////////////////////////////////////////////////////
//                                       BEGIN GAME                                      //
///////////////////////////////////////////////////////////////////////////////////////////

int main() {
	srand(time(NULL));
	createCharacter();
	bool playing = true;

	startGame();

	while (playing == true) {
		printf("What would you like to do now?\n");

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

///////////////////////////////////////////////////////////////////////////////////////////
//                                  GAME HELPER FUNCTIONS                                //
///////////////////////////////////////////////////////////////////////////////////////////

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
	system("clear");
	printf("%s\n", player.name);
	printf("---------- STATUS ----------\n");
	printf("HP: ");
	for(int i = 0; i <= player.currHp; i++) {
		printf("*");
	}
	printf("\n");
	printf("---------- STATS ----------\n");
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
		startBattle();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//                             COMBAT SYSTEM FUNCTIONS                                   //
///////////////////////////////////////////////////////////////////////////////////////////

Enemy generateEnemy() {
	// generate enemy + enemy inventory
	StatModifiers daggerStats = {1, 1, 0};
	Item dagger = {"Dagger", "Allows for fast strikes, but doesn't deal much damage", 1, daggerStats};
	Inventory goblinInventory;
	goblinInventory.items[0] = dagger;
	Enemy newEnemy = {"Goblin", 1, 1, 1, 6, 6, goblinInventory};

	return newEnemy;
}

void startBattle() {
	Enemy enemy = generateEnemy();
	printf("You have engaged with %s\n", enemy.name);
	// begin combat with generated enemy
	combatManager.inCombat = 1;

	// check for player speed vs enemy speed and set the correct turn
	if (player.spd >= enemy.spd) combatManager.playerTurn = 1;

	while (combatManager.inCombat) {
		if (combatManager.playerTurn) playerTurn();
		else enemyTurn();
	}
};

void playerTurn() {
	combatManager.playerTurn = 1;

	char command[5];

	printf("1: Attack\n2: Block\n3: Run\n4: Item\n");
	scanf("%5s", command);

	if (strcmp(command, "1") == 0) {
		printf("Attack\n");
	} else if (strcmp(command, "2") == 0) {
		printf("Block\n");
	} else if (strcmp(command, "3") == 0) {
		combatManager.inCombat = 0;
	} else if (strcmp(command, "4") == 0) {
		printf("Item\n");
	}  else {
		printf("That is not a valid action\n");
	}

}

void enemyTurn() {
	combatManager.playerTurn = 0;
}
