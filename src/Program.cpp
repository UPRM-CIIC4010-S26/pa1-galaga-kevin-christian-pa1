#include "Program.hpp"

bool Program::barFull = false;
bool Program::boostActivated = false;
bool Program::playerHit = false;
int Program::boostValue = 0;
int Program::boostMax = 2000;
int Program::blinkTimer = 0;
int Program::scoreAfterBoost = 0;
int Program::boostCooldown = 0;


Program::Program() {
    Background::sideWalls = std::pair<HitBox, HitBox>{ 
        HitBox(0, 0, 10, GetScreenHeight()), 
        HitBox(GetScreenWidth() - 10, 0, 10, GetScreenHeight())
    };

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{350, 150}, 
            new SpEnemy(350, 150)
        });

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{600, 150}, 
            new SpEnemy(600, 150)
        });

    for (int i = 0; i < 30; i++) {
        
        float x = 250 + 50 * (i%10);
        float y = 200 + 50 * (i/10);

        
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });
    }
}

void Program::Update() {

    // Start background music

    if (!bgmStarted)
    {
        PlayMusicStream(SoundManager::bgm);
        SoundManager::bgm.looping = true;
        SetMusicVolume(SoundManager::bgm, 0.5f);
        bgmStarted = true;
    }

    // Stop, pause, or resume music based on game state

    if (paused || startup)
    {
        PauseMusicStream(SoundManager::bgm);
    }
    else if (!paused && !gameOver && !startup)
    {
        ResumeMusicStream(SoundManager::bgm);
    }

    if (gameOver)
    {
        StopMusicStream(SoundManager::bgm);
        bgmStarted = false;
    }

    UpdateMusicStream(SoundManager::bgm);
    
    for (Animation& a : Animation::animations) a.update();
    for (int i = 0; i < Animation::animations.size(); i++) {
        if (Animation::animations[i].done) Animation::animations.erase(Animation::animations.begin() + i);
    }
    pauseFrames = std::max(pauseFrames - 1, 0);

    if (!startup && !paused && !gameOver && pauseFrames <= 0) {

        // Add points gained to the total score

        score += Enemy::ManageEnemies(player->hitBox);
        StdEnemy::attackReset();
        ManageEnemyRespawns();
        player->update();

        for (std::pair<std::pair<float, float>, Enemy*> p : Enemy::enemies) {
            if (p.second && HitBox::Collision(player->hitBox, p.second->hitBox)) {
                Animation::animations.push_back(
                    Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
                );

                PlaySound(SoundManager::gameOver);
                Projectile::projectiles.clear();
                playerHit = true;
                player->position.first = GetScreenWidth() / 2 - 15;
                p.second->health = 0;
                pauseFrames = 120;
                lives--;
                score/=1.5;
                PlaySound(SoundManager::lifeLost);
            }
        }

        for (Projectile& p : Projectile::projectiles) { 
            p.update(); 

            if (p.ID != 0 && HitBox::Collision(player->hitBox, p.getHitBox()))
            {
                PlayerReset();
            }

        }
        
        int lifeThreshold = score / 1000;
        if(lifeThreshold > lastLifeScore){
            lastLifeScore = lifeThreshold;
            if(lives < 5){
                lives++;
            }
        }
        //boost Bonus implementation

    if (boostCooldown > 0) {
        --boostCooldown;
        // count down every frame
        scoreAfterBoost = score; 
        boostValue = 0;
    }

    else if((score - scoreAfterBoost)/100 != boostValue){
            if(boostValue < boostMax/100 && !barFull){
                //Increase bar (capped at 2k points)
                boostValue++;
                if(boostValue == boostMax/100){
                    barFull = true;
                    boostValue = boostMax/100;
                    blinkTimer = 3*60;
                    blinkPhase = 0;
                    PlaySound(SoundManager::boostAvailable);
                }
            }
        }
        //clear bar and do not count towards bonus the bullets shot at boost
        if ( playerHit || (boostActivated 
                && player->getBoostShotsRemaining() == 0))
        {
            barFull = false;
            blinkTimer = 0;
            boostActivated = false;                  
            playerHit = false;
            boostValue = 0;
        }

        //gameOver if score negative
        if(score < 0){
            gameOver = true;
        }
        
        //Blinking
        if (blinkTimer > 0) {
            blinkTimer--;
            if (blinkTimer % 15 == 0)   
                blinkPhase ^= 1;//toggle
        }


        if (lives <= 0 && pauseFrames <= 0) gameOver = true;
        Projectile::CleanProjectiles();
        Projectile::ProjectileCollision();
    }
}

void Program::Draw() {
    background.Draw();

    std::string scoreText = "Score " + std::to_string(score);
    DrawText(scoreText.c_str(), 10, 10, 24, WHITE);

    if (pauseFrames <= 0 && !gameOver) player->draw();
    for (Animation& a : Animation::animations) a.draw();

    for (int i = 0; i < lives; i++) {
        DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                   Rectangle{10.0f + i * 30, GetScreenHeight() - 30.0f, 20, 20}, 
                    Vector2{0, 0}, 0, WHITE);
    }

    //Draw bar background
    float width = 200.0f;
    DrawRectangle(10, 40, width, 20, GRAY);
    //Update bar filling
    float filling = width * ((float)boostValue / ((float)boostMax/100.0f));
    //Draw only if blink state visible or bar not full
    if (!barFull || blinkPhase == 0)
        DrawRectangle(10, 40, filling, 20, YELLOW);
    
    //Indicate bar is full
    if (barFull) {
        Color textCol = (blinkPhase == 0 ? YELLOW : GRAY);
        DrawText("AVAILABLE", 10 + width + 5, 40, 20, textCol);
        DrawText("Press B", 30 + 20, 40, 20, GRAY);
    }

    for (Projectile p : Projectile::projectiles) p.draw();
    for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) if (p.second) p.second->draw();

    if (startup) DrawStartup();
    if (paused) DrawPauseScreen();
    if (gameOver) DrawGameOver();
}

void Program::ManageEnemyRespawns() {
    delay = std::max(delay - 1, 0);

    respawnCooldown -= 1;
    if (respawnCooldown <= 0) {
        respawnCooldown = 1080/speedFactor;
        for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) {
            if (!p.second && p.first.second != 150) {
                int eType = GetRandomValue(1, 3);

                if (eType == 1) {
                    p.second = new StEnemy(GetScreenWidth() / 2 - 15, 0, true);
                    respawnCooldown /= 2;
                } else {
                    p.second = new StdEnemy(GetScreenWidth() / 2 - 15, 0, true);
                }

                respawns++;
                break;
            } else if (!p.second && p.first.second == 150) {
                p.second = new SpEnemy(GetScreenWidth() / 2 - 15, 0, true);
                respawns++;
                break;
            }
        }
    }

    if(respawns >= 4) {
        count = 4;
        respawns = 0;
    }

    if (count > 0 && delay <= 0) {
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{0, 0}, 
            new DyEnemy(GetScreenWidth(), 300)
        });

        count--;
        delay = 20;
    }
    
    if ((score/1000) != lastRespawnScore){
        lastRespawnScore++;
        speedFactor+=2;
    }
}

void Program::DrawStartup() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Galaga", (GetScreenWidth() / 2 - 237), 75, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawPauseScreen() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Paused", (GetScreenWidth() / 2) - 85, GetScreenHeight() / 2 - 60, 48, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawGameOver() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Game Over", (GetScreenWidth() / 2) - 380, 50, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::KeyInputs() {
    if ((!gameOver && !startup && IsKeyPressed('P')) || (paused && IsKeyPressed(KEY_ENTER))) paused = !paused;
    if (!paused && !startup && IsKeyPressed('O')) gameOver = !gameOver;
    if (!gameOver && !paused && IsKeyPressed('I')) startup = !startup;
    if (IsKeyPressed('H')) HitBox::drawHitbox = !HitBox::drawHitbox;
    if (IsKeyPressed('K')) score += 500;
    
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        Reset();
    }

    if (startup && IsKeyPressed(KEY_ENTER)) {
        startup = false;
    }

    if (!startup && !paused && !gameOver && pauseFrames <= 0) player->keyInputs();
}

void Program::PlayerReset() {
    Animation::animations.push_back(
        Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
    );

    PlaySound(SoundManager::gameOver);
    Projectile::projectiles.clear();
    player->position.first = GetScreenWidth() / 2 - 15;
    pauseFrames = 120;
    lives--;
    playerHit = true;
    score/=1.5;
    PlaySound(SoundManager::lifeLost);
}

void Program::Reset() {

    // Stops music

    StopMusicStream(SoundManager::bgm);

    Enemy::enemies.clear();
    StdEnemy::attackInProgress = false;
    player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    respawnCooldown = 1080;
    respawns = 0;
    count = 0;
    delay = 0;
    lives = 3;
    score = 0;
    Program();

    // Reset Music

    bgmStarted = false;
}