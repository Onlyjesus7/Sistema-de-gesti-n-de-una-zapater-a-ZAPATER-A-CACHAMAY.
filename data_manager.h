// =========================================================
// Archivo: data_manager.h
// Descripcion: Declaraciones de estructuras de datos y funciones de gestion.
// =========================================================
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <glib.h> // Necesario para GList
#include "data_types.h" // ¡IMPORTANTE! Incluir las definiciones de las estructuras
#include <gtk/gtk.h>

// ** DECLARACION DE VARIABLES GLOBALES DE DATOS (declaradas extern en otros archivos) **
// Estas GList son las listas principales de la aplicacion y se definen en data_manager.c
extern GList *lista_productos;
extern GList *lista_clientes;
extern GList *lista_trabajadores;
extern GList *lista_facturas;
extern GList *lista_proveedores; // Mantener por si se usa mas adelante


// Funciones de inicializacion y limpieza del gestor de datos
void init_data_manager(void);
void cleanup_data_manager(void);

// Funciones CRUD para Productos
gboolean add_producto(Producto *nuevo_producto);
Producto* find_producto(const gchar *codigo);
gboolean update_producto(Producto *producto_actualizado);
gboolean delete_producto(const gchar *codigo);
void get_all_productos(GList **productos_out); // Obtener todos los productos

// Funciones CRUD para Clientes
gboolean add_cliente(Cliente *nuevo_cliente);
Cliente* find_cliente(const gchar *cedula);
gboolean update_cliente(Cliente *cliente_actualizado);
gboolean delete_cliente(const gchar *cedula);
void get_all_clientes(GList **clientes_out); // Obtener todos los clientes

// Funciones CRUD para Facturas
gboolean add_factura(Factura *nueva_factura);
Factura* find_factura(const gchar *numero_factura);
gboolean update_factura(Factura *factura_actualizada);
gboolean delete_factura(const gchar *numero_factura);
void get_all_facturas(GList **facturas_out); // Obtener todas las facturas
void get_last_n_facturas(int n, GList **facturas_out); // Obtener las ultimas N facturas

// Funcion para generar el proximo numero de factura automatico
gchar* get_next_invoice_number(void);

// ** NUEVAS FUNCIONES CRUD PARA TRABAJADORES **
gboolean add_trabajador(Trabajador *nuevo_trabajador);
Trabajador* find_trabajador(const gchar *cedula_o_numero_empleado); // Puede buscar por cedula o numero de empleado
gboolean update_trabajador(Trabajador *trabajador_actualizado);
gboolean delete_trabajador(const gchar *cedula_o_numero_empleado);
void get_all_trabajadores(GList **trabajadores_out); // Obtener todos los trabajadores

// ** FUNCIONES PARA GENERAR REPORTES (DECLARACIONES) **
// Estas funciones toman un GtkTextBuffer para escribir el reporte directamente
void dm_show_inventory(GtkTextBuffer *buffer);
void dm_show_cost_sale_ratio(GtkTextBuffer *buffer);
void dm_show_damaged_products(GtkTextBuffer *buffer);
void dm_show_suppliers(GtkTextBuffer *buffer);
void dm_show_low_stock_products(GtkTextBuffer *buffer);
void dm_show_worker_count(GtkTextBuffer *buffer);
void dm_show_customer_purchases(GtkTextBuffer *buffer);
void dm_show_most_least_sold_shoe_type(GtkTextBuffer *buffer);
void dm_show_daily_sales_by_shoe_type(GtkTextBuffer *buffer);
void dm_show_last_seven_invoices(GtkTextBuffer *buffer);


#endif // DATA_MANAGER_H
