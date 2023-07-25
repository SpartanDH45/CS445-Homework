#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
using namespace std;

int roll(int faces){
    return (rand() % faces + 1);
}
//Returns a modifier to attack rolls. If objects in the way there is a -2 penalty
//If the target is out of range the penalty will be set to -200 and be considered
//Not a valid target
int checkTarget(int srcXY, int targXY, int range){
    int srcX = srcXY/100;
    int srcY = srcXY-(srcX*100);
    int targX = targXY/100;
    int targY = targXY-(targX*100);
    int diffX = srcX-targX;
    int diffY = srcY-targY;
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
int mapWidth = 20;
int mapHeight = 20;

int calcXY(int x, int y){
    return ((y * mapWidth) + x);
}



void printMap(char *map){
    for(int i = -1; i <= mapHeight; i++){
        if( i == 2 || i == 9 || i == 10 || i == 17){
            printf("|");
        } else {
            printf("X");
        }
        for(int j = 0; j < mapWidth; j++){
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
                if(j != mapWidth-1){
                    printf(" ");
                }
            }
        }
        if( i == 2 || i == 9 || i == 10 || i == 17){
            printf("|\n");
        } else {
            printf("X\n");
        }
    }
}

void printIntMap(int *map){
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            if(map[calcXY(j, i)] > -1){
                printf("  ");
            } else {
                printf(" ");
            }
            if(map[calcXY(j, i)] * map[calcXY(j, i)] < 100){
                printf("0%d", map[calcXY(j, i)]);
            } else {
                printf("%d", map[calcXY(j, i)]);
            }
        }
        printf("\n");
    }
}

//Order for storage: HP,AC,Spd,Prof,Str,Dex,Con,Int,Wis,Cha,
//                   MDam,MRan,Mabl,RDam,RRan,RAbl,RSav
int zombieDefault[] = {22, 8, 4, 2, 1, -2, 3, 3, 0, -3, 106, 1, 1,  0, 0, 1, 0};
int dhulgenStats[] = {50, 18, 5, 3, 4, 0, 3, 1, 1, -1, 206, 1, 1, 106, 6, 1, 0};
int ogrimStats[] = {50, 18, 5, 3, 2, 2, 3, 1, 4, -1, 108, 1, 1, 208, 12, 5, 2};
int kolgarStats[] = {50, 16, 5, 3, 1, 2, 3, 4, 1, -1, 108, 1, 1, 2010, 20, 4, 0};
int torbinStats[] = {50, 16, 5, 3, 4, 2, 2, 1, 0, 3, 108, 1, 1, 104, 6, 1, 0};
int charMonHP[100], charMonAC[100], charMonSpd[100], charMonProf[100];
int charMonStr[100], charMonDex[100], charMonCon[100];
int charMonInt[100], charMonWis[100], charMonCha[100];
int charMonAttackMDam[100], charMonAttackMRan[100], charMonAttackMAbl[100];
int charMonAttackRDam[100], charMonAttackRRan[100], charMonAttackRAbl[100];
int charMonAttackRSav[100];
char charMonMapMarker[100];
//XY Positions given as a single integer XXYY
int charMonXYPos[100];
//
char mapDisplay[500];
char mapBackground[500] = "_x_x_~__xxxx__~_xx___________xx________x___x____________x________~xxx__xxx~_____~~__~~~~~__~~~~~__~~_____~________~______xx________________x___________________x_xx__~____x___~____________x_xx_x______x_______x_xx________x___x_~___xx_x_~_____x______x_xx________xx__x______x________x_____~________~_____~~__~~~~~__~~~~~__~~___x_~________~_______________________x__x________________x____x_~________~_____";
int idMap[500];
int pathingMap[500] = {-1};
int entityCount = 0;

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
    charMonMapMarker[idNum] = marker;
    charMonXYPos[idNum] = (x*100)+y;
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
            pathingMap[calcXY(x, y)] = getLowestNeighbor(x, y);
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
        int startX = charMonXYPos[i]/100;
        int startY = charMonXYPos[i]-(startX*100);
        int currX = startX;
        int currY = startY;
        int radius = 1;
        pathingMap[calcXY(startX, startY)] = 0;
        //While there is a part of the map not seen by the scan
        while(currX - radius > -1 && currX + radius < 20 && currY - radius > -1 && currY + radius < 20){
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
        int x = charMonXYPos[i] / 100;
        int y = charMonXYPos[i] - (x * 100);
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
        int x = charMonXYPos[i] / 100;
        int y = charMonXYPos[i] - (x * 100);
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
}