#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Role.hpp"

/**
 * @brief Tests player initialization: name, role, coins, and alive status.
 */
TEST_CASE("Player Initialization") {
    Game g;
    Player p("Alice", Role::Governor, &g);
    CHECK(p.getName() == "Alice");
    CHECK(p.getCoins() == 0);
    CHECK(p.isAlive());
    CHECK(p.getRole() == Role::Governor);
}

/**
 * @brief Tests that the gather action adds a coin to the player.
 */
TEST_CASE("Gather and Coins") {
    Game g;
    Player p1("Bob", Role::Governor, &g);
    Player p2("Dan", Role::Merchant, &g);

    p1.gather();
    CHECK(p1.getCoins() == 1);
    p2.gather();
    CHECK(p2.getCoins() == 1);
}

/**
 * @brief Tests that Governor's tax adds 3 coins and gather adds 1 coin.
 */
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


/**
 * @brief Tests that Baron can invest and receive 6 coins after 3 gathers.
 */
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

/**
 * @brief Tests that coup eliminates another player.
 */
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

/**
 * @brief Tests that Judge can cancel another player's bribe.
 */
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

/**
 * @brief Tests that Spy can spy and see another player's coin count.
 */
TEST_CASE("Spy Action") {
    Game g;
    Player spy("Spy", Role::Spy, &g);
    Player target("Target", Role::Governor, &g);

    spy.spyOn(target);
    CHECK(target.getCoins() == 0);
}

/**
 * @brief Tests that winner() returns the correct name when only one player is left.
 */
TEST_CASE("Game Winner") {
    Game g;
    Player p1("Alice", Role::Governor, &g);
    Player p2("Bob", Role::Merchant, &g);

    // Alice eliminates Bob
    for (int i = 0; i < 7; ++i) { p1.gather(); p2.gather(); }
    p1.coup(p2);

    CHECK(g.winner() == "Alice");
}

/**
 * @brief Tests illegal actions: coup without enough coins, acting out of turn, self-coup.
 */
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
///////////////////////////

/**
 * @brief Tests that bribe throws if the player does not have enough coins.
 */
TEST_CASE("Bribe without enough coins throws") {
    Game g;
    Player p("Briber", Role::Merchant, &g);
    Player dummy("Other", Role::Spy, &g);
    
    // Only 3 coins - not enough for bribe (requires 4)
    for (int i = 0; i < 3; ++i) { p.gather(); dummy.gather(); }
    CHECK_THROWS(p.bribe());
}

///////////////////2
/**
 * @brief Tests that coup on a dead player throws an exception.
 */
TEST_CASE("Coup on dead player throws") {
    Game g;
    Player a("A", Role::Governor, &g);
    Player b("B", Role::Spy, &g);
    
    for (int i = 0; i < 7; ++i) { a.gather(); b.gather(); }
    a.coup(b);
    CHECK_FALSE(b.isAlive());
    CHECK_THROWS(a.coup(b));  // Already dead
}
////////////////////3

/**
 * @brief Tests that General cannot preventCoup if it is not their turn.
 */
// TEST_CASE("General preventCoup not on turn") {
//     Game g;
//     Player gen("Gen", Role::General, &g);
//     Player other("Other", Role::Spy, &g);
//
//     gen.gather();
//     other.gather(); // now gen's turn is over
//     CHECK_THROWS(gen.preventCoup(gen));
// }
//////////////////////4
/**
 * @brief Tests that Spy can block arrest after spying on a player.
 */
// TEST_CASE("Spy blocks arrest after spying") {
//     Game g;
//     Player spy("Spy", Role::Spy, &g);
//     Player gov("Gov", Role::Governor, &g);
//
//     spy.gather();
//     gov.gather();
//     spy.spyOn(gov);  // Activates protection
//     spy.endTurn();   // Ends spy turn
//
//     CHECK_THROWS(gov.arrest(spy));  // Should be blocked
// }
////////////////5
/**
 * @brief Tests that sanction on a dead player throws an exception.
 */
TEST_CASE("Sanction on dead player throws") {
    Game g;
    Player gov("Gov", Role::Governor, &g);
    Player target("Target", Role::Spy, &g);

    for (int i = 0; i < 7; ++i) { gov.gather(); target.gather(); }
    gov.coup(target);

    CHECK_THROWS(gov.sanction(target)); // סנקציה על מת?
}

////////////////11
/**
 * @brief Tests that two Judges can cancel each other's bribe.
 */
TEST_CASE("Judge cancels each other's bribe") {
    Game g;
    Player j1("Judge1", Role::Judge, &g);
    Player j2("Judge2", Role::Judge, &g);

    for (int i = 0; i < 4; ++i) { j1.gather(); j2.gather(); }

    j1.bribe();
    j2.judgeBribe(j1);

    CHECK(j1.getCoins() == 0);
}

//////////////////////////12
/**
 * @brief Tests that a dead player cannot perform any actions.
 */
TEST_CASE("Dead player cannot act") {
    Game g;
    Player killer("Killer", Role::Governor, &g);
    Player victim("Victim", Role::Spy, &g);

    for (int i = 0; i < 7; ++i) { killer.gather(); victim.gather(); }
    killer.coup(victim);

    CHECK_THROWS(victim.gather());
    CHECK_THROWS(victim.tax());
    CHECK_THROWS(victim.bribe());
}

///////////////13

/**
 * @brief Tests that Merchant receives a bonus coin repeatedly when starting turn with 3+ coins.
 */
TEST_CASE("Merchant bonus triggers repeatedly") {
    Game g;
    Player m("Merch", Role::Merchant, &g);
    Player d("Dummy", Role::Spy, &g);

    for (int round = 0; round < 10; ++round) {
        m.gather(); d.gather();
    }

    CHECK(m.getCoins() > 10); // כולל בונוסים
}

/**
 * @brief Tests that only alive players are listed in playersNames after a coup.
 */
TEST_CASE("playersNames only lists alive players after coup") {
    Game g;
    Player gov("Avichay", Role::Governor, &g);
    Player spy("Hadar", Role::Spy, &g);
    Player baron("Dani", Role::Baron, &g);

    for (int i = 0; i < 7; ++i) { gov.gather(); spy.gather(); baron.gather(); }
    gov.coup(spy);
    auto names = g.playersNames();
    CHECK(std::find(names.begin(), names.end(), "Hadar") == names.end());
    CHECK(std::find(names.begin(), names.end(), "Avichay") != names.end());
    CHECK(std::find(names.begin(), names.end(), "Dani") != names.end());
}

/**
 * @brief Tests that turn order is correct after a player is eliminated.
 */
TEST_CASE("Turn order after elimination matches expected") {
    Game g;
    Player gov("Avichay", Role::Governor, &g);
    Player spy("Hadar", Role::Spy, &g);
    Player baron("Dani", Role::Baron, &g);

    for (int i = 0; i < 7; ++i) { gov.gather(); spy.gather(); baron.gather(); }
    gov.coup(spy); // Hadar eliminated
    // Next turn should be Dani
    CHECK(g.currentPlayer()->getName() == "Dani");
}



///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Tests interaction: General blocks coup on himself, attacker loses coins, General pays 5 coins.
 */
TEST_CASE("General blocks coup on himself") {
    Game g;
    Player attacker("Attacker", Role::Baron, &g);
    Player general("General", Role::General, &g);
    for (int i = 0; i < 12; ++i) { attacker.gather(); general.gather(); }
    attacker.coup(general); // Attacker tries coup
    general.generalBlockCoup(attacker);
    CHECK(attacker.getCoins() == 7); // Attacker refunded
    CHECK(general.getCoins() == 7); // General paid 5 coins (had 12)
    CHECK(general.isAlive());
}

/**
 * @brief Tests exception: Spy tries to spy on himself.
 */
TEST_CASE("Spy cannot spy on himself") {
    Game g;
    Player spy("Spy", Role::Spy, &g);
    spy.gather();
    CHECK_THROWS(spy.spyOn(spy));
}

/**
 * @brief Tests exception: Baron tries to invest without enough coins.
 */
TEST_CASE("Baron invest without enough coins throws") {
    Game g;
    Player baron("Baron", Role::Baron, &g);
    baron.gather();
    CHECK_THROWS(baron.invest());
}

/**
 * @brief Tests that a player can skip turn if in soft lock (no legal actions).
 */
TEST_CASE("Player can skip turn in soft lock") {
    Game g;
    Player a("A", Role::Governor, &g);
    Player b("B", Role::Spy, &g);
    // a עושה חרם על b, b לא יכול לבצע gather/tax
    for (int i = 0; i < 3; ++i) { a.gather(); b.gather(); }
    a.sanction(b);
    // b לא יכול לעשות gather/tax, אין לו מספיק מטבעות ל-coup/bribe/sanction/arrest
    CHECK_NOTHROW(b.skipTurn());
}
