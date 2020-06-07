#include "./Player.h"
#include "../Const.h"

namespace bomberman
{
    Player::Player(std::shared_ptr<SDL_Texture> _texture, SDL_Renderer* _renderer)
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
            movementUp->addAnimationEntity(AnimationEntity(tileSize * (16 + i), 0, tileSize, tileSize));
        }
        for(int i = 15; i > 7; i--) //This one are reversed because I just mirrored the whole image and copypasted it
        {
            movementLeft->addAnimationEntity(AnimationEntity(tileSize * i, 0, tileSize, tileSize));
            movementDown->addAnimationEntity(AnimationEntity(tileSize * (16 + i), 0, tileSize, tileSize));
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

    void Player::setMovementDirection(unsigned char direction)  //SDL_Flip doesn't work on vita
    {
        movementDirection = direction;
        setMoving(movementDirection != (unsigned char) MovementDirection::None);

        if(movementDirection == (unsigned char) MovementDirection::None)
        {
            movementLeft->pause();
            movementRight->pause();
            movementUp->pause();
            movementDown->pause();
        }
        else if(movementDirection & (unsigned char) MovementDirection::Left)
        {
            movementLeft->play();
            movementRight->pause();
            movementUp->pause();
            movementDown->pause();
        }
        else if(movementDirection & (unsigned char) MovementDirection::Right)
        {
            movementLeft->pause();
            movementRight->play();
            movementUp->pause();
            movementDown->pause();
        }
        else if(movementDirection & (unsigned char) MovementDirection::Up)
        {
            movementLeft->pause();
            movementRight->pause();
            movementUp->play();
            movementDown->pause();
        }
        else if(movementDirection & (unsigned char) MovementDirection::Down)
        {
            movementLeft->pause();
            movementRight->pause();
            movementUp->pause();
            movementDown->play();
        }
    }

    void Player::update(const unsigned int delta)
    {
        // movement
        if(isMoving())
        {
            const int posDiff = static_cast<int>(floor(speed * delta * getWidth()));
            prevPosDeltaX =
                posDiff *
                (isMovingHorizontally() ? (movementDirection & (unsigned char) MovementDirection::Left ? -1 : 1) : 0);
            prevPosDeltaY =
                posDiff * (isMovingVertically() ? (movementDirection & (unsigned char) MovementDirection::Up ? -1 : 1) : 0);
            // move sprite to next tick pos
            if(abs(prevPosDeltaX) == posDiff && abs(prevPosDeltaY) == posDiff)
            {
                prevPosDeltaX *= 0.5;
                prevPosDeltaY *= 0.5;
            }
            setPosition(getPositionX() + prevPosDeltaX, getPositionY() + prevPosDeltaY);
        }

        Creature::update(delta);
    }

    bool Player::isMovingVertically() const
    {
        return movementDirection & (unsigned char) MovementDirection::Up || movementDirection & (unsigned char) MovementDirection::Down;
    }

    bool Player::isMovingHorizontally() const
    {
        return movementDirection & (unsigned char) MovementDirection::Left || movementDirection & (unsigned char) MovementDirection::Right;
    }
} // namespace bomberman
