// =========================================================
// Archivo: data_types.h
// Descripcion: Definiciones de estructuras de datos para la aplicacion.
// =========================================================
#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <glib.h> // Necesario para GList

// Definiciones de estructuras de datos

/**
 * @brief Estructura para representar un Producto en el inventario.
 */
typedef struct {
    char codigo[50];
    char nombre[100];
    char descripcion[250];
    char categoria[50]; // Deportivo, Casual, Vestir
    int cantidad_disponible;
    char ubicacion[100]; // Ubicacion en la zapateria (ej: "Estante A1", "Vitrina 3")
    char proveedor[100];
    char fecha_adquisicion[20]; // Formato,"%Y-%m-%d"
    double costo;               // Costo de adquisicion
    double precio_venta_bs;
    double precio_venta_cop;
    double precio_venta_usd;
    double precio_venta_eur;
    int nivel_stock_minimo;
    char estado_fisico[50]; // Nuevo, Danado
    char movimiento_inventario[250]; // Registro de entradas/salidas/ajustes
} Producto;

/**
 * @brief Estructura para representar un Cliente.
 */
typedef struct {
    char cedula[20];
    char nombre[100];
    char apellido[100];
    char telefono[20];
    char direccion[250];
} Cliente;

/**
 * @brief Estructura para representar un Trabajador.
 */
typedef struct {
    char cedula[20];
    char nombre[100];
    char apellido[100];
    char numero_empleado[50];
    char telefono[20];
    char direccion[250];
} Trabajador;

/**
 * @brief Estructura para representar un detalle de producto dentro de una factura.
 */
typedef struct {
    char codigo_producto[50];
    char nombre_producto[100];
    char tipo_zapato[50];       // Categoria del zapato (Deportivo, Casual, Vestir)
    int cantidad;
    double precio_unitario;     // Precio por unidad al momento de la venta
    double subtotal_detalle;    // Cantidad * Precio Unitario
} DetalleFactura;

/**
 * @brief Estructura para representar una Factura.
 */
typedef struct {
    char numero_factura[50];
    char id_tienda[100];        // "Zapateria Cachamay"
    char eslogan_tienda[250];   // "Fluye con cada paso"
    char fecha[20];             // Formato,"%Y-%m-%d"
    char cedula_cliente[20];    // Cedula del cliente (para buscarlo)
    char nombre_cliente[100];   // Nombre del cliente al momento de la factura
    char apellido_cliente[100]; // Apellido del cliente al momento de la factura
    char telefono_cliente[20];  // Telefono del cliente al momento de la factura
    char direccion_cliente[250]; // Direccion del cliente al momento de la factura
    GList *detalles_productos;  // Lista de punteros a DetalleFactura
    double porcentaje_impuesto; // Ej: 0.013 para 1.3%
    double monto_total;
} Factura;

#endif // DATA_TYPES_H
