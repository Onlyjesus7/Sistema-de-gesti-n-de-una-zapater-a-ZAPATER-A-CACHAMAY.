// =========================================================
// Archivo: ui_elements.h
// Descripcion: Declaraciones de funciones para crear elementos de la interfaz.
// =========================================================
#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <gtk/gtk.h>       // Necesario para GtkWidget, GtkWindow, etc.
#include "callbacks.h"     // Para conectar las senales y acceder a sus declaraciones extern
#include "constants.h"     // Para usar constantes como WINDOW_TITLE
#include "data_manager.h"  // Para poder interactuar con las estructuras de datos

// Funcion para crear la ventana principal y su diseno base
GtkWidget* create_main_window(void);

// Funciones para crear las paginas del GtkStack
GtkWidget* create_inventario_page(void);
GtkWidget* create_productos_page(void);
GtkWidget* create_clientes_page(void);
GtkWidget* create_facturas_page(void);
GtkWidget* create_trabajadores_page(void);
GtkWidget* create_reportes_page(void); // Nueva funcion para crear la pagina de reportes

// Declaracion para el GtkListStore de productos (definicion en ui_elements.c)
extern GtkListStore *product_list_store;

// Declaraciones para los GtkEntry globales (definicion en ui_elements.c)
extern GtkWidget *entry_inventario_codigo_global;
extern GtkWidget *entry_inventario_nombre_global;
extern GtkWidget *entry_inventario_categoria_global;
extern GtkWidget *entry_inventario_cantidad_global;
extern GtkWidget *entry_inventario_precio_bs_global;
extern GtkWidget *entry_inventario_estado_global;
extern GtkWidget *entry_inventario_fecha_adquisicion_global;
extern GtkWidget *entry_inventario_movimiento_global;
extern GtkWidget *entry_inventario_ubicacion_global;
extern GtkWidget *entry_inventario_stock_minimo_global;
extern GtkWidget *entry_inventario_costo_global;
extern GtkWidget *inventory_tree_view_global;

// Declaraciones para los GtkEntry/Label globales de la pagina de Productos
extern GtkWidget *productos_tree_view_global;
extern GtkWidget *productos_category_combo_global;
extern GtkWidget *label_productos_codigo_global;
extern GtkWidget *label_productos_nombre_global;
extern GtkWidget *label_productos_categoria_global;
extern GtkWidget *label_productos_cantidad_global;
extern GtkWidget *label_productos_precio_bs_global;
extern GtkWidget *label_productos_estado_global;
extern GtkWidget *label_productos_fecha_adq_global;
extern GtkWidget *label_productos_movimiento_global;
extern GtkWidget *label_productos_ubicacion_global;
extern GtkWidget *label_productos_stock_min_global;
extern GtkWidget *label_productos_costo_adq_global;

// Declaraciones para los GtkEntry globales de la pagina de Clientes
extern GtkListStore *clientes_list_store;
extern GtkWidget *clientes_tree_view_global;
extern GtkWidget *entry_clientes_cedula_global;
extern GtkWidget *entry_clientes_nombre_global;
extern GtkWidget *entry_clientes_apellido_global;
extern GtkWidget *entry_clientes_telefono_global;
extern GtkWidget *entry_clientes_direccion_global;

// Declaraciones para los GtkEntry globales de la pagina de Facturas
extern GtkListStore *facturas_list_store;
extern GtkWidget *facturas_tree_view_global;
extern GtkWidget *label_factura_numero_global;
extern GtkWidget *label_factura_fecha_global;
extern GtkWidget *entry_factura_cliente_cedula_global;
extern GtkWidget *label_factura_cliente_nombre_global;
extern GtkWidget *label_factura_cliente_telefono_global;
extern GtkWidget *label_factura_cliente_direccion_global;
extern GtkWidget *entry_factura_producto_codigo_global;
extern GtkWidget *entry_factura_producto_cantidad_global;
extern GtkListStore *factura_detalles_list_store; // Modelo para la tabla de detalles de la factura actual
extern GtkWidget *factura_detalles_tree_view_global; // TreeView para los detalles de la factura actual

extern GtkWidget *label_factura_subtotal_global;
extern GtkWidget *label_factura_iva_global;
extern GtkWidget *label_factura_total_global;
extern GtkWidget *label_factura_producto_nombre_global;
extern GtkWidget *label_factura_producto_precio_unitario_global;

// Declaraciones para la pagina de Trabajadores
extern GtkListStore *trabajadores_list_store; // Modelo para la tabla de trabajadores
extern GtkWidget *trabajadores_tree_view_global; // TreeView para la pagina de trabajadores

extern GtkWidget *entry_trabajadores_cedula_global;
extern GtkWidget *entry_trabajadores_nombre_global;
extern GtkWidget *entry_trabajadores_apellido_global;
extern GtkWidget *entry_trabajadores_numero_empleado_global;
extern GtkWidget *entry_trabajadores_telefono_global;
extern GtkWidget *entry_trabajadores_direccion_global;

// Declaraciones para la pagina de Reportes
extern GtkWidget *report_text_view_global; // GtkTextView para mostrar los reportes


// Funciones para actualizar los GtkTreeView (definidas en ui_elements.c)
void update_product_tree_view(void);
void update_productos_page_tree_view(const gchar *filter_category);
void update_clientes_tree_view(void);
void update_facturas_tree_view(void);
void update_trabajadores_tree_view(void);
void update_factura_detalle_tree_view(GList *detalles);


// Definiciones de columnas para los GtkListStore y TreeView
// Para Productos (en Inventario y Productos)
typedef enum {
    PRODUCT_COL_CODIGO,
    PRODUCT_COL_NOMBRE,
    PRODUCT_COL_DESCRIPCION,
    PRODUCT_COL_CATEGORIA,
    PRODUCT_COL_CANTIDAD,
    PRODUCT_COL_UBICACION,
    PRODUCT_COL_PROVEEDOR,
    PRODUCT_COL_FECHA_ADQUISICION,
    PRODUCT_COL_COSTO,
    PRODUCT_COL_PRECIO_BS,
    PRODUCT_COL_PRECIO_COP,
    PRODUCT_COL_PRECIO_USD,
    PRODUCT_COL_PRECIO_EUR,
    PRODUCT_COL_MOVIMIENTO_INVENTARIO,
    PRODUCT_COL_NIVEL_STOCK_MINIMO,
    PRODUCT_COL_ESTADO_FISICO,
    N_PRODUCT_COLUMNS
} ProductColumns;

// Para Clientes
typedef enum {
    CLIENT_COL_CEDULA,
    CLIENT_COL_NOMBRE,
    CLIENT_COL_APELLIDO,
    CLIENT_COL_TELEFONO,
    CLIENT_COL_DIRECCION,
    N_CLIENT_COLUMNS
} ClientColumns;

// Para Trabajadores
typedef enum {
    WORKER_COL_CEDULA,
    WORKER_COL_NOMBRE,
    WORKER_COL_APELLIDO,
    WORKER_COL_NUM_EMPLEADO,
    WORKER_COL_TELEFONO,
    WORKER_COL_DIRECCION,
    N_WORKER_COLUMNS
} WorkerColumns;

// Para Facturas (historial)
typedef enum {
    FACTURA_COL_NUMERO,
    FACTURA_COL_FECHA,
    FACTURA_COL_CEDULA_CLIENTE,
    FACTURA_COL_NOMBRE_CLIENTE,
    FACTURA_COL_APELLIDO_CLIENTE,
    FACTURA_COL_MONTO_TOTAL,
    N_FACTURA_COLUMNS
} FacturaColumns;

// Para Detalles de Factura (productos dentro de una factura)
typedef enum {
    DETALLE_FACTURA_COL_CODIGO_PRODUCTO,
    DETALLE_FACTURA_COL_NOMBRE_PRODUCTO,
    DETALLE_FACTURA_COL_CANTIDAD,
    DETALLE_FACTURA_COL_PRECIO_UNITARIO,
    DETALLE_FACTURA_COL_SUBTOTAL_DETALLE,
    N_DETALLE_FACTURA_COLUMNS
} DetalleFacturaColumns;


#endif // UI_ELEMENTS_H
