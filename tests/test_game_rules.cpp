#include <memory>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "server/game/game_clock.h"
#include "server/game/game_manager.h"
#include "server/game/gun/bomb_encapsulator.h"
#include "server/game/map.h"
#include "server/game/player.h"
#include "server/game_config.h"

namespace {

constexpr int BUY_TIME = 1000;
constexpr int ROUND_TIME = 5000;
constexpr int BOMB_TIME = 3000;
constexpr int AFTER_ROUND_TIME = 1000;
constexpr int NUMBER_OF_ROUNDS = 10;

GunConfig make_gun_config() { return GunConfig{10, 10, 10, 20, 100, 5, 0.4, 100, 1, 0.9, 0.01}; }

GameConfig make_game_config() {
    MapConfig map_config{std::string("test"),
                         100,
                         100,
                         {},
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
                      AFTER_ROUND_TIME,
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
