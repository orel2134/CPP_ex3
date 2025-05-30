// orel2744@gmail.com
// Role.hpp defines the Role enum, representing all possible player roles in the game.
// Each role grants unique abilities and determines the player's special actions.
// Includes a utility function for converting Role values to readable strings.

#ifndef ROLE_HPP
#define ROLE_HPP

#include <string>

// enum representing the available roles in the game.
// each role grants unique abilities to the player.
enum class Role {
    Governor,
    Spy,
    Baron,
    General,
    Judge,
    Merchant,
    Unknown  // fallback role if a player's role is undefined
};

// utility function to convert a Role enum value to a readable string.
// useful for logging, displaying roles in the UI, etc.
inline std::string roleToString(Role role) {
    switch (role) {
        case Role::Governor: return "Governor";
        case Role::Spy:      return "Spy";
        case Role::Baron:    return "Baron";
        case Role::General:  return "General";
        case Role::Judge:    return "Judge";
        case Role::Merchant: return "Merchant";
        case Role::Unknown:  return "Unknown";
        default:             return "Invalid";
    }
}

#endif // ROLE_HPP
