#include "Player.hpp"
#include "Program.hpp"

void Player::draw() {
    if (HitBox::drawHitbox) this->hitBox.draw();
    DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                    Rectangle{this->position.first, this->position.second, 30, 30}, 
                    Vector2{0, 0}, 0, WHITE);
}

void Player::update() {
    
    this->hitBox.box.x = this->position.first;
    this->hitBox.box.y = this->position.second;
    this->cooldown--;

    if (HitBox::Collision(Background::sideWalls.first, this->hitBox)) {
        std::pair<double, double> collision = HitBox::CollisionMargins(this->hitBox, Background::sideWalls.first);
        if (abs(collision.second) < abs(collision.first)) {
            this->position.second += collision.second;
        } else {
            this->position.first += collision.first + 1;
        }
    }
    if (HitBox::Collision(Background::sideWalls.second, this->hitBox)) {
        std::pair<double, double> collision = HitBox::CollisionMargins(this->hitBox, Background::sideWalls.second);
        if (abs(collision.second) < abs(collision.first)) {
            this->position.second += collision.second;
        } else {
            this->position.first += collision.first + 1;
        }
    }

    //boost shots (5 consecutive arcs)
    if (boostShotsRemaining > 0) {

    if (boostDelay <= 0) {

        const int n = 10;
        const float spread = 60.0f;

        const int centreX = position.first + hitBox.box.width / 2;
        const int centreY = position.second;

        for (int k = 0; k < n; k++) {

            float angle = 90.0f + spread * ((k / float(n-1)) - 0.5f);//do an arc with spreaded bullets

            Projectile::projectiles.push_back(
                Projectile(centreX, centreY, angle, 0)
            );
        }

        boostShotsRemaining--;
        boostDelay = boostDelayFrames;
    }
    boostDelay--;
    }
}

void Player::keyInputs() {
    float dt = GetFrameTime();
    if (IsKeyDown('A')) this->position.first -= this->speed;
    if (IsKeyDown('D')) this->position.first += this->speed;
    if (IsKeyPressed(KEY_SPACE)) this->attack();
    if(IsKeyDown('B')) this->activateBoost();

}
    



void Player::attack() {
    if (cooldown <= 0) {
        Projectile::projectiles.push_back(Projectile(position.first + this->hitBox.box.width / 2, position.second, 0));
        PlaySound(SoundManager::shoot);
        cooldown = 30;
    }
}
void Player::activateBoost() {
    if (Program::barFull && boostShotsRemaining == 0) {
        PlaySound(SoundManager::boostShot);
        Program::boostActivated = true;
        Program::boostCooldown = 2.5*60;
        boostShotsRemaining = 5;   // total arcs to fire
        boostDelay = 0;            // fire first immediately
    }

}
