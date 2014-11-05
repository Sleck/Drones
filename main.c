#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t lock;
pthread_mutex_t locationX[100];
pthread_mutex_t locationY[100];

void *Drone(void *threadid);

int main(int argc, char *argv[])
{

    int input;
    int rc;
    printf("How many drones do you want to fly? \n");
    scanf("%d", &input);

    pthread_t drones[input];

    long x;
    int y;
    srand(time(NULL));
    for(x = 1; x <= input; x++){
        printf("Drone #%ld has queued up at the Decoy Center. \n", x);
        pthread_create(&drones[x], NULL, Drone, (void *) x);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(y = 1; y <= input; y++){
        pthread_join(drones[y], NULL);
    }
    pthread_exit(NULL);
    return 0;
}

void *Drone(void *threadid)
{
  int targetLocationX = rand()% 50;
  int targetLocationY = rand()% 50;
  int currentLocationX = 25;
  int currentLocationY = 25;
  int DecoyCenterX = 25;
  int DecoyCenterY = 25;
  long tid;
  tid = (long)threadid;
  int state = 0;
  switch (state){
    case 0: //queue to take off
        pthread_mutex_lock(&lock);
            printf("Drone #%ld is taking off... \n", tid);
            sleep(4);
        pthread_mutex_unlock(&lock);

            state = 1;

    case 1: //flying to target location
        printf("Drone #%ld target location: %d, %d.\n", tid, targetLocationX, targetLocationY);

        int tempTargetLocationX, tempTargetLocationY;

        tempTargetLocationX = targetLocationX - 1;
        tempTargetLocationY = targetLocationY - 1;

        while(!(currentLocationX == (tempTargetLocationX) && currentLocationY == (tempTargetLocationY))){

                if(currentLocationX > tempTargetLocationX){
                    currentLocationX--;
                    pthread_mutex_lock(&locationX[currentLocationX]);
                    sleep(1);
                    pthread_mutex_unlock(&locationX[currentLocationX]);
                }else
                if(currentLocationX < tempTargetLocationX){
                    currentLocationX++;
                    pthread_mutex_lock(&locationX[currentLocationX]);
                    sleep(1);
                    pthread_mutex_unlock(&locationX[currentLocationX]);
                }

                if(currentLocationY > tempTargetLocationY){
                    currentLocationY--;
                    pthread_mutex_lock(&locationY[currentLocationY]);
                    sleep(1);
                    pthread_mutex_unlock(&locationY[currentLocationY]);
                }else
                if(currentLocationY < tempTargetLocationY){
                    currentLocationY++;
                    pthread_mutex_lock(&locationY[currentLocationY]);
                    sleep(1);
                    pthread_mutex_unlock(&locationY[currentLocationY]);
                }

                printf("Drone #%ld is flying to the target location(%d, %d)... Current location: %d, %d \n", tid, targetLocationX, targetLocationY, currentLocationX, currentLocationY);


            }

        if(currentLocationX == tempTargetLocationX && currentLocationY == tempTargetLocationY){
            state = 2;
        }

    case 2: //landing at target location
        pthread_mutex_lock(&locationX[targetLocationX]);
        pthread_mutex_lock(&locationY[targetLocationY]);
            printf("Drone #%ld is landing at the target location(%d, %d)... \n", tid, targetLocationX, targetLocationY);
            sleep(5);
            printf("Drone #%ld has landed at target location: %d, %d\n", tid, targetLocationX, targetLocationY);
        pthread_mutex_unlock(&locationX[targetLocationX]);
        pthread_mutex_unlock(&locationY[targetLocationY]);

            state = 3;

    case 3: //taking off from target location
        pthread_mutex_lock(&locationX[targetLocationX]);
        pthread_mutex_lock(&locationY[targetLocationY]);
            printf("Drone #%ld is returning to the Decoy Center(25, 25)... Taking off... \n", tid);
            sleep(4);
            currentLocationX = targetLocationX;
            currentLocationY = targetLocationY;
        pthread_mutex_unlock(&locationX[targetLocationX]);
        pthread_mutex_unlock(&locationY[targetLocationY]);

            state = 4;

    case 4: //flying back to Decoy Center
        tempTargetLocationX = 24; tempTargetLocationY = 24;

        while(!(currentLocationX == (tempTargetLocationX) && currentLocationY == (tempTargetLocationY))){

                if(currentLocationX > tempTargetLocationX){
                    currentLocationX--;
                    pthread_mutex_lock(&locationX[currentLocationX]);
                    sleep(1);
                    pthread_mutex_unlock(&locationX[currentLocationX]);
                }else if(currentLocationX < tempTargetLocationX){
                    currentLocationX++;
                    pthread_mutex_lock(&locationX[currentLocationX]);
                    sleep(1);
                    pthread_mutex_unlock(&locationX[currentLocationX]);
                }

                if(currentLocationY > tempTargetLocationY){
                    currentLocationY--;
                    pthread_mutex_lock(&locationY[currentLocationY]);
                    sleep(1);
                    pthread_mutex_unlock(&locationY[currentLocationY]);
                }else if(currentLocationY < tempTargetLocationY){
                    currentLocationY++;
                    pthread_mutex_lock(&locationY[currentLocationY]);
                    sleep(1);
                    pthread_mutex_unlock(&locationY[currentLocationY]);
                }

                printf("Drone #%ld is returning to the Decoy Center(25, 25)... Current location: %d, %d \n", tid, currentLocationX, currentLocationY);


            }

        if(currentLocationX == tempTargetLocationX && currentLocationY == tempTargetLocationY){
            state = 5;
        }

    case 5: //landing at Decoy Center
        pthread_mutex_lock(&lock);
            printf("Drone #%ld is landing at the Decoy Center... \n", tid);
            sleep(5);
            printf("Drone #%ld has landed at the Decoy Center.\n", tid, DecoyCenterX, DecoyCenterY);
        pthread_mutex_unlock(&lock);
        break;
  }

  pthread_exit(NULL);
  return 0;
}
