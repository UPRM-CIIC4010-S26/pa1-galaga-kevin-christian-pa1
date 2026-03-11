#include "Projectile.hpp"

void Projectile::draw() {
    if (HitBox::drawHitbox) this->hitBox.draw();
    DrawRectangleGradientV(this->position.first, this->position.second, 2, 5, (ID == 1) ? WHITE : YELLOW, (ID == 1) ? YELLOW : WHITE);
}

void Projectile::update() {
    this->hitBox.box.x = this->position.first;
    this->hitBox.box.y = this->position.second;

    if (ID == 0) {
        this->position.first += this->speed * cos(this->angle * M_PI / 180);
        this->position.second -= this->speed * sin(this->angle * M_PI / 180);
    }

    if (ID == 1) {
        this->position.first += this->speed * cos(this->angle * M_PI / 180);
        this->position.second += this->speed * sin(this->angle * M_PI / 180);
    }

    if (this->position.second > GetScreenHeight() || this->position.second < 0) this->del = true;
}

void Projectile::CleanProjectiles() {
    for (int i = 0; i < projectiles.size(); i++) {
        if (projectiles[i].del) {
            projectiles.erase(projectiles.begin() + i);
        }
    }
}

void Projectile::ProjectileCollision() {
    for (int i = 0; i < projectiles.size(); i++) {
        for (int j = i + 1; j < projectiles.size(); j++) {
            if (projectiles[i].ID == projectiles[j].ID)  // do not delete bullets from same owner
                continue;  
            if (HitBox::Collision(projectiles[i].getHitBox(), projectiles[j].getHitBox())) {
                projectiles[i].del = true;
                projectiles[j].del = true;
            }
        }
        
    }
}