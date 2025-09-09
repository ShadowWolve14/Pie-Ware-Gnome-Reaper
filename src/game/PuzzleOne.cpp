//
// Created by $Will on 08.09.2025.
//

#include "PuzzleOne.h"
#include "../Config.h.in"

PuzzleOne::PuzzleOne(Object_Manager& obj_manager) : object_manager(obj_manager) {}

void PuzzleOne::Load(int level_number)
{
    if (loaded) return;

    if (level_number >= 1)
    {
        MovableWall* stone1 = new MovableWall(
            game::Config::movable_wall_1_start_pos,
            game::Config::movable_wall_hitbox,
            game::Config::movable_wall_1_target_pos,
            game::Config::movable_wall_1_spritesheet,
            object_manager // object_manager übergeben
        );
        MovableWall* stone2 = new MovableWall(
            game::Config::movable_wall_2_start_pos,
            game::Config::movable_wall_hitbox,
            game::Config::movable_wall_2_target_pos,
            game::Config::movable_wall_2_spritesheet,
            object_manager // object_manager übergeben
        );
        MovableWall* stone3 = new MovableWall(
            game::Config::movable_wall_3_start_pos,
            game::Config::movable_wall_hitbox,
            game::Config::movable_wall_3_target_pos,
            game::Config::movable_wall_3_spritesheet,
            object_manager // object_manager übergeben
        );
        MovableWall* stone4 = new MovableWall(
            game::Config::movable_wall_4_start_pos,
            game::Config::movable_wall_hitbox,
            game::Config::movable_wall_4_target_pos,
            game::Config::movable_wall_4_spritesheet,
            object_manager // object_manager übergeben
        );
        MovableWall* stone5 = new MovableWall(
            game::Config::movable_wall_5_start_pos,
            game::Config::movable_wall_hitbox,
            game::Config::movable_wall_5_target_pos,
            game::Config::movable_wall_5_spritesheet,
            object_manager // object_manager übergeben
        );

        object_manager.AddObject(stone1); puzzle_stones.push_back(stone1);
        object_manager.AddObject(stone2); puzzle_stones.push_back(stone2);
        object_manager.AddObject(stone3); puzzle_stones.push_back(stone3);
        object_manager.AddObject(stone4); puzzle_stones.push_back(stone4);
        object_manager.AddObject(stone5); puzzle_stones.push_back(stone5);
    }
    loaded = true;
}

void PuzzleOne::Update()
{
    if (is_solved || puzzle_stones.empty()) return;

    int solved_count = 0;
    for (const auto* stone : puzzle_stones) {
        if (stone->IsSolved()) {
            solved_count++;
        }
    }

    if (solved_count == puzzle_stones.size()) {
        is_solved = true;
    }
}
