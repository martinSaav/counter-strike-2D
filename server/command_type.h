//
// Created by matias on 03/06/25.
//

#ifndef COMMAND_TYPE_H
#define COMMAND_TYPE_H

enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Shoot,
    StartMatch,
    ChangeSkin,
    DefuseBomb,
    Reload,
    EquipWeapon,
    PlantBomb,
    Idle,
    Unknown,
    SwitchWeapon,
    LeaveMatch
};

#endif  // COMMAND_TYPE_H
