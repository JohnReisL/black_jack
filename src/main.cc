#include <iostream>
#include <vector>
#include "blackjack.h"

static const std::string cards[] = {
  "🂲", "🂳", "🂴", "🂵", "🂶", "🂷", "🂸", "🂹", "🂺", "🂻", "🂽", "🂾", "🂱",
  "🂢", "🂣", "🂤", "🂥", "🂦", "🂧", "🂨", "🂩", "🂪", "🂫", "🂭", "🂮", "🂡",
  "🃂", "🃃", "🃄", "🃅", "🃆", "🃇", "🃈", "🃉", "🃊", "🃋", "🃍", "🃎", "🃁",
  "🃒", "🃓", "🃔", "🃕", "🃖", "🃗", "🃘", "🃙", "🃚", "🃛", "🃝", "🃞", "🃑",
  "🂠"
};

void ShowDeck(jrl::Game const& game) {
  //system("clear");
  
  std::cout << "\nDealer: " << cards[52] << " ";

  auto const& tmp = game.dealer.cards[1];
  switch (tmp.GetSuit()) {
  case jrl::Card::Suit::kSpades:
  case jrl::Card::Suit::kClubs:
    std::cout << cards[tmp.GetId()] << " ";
    break;
  case jrl::Card::Suit::kHearts:
  case jrl::Card::Suit::kDiamonds:
    std::cout << "\x1b[31;1m" << cards[tmp.GetId()] << "\033[0m ";
    break;
  default:;
  }
  std::cout << std::endl;
  
  for (size_t i = 0; i < game.player.size(); ++i) {
    std::cout << game.player[i].name << ": ";
    for (auto const& it : game.player[i].cards ) {
      switch (it.GetSuit()) {
      case jrl::Card::Suit::kSpades:
      case jrl::Card::Suit::kClubs:
        std::cout << cards[it.GetId()] << " ";
        break;
      case jrl::Card::Suit::kHearts:
      case jrl::Card::Suit::kDiamonds:
        std::cout << "\x1b[31;1m" << cards[it.GetId()] << "\033[0m ";
        break;
      default:;
      }
    }
    std::cout << "(" << game.player[i].GetTotal() << " баллов)" << std::endl;
  }
}

void ShowDealerCards(jrl::Game const& game) {
  std::cout << "Карты диллера: ";
  for (auto const& it : game.dealer.cards ) {
    switch (it.GetSuit()) {
    case jrl::Card::Suit::kSpades:
    case jrl::Card::Suit::kClubs:
      std::cout << cards[it.GetId()] << " ";
      break;
    case jrl::Card::Suit::kHearts:
    case jrl::Card::Suit::kDiamonds:
      std::cout << "\x1b[31;1m" << cards[it.GetId()] << "\033[0m ";
      break;
    default:;
    }
  }
  std::cout << "(" << game.dealer.GetTotal() << " баллов)" << std::endl;
}

int main() {
  jrl::Game game;
  bool play_game = 1;

  std::cout << "Игроки, представьтесь\n";
  while (game.player.size() < 6) {
    std::string tmp = "";
    std::getline(std::cin, tmp);
    if (tmp == "") {
      if (game.player.size() == 0) {
        game.player.emplace_back("Гэймер(^._.^)/");
      } 
      break;
    } else {
      game.player.emplace_back(tmp);
    }
  }
  
  game.SetNumberOfDecks(1); // Установим стандартное число колод в начале игры.
  
  while (play_game) {
    if (!game.IsEnoughCards()) {
      std::cout << "Число карт слишком мало, "
                << "введите число колод для следующей игры\n";

      std::string input;
      std::getline(std::cin, input);

      if((input.size() == 0) or (input == "0")) {
	input = "incorrect";
      }

      while(input.find_first_not_of("0123456789") != std::string::npos) {
	std::cout << "Число колод должно быть целым положительным числом\n"
		  << "Введите число колод: ";
	std::getline(std::cin, input);
	if((input.size() == 0) or (input == "0")) {
	  input = "incorrect";
	}
      }
      game.SetNumberOfDecks(std::stoi(input));
    }

    game.ShuffleCardsInDeck();
    game.GiveFirstCards();

    for (auto& it : game.player) {
      std::cout << "Игрок: " << it.name << ", баланс: " << it.balance
                << "₽, сделайте ставку\n";

      std::string input;
      std::getline(std::cin, input);

      if((input.size() == 0) or (input == "0")) {
	input = "incorrect";
      }

      while(input.find_first_not_of("0123456789") != std::string::npos) {
	std::cout << "Число должно быть целым и положительным\n";
	std::getline(std::cin, input);
	if((input.size() == 0) or (input == "0")) {
	  input = "incorrect";
	}
      }      
      it.SetBet(std::stoi(input));
    }
    
    for (size_t i = 0; i < game.player.size(); ++i) {
      while (game.player[i].GetTotal() < 21) {
        ShowDeck(game);
        std::string str = "";
        std::cout << "Игрок " << game.player[i].name << ", "
                  << "желаете ли взять ещё одну карту?\nyes - да, no - нет\n";
        std::cin >> str;
        if ((str == "yes") || (str == "y") || (str == "да") || (str == "д")) {
          game.deck.Deal(game.player[i]);
        }
        if ((str == "no") || (str == "n") || (str == "нет") || (str == "н")) {
          break;
        }
        std::cout << "Неизвестная команда\n";
      }
    }
    ShowDeck(game);
    std::cout << "Ходы игроков подошли к концу\n\n";
    game.DealerPlay();
    ShowDealerCards(game);

    for (auto& it : game.player) {
      std::cout << "\nИгрок: " << it.name << ", баланс: " << it.balance
                << std::endl;
    }
    
    if (game.player.size() == 0) {
      std::cout << "Все игроки выбыли\n";
      break;
    }
    
    std::string str = "";
    while (true) {
      std::cout << "\nСыграть ещё одну игру?\nyes - да, no - нет\n";
      std::cin >> str;
      if ((str == "yes") || (str == "y") || (str == "да") || (str == "д")) {
        play_game = 1;
        break;
      }
      if ((str == "no") || (str == "n") || (str == "нет") || (str == "н")) {
        play_game = 0;
        break;
      }
      std::cout << "Неизвестная команда\n";
    }
  } // end of play_game
  
  return 0;
}
