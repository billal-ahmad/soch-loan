#include "home-loan.h"

// Default constructor
HomeLoan::HomeLoan() 
    : _area("Unknown"), _size("Unknown"), _installments(0), _price("0"), _down_payment("0") {}

// Parameterized constructor
HomeLoan::HomeLoan(std::string area, std::string size, int installments, 
                   std::string price, std::string down_payment)
    : _area(area), _size(size), _installments(installments), 
      _price(price), _down_payment(down_payment) {
    
    // Basic validation
    if (_area.empty()) _area = "Unknown Area";
    if (_size.empty()) _size = "Unknown Size";
    if (_installments < 0) _installments = 0;
    if (_price.empty()) _price = "0";
    if (_down_payment.empty()) _down_payment = "0";
}

// Getter methods WITH CONST
std::string HomeLoan::getArea() const {
    return _area.empty() ? "Area Not Specified" : _area;
}

std::string HomeLoan::getSize() const {
    return _size.empty() ? "Size Not Specified" : _size;
}

int HomeLoan::getInstallments() const {
    return _installments < 0 ? 0 : _installments;
}

std::string HomeLoan::getPrice() const {
    return _price.empty() ? "0" : _price;
}

std::string HomeLoan::getDownPayment() const {
    return _down_payment.empty() ? "0" : _down_payment;
}
