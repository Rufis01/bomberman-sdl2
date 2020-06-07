#ifndef _BOMBERMAN_ENTITIES_PLAYER_H_
#define _BOMBERMAN_ENTITIES_PLAYER_H_

#include "./Creature.h"

#include "../Core/Animation.h"

namespace bomberman
{
    /**
     * @brief Enumeration of movement directions
     *
     */
    enum class MovementDirection
    {
        None = 0,
        Up = 1,
        Down = 2,
        Left = 4,
        Right = 8
    };
    /**
     * @brief Player class
     *
     */
    class Player : public Creature
    {
      public:
        /**
         * @brief Create player
         *
         */
        Player(std::shared_ptr<SDL_Texture> texture, SDL_Renderer* renderer);
        /**
         * @brief Set movement direction of player
         *
         * @param direction - movement direction
         */
        void setMovementDirection(unsigned char direction);
        /**
         * @brief Update movement and animation of player
         *
         * @param delta - time in milliseconds
         */
        virtual void update(const unsigned int delta) override;
        unsigned char movementDirection = (unsigned char) MovementDirection::None; // movement direction

      private:
        /**
         * @brief Is movement direction by Y
         *
         */
        bool isMovingVertically() const;
        /**
         * @brief Is movement direction by X
         *
         */
        bool isMovingHorizontally() const;

        const float speed = 0.004f;                                   // speed
        std::shared_ptr<Animation> movementRight;                     // movement animation
        std::shared_ptr<Animation> movementLeft;
        std::shared_ptr<Animation> movementDown;
        std::shared_ptr<Animation> movementUp;
    };
} // namespace bomberman

#endif // _BOMBERMAN_ENTITIES_PLAYER_H_
