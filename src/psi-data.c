/*
 * Data for psi-game result calculation
 *
 * The following data represents the bidding percentages for
 * Netrunner's psi-game. These precentages represent the Game Theory
 * Optimum, GTO. This is the Nash Equilibrium for the psi-game if both
 * players are using this app!
 *
 * Of course, both players are probably *not* using this app, so
 * there's still plenty of scope for doing other things. If yuo notice
 * someone is using these figures, the optimum counter is to switch
 * sides (choose Runner of you are the Corp and vice versa).
 *
 * Using this effectively depends on being able to estimate the credit
 * value of runs. When credits are directly at stake this is easy,
 * especially in an Account Siphon (which is why there's a separate
 * mode for AS). In other cases you'll have to estimate based on the
 * game's state, including how many clicks or credits you've invested
 * in things and how much closer to winning this accesswould put the
 * runner.
 *
 * https://boardgamegeek.com/thread/1174534/theory-analyzing-psi-game
 */


/* ============= Corp-side data ============== */

// standard psi-games

const unsigned short int oneThresholdsCorp[] = {
  100,    // threshold over which the result is at least 1
  100,    // index corresponds to value of run
  25,     // 0 - 10 then 15, 20, 30, 50, 100, 1000
  18,
  8,
  13,
  21,
  22,
  24,
  25,
  23,
  26,
  28,
  30,
  32,
  32,
  33
};

const unsigned short int twoThresholdsCorp[] = {
  100,     // threshold over which the result should be 2
  100,     // note, this is the sum of percentages of one and two
  25 + 75, // index corresponds to value of run
  18 + 51, // 0 - 10 then 15, 20, 30, 50, 100, 1000
  8 + 33,
  13 + 33,
  21 + 37,
  22 + 37,
  24 + 36,
  25 + 36,
  23 + 33,
  26 + 33,
  28 + 33,
  30 + 33,
  32 + 33,
  32 + 33,
  33 + 33,
};

// psi-game result for during an account siphon

const unsigned short int oneSiphonThresholdsCorp[] = {
  100,  // threshold over which the result should be 2
  0,	// index corresponds to remaining corp credits
  16,	// note that 7+ credits are equal
  30,
  7,
  13,
  20,
  23
};

const unsigned short int twoSiphonThresholdsCorp[] = {
  100,      // threshold over which the result should be 2
  23 + 33,  // note that value is sum of percentages for one and two
  20 + 30,  // index corresponds to remaining corp credits
  13 + 29,  // note that 7+ credits are equal
  7 + 26,
  30 + 70,
  16 + 83,
  0 + 100
};

/* ============= Runner-side data ============== */

// standard psi-games

const unsigned short int oneThresholdsRunner[] = {
  100,    // threshold over which the result is at least 1
  100,    // index corresponds to value of run
  75,	  // 0 - 10 then 15, 20, 30, 50, 100, 1000
  66,
  58,
  53,
  50,
  47,
  45,
  44,
  43,
  40,
  38,
  36,
  34,
  34,
  33
};

const unsigned short int twoThresholdsRunner[] = {
  100,      // threshold over which the result should be 2
  100,	    // note, this is the sum of percentages of one and two
  75 + 25,  // index corresponds to value of run
  66 + 33,  // 0 - 10 then 15, 20, 30, 50, 100, 1000
  58 + 33,
  53 + 33,
  50 + 33,
  47 + 33,
  45 + 33,
  44 + 33,
  43 + 33,
  40 + 33,
  38 + 33,
  36 + 33,
  34 + 33,
  34 + 33,
  33 + 33
};

// psi-game result for during an account siphon

const unsigned short int oneSiphonThresholdsRunner[] = {
  0,    // threshold over which the result is at least 1
  100,	// index corresponds to remaining corp credits
  75,	// note that 7+ credits are equal
  60,
  53,
  48,
  52,
  53
};

const unsigned short int twoSiphonThresholdsRunner[] = {
  100,      // threshold over which the result should be 2
  100 + 0,  // note, this is the sum of percentages of one and two
  75 + 25,  // index corresponds to remaining corp credits
  60 + 40,  // note that 7+ credits are equal
  53 + 38,
  48 + 36,
  52 + 32,
  53 + 33
};
