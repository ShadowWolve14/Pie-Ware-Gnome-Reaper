//
// Created by $Will on 08.09.2025.
//

#ifndef PUZZLEONE_H
#define PUZZLEONE_H

#include <vector>
#include "Object_Manager.h"
#include "MovableWall.h"

class PuzzleOne
{
private:
    Object_Manager& object_manager;
    std::vector<MovableWall*> puzzle_stones;
    bool is_solved = false;
    bool loaded = false;

public:
    PuzzleOne(Object_Manager& obj_manager);
    void Load(int level_number);
    void Update();
    bool IsSolved() const { return is_solved; }
};

#endif //PUZZLEONE_H
