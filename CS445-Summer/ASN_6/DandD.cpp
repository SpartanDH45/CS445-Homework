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
    int srcY = srcXY-srcX;
    int targX = targXY/100;
    int targY = targXY-targX;
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
        printf("X");
        for(int j = 0; j < mapWidth; j++){
            if(i == -1 || i == mapWidth){
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
        printf("X\n");
    }
}
int zombieDefault[] = (22, 8, 4, 2, 13, 6, 16, 3, 6, 5, 106, 1, 0, 0); 
int charMonHP[100], charMonAC[100], charMonSpd[100], charMonProf[100];
int charMonStr[100], charMonDex[100], charMonCon[100];
int charMonInt[100], charMonWis[100], charMonCha[100];
int charMonAttackMDam[100], charMonAttackRDam[100];
int charMonAttackMRan[100], charMonAttackRRan[100];
char charMonCharMarker[100];
//XY Positions given as a single integer XXYY
int charMonXYPos[100];
//
char mapDisplay[500];
char mapBackground[500] = "_x_x_~__xxxx__~_xx___________xx________x___x____________x________~xxx__xxx~_____~~__~~~~~__~~~~~__~~_____~________~______xx________________x___________________x_xx__~____x___~____________x_xx_x______x_______x_xx________x___x_~___xx_x_~_____x______x_xx________xx__x______x________x_____~________~_____~~__~~~~~__~~~~~__~~___x_~________~_______________________x__x________________x____x_~________~_____";
char pathingMap[500];

void setCharMon(int idNum, int HP, int AC, int Spd, int Prof, int )

int main(){
    srand((unsigned) time(NULL));
    printMap(mapBackground);
}