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
    static char get_identifier() {return identifier;};

    int get_id();
    std::string get_value();
    static bool represented_by(const std::string& token_str);

protected:
    inline static char identifier = '_';
    int id;
    std::string value;
    ParseReturn* parse(const std::string& token_str);
};


class IndependentVoltageSource: public Component {
public:
    IndependentVoltageSource(const std::string& token_str);

protected:
    inline static char identifier = 'V';

    int id;
    std::string value;
    std::string positive_node;
    std::string negative_node;
};


class CurrentSource: public Component {
public:
    CurrentSource(const std::string& token_str);

protected:
    inline static char identifier = 'I';

    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Resistor: public Component {
public:
    Resistor(const std::string& token_str);

protected:
    inline static char identifier = 'R';

    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Capacitor: public Component {
public:
    Capacitor(const std::string& token_str);

protected:
    inline static char identifier = 'C';

    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Inductor: public Component {
public:
    Inductor(const std::string& token_str);

protected:
    inline static char identifier = 'L';

    int id;
    std::string value;
    std::string input_node;
    std::string output_node;
};


class Diode: public Component {
public:
    Diode(const std::string& token_str);

protected:
    inline static char identifier = 'I';

    int id;
    std::string value;
    std::string anode;
    std::string cathode;
};


class BipolarJunctionTransistor: public Component {
public:
    BipolarJunctionTransistor(const std::string& token_str);

protected:
    inline static char identifier = 'Q';

    int id;
    std::string value;
    std::string collector;
    std::string base;
    std::string emitter;
};


class Mosfet: public Component {
public:
    Mosfet(const std::string& token_str);

protected:
    inline static char identifier = 'M';

    int id;
    std::string value;
    std::string drain;
    std::string gate;
    std::string source;
};


class VoltageControlledCurrentSource: public Component {
public:
    VoltageControlledCurrentSource(const std::string& token_str);

protected:
    inline static char identifier = 'G';

    int id;
    std::string value;
    std::string positive;
    std::string negative;
    std::string control_positive;
    std::string control_negative;
};

#endif
