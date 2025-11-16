#ifndef SUPPLY_ITEM_HPP
#define SUPPLY_ITEM_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "FileIO.hpp"

struct SupplyItem {
  std::string id;
  std::string type;
  int quantity;
  std::string batch;

  static const int MAX_LINES = 1000;

  SupplyItem() : id(""), type(""), quantity(0), batch("") {}
  SupplyItem(std::string id, std::string type, int quantity, std::string batch) {
    // this->id = "SI" + id;
    this->id = id;
    this->type = type;
    this->quantity = quantity;
    this->batch = batch;
  }
  
void displaySupplyItem() {
    std::ostringstream idLine, typeLine, quantityLine, batchLine;
    idLine << "ID: " << this->id;
    typeLine << "Type: " << this->type;
    quantityLine << "Quantity: " << this->quantity;
    batchLine << "Batch: " << this->batch;

    std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;

    std::cout << "║ " << std::left << std::setw(38) << idLine.str() << "║" << std::endl;
    std::cout << "║ " << std::left << std::setw(38) << typeLine.str() << "║" << std::endl;
    std::cout << "║ " << std::left << std::setw(38) << quantityLine.str() << "║" << std::endl;
    std::cout << "║ " << std::left << std::setw(38) << batchLine.str() << "║" << std::endl;

    std::cout << "╚═══════════════════════════════════════╝" << std::endl;
}

  std::string toString() {
    std::string delimiter = ",";
    return 
      this->id + delimiter + 
      this->type + delimiter + 
      std::to_string(this->quantity) + delimiter + 
      this->batch;
  }
};
  
#endif