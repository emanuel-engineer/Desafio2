#ifndef REDNACIONAL_H
#define REDNACIONAL_H
//using namespace std;

class rednacional
{
public:
    rednacional();
    int FijarPreciosCombustible();
    int VentasTotales(int CalcularVentasTotales);
    char AgregarEstacion();
    char EliminarEstacion();

private:
    int CalcularVentasTotales();
};

#endif // REDNACIONAL_H
