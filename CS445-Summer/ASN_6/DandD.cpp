#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
using namespace std;

int roll(int faces){
    int temp = (rand() % faces) + 1;
    printf("Rolled a %d\n", temp);
    return (rand() % faces + 1);
    
}

int rollMult(int quant, int faces){
    int sum = 0;
    for(int i = 0; i < quant; i++){
        sum += roll(faces);
    }
    return sum;
}


int mapWidth = 20;
int mapHeight = 20;

int calcXY(int x, int y){
    return ((y * mapWidth) + x);
}



void printMap(char *map){
    for(int i = -2; i <= mapHeight; i++){
        if( i > -1){
            if( i < 9 ){
                printf(" %d", i+1);
            } else {
                printf("%d", i+1);
            }
        }
        if( i == 2 || i == 9 || i == 10 || i == 17){
            printf("|");
        } else if( i == -1){
            printf("  X");
        } else if( i > -1) {
            printf("X");
        } else {
            printf("   ");
        }
        for(int j = 0; j < mapWidth; j++){
            if(i == -2){
                if( j < 9 ){
                    printf("%d ", j+1);
                } else {
                    printf("%d", j+1);
                }
            }
            if(i == -1 && (j == 2 || j == 6 || j == 7 || j == 12 || j == 13 || j == 17)){
                if(j == 6 || j == 12){
                    printf("==");
                } else {
                    printf("=X");
                }
            } else if(i == mapHeight && (j == 2 || j == 9 || j == 10 || j == 17)){
                if(j == 9){
                    printf("==");
                } else {
                    printf("=X");
                }
            }
            else if(i == -1 || i == mapHeight){
                printf("X");
                if(j != mapWidth-1){
                    printf("X");
                }
            } else {
                printf("%c", map[calcXY(j, i)]);
                if(j != mapWidth-1 && i != -2){
                    printf(" ");
                }
            }
        }
        if( i == 2 || i == 9 || i == 10 || i == 17){
            printf("|\n");
        } else if(i != -2){
            printf("X\n");
        } else {
            printf("\n");
        }
    }
}

void printIntMap(int *map){
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            int temp = map[calcXY(j, i)];
            if(temp > -1){
                printf("  ");
            } else {
                printf(" -");
            }
            //get absolute value of temp
            if(temp < 0){
                temp = temp*-1;
            }

            if(temp < 10){
                printf("0%d", temp);
            } else {
                printf("%d", temp);
            }
        }
        printf("\n");
    }
}

//Order for storage: HP,AC,Spd,Prof,Str,Dex,Con,Int,Wis,Cha,
//                   MDam,MRan,Mabl,RDam,RRan,RAbl,RSav, AttackNum
int zombieDefault[] = {22, 8, 4, 2, 1, -2, 3, 3, 0, -3, 106, 1, 1,  0, 0, 1, 0, 1};
int dhulgenStats[] = {49, 18, 5, 3, 4, 0, 3, 1, 1, -1, 206, 1, 1, 106, 6, 1, 0, 2};
int ogrimStats[] = {43, 18, 5, 3, 2, 2, 3, 1, 4, -1, 108, 1, 1, 208, 12, 5, 2, 1};
int kolgarStats[] = {37, 16, 5, 3, 1, 2, 3, 4, 1, -1, 108, 1, 1, 2010, 20, 4, 1};
int torbinStats[] = {43, 16, 5, 3, 4, 2, 3, 1, 0, 3, 108, 1, 1, 104, 6, 1, 0, 2};
int charMonHP[100], charMonAC[100], charMonSpd[100], charMonProf[100];
int charMonStr[100], charMonDex[100], charMonCon[100];
int charMonInt[100], charMonWis[100], charMonCha[100];
int charMonAttackMDam[100], charMonAttackMRan[100], charMonAttackMAbl[100];
int charMonAttackRDam[100], charMonAttackRRan[100], charMonAttackRAbl[100];
int charMonAttackRSav[100], charMonAttackNum[100];
char charMonMapMarker[100];
//XY Positions given as a single integer XXYY
int charMonXPos[100];
int charMonYPos[100];
//
char mapDisplay[500];
char mapBackground[500] = "_x_x_~__xxxx__~_xx___________xx________x___x____________x________~xxx__xxx~_____~~__~~~~~__~~~~~__~~_____~________~______xx________________x___________________x_xx__~____x___~____________x_xx_x______x_______x_xx________x___x_~___xx_x_~_____x______x_xx________xx__x______x________x_____~________~_____~~__~~~~~__~~~~~__~~___x_~________~_______________________x__x________________x____x_~________~_____";
int idMap[500];
int pathingMap[500] = {-1};
int pcCount = 4;
int entityCount = 0;

void printName(int idNum){
    if(charMonMapMarker[idNum] == 'D'){
        printf("Dhulgen");
    } else if(charMonMapMarker[idNum] == 'O'){
        printf("Ogrim");
    } else if(charMonMapMarker[idNum] == 'K'){
        printf("Kolgar");
    } else if(charMonMapMarker[idNum] == 'T'){
        printf("Torbin");
    } else if(charMonMapMarker[idNum] == 'Z'){
        printf("Zombie");
    }
}

void setCharMon(int idNum, int *stats, char marker, int x, int y){
    charMonHP[idNum] = stats[0];
    charMonAC[idNum] = stats[1];
    charMonSpd[idNum] = stats[2];
    charMonProf[idNum] = stats[3];
    charMonStr[idNum] = stats[4];
    charMonDex[idNum] = stats[5];
    charMonCon[idNum] = stats[6];
    charMonInt[idNum] = stats[7];
    charMonWis[idNum] = stats[8];
    charMonCha[idNum] = stats[9];
    charMonAttackMDam[idNum] = stats[10];
    charMonAttackMRan[idNum] = stats[11];
    charMonAttackMAbl[idNum] = stats[12];
    charMonAttackRDam[idNum] = stats[13];
    charMonAttackRRan[idNum] = stats[14];
    charMonAttackRAbl[idNum] = stats[15];
    charMonAttackRSav[idNum] = stats[16];
    charMonAttackNum[idNum] = stats[17];
    charMonMapMarker[idNum] = marker;
    charMonXPos[idNum] = x;
    charMonYPos[idNum] = y;
    if(x > 19 || x < 0 || y > 19 || y < 0){
        charMonHP[idNum] = 0;
    } else {
        idMap[(y * mapWidth) + x] = idNum;
        mapDisplay[(y * mapWidth) + x] = marker;
    }
    entityCount++;


}
int getLowestNeighbor(int x, int y){
    int lowest = 1000;
    for(int i = -1; i < 2; i+=2){
        if(y + i > -1 && y + i < mapHeight){
            int temp = pathingMap[calcXY(x , y + i)];
            if(temp<lowest){
                lowest = temp;
            }
        }
        if(x + i > -1 && x + i < mapWidth){
            int temp = pathingMap[calcXY(x + i, y)];
            if(temp<lowest){
                lowest = temp;
            }
        }
    }
    return lowest;
}

bool isOpenSpot(int x, int y){
    if(idMap[calcXY(x, y)] != -2){
        return false;
    } else {
        return true;
    }
}

void checkSetPath(int x, int y){
    if( x > -1 && x < mapWidth && y > -1 && y < mapHeight){
        if(isOpenSpot(x, y)){
            int temp = getLowestNeighbor(x, y) + 1;
            if(temp > 99){
                temp = 99;
            }
            pathingMap[calcXY(x, y)] = temp;
        }
    }
    
}

void setPathingMap(){
    if(pathingMap[0] == -1){
        for(int i = 0; i < mapHeight; i++){
            for(int j = 0; j < mapWidth; j++){
                pathingMap[calcXY(j, i)] = 99;
            }
        }
    }
    for(int i = 0; i < 4; i++){
        if(charMonHP[i] != 0){
            int startX = charMonXPos[i];
            int startY = charMonYPos[i];
            int currX = startX;
            int currY = startY;
            int radius = 1;
            pathingMap[calcXY(startX, startY)] = 0;
            //While there is a part of the map not seen by the scan
            while(currX - radius > -1 || currX + radius < 20 || currY - radius > -1 || currY + radius < 20){
                for(int circOffset = 0; circOffset <= radius; circOffset++){
                    checkSetPath(startX-radius, startY+circOffset);
                    checkSetPath(startX-radius, startY-circOffset-1);
                    checkSetPath(startX+radius, startY+circOffset);
                    checkSetPath(startX+radius, startY-circOffset-1);
                    checkSetPath(startX+circOffset, startY-radius);
                    checkSetPath(startX-circOffset-1, startY-radius);
                    checkSetPath(startX+circOffset, startY+radius);
                    checkSetPath(startX-circOffset-1, startY+radius);
                }
                radius++;
            }
        }
    }
}

void setIDMap(){
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            if(mapBackground[(i * mapWidth) + j] == '_'){
                idMap[(i * mapWidth) + j] = -2;
            } else {
                idMap[(i * mapWidth) + j] = -1;
            }
        }
    }
    for(int i = 0; i < entityCount; i++){
        int x = charMonXPos[i];
        int y = charMonYPos[i];
        idMap[(y * mapWidth) + x] = i;
    }
}

void setMapDisplay(){
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            mapDisplay[(i * mapWidth) + j] = mapBackground[(i * mapWidth) + j];
        }
    }
    for(int i = 0; i < entityCount; i++){
        int x = charMonXPos[i];
        int y = charMonYPos[i];
        mapDisplay[(y * mapWidth) + x] = charMonMapMarker[i];
    }
}

void spawnMob(int idNum, int *stats, char marker){
    int x = roll(20)-1;
    int y = roll(20)-1;
    //while (x,y) corresponds to an invalid spawning space (occupied or too close)
    while(mapBackground[(y * mapWidth) + x] != '_' || (y > 15 && (x > 5 && x < 14))){
        x = roll(20)-1;
        x = roll(20)-1;
    }
    setCharMon(idNum, stats, marker, x, y);
}

void moveChar(int idNum, int x, int y){
    charMonXPos[idNum] = x;
    charMonYPos[idNum] = y;
    setMapDisplay();
}

void attack(int idNum, int type, int mod, int targX, int targY){
    int attackBonus;
    int targID = idMap[calcXY(targX,targY)];
    int dieType;
    int dieQuant;
    int damMod = 0;
    int save = 0;
    bool hit = false;
    int dieRoll = roll(20);
    int rollTotal;
    int damage;
    if(type = 1){
        attackBonus = charMonAttackMAbl[idNum];
        dieType = charMonAttackMDam[idNum];
    } else {
        attackBonus = charMonAttackRAbl[idNum];
        dieType = charMonAttackRDam[idNum];
        save = charMonAttackRSav[idNum];
    }
    if(attackBonus == 1){
        attackBonus = charMonStr[idNum] + charMonProf[idNum];
    } else if(attackBonus == 2){
        attackBonus = charMonDex[idNum] + charMonProf[idNum];
    } else if(attackBonus == 3){
        attackBonus = charMonCon[idNum] + charMonProf[idNum];
    } else if(attackBonus == 4){
        attackBonus = charMonInt[idNum] + charMonProf[idNum];
    } else if(attackBonus == 5){
        attackBonus = charMonWis[idNum] + charMonProf[idNum];
    } else {
        attackBonus = charMonCha[idNum] + charMonProf[idNum];
    }
    if(save == 0){
        rollTotal = dieRoll + attackBonus + mod;
        if(rollTotal >= charMonAC[targID]){
            hit = true;
            printName(idNum);
            printf(" successfully hit ");
            printName(targID);
            printf(" for ");
        } else {
            printName(idNum);
            printf(" failed to hit ");
            printName(targID);
            printf(".\n");
        }
    } else {
        if(attackBonus == 1){
            rollTotal = roll(20) + charMonStr[targID];
            damMod = charMonStr[targID];
        } else if(attackBonus == 2){
            rollTotal = roll(20) + charMonDex[targID];
            damMod = charMonDex[targID];
        } else if(attackBonus == 3){
            rollTotal = roll(20) + charMonCon[targID];
        } else if(attackBonus == 4){
            rollTotal = roll(20) + charMonInt[targID];
        } else if(attackBonus == 5){
            rollTotal = roll(20) + charMonWis[targID];
        } else {
            rollTotal = roll(20) + charMonCha[targID];
        }
        if(rollTotal < (8 + attackBonus)){
            hit = true;
            printName(targID);
            printf(" could not resist ");
            printName(idNum);
            printf("'s spell and took ");
        } else {
            printName(targID);
            printf(" was able to resist ");
            printName(idNum);
            printf("'s spell.\n");
        }
    }
    if(hit == true){
        if(dieType > 1000){
            dieQuant = dieType/1000;
            dieType = dieType - (dieQuant * 1000);
        } else {
            dieQuant = dieType/100;
            dieType = dieType - (dieQuant * 100);
        }
        damage = rollMult(dieQuant, dieType) + damMod;
        printf("%d damage!\n", damage);
        charMonHP[targID] -= damage;
        if(charMonHP[targID] < 0){
            charMonHP[targID] == 0;
        }
        //If the target is a zombie, was dropped to 0, and it wasn't from a crit/spell
        if(charMonHP[targID] == 0){
            if(charMonMapMarker[targID] == 'Z' && (dieRoll == 20 || save != 0)){
                if(roll(20) + charMonCon[targID] > 5 + damage){
                    charMonHP[targID] = 1;
                    printf("Using its undead fortitude, the zombie won't stay down!\n");
                }
            }
            if(targID > pcCount-1){
                printf("The ");
                printName(targID);
                printf(" succumbs to its wounds and perishes.\n");
                charMonMapMarker[targID] = '_';

            } else {
                printName(targID);
                printf(" falls unconscious!\n");
                setPathingMap();
            }
        }
    }
    
}

//Returns a modifier to attack rolls. If objects in the way there is a -2 penalty
//If the target is out of range the penalty will be set to -20 and be considered
//Not a valid target
int checkTarget(int idNum, int targX, int targY, int range){
    int diffX = charMonXPos[idNum]-targX;
    int diffY = charMonYPos[idNum]-targY;
    int penalty = 0;
    // If the attack is out of range
    if((diffX*diffX)+(diffY*diffY)>(range*range)){
        if(!((diffX*diffX)+(diffY*diffY) == 2 && range == 1)){
            penalty = -20;
        }
    } else if((diffX*diffX)+(diffY*diffY) == 2 && range > 1){
        penalty -= 3; //Penalty for shooting into melee
    }
    return penalty;
}

void monsterTurn(int idNum){
    int movePool = charMonSpd[idNum];
    int attackPool = charMonAttackNum[idNum];
    int targX = -1;
    int targY = -1;
    bool attacked = false;
    while(attackPool > 0 || movePool > 0){
        while(movePool > 0 && targX != -1){
            int lowest = 99;
            int lowX;
            int lowY;
            int currX = charMonXPos[idNum];
            int currY = charMonYPos[idNum];
            for(int i = -1; i < 2; i++){
                for(int j = -1; j < 2; j++){
                    if(!(i == 0 && j == 0)){
                        if(pathingMap[calcXY(currX+j, currY+i)] == 0){
                            targX = currX+j;
                            targY = currY+i;
                        } else if(pathingMap[calcXY(currX+j, currY+i)] < lowest){
                            lowest = pathingMap[calcXY(currX+j, currY+i)];
                            lowX = currX + j;
                            lowY = currY + i;
                        }
                    }
                }
            }
            if(targX == -1){
                moveChar(idNum, lowX, lowY);
                printMap(mapDisplay);
                movePool--;
            }
        }
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                if(!(i == 0 && j == 0)){
                    if(pathingMap[calcXY(charMonXPos[idNum]+j, charMonYPos[idNum]+i)] == 0){
                        targX = charMonXPos[idNum]+j;
                        targY = charMonYPos[idNum]+i;
                    }
                }
            }
            
        }
        if(targX == -1){
            attackPool = 0;
        } else {
            attack(idNum, 1, 0, targX, targY);
            attackPool--;
        }
    }
}

void playerTurn(int idNum){
    int movePool = charMonSpd[idNum];
    int attackPool = charMonAttackNum[idNum];
    bool actionUsed = false;
    bool turnEnd = false;
    int choice = 0;
    while(turnEnd == false){
        printf("Choose: ");
        if(movePool > 0){
            printf("1 (move), ");
        }
        if(attackPool > 0){
            printf("2 (attack), ");
        }
        printf("3 (end turn).\n");
        printf("You have %d square(s) of movement and %d attack(s)", movePool, attackPool);
        cin >> choice;
        if(choice < 0 || choice > 3){
            printf("Invalid choice. Try again.\n");
        } else {
            while(choice == 1 && movePool > 0){
                int moveChoice = 0;
                int currX = charMonXPos[idNum];
                int currY = charMonYPos[idNum];
                printf("Choose direction: 1 (^Up), 2(<Left), 3(Right>), 4(vDown), or 5(Stop)\n");
                cin >> moveChoice;
                if(moveChoice == 1 && mapDisplay[calcXY(currX, currY-1)] == '_'){
                    moveChar(idNum, currX, currY-1);
                    movePool--;
                } else if(moveChoice == 2 && mapDisplay[calcXY(currX-1, currY)] == '_'){
                    moveChar(idNum, currX-1, currY);
                    movePool--;
                } else if(moveChoice == 3 && mapDisplay[calcXY(currX+1, currY)] == '_'){
                    moveChar(idNum, currX+1, currY);
                    movePool--;
                } else if(moveChoice == 4 && mapDisplay[calcXY(currX, currY+1)] == '_'){
                    moveChar(idNum, currX, currY+1);
                    movePool--;
                } else if(moveChoice == 5){
                    choice = 0;
                } else {
                    printf("Invalid input or the space was occupied. Try again.\n");
                }
                printMap(mapDisplay);
            }
            while(choice == 2 && attackPool > 0){
                int attackChoice = 0;
                printf("Choose attack type: 1 (melee), 2 (ranged), or 3 (cancel).\n");
                cin >> attackChoice;
                if(attackChoice == 1 || attackChoice == 2){
                    int range = charMonAttackMRan[idNum];
                    if(attackChoice == 2){
                        range = charMonAttackRRan[idNum];
                    }
                    int targX = -1;
                    int targY = -1;
                    while(targX == -1){
                        printMap(mapDisplay);
                        printf("Attack range: %d squares.\n", range);
                        printf("What is the x value of the target? (Enter 0 to cancel attack)\n");
                        cin >> targX;
                        if(targX != 0){
                            printf("What is the y value of the target?\n");
                            cin >> targY;
                            if(targX > 0 && targX < 21 && targY > 0 && targY < 21){
                                targX--;
                                targY--;
                                int mod = checkTarget(idNum, targX, targY, range);
                                if(mod < -19){
                                    printf("Target is out of range. Choose another option or set x to 0 to cancel.\n");
                                    targX == -1;
                                    targY == -1;
                                } else {
                                    attack(idNum, attackChoice, mod, targX, targY);
                                    attackPool--;
                                }
                            } else {
                                printf("Target is not on the map. What are you aiming at?\n");
                            }
                        } else {
                            choice = 0;
                        }
                    }
                } else if(attackChoice == 3){
                    choice = 0;
                } else {
                    printf("Invalid choice. Try again.\n");
                }
            }
            if(choice == 3){
                turnEnd == true;
            }
        }
    }
}

int main(){
    srand((unsigned) time(NULL));
    setCharMon(0, dhulgenStats, 'D', 9, 18);
    setCharMon(1, ogrimStats, 'O', 10, 18);
    setCharMon(2, kolgarStats, 'K', 9, 19);
    setCharMon(3, torbinStats, 'T', 10, 19);
    for(int i = 4; i < 24; i++){
        spawnMob(i, zombieDefault, 'Z');
    }
    setIDMap();
    printf("Setting up map display...\n");
    setMapDisplay();
    printf("Calculating paths...\n");
    setPathingMap();
    printMap(mapBackground);
    printf("\n");
    printMap(mapDisplay);
    printf("\n");
    printIntMap(idMap);
    printf("\n");
    printIntMap(pathingMap);
    int initiative[100];
    for(int i = 0; i < entityCount; i++){
        initiative[i] = roll(20)+charMonDex[i];
        printName(i);
        printf(": ID(%d), Initiative(%d)\n", i, initiative[i]);
    }
    int gameState = 0;
    while(gameState == 0){
        for(int i = 25; i > -5; i--){
            for(int j = 0; j < entityCount; i++){
                if(i == initiative[j]){
                    //Take Turn
                    printMap(mapDisplay);
                    if(j > pcCount-1 && charMonHP[j] > 0){
                        printf("Monster turn. Initiative: %d.\n", i);
                        monsterTurn(j);
                    } else {
                        printf("Player turn. Initiative: %d.\n", i);
                        playerTurn(j);
                    }
                    setPathingMap();
                }
            }
        }
        gameState = -1;
        for(int i = 0; i < pcCount; i++){
            if(charMonHP[i] > 0){
                gameState = 0;
            }
        }
        if(gameState == 0){
            gameState = 1;
            for(int i = pcCount; i < entityCount; i++){
                if(charMonHP[i] > 0){
                    gameState = 0;
                }
            }
        }
        srand((unsigned) time(NULL));
    }
    if(gameState == 1){
        printf("You have successfully cleansed your temple of the monsters!\n");
    } else {
        printf("Sadly, your party has fallen at the hands of the monsters...\n");
    }
}