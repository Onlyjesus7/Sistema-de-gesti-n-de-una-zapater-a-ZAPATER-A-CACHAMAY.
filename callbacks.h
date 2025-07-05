// =========================================================
// Archivo: callbacks.h
// Descripcion: Declaraciones de las funciones de callback para la UI.
// =========================================================
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>       // Necesario para los tipos GtkWidget, GtkButton, etc.
#include "constants.h"     // Para usar constantes como nombres de paginas, WINDOW_TITLE, HEADER_TITLE, SLOGAN
#include "data_manager.h"  // Para interactuar con los datos (ej. Producto, GList, funciones CRUD)
#include "ui_elements.h"   // ¡NUEVO! Para acceder a las funciones de actualizacion de TreeView

// ** DECLARACION EXTERNA DE PUNTEROS A WIDGETS GLOBALES. **
// Estas variables se DEFINEN en main.c (main_app_window, main_content_stack)
// y en ui_elements.c (product_list_store, entry_inventario_xxx_global, inventory_tree_view_global).
// Aqui solo se DECLARAN con 'extern' para que los callbacks puedan acceder a ellas.
extern GtkWindow *main_app_window;
extern GtkStack *main_content_stack;
extern GtkListStore *product_list_store; // Inventario
extern GtkListStore *productos_list_store; // Productos
extern GtkListStore *clientes_list_store; // Clientes
extern GtkListStore *facturas_list_store; // Facturas
extern GtkListStore *trabajadores_list_store; // Trabajadores

// Declaraciones externas para los GtkEntry de la pagina de Inventario
extern GtkWidget *entry_inventario_codigo_global;
extern GtkWidget *entry_inventario_nombre_global;
extern GtkWidget *entry_inventario_categoria_global;
extern GtkWidget *entry_inventario_cantidad_global;
extern GtkWidget *entry_inventario_precio_bs_global;
extern GtkWidget *entry_inventario_estado_fisico_global;
extern GtkWidget *entry_inventario_ubicacion_global;
extern GtkWidget *entry_inventario_proveedor_global;
extern GtkWidget *entry_inventario_costo_global;
extern GtkWidget *entry_inventario_stock_min_global;
extern GtkWidget *entry_inventario_movimiento_global;
extern GtkWidget *entry_inventario_precio_cop_global;
extern GtkWidget *entry_inventario_precio_usd_global;
extern GtkWidget *entry_inventario_precio_eur_global;
extern GtkWidget *inventory_tree_view_global; // TreeView para la pagina de Inventario

// Declaraciones para los GtkEntry de la pagina de Productos (para busquedas/filtros)
extern GtkWidget *entry_productos_categoria_filtro_global;
extern GtkWidget *productos_tree_view_global; // TreeView para la pagina de Productos

// Declaraciones para los GtkEntry de la pagina de Clientes
extern GtkWidget *entry_clientes_cedula_global;
extern GtkWidget *entry_clientes_nombre_global;
extern GtkWidget *entry_clientes_apellido_global;
extern GtkWidget *entry_clientes_telefono_global;
extern GtkWidget *entry_clientes_direccion_global;
extern GtkWidget *clientes_tree_view_global; // TreeView para la pagina de Clientes

// Declaraciones para los GtkEntry/Labels de la pagina de Facturas
extern GtkWidget *entry_facturas_numero_factura_global;
extern GtkWidget *entry_facturas_cedula_cliente_global;
extern GtkWidget *label_facturas_nombre_cliente_global;
extern GtkWidget *label_facturas_apellido_cliente_global;
extern GtkWidget *label_facturas_telefono_cliente_global;
extern GtkWidget *label_facturas_direccion_cliente_global;
extern GtkWidget *facturas_tree_view_global; // TreeView principal de facturas
extern GtkWidget *entry_factura_producto_codigo_global;
extern GtkWidget *entry_factura_producto_cantidad_global;
extern GtkListStore *factura_detalles_list_store; // Modelo para la tabla de detalles de la factura actual
extern GtkWidget *factura_detalles_tree_view_global; // TreeView para los detalles de la factura actual

// Variable global para almacenar los detalles de la factura actual en proceso
// Se define en callbacks.c, aqui solo se declara como externa
extern GList *current_invoice_details;

extern GtkWidget *label_factura_subtotal_global;
extern GtkWidget *label_factura_iva_global;
extern GtkWidget *label_factura_total_global;

// ** NUEVAS DECLARACIONES PARA LA PAGINA DE TRABAJADORES **
extern GtkListStore *trabajadores_list_store; // Modelo para la tabla de trabajadores
extern GtkWidget *trabajadores_tree_view_global; // TreeView para la pagina de trabajadores

extern GtkWidget *entry_trabajadores_cedula_global;
extern GtkWidget *entry_trabajadores_nombre_global;
extern GtkWidget *entry_trabajadores_apellido_global;
extern GtkWidget *entry_trabajadores_numero_empleado_global;
extern GtkWidget *entry_trabajadores_telefono_global;
extern GtkWidget *entry_trabajadores_direccion_global;

// ** NUEVAS DECLARACIONES PARA LA PAGINA DE REPORTES **
extern GtkWidget *text_view_reportes_output_global; // TextView donde se mostraran los reportes

// =========================================================
// DECLARACIONES DE FUNCIONES DE CALLBACK
// =========================================================

// Funciones de callback para la ventana principal
void on_main_window_destroy(GtkWidget *widget, gpointer user_data);

// Funciones de callback para los botones del sidebar (navegacion)
void on_btn_inventario_clicked(GtkButton *button, gpointer user_data);
void on_btn_productos_clicked(GtkButton *button, gpointer user_data);
void on_btn_clientes_clicked(GtkButton *button, gpointer user_data);
void on_btn_facturas_clicked(GtkButton *button, gpointer user_data);
void on_btn_trabajadores_clicked(GtkButton *button, gpointer user_data);
void on_btn_reportes_clicked(GtkButton *button, gpointer user_data); // NUEVO

// Funciones de callback para la pagina de Inventario
void on_inventario_crear_clicked(GtkButton *button, gpointer user_data);
void on_inventario_buscar_clicked(GtkButton *button, gpointer user_data);
void on_inventario_modificar_clicked(GtkButton *button, gpointer user_data);
void on_inventario_eliminar_clicked(GtkButton *button, gpointer user_data);
void on_inventario_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data);

// Funciones de callback para la pagina de Productos (para el TreeView con filtro)
void on_productos_aplicar_filtro_clicked(GtkButton *button, gpointer user_data);
void on_productos_limpiar_filtro_clicked(GtkButton *button, gpointer user_data);
void on_productos_marcar_vendido_clicked(GtkButton *button, gpointer user_data);
void on_productos_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data);
void on_productos_category_changed(GtkComboBox *combo_box, gpointer user_data);

// Funciones de callback para la pagina de Clientes
void on_clientes_crear_clicked(GtkButton *button, gpointer user_data);
void on_clientes_buscar_clicked(GtkButton *button, gpointer user_data);
void on_clientes_modificar_clicked(GtkButton *button, gpointer user_data);
void on_clientes_eliminar_clicked(GtkButton *button, gpointer user_data);
void on_clientes_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data);

// Funciones de callback para la pagina de Facturas
void on_facturas_nueva_clicked(GtkButton *button, gpointer user_data);
void on_facturas_buscar_cliente_clicked(GtkButton *button, gpointer user_data);
void on_facturas_anadir_producto_clicked(GtkButton *button, gpointer user_data);
void on_facturas_eliminar_detalle_clicked(GtkButton *button, gpointer user_data); // Eliminar producto de la factura actual
void on_facturas_guardar_clicked(GtkButton *button, gpointer user_data);
void on_facturas_modificar_clicked(GtkButton *button, gpointer user_data);
void on_facturas_eliminar_clicked(GtkButton *button, gpointer user_data);
void on_facturas_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data);
void on_facturas_mostrar_clicked(GtkButton *button, gpointer user_data);

// ** NUEVAS FUNCIONES DE CALLBACK PARA LA PAGINA DE TRABAJADORES **
void on_trabajadores_crear_clicked(GtkButton *button, gpointer user_data);
void on_trabajadores_buscar_clicked(GtkButton *button, gpointer user_data);
void on_trabajadores_modificar_clicked(GtkButton *button, gpointer user_data);
void on_trabajadores_eliminar_clicked(GtkButton *button, gpointer user_data);
void on_trabajadores_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data);

// ** NUEVAS FUNCIONES DE CALLBACK PARA LA PAGINA DE REPORTES **
/*void on_reportes_inventario_clicked(GtkButton *button, gpointer user_data);
void on_reportes_ventas_clicked(GtkButton *button, gpointer user_data);
void on_reportes_clientes_clicked(GtkButton *button, gpointer user_data);
void on_reportes_trabajadores_clicked(GtkButton *button, gpointer user_data);*/

void on_reportes_inventario_clicked(GtkButton *button, gpointer user_data);
void on_reportes_rentabilidad_clicked(GtkButton *button, gpointer user_data);
void on_reportes_productos_danados_clicked(GtkButton *button, gpointer user_data);
void on_reportes_proveedores_clicked(GtkButton *button, gpointer user_data);
void on_reportes_stock_bajo_clicked(GtkButton *button, gpointer user_data);
void on_reportes_conteo_trabajadores_clicked(GtkButton *button, gpointer user_data);
void on_reportes_compras_clientes_clicked(GtkButton *button, gpointer user_data);
void on_reportes_mas_menos_vendidos_clicked(GtkButton *button, gpointer user_data);
void on_reportes_ventas_diarias_clicked(GtkButton *button, gpointer user_data);
void on_reportes_ultimas_facturas_clicked(GtkButton *button, gpointer user_data);


#endif // CALLBACKS_H
