/** Implementation file for table craps simulation
 *
 * \file imgs/apps/examples/craps/craps.cpp
 * \author Andrea Avendano (aa6588@rit.edu)
 * \date 10 April 2020
 *
 * \description
 * Simulates game of craps based on: the minimum bet amount, the initial
 * value of your bankroll, and the winnings that if achieved you would
 * be willing to “walk away from the table”.
 * don't pass bet is default, can make pass bet
 * not martingale system is default, can employ this strategy
 */

#include <boost/program_options.hpp>
#include <ctime>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
namespace po = boost::program_options;

// function that simulates the rolling of 2 dice
int roll_dice(std::vector<int> &dice) {
  int total = 0;
  dice.reserve(2);
  for (auto idx = 0; idx < 2; idx++) {
    dice.push_back(std::rand() % 6 + 1);
    total += dice.back();
  }
  return total;
}

int main(int argc, char *argv[]) {
  // Flag indicating if the program should be run in interactive mode
  // (i.e. requiring a <RET> to be hit between each game)
  bool interactive;

  // Flag indicating that a "Pass" bet should be used as the "line" bet
  // rather than the default "Don't Pass Bar 12" bet
  bool pass;

  // Flag indicating whether the Martingale betting system should be employed
  bool use_martingale_system;

  // Flag indicating whether an "odds" bet should be placed along with the
  // "line" bet (for sessions when the Martingale betting system is
  // being used as well as when it is not)
  bool odds;

  // Number of session simulations to be carried out (i.e. where a session
  // is defined as playing games until desired winnings are achieved -or-
  // the bankroll is exhausted)
  int number_of_simulations;

  // The minimum bet to be placed (i.e. the initial "line" bet as well as
  // the bet to be placd after any winning game -or- as a stand-alone "odds"
  // bet)
  int minimum_bet_amount;

  // The bankroll that play is started with
  int initial_bankroll;

  // Desired winnings, which if achieved, would indicate that session play
  // should be terminated
  int walk_away_winnings;

  // **************************************************************************
  // Implement command-line interface utilizing Boost program options library
  // **************************************************************************
  // Instantiate the "visible options" object that defines the program options
  // that are to be visible to the user
  po::options_description visible_options("Options");
  visible_options.add_options()("help,h", "display this message")(
      "interactive,i", po::bool_switch(&interactive)->default_value(false),
      "interactive mode (require <RET> after each game in a session) "
      "[default is false]")(
      "number-of-simulations,n",
      po::value<int>(&number_of_simulations)->default_value(1),
      "number of 'walk-away-winnings' session simulations to carry out "
      "[default is 1]")(
      "pass,p", po::bool_switch(&pass)->default_value(false),
      "place a Pass bet [default is false] (i.e. place a Don't Pass Bar "
      "12 bet)")("martingale,m",
                 po::bool_switch(&use_martingale_system)->default_value(false),
                 "use the Martingale betting system [default is false]")(
      "odds,o", po::bool_switch(&odds)->default_value(false),
      "place an Odds bet [default is false]");

  // Instantiate the "hidden options" object that defines the program options
  // that are to be hidden from the user -or- that are to be used as
  // positional arguments
  po::options_description hidden_options("Hidden Options");
  hidden_options.add_options()("bet,b", po::value<int>(&minimum_bet_amount),
                               "minimum bet amount")(
      "bankroll,r", po::value<int>(&initial_bankroll), "initial bankroll")(
      "walk-away-winnings,w", po::value<int>(&walk_away_winnings),
      "winnings after which you'll walk away from the table");

  // Instantiate the "all options" object (combining the "visible options"
  // and "hidden options" objects)
  po::options_description all_options("All Options");
  all_options.add(visible_options).add(hidden_options);

  // Instantiate the "positional options" object, defining the REQUIRED
  // positional command-line parameters that must be provided by the user,
  // in order, as the last elements on the command line (these parameters
  // must also be defined above in the "hidden options" object)
  po::positional_options_description positional_options;
  positional_options.add("bet", 1);
  positional_options.add("bankroll", 1);
  positional_options.add("walk-away-winnings", 1);

  // Instantiate the Boost program options variable map (command line to
  // variable mapping definition)
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(all_options)
                .positional(positional_options)
                .run(),
            vm);
  po::notify(vm);

  // Display usage message to the standard output if the --help or -h
  // optional parameter is specified by the user (positional argument
  // names must be specified here explicitly)
  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] bet bankroll walk-away-winnings"
         << endl;
    cout << visible_options << endl;
    return EXIT_SUCCESS;
  }

  // *** ERROR CHECK ***
  // Check that multiple simulations and interactive mode are not specified
  // together (warn and exit if detected)
  if (number_of_simulations > 1 && interactive) {
    cout << "Multiple simulations and interactive mode not permitted" << endl;
    cout << endl;
    return EXIT_FAILURE;
  }

  // *** ERROR CHECK ***
  // Check if the requested number of simulations is less than or equal to 0
  // (warn and exit if detected)
  if (number_of_simulations < 1) {
    cout << "Number of simulations must be greater than or equal to 1" << endl;
    cout << endl;
    return EXIT_FAILURE;
  }

  // If the number of simulations is greater than 1, silence the per
  // game/session reporting dialog (setting the verbose flag to false)
  bool verbose = (number_of_simulations == 1) ? true : false;

  // Display the run-time parameters for the simulation
  cout << "----------------------------------------"
       << "----------------------------------------" << endl;
  cout << "Bet type: " << (pass ? "Pass" : "Don't Pass Bar 12") << endl;
  cout << "Place odds bet: " << (odds ? "Yes" : "No") << endl;
  cout << "Use Martingale betting system: "
       << (use_martingale_system ? "Yes" : "No") << endl;
  cout << "Interactive mode: " << (interactive ? "On" : "Off") << endl;
  cout << "Verbosity: " << (verbose ? "On" : "Off") << endl;
  cout << "Number of simulated sessions to perform: " << number_of_simulations
       << endl;
  cout << "Minimum bet: $" << minimum_bet_amount << endl;
  cout << "Initial bankroll: $" << initial_bankroll << endl;
  cout << "Winnings after which you will walk away: $" << walk_away_winnings
       << endl;
  cout << "----------------------------------------"
       << "----------------------------------------" << endl;
  cout << endl;

  vector<int> dice;
  std::srand(time(NULL));
  const int bankroll = initial_bankroll;
  const int bet_amount = minimum_bet_amount;
  int desired_bet_amount = 0;

  while (initial_bankroll < (walk_away_winnings + bankroll)) {
    if (initial_bankroll <= 0) {
      cout << "money is all gone :( you LOSE" << endl;
      initial_bankroll = (walk_away_winnings + bankroll);
      break;
    }
    cout << "Desired line bet: " << minimum_bet_amount << endl;
    if (minimum_bet_amount > initial_bankroll) {
      desired_bet_amount = initial_bankroll;
    } else {
      desired_bet_amount = minimum_bet_amount;
    }
    cout << "Line bet: " << desired_bet_amount << endl;
    int point = 0;

    // come out roll
    int come_out = roll_dice(dice);
    cout << "Come out roll: " << dice[0] << " + " << dice[1] << " = "
         << come_out << endl;
    dice.clear();

    // pass bet option
    if (pass == true) {

      /************************************
       * Martingale System
       * **********************************/

      if (use_martingale_system == true) {

        if (come_out == 7) {
          initial_bankroll += desired_bet_amount;
          minimum_bet_amount = bet_amount;
          cout << "Result: You rolled a 7 on the come out roll, you WIN! yeet"
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n') {
              break;
            }
          }
        } else if (come_out == 11) {
          initial_bankroll += desired_bet_amount;
          minimum_bet_amount = bet_amount;
          cout
              << "Result: You rolled an 11  on the come out roll, you WIN! yeet"
              << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n') {
              break;
            }
          }

        } else if (come_out == 2) {
          initial_bankroll -= desired_bet_amount;
          minimum_bet_amount = minimum_bet_amount * 2;
          cout << "Result: You rolled a 2 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n') {
              break;
            }
          }

        } else if (come_out == 3) {
          initial_bankroll -= desired_bet_amount;
          minimum_bet_amount = minimum_bet_amount * 2;
          cout << "Result: You rolled a 3 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n') {
              break;
            }
          }
        } else if (come_out == 12) {
          initial_bankroll -= desired_bet_amount;
          minimum_bet_amount = minimum_bet_amount * 2;
          cout << "Result: You rolled a 12 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n') {
              break;
            }
          }
        }

        // if point
        else {
          point = come_out;
          cout << "Point: " << point << endl;
          while (point > 0) {
            vector<int> dice1;
            int roll = roll_dice(dice1);
            cout << "Roll: " << dice1[0] << " + " << dice1[1] << " = " << roll
                 << endl;
            if (roll == 7) {
              initial_bankroll -= desired_bet_amount;
              minimum_bet_amount = minimum_bet_amount * 2;
              point = 0;
              cout << "You rolled a 7 before the point, you LOST rip" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n') {
                  break;
                }
              }
            } else if (roll == point) {
              initial_bankroll += desired_bet_amount;
              minimum_bet_amount = bet_amount;
              point = 0;
              cout << "You rolled the point, you WON! yay" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n') {
                  break;
                }
              }
            }
          }
        }

      }

      /*****************************
       * Regular system
       *****************************/

      else {

        if (come_out == 7) {
          initial_bankroll += minimum_bet_amount;
          cout << "Result: You rolled a 7 on the come out roll, you WIN! yeet"
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 11) {
          initial_bankroll += minimum_bet_amount;
          cout
              << "Result: You rolled an 11  on the come out roll, you WIN! yeet"
              << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 2) {
          initial_bankroll -= minimum_bet_amount;
          cout << "Result: You rolled a 2 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 3) {
          initial_bankroll -= minimum_bet_amount;
          cout << "Result: You rolled a 3 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 12) {
          initial_bankroll -= minimum_bet_amount;
          cout << "Result: You rolled a 12 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        }

        // if point
        else {
          point = come_out;
          cout << "Point: " << point << endl;
          while (point > 0) {
            vector<int> dice1;
            int roll = roll_dice(dice1);
            cout << "Roll: " << dice1[0] << " + " << dice1[1] << " = " << roll
                 << endl;
            if (roll == 7) {
              initial_bankroll -= minimum_bet_amount;
              point = 0;
              cout << "You rolled a 7 before the point, you LOST rip" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n')
                  break;
              }

            } else if (roll == come_out) {
              initial_bankroll += minimum_bet_amount;
              point = 0;
              cout << "You rolled the point, you WON! yay" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n')
                  break;
              }
            }
          }
        }
      }
    }

    // don't pass bet option

    if (pass == false) {

      /************************************
       * Martingale System
       * **********************************/

      if (use_martingale_system == true) {

        if (come_out == 7) {
          initial_bankroll -= desired_bet_amount;
          minimum_bet_amount = minimum_bet_amount * 2;
          cout << "Result: You rolled a 7 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 11) {
          initial_bankroll -= desired_bet_amount;
          minimum_bet_amount = minimum_bet_amount * 2;
          cout << "Result: You rolled an 11  on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 2) {
          initial_bankroll += desired_bet_amount;
          minimum_bet_amount = bet_amount;
          cout << "Result: You rolled a 2 on the come out roll, you WIN! yeet"
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 3) {
          initial_bankroll += desired_bet_amount;
          minimum_bet_amount = bet_amount;
          cout << "Result: You rolled a 3 on the come out roll, you WIN! yeet"
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 12) {
          cout << "Result: You rolled a 12 on the come out roll, pass" << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        }

        // if point
        else {
          point = come_out;
          cout << "Point: " << point << endl;
          while (point > 0) {
            vector<int> dice1;
            int roll = roll_dice(dice1);
            cout << "Roll: " << dice1[0] << " + " << dice1[1] << " = " << roll
                 << endl;
            if (roll == 7) {
              initial_bankroll += desired_bet_amount;
              minimum_bet_amount = bet_amount;
              point = 0;
              cout << "You rolled a 7 before the point, you WON! yay" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n')
                  break;
              }

            } else if (roll == point) {
              initial_bankroll -= desired_bet_amount;
              minimum_bet_amount = minimum_bet_amount * 2;
              point = 0;
              cout << "You rolled the point, you LOST rip" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n')
                  break;
              }
            }
          }
        }
      }

      /*******************************
       * Regular System
       * *****************************/
      else {

        if (come_out == 7) {
          initial_bankroll -= minimum_bet_amount;
          cout << "Result: You rolled a 7 on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 11) {
          initial_bankroll -= minimum_bet_amount;
          cout << "Result: You rolled an 11  on the come out roll, you LOSE :("
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 2) {
          initial_bankroll += minimum_bet_amount;
          cout << "Result: You rolled a 2 on the come out roll, you WIN! yeet"
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 3) {
          initial_bankroll += minimum_bet_amount;
          cout << "Result: You rolled a 3 on the come out roll, you WIN! yeet"
               << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        } else if (come_out == 12) {
          cout << "Result: You rolled a 12 on the come out roll, pass" << endl;
          cout << "Bankroll: " << initial_bankroll << endl;
          cout << endl;
          if (interactive == true) {
            if (cin.get() == '\n')
              break;
          }

        }

        // if point

        else {
          point = come_out;
          cout << "Point: " << point << endl;
          while (point > 0) {
            vector<int> dice1;
            int roll = roll_dice(dice1);
            cout << "roll: " << dice1[0] << " + " << dice1[1] << " = " << roll
                 << endl;
            if (roll == 7) {
              initial_bankroll += minimum_bet_amount;
              point = 0;
              cout << "You rolled a 7 before the point, you WON! yay" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n')
                  break;
              }

            } else if (roll == come_out) {
              initial_bankroll -= minimum_bet_amount;
              point = 0;
              cout << "You rolled the point, you LOST! rip" << endl;
              cout << "Bankroll: " << initial_bankroll << endl;
              cout << endl;
              if (interactive == true) {
                if (cin.get() == '\n')
                  break;
              }
            }
          }
        }
      }
    }
  }

  /**************************************
   *  Adding statistics (doesn't work qwq)
   *  *******************

  if (number_of_simulations == true){
  int win_count = 0;
  vector<double> games;
  games.reserve(number_of_simulations);
  int not_success = 0;
    for (auto idx = 0; idx < number_of_simulations; idx++){
     if (initial_bankroll = walk_away_winnings + bankroll) {
        initial_bankroll = bankroll;
      while (initial_bankroll < (walk_away_winnings + bankroll)){
        if (initial_bankroll < 0) {
        not_success += 1;
        initial_bankroll = bankroll;
         }
     int game_count = 0;
     games[idx] = game_count;
      int point = 0;
      int come_out = roll_dice(dice);
      dice.clear();

        if (pass == true) {
           if (come_out == 7) {
            initial_bankroll += minimum_bet_amount;
            minimum_bet_amount = bet_amount;
            win_count += 1;
            game_count += 1;
          } else if (come_out == 11) {
            initial_bankroll += minimum_bet_amount;
            minimum_bet_amount = bet_amount;
            win_count += 1;
            game_count += 1;

          } else if (come_out == 2) {
            initial_bankroll -= minimum_bet_amount;
            minimum_bet_amount = minimum_bet_amount * 2;
            game_count += 1;

          } else if (come_out == 3) {
            initial_bankroll -= minimum_bet_amount;
            minimum_bet_amount = minimum_bet_amount * 2;
            game_count += 1;
          } else if (come_out == 12) {
            initial_bankroll -= minimum_bet_amount;
            minimum_bet_amount = minimum_bet_amount * 2;
            game_count += 1;

          }

  // if point
          else {
            point = come_out;
            while (point > 0) {
              vector<int> dice1;
              int roll = roll_dice(dice1);
              if (roll == 7) {
                initial_bankroll -= minimum_bet_amount;
                minimum_bet_amount = minimum_bet_amount * 2;
                point = 0;
                game_count += 1;
              } else if (roll == point) {
                initial_bankroll += minimum_bet_amount;
                minimum_bet_amount = bet_amount;
                point = 0;
                game_count += 1;
                win_count += 1;
              }
            }
          }
        }

  }
  }

  cout << "games won: " << win_count << endl;
  double total_games = accumulate(games.begin(),games.end(),0);
  cout << "average games: " << total_games / number_of_simulations << endl;
  }
  */

  return EXIT_SUCCESS;
}
