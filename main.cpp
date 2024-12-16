#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include "./json11-master/json11.hpp"
#include "./JSONLib-master/JSONLib/src/JSONLib.h"


// Enumeración para los diferentes tipos de variantes
enum variant_type { Symbol, Number, List, Proc, Lambda, Cadena };

// Declaración previa del entorno (puedes definirlo posteriormente según tus necesidades)
struct Entorno;

class Variant {
public:
    // Tipos de datos y estructuras auxiliares
    typedef Variant(*proc_type)(const std::vector<Variant>&); // Puntero a función
    typedef std::vector<Variant>::const_iterator iter;        // Iterador constante
    typedef std::map<std::string, Variant> map;               // Mapa para almacenar variables/valores

    // Miembros de la clase
    variant_type type;            // Tipo de la variante
    std::string val;              // Valor en caso de cadenas o símbolos
    std::vector<Variant> list;    // Lista para almacenar otras variantes
    proc_type proc;               // Función del tipo proceso
    Entorno* env;                 // Entorno al que pertenece

    // Constructores
    Variant(variant_type type = Symbol) : type(type), env(nullptr), proc(nullptr) {}
    Variant(variant_type type, const std::string& val) : type(type), val(val), env(nullptr), proc(nullptr) {}
    Variant(proc_type proc) : type(Proc), proc(proc), env(nullptr) {}

    // Métodos
    std::string to_string() const;                      // Convierte la variante a string
    std::string to_json_string() const;                 // Convierte la variante a JSON string
    static Variant from_json_string(const std::string& json); // Crea una variante desde un string JSON
    static Variant parse_json(const json11::Json& job);       // Analiza un objeto JSON
};

// Implementación de los métodos
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
                result.pop_back(); // Elimina la última coma
                result.pop_back(); // y el espacio
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
            if (!list.empty()) result.pop_back(); // Elimina la última coma
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
    return Variant(Symbol); // Valor por defecto si no se reconoce
}
