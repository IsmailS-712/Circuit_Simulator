//
// Created by Oliver on 12/05/2021.
//

#ifndef ICL_CIRCUIT_SIMULATOR_COMPONENTS_H
#define ICL_CIRCUIT_SIMULATOR_COMPONENTS_H

#include <vector>
#include <string>
#include <sstream>
#include <iterator>


struct ParseReturn {
    std::string identifier;
    std::vector<std::string> nodes;
    std::string value;
};


class Component {
public:
    static const char identifier;

    int get_id();
    std::string get_value();
    static bool represented_by(const std::string& token_str);

protected:
    int id;
    std::string value;
    ParseReturn* parse(const std::string& token_str);
};


class IndependentVoltageSource: public Component {
public:
    static const char identifier = 'V';

    IndependentVoltageSource(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string positive_node;
    std::string negative_node;
};


class CurrentSource: public Component {
public:
    static const char identifier = 'I';

    CurrentSource(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Resistor: public Component {
public:
    static const char identifier = 'R';

    Resistor(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Capacitor: public Component {
public:
    static const char identifier = 'C';

    Capacitor(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Inductor: public Component {
public:
    static const char identifier = 'L';

    Inductor(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Diode: public Component {
public:
    static const char identifier = 'I';

    Diode(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string anode;
    std::string cathode;
};


class BipolarJunctionTransistor: public Component {
public:
    static const char identifier = 'Q';

    BipolarJunctionTransistor(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string collector;
    std::string base;
    std::string emitter;
};


class Mosfet: public Component {
public:
    static const char identifier = 'M';

    Mosfet(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string drain;
    std::string gate;
    std::string source;
};


class VoltageControlledCurrentSource: public Component {
public:
    static const char identifier = 'G';

    VoltageControlledCurrentSource(const std::string& token_str);

protected:
    int id;
    std::string value;
    std::string positive;
    std::string negative;
    std::string control_positive;
    std::string control_negative;
};

#endif
