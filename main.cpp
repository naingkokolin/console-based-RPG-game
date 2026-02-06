#include <iostream>
#include <string>
#include <random>
#include <stdexcept>
#include <conio.h>

using namespace std;

class Character
{
protected:
  string name;
  int hp, atk, satk, def, pot;
  bool isBlocking;

public:
  Character(string n, int h, int a, int sa, int d, int p)
      : name(n), hp(h), atk(a), satk(sa), def(d), pot(p), isBlocking(false) {}

  int commitAction(int choice)
  {
    if (choice == 1)
    {
      if (atk <= 0)
        throw runtime_error("No Attack charges!");
      atk--;
      return 10;
    }
    if (choice == 2)
    {
      if (satk <= 0)
        throw runtime_error("No Strong Attack charges!");
      satk--;
      return 20;
    }
    if (choice == 3)
    {
      if (def <= 0)
        throw runtime_error("No Defense charges!");
      def--;
      isBlocking = true;
      return 0;
    }
    if (choice == 4)
    {
      if (pot <= 0)
        throw runtime_error("No Potions left!");
      pot--;
      hp += 20;
      return -1;
    }
    throw runtime_error("Invalid Choice!");
  }

  void applyDamage(int dmg)
  {
    if (dmg <= 0)
      return;
    if (isBlocking)
    {
      dmg /= 2;
      cout << ">> " << name << " blocked! Damage reduced to " << dmg << endl;
    }
    hp -= dmg;
    if (hp < 0)
      hp = 0;
  }

  void refresh() { isBlocking = false; }

  int getHP() const { return hp; }
  int getAtk() const { return atk; }
  int getStrongAtk() const { return satk; }
  int getDef() const { return def; }
  int getPot() const { return pot; }

  bool alive() const { return hp > 0; }
  bool canMove() const { return (atk > 0 || satk > 0 || def > 0 || pot > 0); }
  string getName() const { return name; }
  bool operator>(const Character &other) const { return hp > other.hp; }
  friend void showHP(Character &c);
};

void showHP(Character &character) {
  cout << "\n"
       << character.getName() << " HP: " << character.hp;
}

template <typename T>
void printStats(const T &c)
{
  cout << c.getName() << ": "
       << c.getHP() << " HP | "
       << "Atk:" << c.getAtk() << " "
       << "S.Atk:" << c.getStrongAtk() << " "
       << "Def:" << c.getDef() << " "
       << "Pot:" << c.getPot()
       << endl;
}

class Hero : public Character
{
public:
  Hero() : Character("Hero", 100, 4, 5, 3, 3) {}
};
class Enemy : public Character
{
public:
  Enemy() : Character("Enemy", 100, 4, 5, 3, 3) {}
};

int main()
{
  Hero h;
  Enemy e;
  mt19937 rng(random_device{}());

  while (h.alive() && e.alive() && (h.canMove() || e.canMove()))
  {
    cout << "\n--- BATTLE STATUS ---" << endl;
    printStats(h);
    printStats(e);

    try
    {
      cout << "\nChoices: 1:Atk, 2:S.Atk, 3:Def, 4:Pot" << endl;
      cout << "Choice: ";
      int hChoice;
      if (!(cin >> hChoice))
        throw runtime_error("Input error!");

      int hDmg = h.commitAction(hChoice);

      int eChoice = 0, eDmg = 0;
      bool eValid = false;
      while (!eValid)
      {
        eChoice = uniform_int_distribution<int>(1, 4)(rng);
        try
        {
          eDmg = e.commitAction(eChoice);
          eValid = true;
        }
        catch (exception &e)
        {
          cout << "Error: " << e.what() << endl;
          cin.clear();
          cin.ignore(100, '\n');
        }
      }

      cout << "\n[RESULT]" << endl;
      if (hChoice == 4)
        cout << ">> Hero used a Potion!" << endl;
      if (eChoice == 4)
        cout << ">> Enemy used a Potion!" << endl;

      e.applyDamage(hDmg);
      h.applyDamage(eDmg);

      if (hDmg > 0)
        cout << ">> Hero dealt " << hDmg << " base damage." << endl;
      if (eDmg > 0)
        cout << ">> Enemy dealt " << eDmg << " base damage." << endl;

      h.refresh();
      e.refresh();
    }
    catch (const exception &err)
    {
      cout << "Error: " << err.what() << endl;
      cin.clear();
      cin.ignore(100, '\n');
    }
  }

  cout << "\n=== FINAL RESULT ===" << endl;
  showHP(h);
  showHP(e);
  if (h > e)
    cout << "\nWinner: Hero!" << endl;
  else if (e > h)
    cout << "\nWinner: Enemy!" << endl;
  else
    cout << "\nIt's a Draw!" << endl;

  return 0;
}