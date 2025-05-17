#ifndef PLAYERCREDENTIALS_H
#define PLAYERCREDENTIALS_H
#define invalid_credential (-1)
class PlayerCredentials {
public:
    const int id;
    explicit PlayerCredentials(const int id): id(id) {}
};


#endif  // PLAYERCREDENTIALS_H
