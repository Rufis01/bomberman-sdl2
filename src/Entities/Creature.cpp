#include "./Creature.h"

namespace bomberman
{
    void Creature::revertLastMove()
    {
        setPosition(getPositionX() - prevPosDeltaX, getPositionY() - prevPosDeltaY);
    }
    void Creature::revertLastMoveX()
    {
        setPosition(getPositionX() - prevPosDeltaX, getPositionY());
    }
    void Creature::revertLastMoveY()
    {
        setPosition(getPositionX(), getPositionY() - prevPosDeltaY);
    }

    void Creature::setMoving(bool _moving)
    {
        this->moving = _moving;
    }

    bool Creature::isMoving() const
    {
        return moving;
    }
} // namespace bomberman
