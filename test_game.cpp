#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Role.hpp"

TEST_CASE("Player Initialization") {
    Game g;
    Player p("Alice", Role::Governor, &g);
    CHECK(p.getName() == "Alice");
    CHECK(p.getCoins() == 0);
    CHECK(p.isAlive());
    CHECK(p.getRole() == Role::Governor);
}

TEST_CASE("Gather and Coins") {
    Game g;
    Player p1("Bob", Role::Governor, &g);
    Player p2("Dan", Role::Merchant, &g);

    p1.gather();
    CHECK(p1.getCoins() == 1);
    p2.gather();
    CHECK(p2.getCoins() == 1);
}

TEST_CASE("Governor Tax") {
    Game g;
    Player gov("Gov", Role::Governor, &g);
    Player mer("Mer", Role::Merchant, &g);

    gov.tax();             // Gov's turn
    mer.gather();          // Mer's turn
    gov.gather();          // Gov again
    mer.gather();          // Mer again

    CHECK(gov.getCoins() == 4); // 3 (tax) + 1 (gather)
    CHECK(mer.getCoins() == 2); // 1 + 1 from two gathers
}


TEST_CASE("Baron Investment") {
    Game g;
    Player baron("Baron", Role::Baron, &g);
    Player other("Other", Role::Merchant, &g);

    baron.gather(); other.gather();
    baron.gather(); other.gather();
    baron.gather(); other.gather();
    baron.invest();

    CHECK(baron.getCoins() == 6);
}

TEST_CASE("Basic Coup") {
    Game g;
    Player attacker("Attacker", Role::Governor, &g);
    Player target("Target", Role::Spy, &g);

    for (int i = 0; i < 7; ++i) {
        attacker.gather();
        target.gather();
    }

    attacker.coup(target);
    CHECK_FALSE(target.isAlive());
}

TEST_CASE("Judge Cancels Bribe") {
    Game g;
    Player briber("Briber", Role::Baron, &g);
    Player judge("Judge", Role::Judge, &g);

    for (int i = 0; i < 4; ++i) {
        briber.gather();
        judge.gather();
    }

    briber.bribe();
    judge.judgeBribe(briber);

    CHECK(briber.getCoins() == 0);
}

TEST_CASE("Spy Action") {
    Game g;
    Player spy("Spy", Role::Spy, &g);
    Player target("Target", Role::Governor, &g);

    spy.spyOn(target);
    CHECK(target.getCoins() == 0);
}

TEST_CASE("Game Winner") {
    Game g;
    Player p1("Alice", Role::Governor, &g);
    Player p2("Bob", Role::Merchant, &g);

    // Alice eliminates Bob
    for (int i = 0; i < 7; ++i) { p1.gather(); p2.gather(); }
    p1.coup(p2);

    CHECK(g.winner() == "Alice");
}

TEST_CASE("Attempting illegal actions") {
    Game g;
    Player p1("Alice", Role::Governor, &g);
    Player p2("Bob", Role::Merchant, &g);

    // Not enough coins
    CHECK_THROWS(p1.coup(p2));

    // Acting out of turn
    CHECK_THROWS(p2.gather()); // it's Alice's turn

    // Trying self-coup
    for (int i = 0; i < 7; ++i) { p1.gather(); p2.gather(); }
    CHECK_THROWS(p1.coup(p1));
}
///