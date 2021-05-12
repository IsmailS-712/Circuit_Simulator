//
// Created by Oliver on 12/05/2021.
//

#include <vector>
#include <string>
#include <iterator>

#include "components.h"


int get_id_from_identifier(const std::string& identifier) {
    return std::stoi(identifier.substr(1, identifier.length()));
}


// Definition of Component members
int Component::get_id() {
    return this->id;
}

std::string Component::get_value() {
    return this->value;
}

bool Component::represented_by(const std::string &token_str) {
    return token_str.rfind(identifier, 0) == 0;
}

ParseReturn* Component::parse(const std::string &token_str) {
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


// Definition of IndependentVoltageSource members
IndependentVoltageSource::IndependentVoltageSource(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->positive_node = parsed->nodes[0];
    this->negative_node = parsed->nodes[1];
    delete parsed;
}


// Definition of CurrentSource members
CurrentSource::CurrentSource(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->input_node = parsed->nodes[0];
    this->output_node = parsed->nodes[1];
    delete parsed;
}


// Definition of Resistor members
Resistor::Resistor(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->input_node = parsed->nodes[0];
    this->output_node = parsed->nodes[1];
    delete parsed;
}


// Definition of Capacitor members
Capacitor::Capacitor(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->input_node = parsed->nodes[0];
    this->output_node = parsed->nodes[1];
    delete parsed;
}


// Definition of Inductor members
Inductor::Inductor(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->input_node = parsed->nodes[0];
    this->output_node = parsed->nodes[1];
    delete parsed;
}


// Definition of Diode members
Diode::Diode(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->anode = parsed->nodes[0];
    this->cathode = parsed->nodes[1];
    delete parsed;
}


// Definition of BipolarJunctionTransistor members
BipolarJunctionTransistor::BipolarJunctionTransistor(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->collector = parsed->nodes[0];
    this->base = parsed->nodes[1];
    this->emitter = parsed->nodes[2];
    delete parsed;
}


// Definition of Mosfet members
Mosfet::Mosfet(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->drain = parsed->nodes[0];
    this->gate = parsed->nodes[1];
    this->source = parsed->nodes[2];
    delete parsed;
}


// Definition of VoltageControlledCurrentSource members
VoltageControlledCurrentSource::VoltageControlledCurrentSource(const std::string &token_str) {
    ParseReturn* parsed = this->parse(token_str);
    this->value = parsed->value;

    this->id = get_id_from_identifier(parsed->identifier);

    this->positive = parsed->nodes[0];
    this->negative = parsed->nodes[1];
    this->control_positive = parsed->nodes[2];
    this->control_negative = parsed->nodes[3];
    delete parsed;
}
