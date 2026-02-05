#include <iostream>
#include <string>
#include <random>
#include <stdexcept>
using namespace std;

class Character {
  protected:
    string name;
    int hp;
    int attackCount;
    int strongAttackCount;
    int defenseCount;
    int potionCount;
    bool isDefending;

  public:
    Character(string n, int h, int a, int sa, int d, int p) {
      name = n;
      hp = h;
      attackCount = a;
      strongAttackCount = sa;
      defenseCount = d;
      potionCount = p;
      isDefending = false;
    }

  virtual ~Character() {}

  void attack(Character &target) {
    if (attackCount <= 0) 
      throw runtime_error("No attack left!");
    int damage = 10;
    if (target.isDefending) damage /= 2;

    target.hp -= damage;
    if (target.hp < 0)
      target.hp = 0;
    attackCount--;
    target.isDefending = false;
  }

  void strongAttack(Character &target) {
    if (strongAttackCount <= 0) 
      throw runtime_error("No strong attack left!");

    int damage = 20;
    if (target.isDefending)
      damage /= 2;

    target.hp -= damage;
    if (target.hp < 0)
      target.hp = 0;
    strongAttackCount--;
    target.isDefending = false;
  }

  void defend() {
    if (defenseCount <= 0)
      throw runtime_error("No defense left!");

    isDefending = true;
    defenseCount--;
  }

  void usePotion() {
    if (potionCount <= 0)
      throw runtime_error("No potion left!");

    hp += 20;
    potionCount--;
  }

  int randomNumber() {
    random_device rd;
    mt19937 rng(rd());

    uniform_int_distribution<int> dist(1, 4);
    int random = dist(rng);

    return random;
  }

  // auto attack for enemy AI
  void autoAttack(Character &target) {
    if (isFinished()) return;

    bool moveMade = false;
    while (!moveMade) {
      int choice = randomNumber();

      switch (choice) {
        case 1:
          if (attackCount > 0) {
            attack(target);
            cout << "\nEnemy used Attack!\n\n";
            moveMade = true;
          }
          break;
        case 2:
          if (strongAttackCount > 0) {
            strongAttack(target);
            cout << "\nEnemy used Strong Attack!\n\n";
            moveMade = true;
          }
          break;
        case 3:
          if (defenseCount > 0) {
            defend();
            cout << "\nEnemy is Defending!\n\n";
            moveMade = true;
          }
          break;
        case 4:
          if (potionCount > 0) {
            usePotion();
            cout << "\nEnemy used a Potion!\n\n";
            moveMade = true;
          }
          break;
      }
    }
}

  string getName() const { return name; }
  int getHP() const { return hp; }
  int getAttackCount() const { return attackCount; }
  int getStrongAttackCount() const { return strongAttackCount; }
  int getDefenseCount() const { return defenseCount; }
  int getPotionCount() const { return potionCount; }

  bool isFinished() const {
    return (attackCount == 0 && strongAttackCount == 0 && defenseCount == 0 && potionCount == 0) || hp <= 0;
  }

  bool operator > (Character &other) {
    return hp > other.hp;
  }

  bool operator == (Character &other) {
    return hp == other.hp;
  }

  friend void showName(Character &c);
  friend void showHP(Character &c);
  friend void showCounts(Character &c);
};

class Hero : public Character {
  public:
    Hero() : Character("Hero", 100, 4, 5, 3, 3) {}
};

class Enemy : public Character {
  public:
    Enemy() : Character("Enemy", 100, 4, 5, 3, 3) {}
};

void showHP(Character &c) {
  cout << c.name << " HP: " << c.hp << endl;
}

template <typename T>
void displayCount(const T &c) {
  cout << c.getName() << " : Attack Count Left: " << c.getAttackCount() << endl;
  cout << c.getName() << " : Strong Attack Count Left: " << c.getStrongAttackCount() << endl;
  cout << c.getName() << " : Defense Count Left: " << c.getDefenseCount() << endl;
  cout << c.getName() << " : Potion Count Left: " << c.getPotionCount() << endl << endl;
}

template <typename T>
void displayStats(const T &c) {
  cout << "HP: " << c.getHP() << endl;
}

int main() {
  Hero hero;
  Enemy enemy;

  int choice;
  while (!hero.isFinished() && !enemy.isFinished()) {
    try {
      cout << "\n1. Attack \n2. Strong Attack \n3. Defense \n4. Potion \nChoose: ";
      cin >> choice;

      if (cin.fail())
        throw runtime_error("Invalid Input!");
      
      switch (choice) {
        case 1:
          hero.attack(enemy);
          break;
        
        case 2:
          hero.strongAttack(enemy);
          break;
        
        case 3:
          hero.defend();
          break;
        
        case 4:
          hero.usePotion();
          break;

        default:
          throw runtime_error("Wrong Choice!!!");
      }

      if (!enemy.isFinished()) {
        enemy.autoAttack(hero);
      }

      showHP(hero);
      showHP(enemy);
      cout << endl;
      displayCount(hero);
      displayCount(enemy);
    } catch (exception &e) {
      cout << "Error: " << e.what() << endl;
      cin.clear();
      cin.ignore(100, '\n');
    }
  }

  cout << "\n========= Battle Over ===========\n";

  if (hero == enemy) {
    cout << "Draw! No One Wins!" << endl;
  }

  if (hero > enemy) {
    cout << "Hero Wins!\n";
  } else {
    cout << "Enemy Wins!\n";
  }

  cout << "Hero HP: " << hero.getHP() << endl;
  cout << "Enemy HP: " << enemy.getHP() << endl;

  return 0;
}