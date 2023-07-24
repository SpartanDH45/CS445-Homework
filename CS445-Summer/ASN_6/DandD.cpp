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

int charMonHP[100], charMonAC[100], charMonSpd[100], charMonProf[100];
int charMonStr[100], charMonDex[100], charMonCon[100];
int charMonInt[100], charMonWis[100], charMonCha[100];
int charMonAttackMDam[100], charMonAttackRDam[100];
int charMonAttackMRan[100], charMonAttackRRan[100];
//XY Positions given as a single integer XXYY
int charMonXYPos[100];
//
char mapDisplay[400];
char mapBackground[400] = ".x.x.~..xxxx..~.xx...........xx........x...x............x........~xxx..xxx~.....~~..~~~~~..~~~~~..~~.....~........~......xx................x...................x.xx..~....x...~............x.xx.x......x.......x.xx........x...x.~...xx.x.~.....x......x.xx........xx..x......x........x.....~........~.....~~..~~~~~..~~~~~..~~...x.~........~.......................x";
char pathingMap[400];


int main(){
    srand((unsigned) time(NULL));

}