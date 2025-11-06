#include "stack.hpp"
#include "supply_item.hpp"
#include <iostream>
#include <limits>

Stack<SupplyItem> itemStack;

void addItems() {
  int max = 10;
  int index = 0;
  SupplyItem *temp = new SupplyItem[max];
  std::string batchId;
  int id = FileIO::getFileCount("supplyitem");
  std::string test;
  bool isContinue = true;

  std::cout << "Enter batch id: ";
  std::getline(std::cin, batchId);

  do {
    std::string type;
    int quantity;

    std::cout << "enter item: ";
    std::getline(std::cin, type);

    std::cout << "enter quantity: ";
    std::cin >> quantity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index < max) {
      temp[index++] = SupplyItem(std::to_string(++id), type, quantity, batchId);
    } else {
      int newMax = max * 2;
      SupplyItem *newTemp = new SupplyItem[newMax];
      for (int i = 0; i < max; i++) {
        newTemp[i] = temp[i];
      }
      delete[] temp;
      temp = newTemp;
      max = newMax;
    }

    std::cout << "continue? (press enter to continue): ";
    std::getline(std::cin, test);

    if (!test.empty()) {
      // load into stack
      std::cout << "Loading into stack..." << std::endl;
      itemStack.pushItemsIntoStack(temp, max);
      // update text file
      FileIO::appendObjectsToFile(
          "../core_library/include/data/supply_item.txt", temp, index);
      // update index file
      FileIO::updateFileCount("supplyitem", id);

      // finally exit
      isContinue = false;
    }

  } while (isContinue);

  std::cout << "You entered " << index << " items.\n";
  for (int i = 0; i < index; ++i) {
    temp[i].displaySupplyItem();
  }
  delete[] temp;
}

void runStackProgram() {
  // read and load
  itemStack.loadDataIntoStack();
  std::cout << "Test" << endl;
  ;
  itemStack.displayStackItems();
  // add data into ding dong stack
  addItems();
}
