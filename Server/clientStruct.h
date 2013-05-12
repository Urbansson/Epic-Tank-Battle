#ifndef _CLIENTSTRUCT_H
#define _CLIENTSTRUCT_H

struct client
{
    pthread_t threadId;         //Thread Id
    int mySlot;                 //My Slot on the server
    int sd;                     //TCP-Socket descriptor
    char client_ip_addr[20];    //Clients ip-address
    int free;                   //Flag that indicates if the slot is free. 1 == taken 0 == free
    
    int team;                   //Indicates what team the client are in;
    
    int xLocation;              //Where the client are located on the x-axis
    int yLocation;              //Where the client are located on the y-axis
    
    int forward;                //if the client are moving forward
    int backward;               //if the client are moving backwards
    int turnLeft;               //if the client are turning left
    int turnRight;              //if the client are turning right
    
    int tankAngle;              //The angle of the tank
    int cannonAngle;            //The angle of the cannom
    
    int mouseX;                 //Mouse x pos
    int mouseY;                 //Mouse y pos
    char mouseInput;            //What button on the mouse the client is pressing
    
    int fire;                   // If the client fire or not
    int bulletX;                //Bullet X
    int bulletY;                //Bullet Y
    int bulletHit;              //IF the the clients bullet hit something
    int bulletHitMe;            //If the client got hit by a bullet
    
    int speed;                  //THe tanks speed
    int tankCollision;          // Zero if no collision if collision the one you are col
    
    int healthPoints;           //Health of the tank 100 max 0 dead
    int dead;
};

#endif
