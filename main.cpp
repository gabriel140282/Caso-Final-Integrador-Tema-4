#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include "./json11-master/json11.h"
#include "./JSONLib-master/JSONLib/src/JSONLib.h"

enum variant_type { Symbol, Number, List, Proc, Lambda, Cadena };

struct Entorno;

class Variant {
public:
    typedef Variant(*proc_type)(const std::vector<Variant>&);
    typedef std::vector<Variant>::const_iterator iter;
    typedef std::map<std::string, Variant> map;

    variant_type type;
    std::string val;
    std::vector<Variant> list;
    proc_type proc;
    Entorno* env;

    Variant(variant_type type = Symbol) : type(type), env(nullptr), proc(nullptr) {}
    Variant(variant_type type, const std::string& val) : type(type), val(val), env(nullptr), proc(nullptr) {}
    Variant(proc_type proc) : type(Proc), proc(proc), env(nullptr) {}

    std::string to_string() const;
    std::string to_json_string() const;
    static Variant from_json_string(const std::string& json);
    static Variant parse_json(const json11::Json& job);
};

std::string Variant::to_string() const {
    switch (type) {
        case Symbol: return "Symbol: " + val;
        case Number: return "Number: " + val;
        case List: {
            std::string result = "List: [";
            for (const auto& item : list) {
                result += item.to_string() + ", ";
            }
            if (!list.empty()) {
                result.pop_back();
                result.pop_back();
            }
            result += "]";
            return result;
        }
        case Proc: return "Proc: <function>";
        case Lambda: return "Lambda: <function>";
        case Cadena: return "Cadena: \"" + val + "\"";
        default: return "Unknown";
    }
}

std::string Variant::to_json_string() const {
    switch (type) {
        case Symbol: return "\"" + val + "\"";
        case Number: return val;
        case List: {
            std::string result = "[";
            for (const auto& item : list) {
                result += item.to_json_string() + ",";
            }
            if (!list.empty()) result.pop_back();
            result += "]";
            return result;
        }
        case Cadena: return "\"" + val + "\"";
        default: return "{}";
    }
}

Variant Variant::from_json_string(const std::string& sjson) {
    std::string err;
    json11::Json parsed = json11::Json::parse(sjson, err);
    if (!err.empty()) {
        throw std::invalid_argument("Error al analizar JSON: " + err);
    }
    return parse_json(parsed);
}

Variant Variant::parse_json(const json11::Json& job) {
    if (job.is_string()) {
        return Variant(Cadena, job.string_value());
    } else if (job.is_number()) {
        return Variant(Number, std::to_string(job.number_value()));
    } else if (job.is_array()) {
        Variant varList(List);
        for (const auto& item : job.array_items()) {
            varList.list.push_back(parse_json(item));
        }
        return varList;
    } else if (job.is_object()) {
        Variant varMap(Symbol);
        for (const auto& kv : job.object_items()) {
            Variant key(Cadena, kv.first);
            Variant value = parse_json(kv.second);
            varMap.list.push_back(key);
            varMap.list.push_back(value);
        }
        return varMap;
    }
    return Variant(Symbol);
}
