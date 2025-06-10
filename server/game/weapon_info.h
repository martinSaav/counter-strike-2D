//
// Created by matias on 10/06/25.
//

#ifndef WEAPONINFO_H
#define WEAPONINFO_H
#include "weapon.h"


class WeaponInfo {
public:
    const Weapon weapon_type;
    const int ammo;
    WeaponInfo(const Weapon weapon_type, const int ammo): weapon_type(weapon_type), ammo(ammo) {}
};


#endif  // WEAPONINFO_H
