#ifndef PLAYERCREDENTIALS_H
#define PLAYERCREDENTIALS_H
#define invalid_credential (-1)
class PlayerCredentials {
    int id;

public:
    explicit PlayerCredentials(const int id): id(id) {}
    bool operator<(const PlayerCredentials& other) const { return id < other.id; }
    [[nodiscard]] int get_id() const { return id; }
};


#endif  // PLAYERCREDENTIALS_H
