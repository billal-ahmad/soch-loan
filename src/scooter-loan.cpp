#include "scooter-loan.h"

ScooterLoan::ScooterLoan() : _installments(0) {}

ScooterLoan::ScooterLoan(std::string make, std::string model, std::string distance, std::string charging_time, std::string max_speed, int installments, std::string price, std::string down_payment)
    : _make(make), _model(model), _distance(distance), _charging_time(charging_time), _max_speed(max_speed), _installments(installments), _price(price), _down_payment(down_payment) {}

std::string ScooterLoan::getMake() const { return _make; }
std::string ScooterLoan::getModel() const { return _model; }
std::string ScooterLoan::getDistance() const { return _distance; }
std::string ScooterLoan::getChargingTime() const { return _charging_time; }
std::string ScooterLoan::getMaxSpeed() const { return _max_speed; }
int ScooterLoan::getInstallments() const { return _installments; }
std::string ScooterLoan::getPrice() const { return _price; }
std::string ScooterLoan::getDownPayment() const { return _down_payment; }
