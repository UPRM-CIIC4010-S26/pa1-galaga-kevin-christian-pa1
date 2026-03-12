#pragma once
#include "Hitbox.hpp"
#include "Projectile.hpp"
#include "Background.hpp"
#include "ImageManager.hpp"

class Player {
    private:
        int cooldown = 30;
        int speed = 3;

        int boostShotsRemaining = 0;
        int boostDelay = 0;
        const int boostDelayFrames = 6; // delay between arcs
    
    public:
        std::pair<float, float> position;
        HitBox hitBox;
        int getBoostShotsRemaining() const { return boostShotsRemaining; }

        Player(float x, float y) {
            this->position.first = x;
            this->position.second = y;
            this->hitBox = HitBox(x, y, 30, 30);
        }

        void draw();
        void update();
        void keyInputs();
        void attack();
        void activateBoost();
};