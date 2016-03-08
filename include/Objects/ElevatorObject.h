#ifndef ELEVATOROBJECT_H
#define ELEVATOROBJECT_H

#include <iostream>
#include <GameObject.h>

/**
Elevator. Moves player up and down.
*/
class ElevatorObject : public GameObject
{
    public:
        ElevatorObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, int posX, int posY, LTextureFactory* lTextFact, int altitude);
        virtual ~ElevatorObject();

        void onCollision();
        int move(PlayerPosition* playerPos);
        std::string getTextureName();
    protected:
    private:
        int altitude = ELEVATOR_DEFAULT_ALTITUDE; //(altitude can be overridden)
        int initPosY;
        bool up;
};

#endif // ELEVATOROBJECT_H
