#include "lobby.h"
GameIdentification Lobby::create_match(const std::string& match_name,
                                       const std::string& player_name,
                                       const std::string& map_name) {
    std::lock_guard<std::mutex> guard(mutex);
    if (const auto existing = matches.find(match_name); existing != matches.end()) {
        // Una partida que ya termino solo espera que el acceptor la limpie: su nombre se puede
        // volver a usar
        if (!existing->second->has_match_finished()) {
            throw MatchAlreadyExists();
        }
        existing->second->join();
        matches.erase(existing);
    }

    matches[match_name] = std::make_unique<Match>(game_config, map_name);
    const GameIdentification game_id = matches[match_name]->join_match(player_name);
    matches[match_name]->start();
    return game_id;
}

GameIdentification Lobby::join_match(const std::string& match_name,
                                     const std::string& player_name) {
    std::lock_guard<std::mutex> guard(mutex);
    const auto match = matches.find(match_name);
    if (match == matches.end()) {
        throw MatchNotFound();
    }

    return match->second->join_match(player_name);
}

std::vector<MatchDTO> Lobby::list_matches() {
    std::lock_guard<std::mutex> guard(mutex);
    std::vector<MatchDTO> match_dtos;
    for (auto& [name, match]: matches) {
        if (match->has_match_started() || match->has_match_finished()) {
            // No tiene sentido ofrecer partidas a las que ya no se puede entrar. Las terminadas
            // siguen en el mapa hasta que el acceptor las limpia al aceptar otra conexion.
            continue;
        }
        MatchDTO match_dto(name, match->get_map_name(), match->get_player_count(),
                           match->get_max_player_count());
        match_dtos.push_back(match_dto);
    }
    return match_dtos;
}


void Lobby::remove_finished_matches() {
    std::lock_guard<std::mutex> guard(mutex);
    for (auto it = matches.begin(); it != matches.end();) {
        if (it->second->has_match_finished()) {
            it->second->join();
            it = matches.erase(it);
        } else {
            ++it;
        }
    }
}


void Lobby::kill_all_matches() {
    std::lock_guard<std::mutex> guard(mutex);
    for (auto it = matches.begin(); it != matches.end();) {
        if (it->second->is_alive()) {
            it->second->stop();
        }
        it->second->join();
        ++it;
    }
}
