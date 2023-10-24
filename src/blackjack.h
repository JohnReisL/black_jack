#ifndef BLACKJACK_H
# define BLACKJACK_H

#include <algorithm>
#include <random>
#include <vector>

namespace jrl {

class Card {
 public:
  enum class Rank: unsigned {
    kTwo = 0,  kThree = 1, kFour = 2,   kFive = 3,
    kSix = 4,  kSeven = 5, kEight = 6,  kNine = 7,
    kTen = 8,  kJack = 9,  kQueen = 10, kKing  = 11,
    kAce = 12, kAmount = 13
  };
  enum class Suit: unsigned {
    kHearts = 0, kSpades = 1, kDiamonds = 2, kClubs = 3,
    kAmount = 4
  };
  explicit Card (unsigned id): id_(id) {} //TODO кто такой этот ваш эксплисит
  inline Rank GetRank() const {
    return static_cast<Rank>(id_ % static_cast<unsigned>(Rank::kAmount));
  }
  inline Suit GetSuit() const {
    return static_cast<Suit>((id_ / static_cast<unsigned>(Rank::kAmount))
                             % static_cast<unsigned>(Suit::kAmount));
  }
  unsigned GetId() const {
    constexpr unsigned kRankAmount = static_cast<unsigned>(Rank::kAmount);
    constexpr unsigned kSuitAmount = static_cast<unsigned>(Suit::kAmount);
    return id_ % (kRankAmount * kSuitAmount);
  }

 private:
  unsigned id_;
};

class Hand {
 public:
  unsigned GetTotal() const {
    unsigned sum = 0;
    unsigned aces = 0;
    for (size_t i = 0; i < cards.size(); ++i) {
      switch (cards[i].GetRank()) {
      case jrl::Card::Rank::kTwo:
      case jrl::Card::Rank::kThree:
      case jrl::Card::Rank::kFour:
      case jrl::Card::Rank::kFive:
      case jrl::Card::Rank::kSix:
      case jrl::Card::Rank::kSeven:
      case jrl::Card::Rank::kEight:
      case jrl::Card::Rank::kNine:
      case jrl::Card::Rank::kTen:
        sum += static_cast<unsigned>(cards[i].GetRank()) + 2;
        break;
      case jrl::Card::Rank::kJack :
      case jrl::Card::Rank::kQueen :
      case jrl::Card::Rank::kKing :
        sum += 10;
        break;
      case jrl::Card::Rank::kAce :
        aces += 1;
        break;
      default:;
      }
    }
    if (aces > 0)
      sum += ((sum + 11 + aces - 1) <= 21) ? (11 + aces - 1) : aces;
    return sum;
  }
  void Add(Card card) {
    cards.emplace_back(card);
  }
  void Clear() {
    cards.clear();
  }
  std::vector<Card> cards;
};

class Dealer: public Hand {
 public:

};

class Player: public Hand {
 public:
  Player(std::string name): name(name) {
    balance = 100;
  }
  void SetBet(unsigned money) {
    if (money < balance) {
      money_to_lose = money;
      balance -= money_to_lose;
    } else {
      money_to_lose = balance;
      balance = 0;
    }
  }
  
  std::string name;
  unsigned balance;
  unsigned money_to_lose;
};

class Deck {
 public:
  void Seed(unsigned seed = 0) {
    if (seed == 0) {
      std::random_device rd;
      rand_.seed(rd()); // TODO: Что Такое operator () ?!?
    } else {
      rand_.seed(seed);
    }
  }
  void PrepareDeck(unsigned decks_number) {
    deck_.clear();
    for (size_t i = 0; i < (decks_number * 52); ++i) {
      deck_.emplace_back(i);
    }
  }
  void Shuffle() {
    std::shuffle(deck_.begin(), deck_.end(), rand_); // TODO итератор это кто
  }
  inline void Deal(Hand& hand) {
    if (deck_.size() == 0) return;
    hand.Add(deck_.back());
    deck_.pop_back();
  }
  inline unsigned TotalCards() const {
    return deck_.size();
  }
  
 private:
  std::mt19937 rand_;
  std::vector<Card> deck_;
};

class Game {
 public:
  inline void ShuffleCardsInDeck() {
    deck.Shuffle();
  }
  inline bool IsEnoughCards() const {
    return (deck.TotalCards() > kLowCardLevel);
  }
  inline void SetNumberOfDecks (unsigned n) {
    deck.PrepareDeck(n);
  }
  void GiveFirstCards() {
    dealer.Clear();
    for (auto& it: player) {
      it.Clear();
    }
    deck.Deal(dealer);
    deck.Deal(dealer);
    for (auto& it: player) {
      deck.Deal(it);
      deck.Deal(it);
    }
  }
  void DealerPlay() {
    while (true) {
      if (dealer.GetTotal() < 17) {
        deck.Deal(dealer);
      } else {
        break;
      }
    }
    if (dealer.GetTotal() <= 21) {
      DealerWin(dealer.GetTotal());
    } else {
      DealerLose();
    }
    for (size_t i = 0; i < player.size(); ++i) {
      if (player[i].balance == 0) {
        player.erase(player.begin() + i--);
      }
    }
  }
  
  static unsigned const kLowCardLevel = 14;
  Deck deck;
  Dealer dealer;
  std::vector<Player> player;

 private:
  void DealerWin(unsigned dealer_total) {
    for (auto& it : player) {
      if ((it.GetTotal() <= 21) && (it.GetTotal() > dealer_total)) {
        it.balance += (it.money_to_lose * 2);
      }
      if (it.GetTotal() == dealer_total)
        it.balance += it.money_to_lose;
      it.money_to_lose = 0;
    }
  }
  void DealerLose() {
    for (auto& it : player) {
      if (it.GetTotal() <= 21) {
        it.balance += (it.money_to_lose * 2);
      } 
      it.money_to_lose = 0;
    }
  }
};

} // namespace jrl

#endif // BLACKJACK_H
