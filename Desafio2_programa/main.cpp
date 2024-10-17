#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

// Forward declarations
class EstacionServicio;
class Surtidor;
class Transaccion;

// Enums
enum class TipoCombustible { Regular, Premium, EcoExtra };
enum class MetodoPago { Efectivo, TDebito, TCredito };
enum class Region { Norte, Centro, Sur };

// Utility classes
class Ubicacion {
public:
    float latitud;
    float longitud;
    float altitud;
};

class Gerente {
public:
    std::string nombre;
};

class Precios {
public:
    static float precioGlobal;
    static float precioNorte;
    static float precioCentro;
    static float precioSur;
};

float Precios::precioGlobal = 0.0f;
float Precios::precioNorte = 0.0f;
float Precios::precioCentro = 0.0f;
float Precios::precioSur = 0.0f;

class Tanque {
public:
    float capacidadRegular;
    float capacidadPremium;
    float capacidadEcoExtra;
    float nivelRegular;
    float nivelPremium;
    float nivelEcoExtra;

    Tanque() : capacidadRegular(0), capacidadPremium(0), capacidadEcoExtra(0),
        nivelRegular(0), nivelPremium(0), nivelEcoExtra(0) {}

    void inicializarCapacidades() {
        capacidadRegular = 100 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100));
        capacidadPremium = 100 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100));
        capacidadEcoExtra = 100 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100));
        nivelRegular = capacidadRegular;
        nivelPremium = capacidadPremium;
        nivelEcoExtra = capacidadEcoExtra;
    }
};

class Transaccion {
public:
    time_t fecha;
    TipoCombustible tipoCombustible;
    float cantidad;
    MetodoPago metodoPago;
    std::string documentoCliente;
    float monto;

    Transaccion(TipoCombustible tipo, float cant, MetodoPago metodo, const std::string& doc, float mont)
        : fecha(time(nullptr)), tipoCombustible(tipo), cantidad(cant), metodoPago(metodo),
        documentoCliente(doc), monto(mont) {}
};

class Surtidor {
public:
    std::string codigo;
    std::string modelo;
    bool activo;
    std::vector<Transaccion> transacciones;

    Surtidor(const std::string& cod, const std::string& mod)
        : codigo(cod), modelo(mod), activo(true) {}

    void registrarVenta(const Transaccion& trans) {
        transacciones.push_back(trans);
    }
};

class EstacionServicio {
public:
    std::string nombre;
    std::string codigo;
    Gerente* gerente;
    Region region;
    Ubicacion ubicacion;
    Tanque tanque;
    std::vector<Surtidor*> surtidores;

    EstacionServicio(const std::string& nom, const std::string& cod, Gerente* ger, Region reg)
        : nombre(nom), codigo(cod), gerente(ger), region(reg) {
        tanque.inicializarCapacidades();
    }

    void agregarSurtidor(Surtidor* surtidor) {
        surtidores.push_back(surtidor);
    }

    bool eliminarSurtidor(const std::string& codigo) {
        for (auto it = surtidores.begin(); it != surtidores.end(); ++it) {
            if ((*it)->codigo == codigo) {
                delete *it;
                surtidores.erase(it);
                return true;
            }
        }
        return false;
    }

    void activarSurtidor(const std::string& codigo) {
        for (auto& surtidor : surtidores) {
            if (surtidor->codigo == codigo) {
                surtidor->activo = true;
                break;
            }
        }
    }

    void desactivarSurtidor(const std::string& codigo) {
        for (auto& surtidor : surtidores) {
            if (surtidor->codigo == codigo) {
                surtidor->activo = false;
                break;
            }
        }
    }

    void consultarTransacciones() const {
        for (const auto& surtidor : surtidores) {
            std::cout << "Surtidor: " << surtidor->codigo << std::endl;
            for (const auto& trans : surtidor->transacciones) {
                std::cout << "  Fecha: " << ctime(&trans.fecha)
                << "  Tipo: " << static_cast<int>(trans.tipoCombustible)
                << "  Cantidad: " << trans.cantidad
                << "  Monto: " << trans.monto << std::endl;
            }
        }
    }

    void reportarVentas() const {
        float totalRegular = 0, totalPremium = 0, totalEcoExtra = 0;
        for (const auto& surtidor : surtidores) {
            for (const auto& trans : surtidor->transacciones) {
                switch (trans.tipoCombustible) {
                case TipoCombustible::Regular:
                    totalRegular += trans.cantidad;
                    break;
                case TipoCombustible::Premium:
                    totalPremium += trans.cantidad;
                    break;
                case TipoCombustible::EcoExtra:
                    totalEcoExtra += trans.cantidad;
                    break;
                }
            }
        }
        std::cout << "Ventas totales:" << std::endl
                  << "Regular: " << totalRegular << " litros" << std::endl
                  << "Premium: " << totalPremium << " litros" << std::endl
                  << "EcoExtra: " << totalEcoExtra << " litros" << std::endl;
    }

    bool simularVenta() {
        if (surtidores.empty()) {
            std::cout << "No hay surtidores disponibles." << std::endl;
            return false;
        }

        int surtidorIndex = rand() % surtidores.size();
        Surtidor* surtidorElegido = surtidores[surtidorIndex];

        if (!surtidorElegido->activo) {
            std::cout << "El surtidor elegido no está activo." << std::endl;
            return false;
        }

        TipoCombustible tipoCombustible = static_cast<TipoCombustible>(rand() % 3);
        float cantidadSolicitada = 3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 17));
        float cantidadDisponible;

        switch (tipoCombustible) {
        case TipoCombustible::Regular:
            cantidadDisponible = tanque.nivelRegular;
            break;
        case TipoCombustible::Premium:
            cantidadDisponible = tanque.nivelPremium;
            break;
        case TipoCombustible::EcoExtra:
            cantidadDisponible = tanque.nivelEcoExtra;
            break;
        }

        float cantidadVendida = std::min(cantidadSolicitada, cantidadDisponible);
        float precioLitro;

        switch (region) {
        case Region::Norte:
            precioLitro = Precios::precioNorte;
            break;
        case Region::Centro:
            precioLitro = Precios::precioCentro;
            break;
        case Region::Sur:
            precioLitro = Precios::precioSur;
            break;
        }

        float monto = cantidadVendida * precioLitro;
        MetodoPago metodoPago = static_cast<MetodoPago>(rand() % 3);
        std::string documentoCliente = "DOC" + std::to_string(rand() % 1000000);

        Transaccion nuevaTransaccion(tipoCombustible, cantidadVendida, metodoPago, documentoCliente, monto);
        surtidorElegido->registrarVenta(nuevaTransaccion);

        // Actualizar nivel de tanque
        switch (tipoCombustible) {
        case TipoCombustible::Regular:
            tanque.nivelRegular -= cantidadVendida;
            break;
        case TipoCombustible::Premium:
            tanque.nivelPremium -= cantidadVendida;
            break;
        case TipoCombustible::EcoExtra:
            tanque.nivelEcoExtra -= cantidadVendida;
            break;
        }

        std::cout << "Venta simulada:" << std::endl
                  << "Surtidor: " << surtidorElegido->codigo << std::endl
                  << "Tipo de combustible: " << static_cast<int>(tipoCombustible) << std::endl
                  << "Cantidad vendida: " << cantidadVendida << " litros" << std::endl
                  << "Monto: $" << monto << std::endl;

        return true;
    }
};

class RedNacional {


public:
    std::vector<EstacionServicio*> estaciones;
    void agregarEstacion(EstacionServicio* estacion) {
        estaciones.push_back(estacion);
    }

    bool eliminarEstacion(const std::string& codigo) {
        for (auto it = estaciones.begin(); it != estaciones.end(); ++it) {
            if ((*it)->codigo == codigo) {
                if (!(*it)->surtidores.empty()) {
                    std::cout << "No se puede eliminar la estación. Aún tiene surtidores activos." << std::endl;
                    return false;
                }
                delete *it;
                estaciones.erase(it);
                return true;
            }
        }
        return false;
    }

    void calcularVentasTotales() const {
        for (const auto& estacion : estaciones) {
            std::cout << "Estación: " << estacion->nombre << std::endl;
            estacion->reportarVentas();
            std::cout << std::endl;
        }
    }

    void fijarPreciosCombustible(float precioGlobal, float precioNorte, float precioCentro, float precioSur) {
        Precios::precioGlobal = precioGlobal;
        Precios::precioNorte = precioNorte;
        Precios::precioCentro = precioCentro;
        Precios::precioSur = precioSur;
    }

    bool verificarFugas(const std::string& codigoEstacion) const {
        for (const auto& estacion : estaciones) {
            if (estacion->codigo == codigoEstacion) {
                float porcentajeRegular = (estacion->tanque.nivelRegular / estacion->tanque.capacidadRegular) * 100;
                float porcentajePremium = (estacion->tanque.nivelPremium / estacion->tanque.capacidadPremium) * 100;
                float porcentajeEcoExtra = (estacion->tanque.nivelEcoExtra / estacion->tanque.capacidadEcoExtra) * 100;

                std::cout << "Verificación de fugas para la estación " << estacion->nombre << ":" << std::endl;
                std::cout << "Regular: " << std::fixed << std::setprecision(2) << porcentajeRegular << "% de capacidad" << std::endl;
                std::cout << "Premium: " << std::fixed << std::setprecision(2) << porcentajePremium << "% de capacidad" << std::endl;
                std::cout << "EcoExtra: " << std::fixed << std::setprecision(2) << porcentajeEcoExtra << "% de capacidad" << std::endl;

                if (porcentajeRegular < 95 || porcentajePremium < 95 || porcentajeEcoExtra < 95) {
                    std::cout << "¡Alerta! Posible fuga detectada." << std::endl;
                    return true;
                } else {
                    std::cout << "No se detectaron fugas." << std::endl;
                    return false;
                }
            }
        }
        std::cout << "Estación no encontrada." << std::endl;
        return false;
    }
};



int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    RedNacional redNacional;

    // Inicializar algunas estaciones y surtidores para demostración
    Gerente* gerente1 = new Gerente{"Juan Pérez"};
    EstacionServicio* estacion1 = new EstacionServicio("Estación Central", "ES001", gerente1, Region::Centro);
    estacion1->agregarSurtidor(new Surtidor("S001", "Modelo A"));
    estacion1->agregarSurtidor(new Surtidor("S002", "Modelo B"));
    redNacional.agregarEstacion(estacion1);

    Gerente* gerente2 = new Gerente{"María López"};
    EstacionServicio* estacion2 = new EstacionServicio("Estación Norte", "ES002", gerente2, Region::Norte);
    estacion2->agregarSurtidor(new Surtidor("S003", "Modelo A"));
    redNacional.agregarEstacion(estacion2);

    // Fijar precios de combustible
    redNacional.fijarPreciosCombustible(3000, 2900, 3000, 3100);

    int opcion;
    do {
        std::cout << "\nMenú Principal:" << std::endl;
        std::cout << "1. Gestión de la red" << std::endl;
        std::cout << "2. Gestión de estaciones de servicio" << std::endl;
        std::cout << "3. Verificar fugas de combustible" << std::endl;
        std::cout << "4. Simular venta" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1: {
            // Submenu para gestión de la red
            int subOpcion;
            do {
                std::cout << "\nGestión de la Red:" << std::endl;
                std::cout << "1. Agregar estación de servicio" << std::endl;
                std::cout << "2. Eliminar estación de servicio" << std::endl;
                std::cout << "3. Calcular ventas totales" << std::endl;
                std::cout << "4. Fijar precios de combustible" << std::endl;
                std::cout << "0. Volver al menú principal" << std::endl;
                std::cout << "Seleccione una opción: ";
                std::cin >> subOpcion;

                switch (subOpcion) {
                case 1: {
                    std::string nombre, codigo;
                    int regionInt;
                    std::cout << "Nombre de la estación: ";
                    std::cin >> nombre;
                    std::cout << "Código de la estación: ";
                    std::cin >> codigo;
                    std::cout << "Región (0: Norte, 1: Centro, 2: Sur): ";
                    std::cin >> regionInt;
                    Gerente* nuevoGerente = new Gerente{"Nuevo Gerente"};
                    EstacionServicio* nuevaEstacion = new EstacionServicio(nombre, codigo, nuevoGerente, static_cast<Region>(regionInt));
                    redNacional.agregarEstacion(nuevaEstacion);
                    std::cout << "Estación agregada con éxito." << std::endl;
                    break;
                }
                case 2: {
                    std::string codigo;
                    std::cout << "Código de la estación a eliminar: ";
                    std::cin >> codigo;
                    if (redNacional.eliminarEstacion(codigo)) {
                        std::cout << "Estación eliminada con éxito." << std::endl;
                    } else {
                        std::cout << "No se pudo eliminar la estación." << std::endl;
                    }
                    break;
                }
                case 3:
                    redNacional.calcularVentasTotales();
                    break;
                case 4: {
                    float precioGlobal, precioNorte, precioCentro, precioSur;
                    std::cout << "Precio global: ";
                    std::cin >> precioGlobal;
                    std::cout << "Precio Norte: ";
                    std::cin >> precioNorte;
                    std::cout << "Precio Centro: ";
                    std::cin >> precioCentro;
                    std::cout << "Precio Sur: ";
                    std::cin >> precioSur;
                    redNacional.fijarPreciosCombustible(precioGlobal, precioNorte, precioCentro, precioSur);
                    std::cout << "Precios actualizados con éxito." << std::endl;
                    break;
                }
                }
            } while (subOpcion != 0);
            break;
        }
        case 2: {
            // Submenu para gestión de estaciones de servicio
            std::string codigoEstacion;
            std::cout << "Ingrese el código de la estación: ";
            std::cin >> codigoEstacion;
            EstacionServicio* estacionSeleccionada = nullptr;
            for (auto& estacion : redNacional.estaciones) {
                if (estacion->codigo == codigoEstacion) {
                    estacionSeleccionada = estacion;
                    break;
                }
            }
            if (estacionSeleccionada == nullptr) {
                std::cout << "Estación no encontrada." << std::endl;
                break;
            }
            int subOpcion;
            do {
                std::cout << "\nGestión de Estación de Servicio:" << std::endl;
                std::cout << "1. Agregar surtidor" << std::endl;
                std::cout << "2. Eliminar surtidor" << std::endl;
                std::cout << "3. Activar surtidor" << std::endl;
                std::cout << "4. Desactivar surtidor" << std::endl;
                std::cout << "5. Consultar transacciones" << std::endl;
                std::cout << "6. Reportar ventas" << std::endl;
                std::cout << "0. Volver al menú principal" << std::endl;
                std::cout << "Seleccione una opción: ";
                std::cin >> subOpcion;

                switch (subOpcion) {
                case 1: {
                    std::string codigo, modelo;
                    std::cout << "Código del surtidor: ";
                    std::cin >> codigo;
                    std::cout << "Modelo del surtidor: ";
                    std::cin >> modelo;
                    estacionSeleccionada->agregarSurtidor(new Surtidor(codigo, modelo));
                    std::cout << "Surtidor agregado con éxito." << std::endl;
                    break;
                }
                case 2: {
                    std::string codigo;
                    std::cout << "Código del surtidor a eliminar: ";
                    std::cin >> codigo;
                    if (estacionSeleccionada->eliminarSurtidor(codigo)) {
                        std::cout << "Surtidor eliminado con éxito." << std::endl;
                    } else {
                        std::cout << "No se pudo eliminar el surtidor." << std::endl;
                    }
                    break;
                }
                case 3: {
                    std::string codigo;
                    std::cout << "Código del surtidor a activar: ";
                    std::cin >> codigo;
                    estacionSeleccionada->activarSurtidor(codigo);
                    std::cout << "Surtidor activado." << std::endl;
                    break;
                }
                case 4: {
                    std::string codigo;
                    std::cout << "Código del surtidor a desactivar: ";
                    std::cin >> codigo;
                    estacionSeleccionada->desactivarSurtidor(codigo);
                    std::cout << "Surtidor desactivado." << std::endl;
                    break;
                }
                case 5:
                    estacionSeleccionada->consultarTransacciones();
                    break;
                case 6:
                    estacionSeleccionada->reportarVentas();
                    break;
                }
            } while (subOpcion != 0);
            break;
        }
        case 3: {
            std::string codigoEstacion;
            std::cout << "Ingrese el código de la estación para verificar fugas: ";
            std::cin >> codigoEstacion;
            redNacional.verificarFugas(codigoEstacion);
            break;
        }
        case 4: {
            std::string codigoEstacion;
            std::cout << "Ingrese el código de la estación para simular una venta: ";
            std::cin >> codigoEstacion;
            for (auto& estacion : redNacional.estaciones) {
                if (estacion->codigo == codigoEstacion) {
                    estacion->simularVenta();
                    break;
                }
            }
            break;
        }
        }
    } while (opcion != 0);

    // Liberar memoria
    for (auto& estacion : redNacional.estaciones) {
        delete estacion->gerente;
        for (auto& surtidor : estacion->surtidores) {
            delete surtidor;
        }
        delete estacion;
    }

    return 0;
}
