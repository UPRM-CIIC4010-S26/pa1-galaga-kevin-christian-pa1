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
}

void Player::keyInputs() {
    float dt = GetFrameTime();
    if (IsKeyDown('A')) this->position.first -= this->speed;
    if (IsKeyDown('D')) this->position.first += this->speed;
    if (IsKeyDown(KEY_SPACE)) this->attack();
    if(IsKeyDown('B')) this->activateBoost();

}
    



void Player::attack() {
    if (cooldown <= 0) {
        Projectile::projectiles.push_back(Projectile(position.first + + this->hitBox.box.width / 2, position.second, 0));
        PlaySound(SoundManager::shoot);
        cooldown = 30;
    }
}
void Player::activateBoost() {
    if (cooldown <= 0) {
        for(int i = 0; i < 10; i++){
            Projectile::projectiles.push_back(
                Projectile(position.first + this->hitBox.box.width/2,
                        position.second,
                        0)
            );
            PlaySound(SoundManager::shoot);
            i++;
            cooldown = 5;   
        }
        cooldown = 0;

    }
}