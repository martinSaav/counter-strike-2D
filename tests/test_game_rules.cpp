#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "server/game/game_clock.h"
#include "server/game/game_manager.h"
#include "server/game/gun/bomb_encapsulator.h"
#include "server/game/impact_info.h"
#include "server/game/map.h"
#include "server/game/player.h"
#include "server/game_config.h"
#include "server/gun_shop.h"

namespace {

constexpr int BUY_TIME = 1000;
constexpr int ROUND_TIME = 5000;
constexpr int BOMB_TIME = 3000;
constexpr int AFTER_ROUND_TIME = 1000;
constexpr int NUMBER_OF_ROUNDS = 10;

GunConfig make_gun_config() { return GunConfig{10, 10, 10, 20, 100, 5, 0.4, 100, 1, 0.9, 0.01}; }

GameConfig make_game_config(const int after_round_time = AFTER_ROUND_TIME,
                            const std::vector<Structure>& structures = {}) {
    MapConfig map_config{std::string("test"),
                         100,
                         100,
                         structures,
                         {BombSite{10, 10, 40, 40}},
                         Site{80, 80, 10, 10, {{82, 82}}},
                         Site{0, 0, 10, 10, {{2, 2}}},
                         {}};
    return GameConfig{100,
                      NUMBER_OF_ROUNDS,
                      800,
                      5,
                      5,
                      200,
                      make_gun_config(),
                      make_gun_config(),
                      make_gun_config(),
                      make_gun_config(),
                      make_gun_config(),
                      2500,
                      2500,
                      200,
                      600,
                      200,
                      BUY_TIME,
                      BOMB_TIME,
                      after_round_time,
                      600,
                      2,
                      30,
                      std::move(map_config),
                      70,
                      150,
                      ROUND_TIME};
}

class GameRulesTest: public ::testing::Test {
protected:
    GameConfig config = make_game_config();
    GameClock clock{config};
    Map map{config};
    GameManager manager{map, clock, config};
    std::vector<std::shared_ptr<Player>> players;

    void SetUp() override {
        players.push_back(std::make_shared<Player>("tt", config, 2, 2, Team::Terrorists));
        players.push_back(std::make_shared<Player>("ct", config, 82, 82, Team::CounterTerrorists));
    }
};

}  // namespace

TEST_F(GameRulesTest, ClockCountsDownEachStage) {
    EXPECT_DOUBLE_EQ(clock.get_stage_time_left(), BUY_TIME);
    clock.advance(400);
    EXPECT_DOUBLE_EQ(clock.get_stage_time_left(), BUY_TIME - 400);

    clock.advance(600);  // termina la compra
    EXPECT_EQ(clock.getCurrentStage(), Stages::PrePlantStage);
    EXPECT_DOUBLE_EQ(clock.get_stage_time_left(), ROUND_TIME);
}

TEST_F(GameRulesTest, RoundTimeExpiresAfterRoundTime) {
    clock.advance(BUY_TIME);
    clock.advance(ROUND_TIME - 1);
    EXPECT_FALSE(clock.has_round_time_expired());
    clock.advance(1);
    EXPECT_TRUE(clock.has_round_time_expired());
}

TEST_F(GameRulesTest, CounterTerroristsWinWhenRoundTimeRunsOut) {
    clock.advance(BUY_TIME);
    clock.advance(ROUND_TIME);
    manager.check_winning_cond(players);

    const TimeInformation info = manager.get_time_information();
    EXPECT_TRUE(info.round_ended);
    EXPECT_EQ(info.round_winner, Team::CounterTerrorists);
    EXPECT_EQ(info.stage, RoundStage::AfterRound);
}

TEST_F(GameRulesTest, SidesSwitchAtTheStartOfTheSecondHalf) {
    EXPECT_EQ(manager.get_halftime_round(), NUMBER_OF_ROUNDS / 2 + 1);
    for (int round = 1; round < NUMBER_OF_ROUNDS / 2 + 1; round++) {
        EXPECT_FALSE(manager.has_to_switch_sides()) << "round " << round;
        manager.advance_round(players);
    }
    EXPECT_TRUE(manager.has_to_switch_sides());
}

TEST_F(GameRulesTest, MatchEndsWhenATeamWinsMoreThanHalfTheRounds) {
    EXPECT_EQ(manager.get_rounds_to_win(), NUMBER_OF_ROUNDS / 2 + 1);
    for (int round = 0; round < manager.get_rounds_to_win(); round++) {
        EXPECT_FALSE(manager.has_ended());
        clock.reset();
        clock.advance(BUY_TIME);
        clock.advance(ROUND_TIME);
        manager.check_winning_cond(players);
        manager.advance_round(players);
    }
    EXPECT_TRUE(manager.has_ended());
}

TEST_F(GameRulesTest, BombCountsDownFromBombTimeAfterPlanting) {
    clock.advance(BUY_TIME);
    clock.advance(1000);
    clock.set_post_plant_stage();
    clock.advance(1000);
    EXPECT_NEAR(clock.get_stage_time_left(), BOMB_TIME - 1000, 1);
    EXPECT_FALSE(clock.has_bomb_exploded());
}

TEST_F(GameRulesTest, DroppedBombCanBePickedUpAgain) {
    auto& terrorist = players[0];
    terrorist->equip_bomb(std::make_unique<BombEncapsulator>(config));
    terrorist->switch_weapon(GunType::Bomb);

    terrorist->drop_equipped_weapon(manager);
    EXPECT_EQ(terrorist->get_player_info().bomb, Weapon::None);

    manager.pick_weapon(terrorist);
    EXPECT_EQ(terrorist->get_player_info().bomb, Weapon::Bomb);
}

TEST_F(GameRulesTest, PlantedBombLastsBombTimeWithRealTicks) {
    const double tick = 1000.0 / 30;
    clock.advance(BUY_TIME);
    for (int i = 0; i < 30; i++) {
        clock.advance(tick);
        manager.check_winning_cond(players);
    }
    manager.plant_bomb(50, 50);
    int ticks = 0;
    while (!manager.get_time_information().round_ended && ticks < 1000) {
        clock.advance(tick);
        manager.check_winning_cond(players);
        ticks++;
    }
    EXPECT_NEAR(ticks * tick, BOMB_TIME, 2 * tick);
}

TEST_F(GameRulesTest, HoldingDefuseFinishesAfterDefuseTime) {
    // El cliente manda un pedido de desactivar por frame mientras se mantiene F
    const double tick = 1000.0 / 30;
    auto& counter_terrorist = players[1];
    clock.advance(BUY_TIME);
    auto [x, y] = counter_terrorist->get_location();
    manager.plant_bomb(x, y);
    int ticks = 0;
    while (!manager.get_time_information().round_ended && ticks < 300) {
        manager.start_defusing(counter_terrorist);
        clock.advance(tick);
        manager.has_finished_defusing(*counter_terrorist);
        manager.check_winning_cond(players);
        ticks++;
    }
    const TimeInformation info = manager.get_time_information();
    EXPECT_TRUE(info.round_ended);
    EXPECT_EQ(info.round_winner, Team::CounterTerrorists);
    EXPECT_NEAR(ticks * tick, 2500, 2 * tick);
}

TEST_F(GameRulesTest, HoldingClickPlantsAfterTimeToPlant) {
    // Mantener el click con la bomba equipada manda un disparo por frame
    const double tick = 1000.0 / 30;
    auto terrorist = std::make_shared<Player>("planter", config, 42, 42, Team::Terrorists);
    terrorist->equip_bomb(std::make_unique<BombEncapsulator>(config));
    terrorist->switch_weapon(GunType::Bomb);
    clock.advance(BUY_TIME);
    int ticks = 0;
    while (!manager.get_time_information().bomb_planted && ticks < 300) {
        terrorist->shoot(Position(42, 42), static_cast<float>(clock.get_time()));
        terrorist->update(manager);
        clock.advance(tick);
        ticks++;
    }
    EXPECT_TRUE(manager.get_time_information().bomb_planted);
    EXPECT_NEAR(ticks * tick, 2500, 3 * tick);
}

TEST_F(GameRulesTest, CannotPlantOutsideABombSite) {
    // La zona de bomba del mapa de prueba va de (40, 40) a (50, 50)
    const double tick = 1000.0 / 30;
    auto terrorist = std::make_shared<Player>("planter", config, 10, 10, Team::Terrorists);
    terrorist->equip_bomb(std::make_unique<BombEncapsulator>(config));
    terrorist->switch_weapon(GunType::Bomb);
    clock.advance(BUY_TIME);
    for (int i = 0; i < 150; i++) {
        terrorist->shoot(Position(10, 10), static_cast<float>(clock.get_time()));
        terrorist->update(manager);
        clock.advance(tick);
    }
    EXPECT_FALSE(manager.get_time_information().bomb_planted);
    EXPECT_EQ(terrorist->get_player_info().bomb, Weapon::Bomb);
}

namespace {

int money_of(const std::shared_ptr<Player>& player) { return player->get_player_info().money; }

// Agrega al mapa un jugador en (x, y), con sus chunks calculados como al hacer spawn
std::shared_ptr<Player> place_player(Map& map, GameConfig& config, const std::string& name,
                                     const int x, const int y, const Team team) {
    auto player = std::make_shared<Player>(name, config, x, y, team);
    player->set_location(Position(x, y), map.calculate_player_chunks(x, y));
    map.add_player(player);
    return player;
}

}  // namespace

TEST_F(GameRulesTest, VerticalShotHitsAPlayerInTheSameColumn) {
    // Antes la componente x de la velocidad era 0 y la colision daba NaN: la bala lo atravesaba
    auto shooter = place_player(map, config, "shooter", 20, 20, Team::Terrorists);
    place_player(map, config, "target", 20, 60, Team::CounterTerrorists);
    const ImpactInfo impact = map.trace_bullet_path(20, 20, Position(20, 90), *shooter);
    ASSERT_TRUE(impact.impacted_player.has_value());
    EXPECT_EQ(impact.impacted_player.value()->get_username(), "target");
}

TEST_F(GameRulesTest, HorizontalShotHitsAPlayerInTheSameRow) {
    auto shooter = place_player(map, config, "shooter", 20, 20, Team::Terrorists);
    place_player(map, config, "target", 60, 20, Team::CounterTerrorists);
    const ImpactInfo impact = map.trace_bullet_path(20, 20, Position(90, 20), *shooter);
    ASSERT_TRUE(impact.impacted_player.has_value());
    EXPECT_EQ(impact.impacted_player.value()->get_username(), "target");
    EXPECT_EQ(impact.impact_position, std::make_pair(60, 20));
}

TEST_F(GameRulesTest, OverlappingStructuresAreAllKept) {
    // Dos paredes que se superponen: antes la segunda se descartaba en silencio
    GameConfig walls_config = make_game_config(
            AFTER_ROUND_TIME,
            {Structure(10, 10, Position(50, 50)), Structure(10, 10, Position(55, 55))});
    Map walls_map{walls_config};
    auto shooter = place_player(walls_map, walls_config, "shooter", 60, 95, Team::Terrorists);
    // La bala baja por x = 60 y tiene que frenar en el borde de la segunda pared (y = 65)
    const ImpactInfo impact = walls_map.trace_bullet_path(60, 95, Position(60, 0), *shooter);
    EXPECT_FALSE(impact.impacted_player.has_value());
    EXPECT_EQ(impact.impact_position, std::make_pair(60, 65));
}

TEST_F(GameRulesTest, HalftimeRoundPaysTheWinnerTheWinnerMoney) {
    for (int round = 1; round < manager.get_halftime_round(); round++) {
        manager.advance_round(players);
    }
    clock.reset();
    clock.advance(BUY_TIME);
    clock.advance(ROUND_TIME);
    manager.check_winning_cond(players);  // se termina el tiempo: gana CT
    const int terrorist_money = money_of(players[0]);
    const int counter_terrorist_money = money_of(players[1]);
    manager.advance_round(players);
    EXPECT_EQ(money_of(players[1]) - counter_terrorist_money, config.round_winner_money);
    EXPECT_EQ(money_of(players[0]) - terrorist_money, config.round_loser_money);
}

TEST_F(GameRulesTest, TiedMatchHasNoWinner) { EXPECT_EQ(manager.get_match_winner(), Team::None); }

TEST_F(GameRulesTest, HoldingDefuseAfterDefusingLetsTheRoundEnd) {
    // Con un fin de ronda mas largo que el desactivado, volver a desactivar reiniciaba el reloj
    GameConfig long_config = make_game_config(2 * 2500 + 1000);
    GameClock long_clock{long_config};
    Map long_map{long_config};
    GameManager long_manager{long_map, long_clock, long_config};
    auto counter_terrorist =
            place_player(long_map, long_config, "ct", 30, 30, Team::CounterTerrorists);
    const double tick = 1000.0 / 30;
    long_clock.advance(BUY_TIME);
    long_manager.plant_bomb(30, 30);
    bool round_finished = false;
    for (int ticks = 0; ticks < 1000 && !round_finished; ticks++) {
        long_manager.start_defusing(counter_terrorist);
        try {
            long_clock.advance(tick);
        } catch (const RoundAlreadyFinished&) {
            round_finished = true;
        }
        long_manager.has_finished_defusing(*counter_terrorist);
        long_manager.check_winning_cond({counter_terrorist});
    }
    EXPECT_TRUE(round_finished);
}

TEST_F(GameRulesTest, CannotFinishPlantingAfterTheRoundEnded) {
    GameConfig long_config = make_game_config(10000);
    GameClock long_clock{long_config};
    Map long_map{long_config};
    GameManager long_manager{long_map, long_clock, long_config};
    auto terrorist = place_player(long_map, long_config, "planter", 42, 42, Team::Terrorists);
    auto counter_terrorist =
            place_player(long_map, long_config, "ct", 82, 82, Team::CounterTerrorists);
    const std::vector<std::shared_ptr<Player>> everyone = {terrorist, counter_terrorist};
    terrorist->equip_bomb(std::make_unique<BombEncapsulator>(long_config));
    terrorist->switch_weapon(GunType::Bomb);
    const double tick = 1000.0 / 30;
    long_clock.advance(BUY_TIME);
    long_clock.advance(ROUND_TIME - 500);  // empieza a plantar medio segundo antes del final
    for (int i = 0; i < 150; i++) {
        terrorist->shoot(Position(42, 42), static_cast<float>(long_clock.get_time()));
        terrorist->update(long_manager);
        long_clock.advance(tick);
        long_manager.check_winning_cond(everyone);
    }
    const TimeInformation info = long_manager.get_time_information();
    EXPECT_FALSE(info.bomb_planted);
    EXPECT_EQ(info.round_winner, Team::CounterTerrorists);
    EXPECT_EQ(info.stage, RoundStage::AfterRound);
}

TEST_F(GameRulesTest, DyingWithTheKnifeDropsThePrimary) {
    auto& terrorist = players[0];
    terrorist->equip_weapon(GunShop::get_gun(Weapon::AK47, config));
    terrorist->switch_weapon(GunType::Knife);
    terrorist->receive_damage(manager, config.player_health);
    ASSERT_TRUE(terrorist->is_dead());
    bool ak_on_the_floor = false;
    for (const auto& dropped: map.get_dropped_weapons()) {
        ak_on_the_floor = ak_on_the_floor || dropped.get_weapon() == Weapon::AK47;
    }
    EXPECT_TRUE(ak_on_the_floor);
}

TEST_F(GameRulesTest, PickingUpAPrimaryNeverDestroysTheCurrentOne) {
    auto& terrorist = players[0];
    terrorist->equip_weapon(GunShop::get_gun(Weapon::AK47, config));
    map.drop_weapon(GunShop::get_gun(Weapon::AWP, config), 2, 2);
    manager.pick_weapon(terrorist);
    // Sigue con su AK y el AWP queda en el piso hasta que tire el AK
    EXPECT_EQ(terrorist->get_primary_weapon_info().weapon_type, Weapon::AK47);
    EXPECT_EQ(map.get_dropped_weapons().size(), 1u);
}
