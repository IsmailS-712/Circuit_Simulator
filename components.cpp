//
// Created by Oliver on 12/05/2021.
//

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
    std::string get_value() {
        return this->value;
    };

    bool represented_by(const std::string& token_str) {
        return token_str.rfind(this->identifier, 0) == 0;
    };

protected:
    char identifier;
    std::string value;

    ParseReturn* parse(const std::string& token_str) {
        auto* parsed = new ParseReturn;

        std::istringstream buf(token_str);
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> tokens(beg, end);

        parsed->identifier = tokens[0];
        parsed->value = tokens[tokens.size() - 1];

        for (int i = 1; i < tokens.size(); i++) {
            parsed->nodes.push_back(tokens[i]);
        }
        return parsed;
    }
};


class IndependentVoltageSource: public Component {
public:
    IndependentVoltageSource(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->positive_node = parsed->nodes[0];
        this->negative_node = parsed->nodes[1];
        delete parsed;
    }

protected:
    char identifier = 'V';

    std::string positive_node;
    std::string negative_node;
};


class CurrentSource: public Component {
public:
    CurrentSource(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->input_node = parsed->nodes[0];
        this->output_node = parsed->nodes[1];
        delete parsed;
    }

protected:
    char identifier = 'I';

    std::string input_node;
    std::string output_node;
};


class Resistor: public Component {
public:
    Resistor(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->input_node = parsed->nodes[0];
        this->output_node = parsed->nodes[1];
        delete parsed;
    }

protected:
    char identifier = 'R';

    std::string input_node;
    std::string output_node;
};


class Capacitor: public Component {
public:
    Capacitor(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->input_node = parsed->nodes[0];
        this->output_node = parsed->nodes[1];
        delete parsed;
    }

protected:
    char identifier = 'C';

    std::string input_node;
    std::string output_node;
};


class Inductor: public Component {
public:
    Inductor(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->input_node = parsed->nodes[0];
        this->output_node = parsed->nodes[1];
        delete parsed;
    }

protected:
    char identifier = 'L';

    std::string input_node;
    std::string output_node;
};


class Diode: public Component {
public:
    Diode(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->anode = parsed->nodes[0];
        this->cathode = parsed->nodes[1];
        delete parsed;
    }

protected:
    char identifier = 'I';

    std::string anode;
    std::string cathode;
};


class BipolarJunctionTransistor: public Component {
public:
    BipolarJunctionTransistor(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->collector = parsed->nodes[0];
        this->base = parsed->nodes[1];
        this->emitter = parsed->nodes[2];
        delete parsed;
    }

protected:
    char identifier = 'Q';

    std::string collector;
    std::string base;
    std::string emitter;
};


class Mosfet: public Component {
public:
    Mosfet(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->drain = parsed->nodes[0];
        this->gate = parsed->nodes[1];
        this->source = parsed->nodes[2];
        delete parsed;
    }

protected:
    char identifier = 'M';

    std::string drain;
    std::string gate;
    std::string source;
};


class VoltageControlledCurrentSource: public Component {
public:
    VoltageControlledCurrentSource(const std::string& token_str) {
        ParseReturn* parsed = this->parse(token_str);
        this->value = parsed->value;

        this->positive = parsed->nodes[0];
        this->negative = parsed->nodes[1];
        this->control_positive = parsed->nodes[2];
        this->control_negative = parsed->nodes[3];
        delete parsed;
    }

protected:
    char identifier = 'G';

    std::string positive;
    std::string negative;
    std::string control_positive;
    std::string control_negative;
};
