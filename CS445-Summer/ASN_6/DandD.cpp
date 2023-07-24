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
    if((diffX*diffX)+(diffY*diffY)>(range*range)){
        penalty = -20;
    }
    return penalty;
}
int mapWidth = 20;
int mapHeight = 20;
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
                printf("%c", map[(i*mapWidth)+j]);
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
int zombieDefault[] = (22, 8, 4, 2, 13, 6, 16, 3, 6, 5, 106, 1, 0, 0); 
int charMonHP[100], charMonAC[100], charMonSpd[100], charMonProf[100];
int charMonStr[100], charMonDex[100], charMonCon[100];
int charMonInt[100], charMonWis[100], charMonCha[100];
int charMonAttackMDam[100], charMonAttackRDam[100];
int charMonAttackMRan[100], charMonAttackRRan[100];
char charMonMapMarker[100];
//XY Positions given as a single integer XXYY
int charMonXYPos[100];
//
char mapDisplay[500]  = "_x_x_~__xxxx__~_xx___________xx________x___x____________x________~xxx__xxx~_____~~__~~~~~__~~~~~__~~_____~________~______xx________________x___________________x_xx__~____x___~____________x_xx_x______x_______x_xx________x___x_~___xx_x_~_____x______x_xx________xx__x______x________x_____~________~_____~~__~~~~~__~~~~~__~~___x_~________~_______________________x__x________________x____x_~________~_____";
char mapBackground[500] = "_x_x_~__xxxx__~_xx___________xx________x___x____________x________~xxx__xxx~_____~~__~~~~~__~~~~~__~~_____~________~______xx________________x___________________x_xx__~____x___~____________x_xx_x______x_______x_xx________x___x_~___xx_x_~_____x______x_xx________xx__x______x________x_____~________~_____~~__~~~~~__~~~~~__~~___x_~________~_______________________x__x________________x____x_~________~_____";
int idMap[500];
int pathingMap[500] = (-1);

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
    charMonAttackRDam[idNum] = stats[12];
    charMonAttackRRan[idNum] = stats[13];
    charMonMapMarker[idNum] = marker;
    charMonXYPos[idNum] = (x*100)+y;
    if(x > 19 || x < 0 || y > 19 || y < 0){
        charMonHP[idNum] = 0;
    } else {
        idMap[(y * mapWidth) + x] = idNum;
        mapDisplay[(y * mapWidth) + x] = marker;
    }


}
void getLowestNeighbor(int x, int y){
    
}

void setPathingMap(){
    if(pathingMap[0] == -1){
        for(int i = 0; i < mapHeight; i++){
            for(int j = 0; j < mapWidth; j++){
                pathingMap[(i*mapWidth) + j] = 100;
            }
        }
    }
    for(int i = 0; i < 4; i++){
        int startX = charMonXYPos[i]/100;
        int startY = charMonXYPos[i]-(startX*100);
        int currX = startX;
        int currY = startY;
        int radius = 1;
        pathingMap[(startY * mapWidth) + startX] = 0;
        //While there is a part of the map not seen by the scan
        while(currX - radius > -1 && currX + radius < 20 && currY - radius > -1 && currY + radius < 20){
            for(int circOffset = 0; circOffset <= radius; circOffset++){
                
            }
        }

    }
}

int main(){
    srand((unsigned) time(NULL));
    printMap(mapBackground);
}