#ifndef SUPPLY_ITEM_HPP
#define SUPPLY_ITEM_HPP

#include <string>
#include <iostream>
#include "FileIO.hpp"

struct SupplyItem {
  std::string id;
  std::string type;
  int quantity;
  std::string batch;

  static const int MAX_LINES = 1000;

  SupplyItem() : id(""), type(""), quantity(0), batch("") {}
  SupplyItem(std::string id, std::string type, int quantity, std::string batch) {
    this->id = "SI" + id;
    this->type = type;
    this->quantity = quantity;
    this->batch = batch;
  }
  
  void displaySupplyItem() {
    std::cout << this->id << std::endl;
    std::cout << this->type << std::endl;
    std::cout << this->quantity << std::endl;
    std::cout << this->batch << std::endl;
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