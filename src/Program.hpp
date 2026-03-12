#pragma once

#include <iostream>
#include <string>
#include "raylib-cpp/raylib-cpp.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "StdEnemy.hpp"
#include "SpEnemy.hpp"
#include "StEnemy.hpp"
#include "DyEnemy.hpp"
class Program {
    private:
        Background background = Background();
        Player* player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
        int respawnCooldown = 1080;
        int respawns = 0;
        int count = 0;
        int delay = 0;
        int lives = 3;
        int pauseFrames = 0;
        int score = 0;

        bool startup = true;
        bool paused = false;
        bool gameOver = false;

        //this variable will store score in terms of thousands ( 1 for 1k, 2 for 2k, and so on)
        int lastLifeScore = 0;
        //this will store respawn cooldown score (in terms of 1000... 1 for 1000, 2 for 2000...)
        int lastRespawnScore = 0;
        //this will control respawning speed by dividing base value
        float speedFactor = 1;
        //this variable controls the boost bar
        int blinkPhase = 0;
        

        

        

    public:
        static bool barFull;
        static int boostValue;
        static int boostMax;
        static int blinkTimer;
        static bool boostActivated;
        static int scoreAfterBoost;
        static bool playerHit;
        //Cooldown for boost
        static int boostCooldown;

        Program();

        void Update();
        void Draw();
        void ManageEnemyRespawns();
        void DrawStartup();
        void DrawPauseScreen();
        void DrawGameOver();
        void KeyInputs();
        void PlayerReset();
        void Reset();

        ~Program() {}
};