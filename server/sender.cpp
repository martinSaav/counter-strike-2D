#include "sender.h"

#include <list>

#include "catedra/liberror.h"
#include "common/dto/game_ready_response.h"
#include "common/dto/game_state_update.h"
#include "dto/game_config_info.h"
#include "dto/map_config_info.h"
#include "dto/structure_info.h"

#include "skin_translator.h"


void Sender::send_status(const MatchStatusDTO& status) const {
    const auto player_dtos = status.players;
    std::list<PlayerInfo> players;
    for (const auto& player_dto: player_dtos) {
        PlayerInfo player(player_dto.username, player_dto.position_x, player_dto.position_y,
                          player_dto.health, player_dto.status, player_dto.money, player_dto.kills,
                          player_dto.deaths, player_dto.last_action, player_dto.aim_x,
                          player_dto.aim_y, SkinTranslator::code_to_string(player_dto.skin),
                          player_dto.primary_weapon, player_dto.primary_weapon_ammo,
                          player_dto.secondary_weapon, player_dto.secondary_weapon_ammo,
                          player_dto.knife, player_dto.bomb, player_dto.current_weapon,
                          player_dto.current_weapon_ammo);
        players.push_back(player);
    }
    const GameStateUpdate state(status.game_started, status.game_ended, status.round,
                                status.round_time, status.round_started, status.round_ended,
                                status.bomb_planted, status.bomb_x, status.bomb_y,
                                status.bomb_timer, status.round_winner, status.game_winner,
                                std::move(players), status.dropped_weapons);
    protocol.send_message(state);
}

void Sender::send_game_config() const {
    std::vector<StructureInfo> structures_info;
    const MapConfig& map_cfg = config.map_config;
    const BombSite& bomb_site_cfg = map_cfg.bombsite;
    const Site& ct_site = map_cfg.ct_site;
    const Site& tt_site = map_cfg.tt_site;
    for (auto& structure: map_cfg.structures) {
        auto [x, y] = structure.get_position();
        structures_info.emplace_back(structure.height, structure.width, x, y);
    }
    std::vector<std::pair<int32_t, int32_t>> ct_spawns;
    std::vector<std::pair<int32_t, int32_t>> tt_spawns;
    ct_spawns.reserve(ct_site.spawns.size());
    for (const auto& [x, y]: ct_site.spawns) {
        ct_spawns.emplace_back(x, y);
    }
    tt_spawns.reserve(tt_site.spawns.size());
    for (const auto& [x, y]: tt_site.spawns) {
        tt_spawns.emplace_back(x, y);
    }
    MapConfigInfo map_config(
            config.map_config.map_name, config.map_config.map_width, config.map_config.map_height,
            std::move(structures_info),
            BombSiteInfo{bomb_site_cfg.bomb_site_height, bomb_site_cfg.bomb_site_width,
                         bomb_site_cfg.x, bomb_site_cfg.y},
            SiteInfo{ct_site.x, ct_site.y, ct_site.site_width, ct_site.site_height,
                     std::move(ct_spawns)},
            SiteInfo{tt_site.x, tt_site.y, tt_site.site_width, tt_site.site_height,
                     std::move(tt_spawns)});
    GameConfigInfo game_cfg_message(
            config.player_health, config.number_of_rounds, config.starting_money, config.ct_amount,
            config.tt_amount, config.ammo_price, config.knife_config.get_info(),
            config.glock_config.get_info(), config.ak_config.get_info(),
            config.awp_config.get_info(), config.m3_config.get_info(), config.defuse_time,
            config.time_to_plant, config.bomb_dmg, config.round_winner_money,
            config.round_loser_money, config.buy_time, config.bomb_time, config.after_round_time,
            config.money_per_kill, config.tiles_per_movement, config.game_rate,
            std::move(map_config));
    protocol.send_message(game_cfg_message);
}


void Sender::run() {
    while (should_keep_running()) {
        try {
            if (auto status_v = sender_queue->pop();
                std::holds_alternative<MatchStatusDTO>(status_v)) {
                send_status(std::get<MatchStatusDTO>(status_v));
            } else {
                protocol.send_message(GameReadyResponse(true));
                send_game_config();
            }
        } catch (const ClosedQueue&) {
            running = false;
            return;
        } catch (const LibError&) {
            break;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception: " << e.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";
            break;
        }
    }
    running = false;
}


bool Sender::is_running() { return running; }


void Sender::stop() {
    Thread::stop();
    try {
        sender_queue->close();
    } catch (const ClosedQueue&) {}
    running = false;
}
