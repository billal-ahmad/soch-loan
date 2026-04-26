#include "car-loan.h"

CarLoan::CarLoan() : _installments(0) {}

CarLoan::CarLoan(std::string make, std::string model, std::string engine, std::string used, std::string year, int installments, std::string price, std::string down_payment)
    : _make(make), _model(model), _engine(engine), _used(used), _manufacturing_year(year), _installments(installments), _price(price), _down_payment(down_payment) {}

std::string CarLoan::getMake() const { return _make; }
std::string CarLoan::getModel() const { return _model; }
std::string CarLoan::getEngine() const { return _engine; }
std::string CarLoan::getUsed() const { return _used; }
std::string CarLoan::getManufacturingYear() const { return _manufacturing_year; }
int CarLoan::getInstallments() const { return _installments; }
std::string CarLoan::getPrice() const { return _price; }
std::string CarLoan::getDownPayment() const { return _down_payment; }
