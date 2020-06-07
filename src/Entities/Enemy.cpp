#include <chrono>
#include <functional>
#include <random>

#include "../Const.h"
#include "./Enemy.h"

namespace bomberman
{
    Enemy::Enemy(std::shared_ptr<SDL_Texture> _texture, SDL_Renderer* _renderer)
        : Creature(_texture, _renderer)
    {
        // movement animation
        movementRight = std::make_shared<Animation>();
        movementLeft = std::make_shared<Animation>();
        movementUp = std::make_shared<Animation>();
        movementDown = std::make_shared<Animation>();

        for(int i = 0; i < 8; i++)
        {
            movementRight->addAnimationEntity(AnimationEntity(tileSize * i, 0, tileSize, tileSize));
            movementDown->addAnimationEntity(AnimationEntity(tileSize * (16 + i), 0, tileSize, tileSize));  //I totally didn't mess the order up when editing the sprites
        }
        for(int i = 15; i > 7; i--) //This one are reversed because I just mirrored the whole image and copypasted it
        {
            movementLeft->addAnimationEntity(AnimationEntity(tileSize * i, 0, tileSize, tileSize));
            movementUp->addAnimationEntity(AnimationEntity(tileSize * (16 + i), 0, tileSize, tileSize));    //I totally didn't mess the order up when editing the sprites
        }

        movementRight->setSprite(this);
        movementLeft->setSprite(this);
        movementUp->setSprite(this);
        movementDown->setSprite(this);
        addAnimation(movementRight);
        addAnimation(movementLeft);
        addAnimation(movementUp);
        addAnimation(movementDown);
    }

    void Enemy::moveTo(const int x, const int y)
    {
        // start a moving, see update func
        setMoving(true);
        // save new position
        newPositionX = getPositionX() + x;
        newPositionY = getPositionY() + y;
        // flip
        if(x < 0)
        {
            movementLeft->play();
            movementRight->pause();
            movementUp->pause();
            movementDown->pause();
        }
        else if(x > 0)
        {
            movementLeft->pause();
            movementRight->play();
            movementUp->pause();
            movementDown->pause();
        }
        else if(y < 0)
        {
            movementLeft->pause();
            movementRight->pause();
            movementUp->play();
            movementDown->pause();
        }
        else if(y > 0)
        {
            movementLeft->pause();
            movementRight->pause();
            movementUp->pause();
            movementDown->play();
        }
    }

    void Enemy::moveToCell(std::pair<int, int> pathToCell)
    {
        // save moving path
        path = pathToCell;
        movingToCell = true;
        // move enemy to nearest cell
        newPositionX = getPositionX() - ((getPositionX() - newPositionX) % getWidth());
        newPositionY = getPositionY() - ((getPositionY() - newPositionY) % getHeight());
    }

    bool Enemy::isMovingToCell() const
    {
        return movingToCell;
    }

    bool Enemy::canAttack() const
    {
        return aiType == AIType::Chasing;
    }

    int Enemy::getAttackRadius() const
    {
        return attackRadius * getWidth();
    }

    void Enemy::setAIType(AIType type)
    {
        aiType = type;
    }

    void Enemy::update(const unsigned int delta)
    {
        Creature::update(delta);
        // simple wandering moving
        if(isMoving())
        {
            updateMovement(delta);
        }
        // moving to cell
        else if(movingToCell)
        {
            moveTo(path.second * getWidth(), path.first * getHeight());
        }
        // new random path
        else
        {
            generateNewPath();
        }
    }

    void Enemy::updateMovement(const unsigned int delta)
    {
        // calculate consts for movement
        const int newPositionDiffX = getPositionX() - newPositionX;
        const int newPositionDiffY = getPositionY() - newPositionY;
        const signed char signOfX = (newPositionDiffX > 0) ? 1 : ((newPositionDiffX < 0) ? -1 : 0);
        const signed char signOfY = (newPositionDiffY > 0) ? 1 : ((newPositionDiffY < 0) ? -1 : 0);
        const int posDiff = static_cast<int>(floor((canAttack() ? attackSpeed : baseSpeed) * delta * getWidth()));

        prevPosDeltaX = posDiff * -signOfX;
        prevPosDeltaY = posDiff * -signOfY;

        // finish movement
        if(newPositionDiffX * signOfX <= posDiff && newPositionDiffY * signOfY <= posDiff)
        {
            movementLeft->pause();
            movementRight->pause();
            setMoving(false);
            movingToCell = false;
            /*std::cout << "MOVEMENT ENDED ("<<newPositionDiffX * signOfX<<", "<<newPositionDiffY * signOfY<<") --> Moving enemy to (" << newPositionX << ", " << newPositionY << ")\n";
            if(newPositionDiffX * signOfX < 0 )
            {
                std::cout << "PosX: " << getPositionX() << std::endl
                          << "newPositionX: " << newPositionX << std::endl 
                          << "newPositionDiffX: " << newPositionDiffX << std::endl 
                          << "signOfX: " << (int)signOfX << std::endl;
            }
            if(newPositionDiffY * signOfY < 0 )
            {
                std::cout << "PosY: " << getPositionY() << std::endl
                          << "newPositionY: " << newPositionY << std::endl 
                          << "newPositionDiffY: " << newPositionDiffY << std::endl 
                          << "signOfY: " << (int)signOfY << std::endl;
            }*/
            setPosition(newPositionX, newPositionY);
            return;
        }
        // move sprite to next tick pos
        setPosition(getPositionX() - static_cast<int>(floor(posDiff)) * signOfX,
                    getPositionY() - static_cast<int>(floor(posDiff)) * signOfY);
            /*std::cout << "Moving enemy to (" << getPositionX() - static_cast<int>(floor(posDiff)) * signOfX << ", "
            << getPositionY() - static_cast<int>(floor(posDiff)) * signOfY << ")\n";*/
    }

    void Enemy::generateNewPath()
    {
        // create random func
        const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        auto randPath = std::bind(std::uniform_int_distribution<int>(1, 10),
                                  std::mt19937(static_cast<unsigned int>(seed)));
        auto randSign = std::bind(std::uniform_int_distribution<int>(-1, 1),
                                  std::mt19937(static_cast<unsigned int>(seed)));

        // generate direction and cells
        const int randUpDown = randPath();
        const int randUpDownSign = randSign();
        if(randUpDownSign != 0)
        {
            moveTo(0, randUpDown * randUpDownSign * getHeight());
            return;
        }
        const int randLeftRight = randPath();
        const int randLeftRightSign = randSign();
        if(randLeftRightSign != 0)
        {
            moveTo(randLeftRight * randLeftRightSign * getWidth(), 0);
        }
    }
} // namespace bomberman
