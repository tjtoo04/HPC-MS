#include <iostream>
#include "supply_item.hpp"
#include "stack.hpp"

Stack<SupplyItem> itemStack;

int getIntInput(const std::string &prompt, int min, int max)
{
  int choice;
  while (true)
  {
    std::cout << prompt;
    if (std::cin >> choice)
    {
      if (choice >= min && choice <= max)
      {
        if (std::cin.peek() == '\n') std::cin.ignore();
        return choice;
      }
      std::cout << "Invalid choice. Please enter between " << min << " and " << max << ".\n";
    }
    else
    {
      std::cout << "Invalid input. Please enter a number.\n";
      std::cin.clear();
      std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); 
    }
  }
}

void addItems() {
  int maxSize = 10;
  int itemIndex = 0;
  SupplyItem* temp = new SupplyItem[maxSize];
  std::string batchId;
  int id = FileIO::getFileCount("supplyitem");
  std::string testInput;
  bool isContinue = true;

  std::cout << "Enter batch id: ";
  std::getline(std::cin, batchId);

  do {
    std::string itemType;
    int quantity;

    std::cout << "Enter item: ";
    std::getline(std::cin, itemType);

    std::cout << "Enter quantity: ";
    std::cin >> quantity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (itemIndex >= maxSize) {
      int newMaxSize = maxSize * 2;
      SupplyItem* newTemp = new SupplyItem[newMaxSize];
      for (int i = 0; i < maxSize; ++i) {
        newTemp[i] = temp[i];
      }
      delete[] temp;
      temp = newTemp;
      maxSize = newMaxSize;
    }

    temp[itemIndex++] = SupplyItem(std::to_string(++id), itemType, quantity, batchId);

    std::cout << "Continue? (press Enter to continue, or any key then Enter to stop): ";
    std::getline(std::cin, testInput);

    if (!testInput.empty()) {
      std::cout << "Loading into stack..." << std::endl;
      itemStack.pushItemsIntoStack(temp, itemIndex);
      FileIO::appendObjectsToFile("../core_library/include/data/supply_item.txt", temp, itemIndex);
      FileIO::updateFileCount("supplyitem", id);

      isContinue = false;
    }

  } while (isContinue);

  std::cout << "You entered " << itemIndex << " items.\n";
  for (int i = 0; i < itemIndex; ++i) {
    temp[i].displaySupplyItem();
  }

  delete[] temp;
}


void useLastAddedItem() {
  std::string test;
  SupplyItem lastItem = itemStack.peek();
  lastItem.displaySupplyItem();
  std::cout << "This is the last item, do you want to use it? (press enter to confirm)" << std::endl;
  std::getline(std::cin, test);
  if (test.empty()) {
    int id = FileIO::getFileCount("supplyitem");
    itemStack.pop();
    // update text file, update index file
    FileIO::removeLastLineFromFile("../core_library/include/data/supply_item.txt");
    FileIO::updateFileCount("supplyitem", --id);
  }
}

void displayAllStackItems() {
  itemStack.displayStackItems();
}

void displayLogo() {
  std::cout << "  /$$$$$$   /$$                         /$$      " << std::endl;
  std::cout << " /$$__  $$ | $$                        | $$      " << std::endl;
  std::cout << "| $$  \\__//$$$$$$    /$$$$$$   /$$$$$$$| $$   /$$" << std::endl;
  std::cout << "|  $$$$$$|_  $$_/   |____  $$ /$$_____/| $$  /$$/" << std::endl;
  std::cout << " \\____  $$ | $$      /$$$$$$$| $$      | $$$$$$/ " << std::endl;
  std::cout << " /$$  \\ $$ | $$ /$$ /$$__  $$| $$      | $$_  $$ " << std::endl;
  std::cout << "|  $$$$$$/ |  $$$$/|  $$$$$$$|  $$$$$$$| $$ \\  $$" << std::endl;
  std::cout << " \\______/   \\___/   \\_______/ \\_______/|__/  \\__/" << std::endl;
  std::cout << "                                                 " << std::endl;
}

void displayMainMenu() {
  cout << "╔════════════════════════════════════════╗" << endl;
  cout << "║ \033[1;31m1. Add items to stack\033[0m                  ║" << std::endl;
  cout << "║ \033[1;33m2. Use Last Added Item\033[0m                 ║" << std::endl;
  cout << "║ \033[1;36m3. Display all stack items\033[0m             ║" << std::endl;
  cout << "║ \033[1;32m4. Back to main menu\033[0m                   ║" << std::endl;
  cout << "╚════════════════════════════════════════╝" << endl;
}

void runStackProgram() {
  // read and load
  itemStack.loadDataIntoStack();
  bool isContinue = true;
  do {
    displayLogo();
    displayMainMenu();
    int choice = getIntInput("Please select an option: ", 1, 4);
    switch (choice) {
      case 1:
        addItems();
        break;
      case 2:
        useLastAddedItem();
        break;
      case 3:
        displayAllStackItems();
        break;
      case 4:
        isContinue = false;
    }
  } while (isContinue);
  
}