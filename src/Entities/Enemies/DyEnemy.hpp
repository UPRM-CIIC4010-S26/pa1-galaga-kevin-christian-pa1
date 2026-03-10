#pragma once
#include "Enemy.hpp"

class DyEnemy : public Enemy {
    private: 
        float angle = 135;
        float aimAngle = 225;
        bool loop = false;
        Rectangle texture;  // Stores the sprite used to draw the enemy

    public:
        DyEnemy(float x, float y) : Enemy(x, y) { 
            this->cooldown = GetRandomValue(90, 300);
            this->health = 1; 
            this->points = 150;

            // Randomly selects one of two textures when the enemy is created

            if (GetRandomValue(0, 1) == 0)
            {
                texture = {2, 128, 13, 14};
            }
            else
            {
                texture = {2, 147, 13, 13};
            }
        }

        void draw() override;
        void update(std::pair<float, float> pos, HitBox target) override;
        void attack(HitBox target) override;
};