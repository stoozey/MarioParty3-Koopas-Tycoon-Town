// NAME: Hotel
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Number|HOTEL_INDEX

//region includes

#include "ultra64.h"

//endregion

//region defines

//region custom types

#define bool int
#define true 1
#define false 0

//endregion

//region asset ids

#define ASSET_PIC_KOOPA 2
#define ASSET_PIC_TOAD 3

#define ASSET_ANIM_WALKING 0
#define ASSET_ANIM_RUNNING 1
#define ASSET_ANIM_JUMP 2
#define ASSET_ANIM_SAD 3
#define ASSET_ANIM_DIZZY 4
#define ASSET_ANIM_HAPPY 5
#define ASSET_ANIM_GET_STAR 6

#define ASSET_SONG_DEFAULT 25
#define ASSET_SONG_GET_STAR 111

#define ASSET_SOUND_COIN 582
#define ASSET_SOUND_HOTEL_UPGRADE 108
#define ASSET_SOUND_HOTEL_DOWNGRADE 90
#define ASSET_SOUND_KOOPA 221
#define ASSET_SOUND_CASH_REGISTER 583

//endregion

//region values

#define MAX_STARS 99
#define MAX_COIN_INVESTMENT 99
#define HOTEL_MAX_COINS 255
#define MIN_COINS 1

#define TOTAL_PLAYERS 4
#define TOTAL_HOTELS 5

//endregion

//region choices

#define CHOICE_INTRO_YES 0
#define CHOICE_INTRO_NO 1

//endregion

//region messages

#define MSG_DYNAMIC_START "\x0B"
#define MSG_INSERT "\x11"
#define MSG_PIC_INDENT "\x1A\x1A\x1A\x1A"
#define MSG_PIC_OPTION_INDENT "\x1A\x1A\x1A\x1A\x1A\x1A"
#define MSG_DOT "\x85"
#define MSG_NEWLINE "\x0A"
#define MSG_APOSTROPHE "\x5C"
#define MSG_EXCLAMATION "\xC2"
#define MSG_QUESTION "\xC3"
#define MSG_END "\xFF"

#define MSG_NEWLINE_OPTION "\n"
#define MSG_OPTION_START "\x0C"
#define MSG_OPTION_END "\x0D"

#define MSG_SPACE "\x10"

#define MSG_ICON_TERMINATOR "\x20"
#define MSG_ICON_COIN "\x29"
#define MSG_ICON_CONTROL_STICK "\x28"
#define MSG_ICON_A "\x21"
#define MSG_ICON_B "\x22"
#define MSG_IDFK "\x19"
#define MSG_X "\x3E"

//endregion

//region hotels

#define HOTEL_1STAR_COINS 1
#define HOTEL_2STAR_COINS 20
#define HOTEL_3STAR_COINS 50

#define GET_HOTEL_INVESTMENT(hotel, playerIndex) ((hotel >> (playerIndex * 0x8)) & 0xff)
#define SET_HOTEL_PLAYER_INVESTMENT(hotel, playerIndex, investment) *hotel = ((*hotel & ~(0xff << (playerIndex * 0x8)) | (investment << (playerIndex * 0x8))))

#define HOTEL hotels[HOTEL_INDEX]

//endregion

//endregion

//region messages

char* msgNotEnoughCoins =
        MSG_PIC_INDENT
        "Look" MSG_APOSTROPHE "s like you don" MSG_APOSTROPHE "t have"
        MSG_NEWLINE

        MSG_PIC_INDENT
        "enough coins to invest" MSG_DOT MSG_DOT MSG_DOT
        MSG_NEWLINE

        MSG_PIC_INDENT
        "Come back when you ain" MSG_APOSTROPHE "t broke" MSG_EXCLAMATION
        MSG_END;

char* msgWelcome =
        MSG_PIC_INDENT
        "Welcome " MSG_INSERT MSG_EXCLAMATION
        MSG_END;

char* msgDoYouWantToInvestDoesntOwn =
        MSG_PIC_INDENT
        "This Hotel could be yours for an"
        MSG_NEWLINE

        MSG_PIC_INDENT
        "investment of only %d coins" MSG_EXCLAMATION
        MSG_NEWLINE

        MSG_PIC_INDENT
        "Want to invest" MSG_QUESTION
        MSG_NEWLINE

        MSG_PIC_OPTION_INDENT MSG_OPTION_START
        "Yes"
        MSG_OPTION_END MSG_NEWLINE_OPTION

        MSG_PIC_OPTION_INDENT MSG_OPTION_START
        "No"
        MSG_OPTION_END MSG_NEWLINE_OPTION;

char* msgDoYouWantToInvestOwns =
        MSG_PIC_INDENT
        "You already own this Hotel" MSG_DOT
        MSG_NEWLINE

        MSG_PIC_INDENT
        "Care to change the number of"
        MSG_NEWLINE

        MSG_PIC_INDENT
        "number of coins you have invested" MSG_QUESTION
        MSG_NEWLINE

        MSG_PIC_OPTION_INDENT MSG_OPTION_START
        "Yes"
        MSG_OPTION_END MSG_NEWLINE_OPTION

        MSG_PIC_OPTION_INDENT MSG_OPTION_START
        "No"
        MSG_OPTION_END MSG_NEWLINE_OPTION;

char* msgCoinCounter =
        MSG_DYNAMIC_START
        MSG_PIC_INDENT
        "How much would you like to invest" MSG_QUESTION
        MSG_NEWLINE

        "%s"
        "%s"
        "%s"
        "%s"
        MSG_NEWLINE

        MSG_PIC_OPTION_INDENT
        MSG_ICON_CONTROL_STICK MSG_DOT MSG_DOT MSG_DOT "Change Amount"
        MSG_NEWLINE

        MSG_PIC_OPTION_INDENT
        MSG_ICON_A MSG_DOT MSG_DOT MSG_DOT "Confirm"
        MSG_NEWLINE;

char* msgCantAffordCoins =
        MSG_PIC_INDENT
        "Broke ass"
        MSG_END;

char* msgInvested =
        MSG_PIC_INDENT
        "Thank you for investing" MSG_EXCLAMATION
        MSG_NEWLINE

        MSG_PIC_INDENT
        "The new total coins is %s" MSG_EXCLAMATION MSG_EXCLAMATION
        MSG_NEWLINE

        MSG_PIC_INDENT
        "Wow" MSG_EXCLAMATION MSG_EXCLAMATION MSG_EXCLAMATION MSG_EXCLAMATION MSG_EXCLAMATION MSG_EXCLAMATION
        MSG_END;

char* msgDidntInvest =
        MSG_PIC_INDENT
        "Byeeeeeeeeeeeeeee" MSG_EXCLAMATION
        MSG_END;

char* msgHotelNewOwner =
        MSG_PIC_INDENT
        "The new owner of this hotel is " MSG_INSERT MSG_EXCLAMATION
        MSG_END;

char* msgHotelSameOwner =
        MSG_PIC_INDENT
        "The owner is still " MSG_INSERT
        MSG_END;

char* msgHotelStarsIncreased =
        MSG_PIC_INDENT
        "The total investment in this hotel"
        MSG_NEWLINE

        MSG_PIC_INDENT
        "has reached %d coins" MSG_EXCLAMATION
        MSG_NEWLINE

        MSG_PIC_INDENT
        "We can build up this Hotel" MSG_EXCLAMATION
        MSG_END;

char* msgHotelStarsDecreased =
        MSG_PIC_INDENT
        "The total investment in this hotel"
        MSG_NEWLINE

        MSG_PIC_INDENT
        "has dropped to %d coins" MSG_DOT MSG_DOT MSG_DOT
        MSG_NEWLINE

        MSG_PIC_INDENT
        "We will have to downsize" MSG_DOT
        MSG_END;

char* msgHotelLostOwner =
        MSG_PIC_INDENT
        "This hotel no longer has any investments" MSG_DOT MSG_DOT MSG_DOT
        MSG_END;

char* msgHotelReachedMaxCoins =
        MSG_PIC_INDENT
        "This hotel has reached the"
        MSG_NEWLINE

        MSG_PIC_INDENT
        "maximum amount of coins" MSG_EXCLAMATION
        MSG_END;

char* characterCoinsString =
        MSG_PIC_OPTION_INDENT
        "%s " MSG_ICON_COIN MSG_ICON_TERMINATOR MSG_X MSG_ICON_TERMINATOR "%s" MSG_IDFK
        MSG_NEWLINE;

//endregion

//region util

void WriteNumberString(char* numberString, int number)
{
    char* minus = ((number < 0) ? " \x3d" : " ");
    number *= ((number < 0) ? -1 : 1);
    sprintf(numberString, "%s%d", minus, number);
}

void PlaySong(int soundId, int revertTime)
{
    func_8004A520(soundId);

    if (revertTime <= 0) return;

    SleepProcess(revertTime);
    func_8004A520(GetBoardAudioIndex());
}

void ProcessMessage()
{
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

void Message(int picture, char* message)
{
    if (picture == ASSET_PIC_KOOPA)
        PlaySound(ASSET_SOUND_KOOPA);

    ShowMessage(picture, message, 0, 0, 0, 0, 0);
    ProcessMessage();
}

void MessageCharacter(int picture, char* message, int characterStringId)
{
    ShowMessage(picture, message, characterStringId, 0, 0, 0, 0);
    ProcessMessage();
}

//endregion

//region hotels

u32* hotels[TOTAL_HOTELS] = {
        (u32*) 0x800CD098,
        (u32*) 0x800CD09C,
        (u32*) 0x800CD0A0,
        (u32*) 0x800CD0A4,
        (u32*) 0x800CD05E
};

int HotelGetTotalCoins()
{
    int totalCoins = 0;
    for (int i = 0; i < TOTAL_PLAYERS; i++)
        totalCoins += GET_HOTEL_INVESTMENT(*HOTEL, i);

    return totalCoins;
}

int HotelGetStarCount()
{
    int starCount = 0;
    int totalCoins = HotelGetTotalCoins();
    if (totalCoins >= HOTEL_3STAR_COINS)
        starCount = 3;
    else if (totalCoins >= HOTEL_2STAR_COINS)
        starCount = 2;
    else if (totalCoins >= HOTEL_1STAR_COINS)
        starCount = 1;

    return starCount;
}

int HotelGetHighestInvestorIndex()
{
    int highestInvestorIndex = -1;
    int highestInvestment = 0;
    for (int i = 0; i < TOTAL_PLAYERS; i++)
    {
        int currentInvestment = GET_HOTEL_INVESTMENT(*HOTEL, i);
        if (currentInvestment <= highestInvestment) continue;

        highestInvestment = currentInvestment;
        highestInvestorIndex = i;
    }

    return highestInvestorIndex;
}

//endregion

//region player

struct player {
    s8 unk0;
    s8 cpu_difficulty;
    s8 controller;
    u8 character;
    /**
     * Miscellaneous flags.
     * 1: Is CPU player
     */
    u8 flags;
    s8 pad0[5];
    /**
     * Current coin count.
     */
    s16 coins; // 10
    /**
     * Coins obtained during a Mini-Game.
     */
    s16 minigame_coins; // 12
    s8 stars; // 14

    u8 cur_chain_index; // 15
    u8 cur_space_index; // 16
    u8 next_chain_index; // 17
    u8 next_space_index; // 18
    u8 unk1_chain_index; // 19
    u8 unk1_space_index; // 20
    u8 reverse_chain_index; // 21
    u8 reverse_space_index; // 22

    u8 flags2; // 23
    u8 items[3]; // 24
    u8 bowser_suit_flag; // 27
    u8 turn_color_status; // 28

    s8 pad1[7]; // 29 - 35

    void *obj; // 36 // struct object *
    s16 minigame_star; // 40
    s16 coin_star; // 42
    s8 happening_space_count; // 44
    s8 red_space_count;
    s8 blue_space_count;
    s8 chance_space_count;
    s8 bowser_space_count; // 48
    s8 battle_space_count;
    s8 item_space_count;
    s8 bank_space_count;
    s8 game_guy_space_count; // 52

    // s8 pad2[3];
};

struct OSContPadPadded {
    OSContPad cont;
    u16 padding;
};

extern struct player *GetPlayerStruct(s32 player_index);
extern struct OSContPadPadded D_800CC3F4[4];

const char* CHARACTER_NAMES[8] = {
        "MARIO",
        "LUIGI",
        "PEACH",
        "YOSHI",
        "WARIO",
        "D K",
        "WALUIGI",
        "DAISY"
};

int PlayerGetCharacterStringId(int playerIndex)
{
    struct player *player = GetPlayerStruct(playerIndex);
    return (0x1C00 + player->character);
}

const char* GetCharacterName(int playerIndex)
{
    struct player *player = GetPlayerStruct(playerIndex);
    return CHARACTER_NAMES[player->character];
}

void PlayerWriteCoinsString(char* coinsString, int playerIndex, int coins)
{
    char numberString[4];
    WriteNumberString(numberString, coins);

    const char* characterName = GetCharacterName(playerIndex);
    sprintf(coinsString, characterCoinsString, characterName, numberString);
}

void WriteCoinCounterString(char* coinCounter, int overridePlayerId, int overrideCoins)
{
    char* coinsStrings[TOTAL_PLAYERS][64];
    for (int i = 0; i < TOTAL_PLAYERS; i++)
    {
        int coins = GET_HOTEL_INVESTMENT(*HOTEL, i);
        if (overridePlayerId == i)
            coins += overrideCoins;

        char* coinString = coinsStrings[i];
        PlayerWriteCoinsString(coinString, i, coins);
    }

    sprintf(coinCounter, msgCoinCounter, coinsStrings[0], coinsStrings[1], coinsStrings[2], coinsStrings[3]);
}

int PlayerGetButtonsPressed(struct player *player, u32 buttons)
{
    return (D_800CC3F4[player->controller].cont.button & buttons) == buttons;
}

int PlayerGetJoystickHorizontalState(struct player *player)
{
    s8 stick_x = D_800CC3F4[player->controller].cont.stick_x;

    // According to docs, -80 <= stick_y <= 80
    if (stick_x < -50) {
        return -1;
    }
    if (stick_x > 60) {
        return 1;
    }
    return 0;
}

void PlayerPlayAnimation(int playerIndex, int animationId)
{
    func_800F2304(playerIndex, animationId, 0);
}

void PlayerPlaySound(int playerIndex, int soundId)
{
    func_8004ACE0(soundId, playerIndex);
}

void PlayerIncrementStars(int playerIndex, int starCount)
{
    struct player* player = GetPlayerStruct(playerIndex);
    player->stars += starCount;

    if (starCount > 0)
    {
        PlayerPlayAnimation(playerIndex, ASSET_ANIM_GET_STAR);
        PlayerPlaySound(playerIndex, 610);
        PlaySong(ASSET_SONG_GET_STAR, 110);
    }
}

int PlayerGetInvestmentAmount(int playerIndex)
{
    struct player *player = GetPlayerStruct(playerIndex);
    s32 coins = player->coins;

    int totalCoins = HotelGetTotalCoins();
    int invested = GET_HOTEL_INVESTMENT(*HOTEL, playerIndex);

    int amountSelected = 0;
    char* amountSelectedStrings[TOTAL_PLAYERS];

    char coinCounter[256];
    WriteCoinCounterString(coinCounter, playerIndex, 0);
    func_800EC92C(ASSET_PIC_KOOPA, coinCounter, 0, 0, 0, 0);

    bool isCpu = (PlayerIsCPU(playerIndex) != 0);
    bool cancelled = false;
    int coinsLeft, newInvested, totalWorth;
    while (!cancelled)
    {
        coinsLeft = (coins - amountSelected);
        newInvested = (invested + amountSelected);
        totalWorth = (totalCoins + amountSelected);

        int amount = 0;
        if (isCpu)
        {
            amount = (coinsLeft > 0);
            cancelled = (amount == 0);
        }
        else
        {
            amount = PlayerGetJoystickHorizontalState(player);
            cancelled = PlayerGetButtonsPressed(player, A_BUTTON);
        }

        if (amount == 0)
        {
            SleepVProcess();
            continue;
        }

        if (((amount > 0) && (coinsLeft >= amount)) || ((amount < 0) && ((newInvested + amount) >= 0)))
        {
            amountSelected += amount;
            PlaySound(ASSET_SOUND_COIN);
        }

        while ((amountSelected > MAX_COIN_INVESTMENT) || ((totalCoins + amountSelected) >= HOTEL_MAX_COINS))
            amountSelected--;

        WriteCoinCounterString(coinCounter, playerIndex, amountSelected);
        func_8005B6BC(2, 0, 0);
        func_8005B43C(2, coinCounter, -1, -1);

        SleepVProcess();
        SleepProcess(2);
    }

    CloseMessage();
    func_800EC6EC();

    return amountSelected;
}

void PlayerInvestInHotel(int playerIndex, int amount)
{
    if (amount == 0)
    {
        Message(ASSET_PIC_KOOPA, msgDidntInvest);
        return;
    }

    // take coins
    AdjustPlayerCoinsGradual(playerIndex, -amount);
    ShowPlayerCoinChange(playerIndex, -amount);

    // set investment
    int previousHighestIndex = HotelGetHighestInvestorIndex();
    int previousStarsCount = HotelGetStarCount();

    int investment = (GET_HOTEL_INVESTMENT(*HOTEL, playerIndex) + amount);
    SET_HOTEL_PLAYER_INVESTMENT(HOTEL, playerIndex, investment);

    int newHighestIndex = HotelGetHighestInvestorIndex();
    int newStarCount = HotelGetStarCount();

    bool playerIndexChanged = (previousHighestIndex != newHighestIndex);
    bool starCountChanged = (previousStarsCount != newStarCount);
    if ((!playerIndexChanged) && (!starCountChanged)) return;

    PlaySound(ASSET_SOUND_CASH_REGISTER);

    int totalCoins = HotelGetTotalCoins();
    if (playerIndexChanged)
    {
        if (newHighestIndex == -1)
        {
            Message(ASSET_PIC_KOOPA, msgHotelLostOwner);
        }
        else
        {
            char message[256];
            sprintf(message, msgHotelNewOwner, totalCoins);
            int characterStringId = PlayerGetCharacterStringId(newHighestIndex);
            MessageCharacter(ASSET_PIC_KOOPA, message, characterStringId);
        }
    }
    else
    {
        int characterStringId = PlayerGetCharacterStringId(previousHighestIndex);
        MessageCharacter(ASSET_PIC_KOOPA, msgHotelSameOwner, characterStringId);
    }

    int starDifference = (newStarCount - previousStarsCount);
    if (starCountChanged)
    {
        bool increased = (starDifference > 0);
        int soundId = ((increased) ? ASSET_SOUND_HOTEL_UPGRADE : ASSET_SOUND_HOTEL_DOWNGRADE);
        PlaySound(soundId);

        char message[256];
        char *baseMessage = ((increased) ? msgHotelStarsIncreased : msgHotelStarsDecreased);
        sprintf(message, baseMessage, totalCoins);
        Message(ASSET_PIC_KOOPA, message);
    }

    if (playerIndexChanged)
    {
        if (previousHighestIndex != -1)
            PlayerIncrementStars(previousHighestIndex, -previousStarsCount);

        if (newHighestIndex != -1)
            PlayerIncrementStars(newHighestIndex, newStarCount);
    }
    else if (starDifference != 0)
    {
        PlayerIncrementStars(previousHighestIndex, starDifference);
    }
}

//endregion

void main() {
    // intro
    s32 playerIndex = GetCurrentPlayerIndex();
    const char* characterName = GetCharacterName(playerIndex);
    ShowMessage(ASSET_PIC_KOOPA, msgWelcome, characterName, 0, 0, 0, 0);
    ProcessMessage();

    bool canInvest = ((PlayerHasCoins(playerIndex, MIN_COINS)) || (GET_HOTEL_INVESTMENT(*HOTEL, playerIndex) > 0));
    if (!canInvest)
    {
        Message(ASSET_PIC_KOOPA, msgNotEnoughCoins);
        return;
    }

    int ownerIndex = HotelGetHighestInvestorIndex();
    bool ownsHotel = (ownerIndex == playerIndex);
    if (ownsHotel)
    {
        ShowMessage(ASSET_PIC_KOOPA, msgDoYouWantToInvestOwns, 0, 0, 0, 0, 0);
    }
    else
    {
        int ownerInvestment = ((ownerIndex == -1) ? 0 : GET_HOTEL_INVESTMENT(*HOTEL, ownerIndex));
        int investmentNeeded = (ownerInvestment + 1);
        char message[256];
        sprintf(message, msgDoYouWantToInvestDoesntOwn, investmentNeeded);
        ShowMessage(ASSET_PIC_KOOPA, message, 0, 0, 0, 0, 0);
    }

    s32 choice = GetBasicPromptSelection(2, 0);
    CloseMessage();
    func_800EC6EC();

    switch (choice)
    {
        case CHOICE_INTRO_YES:
            break;

        case CHOICE_INTRO_NO:
            return;
    }

    // invest
    int amountSelected = PlayerGetInvestmentAmount(playerIndex);
    PlayerInvestInHotel(playerIndex, amountSelected);
}