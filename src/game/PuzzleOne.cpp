//
// Created by $Will on 08.09.2025.
//

#include "PuzzleOne.h"

#include "Store.h"
#include "../Config.h.in"

PuzzleOne::PuzzleOne(Object_Manager& obj_manager) : object_manager(obj_manager)
{
    stone_solve_sound = LoadSound(game::Config::kSfxPuzzleStoneSolve);
    puzzle_complete_sound = LoadSound(game::Config::kSfxPuzzleCompleteAndFairySpawn);
    SetSoundVolume(stone_solve_sound, game::core::Store::volume);
    SetSoundVolume(puzzle_complete_sound, game::core::Store::volume);
}

PuzzleOne::~PuzzleOne()
{
    UnloadSound(stone_solve_sound);
    UnloadSound(puzzle_complete_sound);
}

void PuzzleOne::Load(int level_number)
{
    if (loaded) return;

    Vector2 start_pos1, start_pos2, start_pos3, start_pos4, start_pos5;

    switch (level_number)
    {
        case 1:
            start_pos1 = game::Config::movable_wall_1_start_pos_L1;
            start_pos2 = game::Config::movable_wall_2_start_pos_L1;
            start_pos3 = game::Config::movable_wall_3_start_pos_L1;
            start_pos4 = game::Config::movable_wall_4_start_pos_L1;
            start_pos5 = game::Config::movable_wall_5_start_pos_L1;
            break;
        case 2:
            start_pos1 = game::Config::movable_wall_1_start_pos_L2;
            start_pos2 = game::Config::movable_wall_2_start_pos_L2;
            start_pos3 = game::Config::movable_wall_3_start_pos_L2;
            start_pos4 = game::Config::movable_wall_4_start_pos_L2;
            start_pos5 = game::Config::movable_wall_5_start_pos_L2;
            break;
        case 3:
            start_pos1 = game::Config::movable_wall_1_start_pos_L3;
            start_pos2 = game::Config::movable_wall_2_start_pos_L3;
            start_pos3 = game::Config::movable_wall_3_start_pos_L3;
            start_pos4 = game::Config::movable_wall_4_start_pos_L3;
            start_pos5 = game::Config::movable_wall_5_start_pos_L3;
            break;
        default:
            start_pos1 = game::Config::movable_wall_1_start_pos_L1;
            start_pos2 = game::Config::movable_wall_2_start_pos_L1;
            start_pos3 = game::Config::movable_wall_3_start_pos_L1;
            start_pos4 = game::Config::movable_wall_4_start_pos_L1;
            start_pos5 = game::Config::movable_wall_5_start_pos_L1;
            break;
    }

    MovableWall* stone1 = new MovableWall(start_pos1, game::Config::movable_wall_hitbox, game::Config::movable_wall_1_target_pos, game::Config::movable_wall_1_spritesheet, object_manager);
    MovableWall* stone2 = new MovableWall(start_pos2, game::Config::movable_wall_hitbox, game::Config::movable_wall_2_target_pos, game::Config::movable_wall_2_spritesheet, object_manager);
    MovableWall* stone3 = new MovableWall(start_pos3, game::Config::movable_wall_hitbox, game::Config::movable_wall_3_target_pos, game::Config::movable_wall_3_spritesheet, object_manager);
    MovableWall* stone4 = new MovableWall(start_pos4, game::Config::movable_wall_hitbox, game::Config::movable_wall_4_target_pos, game::Config::movable_wall_4_spritesheet, object_manager);
    MovableWall* stone5 = new MovableWall(start_pos5, game::Config::movable_wall_hitbox, game::Config::movable_wall_5_target_pos, game::Config::movable_wall_5_spritesheet, object_manager);

    object_manager.AddObject(stone1); puzzle_stones.push_back(stone1);
    object_manager.AddObject(stone2); puzzle_stones.push_back(stone2);
    object_manager.AddObject(stone3); puzzle_stones.push_back(stone3);
    object_manager.AddObject(stone4); puzzle_stones.push_back(stone4);
    object_manager.AddObject(stone5); puzzle_stones.push_back(stone5);

    loaded = true;
}

void PuzzleOne::Update()
{
    if (is_solved || puzzle_stones.empty()) return;

    if (initial_sound_delay_timer > 0.0f)
    {
        initial_sound_delay_timer -= GetFrameTime();
    }

    int current_solved_count = 0;
    for (const auto* stone : puzzle_stones) {
        if (stone->IsSolved()) {
            current_solved_count++;
        }
    }

    if (initial_sound_delay_timer <= 0.0f && current_solved_count > previously_solved_count)
    {
        if (current_solved_count == puzzle_stones.size())
        {
            is_solved = true;
            PlaySound(puzzle_complete_sound);
        }
        else
        {
            PlaySound(stone_solve_sound);
        }
    }
    previously_solved_count = current_solved_count;
}
