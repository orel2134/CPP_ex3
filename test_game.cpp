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
TEST_CASE("General preventCoup not on turn") {
    Game g;
    Player gen("Gen", Role::General, &g);
    Player other("Other", Role::Spy, &g);

    gen.gather();
    other.gather(); // now gen's turn is over
    CHECK_THROWS(gen.preventCoup(gen));
}
//////////////////////4
/**
 * @brief Tests that Spy can block arrest after spying on a player.
 */
TEST_CASE("Spy blocks arrest after spying") {
    Game g;
    Player spy("Spy", Role::Spy, &g);
    Player gov("Gov", Role::Governor, &g);

    spy.gather();
    gov.gather();
    spy.spyOn(gov);  // Activates protection
    spy.endTurn();   // Ends spy turn

    CHECK_THROWS(gov.arrest(spy));  // Should be blocked
}
////////////////5
/**
 * @brief Tests that calling winner before the game ends throws an exception.
 */
TEST_CASE("Calling winner before game ends throws") {
    Game g;
    Player a("A", Role::Spy, &g);
    Player b("B", Role::Merchant, &g);

    CHECK_THROWS(g.winner());
}
//////////////////6
/**
 * @brief Tests that only Baron can call invest.
 */
TEST_CASE("Non-Baron calling invest throws") {
    Game g;
    Player p("NotBaron", Role::Governor, &g);
    Player dummy("Other", Role::Spy, &g);

    for (int i = 0; i < 3; ++i) { p.gather(); dummy.gather(); }

    CHECK_THROWS(p.invest());
}
//////////////7
/**
 * @brief Tests that dead players are skipped in the turn rotation.
 */
TEST_CASE("Dead players are skipped in turn rotation") {
    Game g;
    Player a("A", Role::Governor, &g);
    Player b("B", Role::Merchant, &g);
    Player c("C", Role::Baron, &g);

    for (int i = 0; i < 7; ++i) { a.gather(); b.gather(); c.gather(); }
    a.coup(b);  // b is eliminated

    // Now current turn should be c
    CHECK(g.currentPlayer()->getName() == "C");
}

/////////////8
/**
 * @brief Tests that only one action is allowed per turn without a bribe.
 */
TEST_CASE("Only one action per turn without bribe") {
    Game g;
    Player p("Gov", Role::Governor, &g);
    Player dummy("Other", Role::Spy, &g);

    p.gather();
    CHECK_THROWS(p.tax()); // אסור לבצע עוד פעולה באותו תור
}
////////////////////9

/**
 * @brief Tests that bribe allows an extra action in the same turn.
 */
TEST_CASE("Bribe allows extra action") {
    Game g;
    Player p("Merchant", Role::Merchant, &g);
    Player dummy("Other", Role::Spy, &g);

    for (int i = 0; i < 4; ++i) { p.gather(); dummy.gather(); }

    p.bribe(); // פעולה ראשונה
    CHECK_NOTHROW(p.gather()); // פעולה נוספת מותרת
}
//////////////10
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
 * @brief Tests that bribe allows an extra action and coup in the same turn.
 */
TEST_CASE("Bribe allows extra action and coup eliminates player") {
    Game g;
    Player gov("Avichay", Role::Governor, &g);
    Player spy("Hadar", Role::Spy, &g);
    Player baron("Dani", Role::Baron, &g);

    gov.tax();
    spy.gather();
    baron.gather();
    // Give Avichay enough coins for bribe + coup
    gov.addCoins(11);
    gov.bribe(); // should not end turn
    CHECK(spy.isAlive());
    gov.coup(spy);
    CHECK_FALSE(spy.isAlive());
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

/**
 * @brief Tests that sanction wears off after one turn and allows action again.
 */
TEST_CASE("Sanction wears off after turn and allows action again") {
    Game g;
    Player gov("Avichay", Role::Governor, &g);
    Player spy("Hadar", Role::Spy, &g);
    Player baron("Dani", Role::Baron, &g);

    gov.gather();
    spy.gather();
    baron.sanction(gov); // Sanction Avichay
    // Next turn: Avichay's action is blocked
    CHECK_THROWS(gov.tax());
    // Next turn: Hadar
    spy.gather();
    // Next turn: Avichay should be able to act again
    CHECK_NOTHROW(gov.gather());
}

/**
 * @brief Tests that a player cannot bribe twice in the same turn.
 */
TEST_CASE("Cannot bribe if already bribed this turn") {
    Game g;
    Player gov("Avichay", Role::Governor, &g);
    Player spy("Hadar", Role::Spy, &g);
    Player baron("Dani", Role::Baron, &g);

    gov.gather();
    spy.gather();
    baron.gather();
    gov.addCoins(10);
    gov.bribe();
    CHECK_THROWS(gov.bribe());
}

/**
 * @brief Tests that coup cannot be performed on self or on an already eliminated player.
 */
TEST_CASE("Coup cannot be performed on self or already eliminated player") {
    Game g;
    Player gov("Avichay", Role::Governor, &g);
    Player spy("Hadar", Role::Spy, &g);
    Player baron("Dani", Role::Baron, &g);
    for (int i = 0; i < 7; ++i) { gov.gather(); spy.gather(); baron.gather(); }
    CHECK_THROWS(gov.coup(gov));
    gov.coup(spy);
    CHECK_FALSE(spy.isAlive());
    CHECK_THROWS(gov.coup(spy));
}

/**
 * @brief Tests that Judge can only cancel a bribe if it was actually made.
 */
TEST_CASE("Judge can cancel bribe only if bribe was made") {
    Game g;
    Player judge("Judge", Role::Judge, &g);
    Player baron("Baron", Role::Baron, &g);
    for (int i = 0; i < 4; ++i) { judge.gather(); baron.gather(); }
    CHECK_THROWS(judge.judgeBribe(baron)); // No bribe yet
    baron.bribe();
    CHECK_NOTHROW(judge.judgeBribe(baron));
}

/**
 * @brief Tests that Spy cannot spyOn self and cannot spy if dead.
 */
TEST_CASE("Spy cannot spyOn self and cannot spy if dead") {
    Game g;
    Player spy("Spy", Role::Spy, &g);
    Player gov("Gov", Role::Governor, &g);
    spy.gather(); gov.gather();
    CHECK_THROWS(spy.spyOn(spy));
    for (int i = 0; i < 7; ++i) { gov.gather(); spy.gather(); }
    gov.coup(spy);
    CHECK_FALSE(spy.isAlive());
    CHECK_THROWS(spy.spyOn(gov));
}

/**
 * @brief Tests that winner returns the correct player after multiple eliminations.
 */
TEST_CASE("Game winner is correct after multiple eliminations") {
    Game g;
    Player a("A", Role::Governor, &g);
    Player b("B", Role::Merchant, &g);
    Player c("C", Role::Baron, &g);
    for (int i = 0; i < 7; ++i) { a.gather(); b.gather(); c.gather(); }
    a.coup(b);
    for (int i = 0; i < 7; ++i) { a.gather(); c.gather(); }
    a.coup(c);
    CHECK(g.winner() == "A");
}

/**
 * @brief Tests that a player eliminated in the same round cannot act anymore.
 */
TEST_CASE("Cannot act after being eliminated in the same round") {
    Game g;
    Player a("A", Role::Governor, &g);
    Player b("B", Role::Merchant, &g);
    for (int i = 0; i < 7; ++i) { a.gather(); b.gather(); }
    a.coup(b);
    CHECK_FALSE(b.isAlive());
    CHECK_THROWS(b.gather());
    CHECK_THROWS(b.tax());
    CHECK_THROWS(b.bribe());
}
