#pragma once
#include "raylib-cpp/raylib-cpp.hpp"

class SoundManager {
    public:
    static inline Sound attack;
    static inline Sound dead;
    static inline Sound gameOver;
    static inline Sound hit;
    static inline Sound shoot;
    static inline Sound boostShot;
    static inline Sound boostAvailable;
    static inline Sound lifeLost;
    static inline Music bgm;

    static void Load() {
        attack = LoadSound("audio/attack.mp3");
        dead = LoadSound("audio/dead.mp3");
        gameOver = LoadSound("audio/gameOver.mp3");
        hit = LoadSound("audio/hit.mp3");
        shoot = LoadSound("audio/shoot.mp3");
        boostShot = LoadSound("audio/boostShot.mp3");
        boostAvailable = LoadSound("audio/gameBonusRetro.mp3");
        lifeLost = LoadSound("audio/lifeLost.mp3");
        bgm = LoadMusicStream("audio/bgm.mp3");
    }

    static void Unload() {
        UnloadSound(attack);
        UnloadSound(dead);
        UnloadSound(gameOver);
        UnloadSound(hit);
        UnloadSound(shoot);
        UnloadSound(boostShot);
        UnloadSound(boostAvailable);
        UnloadSound(lifeLost);
        UnloadMusicStream(bgm);
    }
};
