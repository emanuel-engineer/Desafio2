#ifndef REDNACIONAL_H
#define REDNACIONAL_H
//using namespace std;

class rednacional
{
public:
    rednacional();
    int FijarPreciosCombustibleGlobal();
    int VentasTotales(int CalcularVentasTotales);
    char AgregarEstacion();
    char EliminarEstacion();

private:
    int CalcularVentasTotales();
};

#endif // REDNACIONAL_H
