# Koopas Tycoon Town for Mario Party 3
This mod adds support for the hotel system seen in Mario Party 8's Koopas Tycoon Town, via [PartyPlanner64](partyplanner64.github.io).
Due to memory restrictions, you can only have 4 hotels on one map--although you can push this to 5 if you do not have any star spaces on your board.

## How to add to your board
### init.c
Add the Init event as a before turn event on the board properties tab. This clears the memory associated with each hotel. Hotels will have random investments if you don't do this.

![](https://imgur.com/EOdjcoA.png)

---

### hotel.c
Add the Hotel event to a space and mark it as a passing event. Set `HOTEL_INDEX` to the hotel that it represents (0-4).

![](https://imgur.com/bGmQGs6.png)

---
