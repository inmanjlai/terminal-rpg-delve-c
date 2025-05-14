# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <stdbool.h>
# include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////
//                          DEFINING FUNDAMENTAL GAME STRUCTURES                         //
///////////////////////////////////////////////////////////////////////////////////////////

#define logSize 5
int logIndex = 0;
int playing;

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
	int armorClass;
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
	int armorClass;
	int str;
	int spd;
	int wis;
	int currHp;
	int maxHp;
	Inventory inventory;
} Enemy;

typedef struct {
	int playerTurn;
	int inCombat;
	Enemy currEnemy;
	char combatLog[logSize][100];
} CombatManager;

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
void attack();
void addLog();
void drawUi();
void drawMainUi();

///////////////////////////////////////////////////////////////////////////////////////////
//                                       BEGIN GAME                                      //
///////////////////////////////////////////////////////////////////////////////////////////

int main() {
	srand(time(NULL));
	createCharacter();
	playing = 1;

	startGame();

	while (playing) {
		drawMainUi();

		char command[50];
		scanf("%s", command);

		if (strcmp(command, "1") == 0) {
			printInventory();
		} else if (strcmp(command, "2") == 0) {
			printStats();
		} else if (strcmp(command, "3") == 0) {
			nextRoom();
		} else if (strcmp(command, "4") == 0) {
			playing = 0;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
//                                  GAME HELPER FUNCTIONS                                //
///////////////////////////////////////////////////////////////////////////////////////////

void drawMainUi() {
    system("clear"); // Optional: clear screen for clean redraw

    // Top info
    printf("=== The Dungeon ===\n");
    printf("HP: %d\n", player.currHp);
    printf("------------------------------\n");

    // Horizontal action menu
    printf("1: Inventory  2: Status  3: Move  4: Quit\n");

    // Optional: Display recent log line
    if (logIndex > 0) {
        printf("\nLast message: %s\n", combatManager.combatLog[logIndex - 1]);
    }

    printf("\nChoose an action: ");
}

void drawUi() {
    system("clear");  // Clear screen each time

    printf("========================\n");
    printf("      COMBAT STATUS     \n");
    printf("========================\n");
    printf("Your HP: %d/%d      %s HP: %d/%d\n", player.currHp, player.maxHp, combatManager.currEnemy.name, combatManager.currEnemy.currHp, combatManager.currEnemy.maxHp);
    printf("\n");

    printf("-- Combat Log --\n");
    for (int i = 0; i < logIndex; ++i) {
        printf("%s\n", combatManager.combatLog[i]);
    }
    printf("\n");

    printf("------------------------\n");
    printf("1: Attack  2: Block  3: Run  4: Item\n");
    printf("Choose an action: ");
}

void createCharacter() {
	system("clear");
	printf("You wake up in a strange place... You don't remember much\n");
	printf("What's your name? ");
	scanf("%s50", player.name);
	printf("Welcome to the world of Delve, %s\n", player.name);
	player.maxHp = 10;
	player.currHp = 10;
	player.armorClass = 12;
	player.str = 1;
	player.spd = 1;
	player.wis = 1;
	Inventory inventory;
	player.inventory = inventory;
	player.inventory.inventoryCount = 0;
}

void printInventory() {
	system("clear");
	if (player.inventory.inventoryCount == 0) printf("Your inventory is empty\n\n");
	else {
		for (int i = 0; i < player.inventory.inventoryCount; i++){
			printf("%s\n", player.inventory.items[i]);
		}
		printf("\n");
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
	printf("WIS %d\n\n", player.wis);
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
	Enemy newEnemy = {"Goblin", 10, 1, 1, 1, 6, 6, goblinInventory};

	return newEnemy;
}

void startBattle() {
	drawUi();
	combatManager.currEnemy = generateEnemy();
        char msg[100];
        snprintf(msg, sizeof(msg), "You encountered %s", combatManager.currEnemy.name);
        addLog(msg);
	// begin combat with generated enemy
	combatManager.inCombat = 1;

	// check for player speed vs enemy speed and set the correct turn
	if (player.spd >= combatManager.currEnemy.spd) combatManager.playerTurn = 1;

	while (combatManager.inCombat) {
		if (combatManager.playerTurn) playerTurn();
		else enemyTurn();
	}
};

void playerTurn() {
	combatManager.playerTurn = 1;

	drawUi();

	char command[5];
	scanf("%5s", command);

	if (strcmp(command, "1") == 0) {
		attack();
	} else if (strcmp(command, "2") == 0) {
		addLog("You brace for an attack");
	} else if (strcmp(command, "3") == 0) {
		addLog("You flee");
		combatManager.inCombat = 0;
	} else if (strcmp(command, "4") == 0) {
		addLog("You fumble for an item");
	}  else {
		printf("That is not a valid action\n\n");
	}

}

void enemyTurn() {
	combatManager.playerTurn = 0;

	int attackRoll = (rand() % 21);
	int damageRoll = 0;
	if (attackRoll == 20) damageRoll = (rand() % combatManager.currEnemy.str * 2 + 1);
	else if (attackRoll >= player.armorClass) damageRoll = (rand() % combatManager.currEnemy.str + 1);

	char msg[100];
	snprintf(msg, sizeof(msg), "%s attacked %s for %d damage", combatManager.currEnemy.name, player.name, damageRoll);
	addLog(msg);

	player.currHp -= damageRoll;
	if (player.currHp <= 0) {
		printf("You have died\n\n");
		combatManager.inCombat = 0;
		playing = false;
	} else {
		playerTurn();
	}
}

void attack() {
	// roll a d20
	int attackRoll = (rand() % 21);
	int damageRoll = 0;
	// if the result of the d20 is >= the enemy's armorClass then you landed a hit

	if (attackRoll == 20) {
		// if the attack roll is a 20, its a critical hit
		damageRoll = (rand() % (player.str * 2 + 1));
	} else if (attackRoll >= combatManager.currEnemy.armorClass) {
		// roll a dice with the max value being the player's str stat
		damageRoll = (rand() % (player.str + 1));
	}
	char msg[100];
	snprintf(msg, sizeof(msg), "You attacked %s for %d damage", combatManager.currEnemy.name, damageRoll);
	addLog(msg);
	// deal damage to the enemy bsaed on the damageRoll calculated above
	combatManager.currEnemy.currHp -= damageRoll;
	if (combatManager.currEnemy.currHp <= 0) {
		// do enemy death logic here (show item drops, exp gained etc)
		char msg[100];
		snprintf(msg, sizeof(msg), "You defeated %s", combatManager.currEnemy.name);
		addLog(msg);
		combatManager.inCombat = 0;
	} else {
		enemyTurn();
	}
}

void addLog(const char *message) {
    if (logIndex < logSize) {
        snprintf(combatManager.combatLog[logIndex++], 100, "%s", message);
    } else {
        for (int i = 1; i < logSize; ++i) {
            snprintf(combatManager.combatLog[i - 1], 100, "%s", combatManager.combatLog[i]);
        }
        snprintf(combatManager.combatLog[logSize - 1], 100, "%s", message);
    }
}
