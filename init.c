// NAME: Init
// GAMES: MP3_USA
// EXECUTION: Direct

///region includes

#include "ultra64.h"

///endregion

///region defines

#define bool int
#define true 1
#define false 0

#define TOTAL_PLAYERS 4
#define TOTAL_HOTELS 5

#define GET_HOTEL_INVESTMENT(hotel, playerIndex) ((hotel >> (playerIndex * 0x8)) & 0xff)
#define SET_HOTEL_PLAYER_INVESTMENT(hotel, playerIndex, investment) *hotel = ((*hotel & ~(0xff << (playerIndex * 0x8)) | (investment << (playerIndex * 0x8))))

///endregion

u32 *hotels[TOTAL_HOTELS] = {
        (u32*) 0x800CD098,
        (u32*) 0x800CD09C,
        (u32*) 0x800CD0A0,
        (u32*) 0x800CD0A4,
        (u32*) 0x800CD05E
};

bool initiated = false;

void main() {
    if (initiated) return;
    initiated = true;

    for (int i = 0; i < TOTAL_HOTELS; i++) {
        for (int j = 0; j < TOTAL_PLAYERS; j++) {
            u32 *hotel = hotels[i];
            SET_HOTEL_PLAYER_INVESTMENT(hotel, j, 0);
        }
    }
}