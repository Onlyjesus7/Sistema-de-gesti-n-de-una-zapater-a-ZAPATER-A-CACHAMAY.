// =========================================================
// Archivo: callbacks.c
// Descripcion: Implementacion de las funciones de callback para la UI.
// =========================================================
#include "callbacks.h" // Incluye la cabecera con todas las declaraciones extern necesarias
#include <stdio.h>     // Para g_print (imprimir en consola), snprintf
#include <stdlib.h>    // Para atof (convertir cadena a double), atoi (convertir cadena a int)
#include <string.h>    // Para strcpy (copiar cadenas), strlen, strcmp
#include <time.h>      // Para time, localtime, strftime (para fechas)

// ** ESTAS SON SOLO DECLARACIONES EXTERNAS DE VARIABLES GLOBALES. **
// Las DEFINICIONES (inicializacion a NULL) de estas variables estan en otros archivos .c.
// main_app_window y main_content_stack se definen en main.c.
// product_list_store, entry_inventario_xxx_global y inventory_tree_view_global se definen en ui_elements.c.
extern GtkWindow *main_app_window;
extern GtkStack *main_content_stack;
extern GtkListStore *product_list_store; // Inventario
extern GtkListStore *productos_list_store; // Productos
extern GtkListStore *clientes_list_store; // Clientes
extern GtkListStore *facturas_list_store; // Facturas
extern GtkListStore *trabajadores_list_store; // Trabajadores

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

extern GtkWidget *inventory_tree_view_global; // TreeView de Inventario


// Declaraciones para la pagina de Productos
extern GtkWidget *productos_tree_view_global; // TreeView de Productos
extern GtkWidget *productos_category_combo_global; // Combobox de categoria en Productos

// Labels de solo lectura para mostrar detalles del producto seleccionado en pagina de Productos
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

// Declaraciones para la pagina de Clientes
extern GtkWidget *clientes_tree_view_global; // TreeView para la pagina de clientes

extern GtkWidget *entry_clientes_cedula_global;
extern GtkWidget *entry_clientes_nombre_global;
extern GtkWidget *entry_clientes_apellido_global;
extern GtkWidget *entry_clientes_telefono_global;
extern GtkWidget *entry_clientes_direccion_global;

// DECLARACIONES PARA LA PAGINA DE FACTURAS
extern GtkWidget *facturas_tree_view_global; // TreeView para la pagina de facturas

// Campos de entrada y visualizacion de la factura actual
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

// DECLARACIONES PARA LA PAGINA DE TRABAJADORES
extern GtkWidget *trabajadores_tree_view_global; // TreeView para la pagina de trabajadores

extern GtkWidget *entry_trabajadores_cedula_global;
extern GtkWidget *entry_trabajadores_nombre_global;
extern GtkWidget *entry_trabajadores_apellido_global;
extern GtkWidget *entry_trabajadores_numero_empleado_global;
extern GtkWidget *entry_trabajadores_telefono_global;
extern GtkWidget *entry_trabajadores_direccion_global;

// DECLARACION PARA LA PAGINA DE REPORTES
extern GtkWidget *text_view_reportes_output_global; // TextView donde se mostraran los reportes


// Variable para almacenar temporalmente los detalles de la factura que se esta creando/modificando
GList *current_invoice_details = NULL;


/**
 * @brief Funcion auxiliar para obtener el texto de un GtkEntry.
 * @param entry_widget Puntero al widget GtkEntry.
 * @return El texto actual del GtkEntry, o NULL si el widget no es valido.
 */
static const gchar* get_entry_text(GtkWidget *entry_widget) {
    if (entry_widget && GTK_IS_ENTRY(entry_widget)) {
        return gtk_entry_get_text(GTK_ENTRY(entry_widget));
    }
    return NULL;
}

/**
 * @brief Limpia los campos de entrada de texto de la pagina de Inventario.
 */
static void clear_inventory_entry_fields(void) {
    if (entry_inventario_codigo_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_codigo_global), "");
    if (entry_inventario_nombre_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_nombre_global), "");
    if (entry_inventario_categoria_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_categoria_global), "");
    if (entry_inventario_cantidad_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_cantidad_global), "");
    if (entry_inventario_precio_bs_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_precio_bs_global), "");
    if (entry_inventario_estado_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_estado_global), "");

    // Limpiar nuevos campos de Inventario
    if (entry_inventario_fecha_adquisicion_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_fecha_adquisicion_global), "");
    if (entry_inventario_movimiento_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_movimiento_global), "");
    if (entry_inventario_ubicacion_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_ubicacion_global), "");
    if (entry_inventario_stock_minimo_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_stock_minimo_global), "");
    if (entry_inventario_costo_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_costo_global), "");
}

/**
 * @brief Limpia las etiquetas de detalle del producto en la pagina de Productos.
 */
static void clear_productos_detail_labels(void) {
    if (label_productos_codigo_global) gtk_label_set_text(GTK_LABEL(label_productos_codigo_global), "");
    if (label_productos_nombre_global) gtk_label_set_text(GTK_LABEL(label_productos_nombre_global), "");
    if (label_productos_categoria_global) gtk_label_set_text(GTK_LABEL(label_productos_categoria_global), "");
    if (label_productos_cantidad_global) gtk_label_set_text(GTK_LABEL(label_productos_cantidad_global), "");
    if (label_productos_precio_bs_global) gtk_label_set_text(GTK_LABEL(label_productos_precio_bs_global), "");
    if (label_productos_estado_global) gtk_label_set_text(GTK_LABEL(label_productos_estado_global), "");
    if (label_productos_fecha_adq_global) gtk_label_set_text(GTK_LABEL(label_productos_fecha_adq_global), "");
    if (label_productos_movimiento_global) gtk_label_set_text(GTK_LABEL(label_productos_movimiento_global), "");
    if (label_productos_ubicacion_global) gtk_label_set_text(GTK_LABEL(label_productos_ubicacion_global), "");
    if (label_productos_stock_min_global) gtk_label_set_text(GTK_LABEL(label_productos_stock_min_global), "");
    if (label_productos_costo_adq_global) gtk_label_set_text(GTK_LABEL(label_productos_costo_adq_global), "");
}

/**
 * @brief Limpia los campos de entrada de texto de la pagina de Clientes.
 */
static void clear_clientes_entry_fields(void) {
    if (entry_clientes_cedula_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_cedula_global), "");
    if (entry_clientes_nombre_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_nombre_global), "");
    if (entry_clientes_apellido_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_apellido_global), "");
    if (entry_clientes_telefono_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_telefono_global), "");
    if (entry_clientes_direccion_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_direccion_global), "");
}

/**
 * @brief Limpia los campos de entrada y visualizacion de la pagina de Facturas para una nueva factura.
 */
static void clear_facturas_page_fields(void) {
    if (label_factura_numero_global) gtk_label_set_text(GTK_LABEL(label_factura_numero_global), "AUTO GENERADO");
    if (label_factura_fecha_global) gtk_label_set_text(GTK_LABEL(label_factura_fecha_global), "");
    if (entry_factura_cliente_cedula_global) gtk_entry_set_text(GTK_ENTRY(entry_factura_cliente_cedula_global), "");
    if (label_factura_cliente_nombre_global) gtk_label_set_text(GTK_LABEL(label_factura_cliente_nombre_global), "");
    if (label_factura_cliente_telefono_global) gtk_label_set_text(GTK_LABEL(label_factura_cliente_telefono_global), "");
    if (label_factura_cliente_direccion_global) gtk_label_set_text(GTK_LABEL(label_factura_cliente_direccion_global), "");
    if (entry_factura_producto_codigo_global) gtk_entry_set_text(GTK_ENTRY(entry_factura_producto_codigo_global), "");
    if (entry_factura_producto_cantidad_global) gtk_entry_set_text(GTK_ENTRY(entry_factura_producto_cantidad_global), "");

    // Limpiar la tabla de detalles de la factura actual
    if (factura_detalles_list_store) {
        gtk_list_store_clear(factura_detalles_list_store);
    }
    // Liberar los detalles de la factura actual si existen
    if (current_invoice_details) {
        // g_print("[CALLBACKS] Liberando current_invoice_details (%d elementos)...\n", g_list_length(current_invoice_details));
        // Se usa g_free directamente porque DetalleFactura no contiene punteros a otros datos malloc'd
        g_list_free_full(current_invoice_details, g_free);
        current_invoice_details = NULL;
    }

    // Resetear totales
    if (label_factura_subtotal_global) gtk_label_set_text(GTK_LABEL(label_factura_subtotal_global), "0.00 Bs");
    if (label_factura_iva_global) gtk_label_set_text(GTK_LABEL(label_factura_iva_global), "0.00 Bs");
    if (label_factura_total_global) gtk_label_set_text(GTK_LABEL(label_factura_total_global), "0.00 Bs");
}

/**
 * @brief Limpia los campos de entrada de texto de la pagina de Trabajadores.
 */
static void clear_trabajadores_entry_fields(void) {
    if (entry_trabajadores_cedula_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_cedula_global), "");
    if (entry_trabajadores_nombre_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_nombre_global), "");
    if (entry_trabajadores_apellido_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_apellido_global), "");
    if (entry_trabajadores_numero_empleado_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_numero_empleado_global), "");
    if (entry_trabajadores_telefono_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_telefono_global), "");
    if (entry_trabajadores_direccion_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_direccion_global), "");
}

/**
 * @brief Actualiza los labels de subtotal, IVA y total en la pagina de Facturas.
 */
static void update_factura_totals_labels(void) {
    double subtotal = 0.0;
    GList *iter;
    for (iter = current_invoice_details; iter != NULL; iter = iter->next) {
        DetalleFactura *detalle = (DetalleFactura*)iter->data;
        subtotal += detalle->subtotal_detalle;
    }

    double iva_rate = 0.013; // 1.3% IVA
    double iva_amount = subtotal * iva_rate;
    double total = subtotal + iva_amount;

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.2f Bs", subtotal);
    gtk_label_set_text(GTK_LABEL(label_factura_subtotal_global), buffer);

    snprintf(buffer, sizeof(buffer), "%.2f Bs", iva_amount);
    gtk_label_set_text(GTK_LABEL(label_factura_iva_global), buffer);

    snprintf(buffer, sizeof(buffer), "%.2f Bs", total);
    gtk_label_set_text(GTK_LABEL(label_factura_total_global), buffer);
}


/**
 * @brief Muestra un mensaje al usuario utilizando un GtkMessageDialog.
 * @param parent_window La ventana padre para el dialogo.
 * @param type El tipo de mensaje (GTK_MESSAGE_INFO, GTK_MESSAGE_ERROR, etc.).
 * @param title El titulo del dialogo.
 * @param message El mensaje a mostrar.
 */
static void show_message_dialog(GtkWindow *parent_window, GtkMessageType type, const gchar *title, const gchar *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent_window,
                                               GTK_DIALOG_MODAL,
                                               type,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


// =========================================================
// Implementacion de callbacks de la ventana principal
// =========================================================
/**
 * @brief Maneja la senal 'destroy' de la ventana principal.
 * Libera recursos y sale de la aplicacion.
 * @param widget El widget que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_main_window_destroy(GtkWidget *widget, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_main_window_destroy - Cierre de la aplicacion.\n");
    cleanup_data_manager(); // Llama a la funcion para limpiar datos del data_manager
    gtk_main_quit(); // Sale del bucle principal de GTK
}

// =========================================================
// Implementacion de callbacks de navegacion de la barra lateral
// =========================================================
/**
 * @brief Funcion auxiliar para cambiar la pagina visible del GtkStack.
 * @param page_name El nombre de la pagina a la que se desea cambiar.
 */
static void switch_stack_page(const gchar *page_name) {
    if (main_content_stack) {
        gtk_stack_set_visible_child_name(main_content_stack, page_name);
        g_print("[CALLBACKS] Cambiando a la pagina: %s\n", page_name);
    } else {
        g_printerr("[CALLBACKS] Error: GtkStack principal no esta disponible (NULL).\n");
    }
}
/**
 * @brief Callback para el boton "Inventario".
 */
void on_btn_inventario_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_btn_inventario_clicked - Ir a Inventario.\n");
    switch_stack_page(PAGE_INVENTARIO);
    update_product_tree_view(); // Actualizar la vista de productos al entrar en esta pagina
}
/**
 * @brief Callback para el boton "Productos".
 */
void on_btn_productos_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_btn_productos_clicked - Ir a Productos.\n");
    switch_stack_page(PAGE_PRODUCTOS);
    // Cargar todos los productos inicialmente en la pagina de Productos
    update_productos_page_tree_view("Todos");
    clear_productos_detail_labels(); // Limpiar detalles al cambiar de pagina
}
/**
 * @brief Callback para el boton "Clientes".
 */
void on_btn_clientes_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_btn_clientes_clicked - Ir a Clientes.\n");
    switch_stack_page(PAGE_CLIENTES);
    update_clientes_tree_view(); // Actualizar la vista de clientes al entrar en esta pagina
    clear_clientes_entry_fields(); // Limpiar campos de entrada de clientes
}
/**
 * @brief Callback para el boton "Facturas".
 */
void on_btn_facturas_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_btn_facturas_clicked - Ir a Facturas.\n");
    switch_stack_page(PAGE_FACTURAS);
    update_facturas_tree_view(); // Actualizar la vista de facturas al entrar en esta pagina
    clear_facturas_page_fields(); // Limpiar campos de factura para empezar una nueva
    // Generar un nuevo numero de factura y fecha al entrar a la pagina
    gchar *new_invoice_num = get_next_invoice_number();
    gtk_label_set_text(GTK_LABEL(label_factura_numero_global), new_invoice_num);
    g_free(new_invoice_num); // Liberar la cadena despues de usarla

    time_t rawtime;
    struct tm *info;
    char date_buffer[11]; //,"%Y-%m-%d" + null terminator
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", info);
    gtk_label_set_text(GTK_LABEL(label_factura_fecha_global), date_buffer);
}
/**
 * @brief Callback para el boton "Trabajadores".
 */
void on_btn_trabajadores_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_btn_trabajadores_clicked - Ir a Trabajadores.\n");
    switch_stack_page(PAGE_TRABAJADORES);
    update_trabajadores_tree_view(); // Actualizar la vista de trabajadores al entrar en esta pagina
    clear_trabajadores_entry_fields(); // Limpiar campos de entrada de trabajadores
}
/**
 * @brief Callback para el boton "Reportes".
 */
void on_btn_reportes_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Signal: on_btn_reportes_clicked - Ir a Reportes.\n");
    switch_stack_page(PAGE_REPORTES);
}

// =========================================================
// Implementacion de callbacks de Inventario (CRUD de Productos)
// =========================================================

/**
 * @brief Callback que se activa cuando la seleccion de fila en el TreeView de inventario cambia.
 * Rellena los campos de entrada con los datos del producto seleccionado.
 * @param selection El objeto GtkTreeSelection del TreeView.
 * @param user_data Datos de usuario (no usados).
 */
void on_inventario_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data) {
    g_print("[CALLBACKS] Seleccion de TreeView de Inventario ha cambiado.\n");
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Obtener la fila seleccionada
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *codigo, *nombre, *categoria, *estado, *fecha_adquisicion, *movimiento, *ubicacion;
        gint cantidad, stock_minimo;
        gdouble precio_bs, costo;

        // Obtener los datos de la fila seleccionada por su indice de columna
        gtk_tree_model_get(model, &iter,
                           0, &codigo,          // Col 0: Codigo
                           1, &nombre,          // Col 1: Nombre
                           2, &categoria,       // Col 2: Categoria
                           3, &cantidad,        // Col 3: Cantidad
                           4, &precio_bs,       // Col 4: Precio Bs
                           5, &estado,          // Col 5: Estado Fisico
                           6, &fecha_adquisicion, // Col 6: Fecha Adquisicion
                           7, &movimiento,      // Col 7: Movimiento Inventario
                           8, &ubicacion,       // Col 8: Ubicacion
                           9, &stock_minimo,    // Col 9: Nivel Stock Minimo
                           10, &costo,           // Col 10: Costo Adquisicion
                           -1); // Fin de la lista de argumentos

        // Rellenar los campos de entrada globales
        if (entry_inventario_codigo_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_codigo_global), codigo);
        if (entry_inventario_nombre_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_nombre_global), nombre);
        if (entry_inventario_categoria_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_categoria_global), categoria);

        char buffer[64]; // Suficiente para numeros
        sprintf(buffer, "%d", cantidad);
        if (entry_inventario_cantidad_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_cantidad_global), buffer);

        sprintf(buffer, "%.2f", precio_bs);
        if (entry_inventario_precio_bs_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_precio_bs_global), buffer);

        if (entry_inventario_estado_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_estado_global), estado);

        // Rellenar nuevos campos
        if (entry_inventario_fecha_adquisicion_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_fecha_adquisicion_global), fecha_adquisicion);
        if (entry_inventario_movimiento_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_movimiento_global), movimiento);
        if (entry_inventario_ubicacion_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_ubicacion_global), ubicacion);
        sprintf(buffer, "%d", stock_minimo);
        if (entry_inventario_stock_minimo_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_stock_minimo_global), buffer);
        sprintf(buffer, "%.2f", costo);
        if (entry_inventario_costo_global) gtk_entry_set_text(GTK_ENTRY(entry_inventario_costo_global), buffer);


        // Liberar las cadenas obtenidas de gtk_tree_model_get (son g_malloc-ed)
        g_free(codigo);
        g_free(nombre);
        g_free(categoria);
        g_free(estado);
        g_free(fecha_adquisicion);
        g_free(movimiento);
        g_free(ubicacion);

    } else {
        // Si no hay seleccion, limpiar todos los campos de entrada
        g_print("[CALLBACKS] No hay seleccion en el TreeView de Inventario. Limpiando todos los campos de entrada.\n");
        clear_inventory_entry_fields();
    }
}


/**
 * @brief Maneja el clic del boton "Crear Producto".
 * Obtiene los datos de todos los campos de entrada y anade un nuevo producto.
 */
void on_inventario_crear_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Crear Producto' presionado.\n");
    Producto nuevo_producto;
    const gchar *codigo_str = get_entry_text(entry_inventario_codigo_global);
    const gchar *nombre_str = get_entry_text(entry_inventario_nombre_global);
    const gchar *categoria_str = get_entry_text(entry_inventario_categoria_global);
    const gchar *cantidad_str = get_entry_text(entry_inventario_cantidad_global);
    const gchar *precio_bs_str = get_entry_text(entry_inventario_precio_bs_global);
    const gchar *estado_str = get_entry_text(entry_inventario_estado_global);
    const gchar *fecha_adquisicion_str = get_entry_text(entry_inventario_fecha_adquisicion_global);
    const gchar *movimiento_str = get_entry_text(entry_inventario_movimiento_global);
    const gchar *ubicacion_str = get_entry_text(entry_inventario_ubicacion_global);
    const gchar *stock_minimo_str = get_entry_text(entry_inventario_stock_minimo_global);
    const gchar *costo_str = get_entry_text(entry_inventario_costo_global);

    // Validacion basica de campos obligatorios
    if (!codigo_str || strlen(codigo_str) == 0 ||
        !nombre_str || strlen(nombre_str) == 0 ||
        !cantidad_str || strlen(cantidad_str) == 0 ||
        !precio_bs_str || strlen(precio_bs_str) == 0)
    {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Creacion", "Por favor, complete los campos obligatorios: Codigo, Nombre, Cantidad, Precio (Bs).");
        return;
    }

    // Copiar los datos de los GtkEntry a la estructura Producto
    strcpy(nuevo_producto.codigo, codigo_str);
    strcpy(nuevo_producto.nombre, nombre_str);
    strcpy(nuevo_producto.categoria, categoria_str ? categoria_str : "");
    nuevo_producto.cantidad_disponible = atoi(cantidad_str);
    nuevo_producto.precio_venta_bs = atof(precio_bs_str);
    strcpy(nuevo_producto.estado_fisico, estado_str ? estado_str : "Nuevo");

    // Copiar los nuevos campos
    strcpy(nuevo_producto.fecha_adquisicion, fecha_adquisicion_str ? fecha_adquisicion_str : "");
    strcpy(nuevo_producto.movimiento_inventario, movimiento_str ? movimiento_str : "");
    strcpy(nuevo_producto.ubicacion, ubicacion_str ? ubicacion_str : "");
    nuevo_producto.nivel_stock_minimo = atoi(stock_minimo_str ? stock_minimo_str : "0");
    nuevo_producto.costo = atof(costo_str ? costo_str : "0.0");


    // Asignar valores por defecto para campos no expuestos en la UI o no relevantes en este momento
    strcpy(nuevo_producto.descripcion, "Sin descripcion");
    strcpy(nuevo_producto.proveedor, "Proveedor Desconocido");
    nuevo_producto.precio_venta_cop = nuevo_producto.precio_venta_bs * 100; // Ejemplo de conversion
    nuevo_producto.precio_venta_usd = nuevo_producto.precio_venta_bs / 40; // Ejemplo de conversion
    nuevo_producto.precio_venta_eur = nuevo_producto.precio_venta_usd * 0.9; // Ejemplo de conversion

    if (add_producto(&nuevo_producto)) {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Creacion Exitosa", "Producto creado exitosamente.");
        clear_inventory_entry_fields(); // Limpiar campos despues de crear
        update_product_tree_view();     // Actualizar la lista mostrada en el TreeView de Inventario
        update_productos_page_tree_view("Todos"); // Tambien actualizar la pagina de Productos
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Creacion", "Fallo al crear el producto. Posiblemente un producto con el codigo ya existe.");
    }
}

/**
 * @brief Maneja el clic del boton "Buscar Producto".
 * Obtiene el codigo del campo de entrada, busca el producto y rellena todos los campos si lo encuentra.
 * Tambien selecciona la fila correspondiente en el TreeView.
 */
void on_inventario_buscar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Buscar Producto' presionado.\n");
    const gchar *codigo_str = get_entry_text(entry_inventario_codigo_global);

    if (!codigo_str || strlen(codigo_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Campo Vacio", "Por favor, ingrese un codigo en el campo 'Codigo' para buscar.");
        return;
    }

    Producto *p = find_producto(codigo_str); // Buscar el producto en el data_manager
    if (p) {
        g_print("[CALLBACKS] Producto encontrado: %s (Codigo: %s).\n", p->nombre, p->codigo);

        // Rellenar todos los campos con los datos del producto encontrado
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_codigo_global), p->codigo);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_nombre_global), p->nombre);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_categoria_global), p->categoria);

        char buffer[64];
        sprintf(buffer, "%d", p->cantidad_disponible);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_cantidad_global), buffer);

        sprintf(buffer, "%.2f", p->precio_venta_bs);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_precio_bs_global), buffer);

        gtk_entry_set_text(GTK_ENTRY(entry_inventario_estado_global), p->estado_fisico);

        // Rellenar nuevos campos
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_fecha_adquisicion_global), p->fecha_adquisicion);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_movimiento_global), p->movimiento_inventario);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_ubicacion_global), p->ubicacion);
        sprintf(buffer, "%d", p->nivel_stock_minimo);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_stock_minimo_global), buffer);
        sprintf(buffer, "%.2f", p->costo);
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_costo_global), buffer);


        // Opcional: Seleccionar la fila correspondiente en el TreeView despues de la busqueda
        if (inventory_tree_view_global && product_list_store) {
            GtkTreePath *path = NULL;
            GtkTreeIter iter;
            gboolean found = FALSE;

            // Iterar sobre el modelo para encontrar el producto por codigo
            if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(product_list_store), &iter)) {
                do {
                    gchar *current_codigo;
                    gtk_tree_model_get(GTK_TREE_MODEL(product_list_store), &iter, 0, &current_codigo, -1);
                    if (current_codigo && strcmp(current_codigo, codigo_str) == 0) {
                        path = gtk_tree_model_get_path(GTK_TREE_MODEL(product_list_store), &iter);
                        found = TRUE;
                        g_free(current_codigo);
                        break;
                    }
                    g_free(current_codigo);
                } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(product_list_store), &iter));
            }

            if (found && path) {
                GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(inventory_tree_view_global));
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(inventory_tree_view_global), path, NULL, FALSE, 0.0, 0.0);
                gtk_tree_path_free(path);
            }
        }

    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Producto No Encontrado", "Producto con el codigo especificado no encontrado.");
        // Limpiar campos si no se encuentra, pero mantener el codigo buscado
        clear_inventory_entry_fields();
        gtk_entry_set_text(GTK_ENTRY(entry_inventario_codigo_global), codigo_str); // Deja el codigo buscado
        // Deseleccionar cualquier fila en el TreeView
        if (inventory_tree_view_global) {
            GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(inventory_tree_view_global));
            gtk_tree_selection_unselect_all(selection);
        }
    }
}

/**
 * @brief Maneja el clic del boton "Modificar Producto".
 * Obtiene los datos actualizados de todos los campos de entrada y modifica el producto existente.
 */
void on_inventario_modificar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Modificar Producto' presionado.\n");
    Producto producto_modificado; // Estructura temporal para los datos actualizados
    const gchar *codigo_str = get_entry_text(entry_inventario_codigo_global);
    const gchar *nombre_str = get_entry_text(entry_inventario_nombre_global);
    const gchar *categoria_str = get_entry_text(entry_inventario_categoria_global);
    const gchar *cantidad_str = get_entry_text(entry_inventario_cantidad_global);
    const gchar *precio_bs_str = get_entry_text(entry_inventario_precio_bs_global);
    const gchar *estado_str = get_entry_text(entry_inventario_estado_global);
    const gchar *fecha_adquisicion_str = get_entry_text(entry_inventario_fecha_adquisicion_global);
    const gchar *movimiento_str = get_entry_text(entry_inventario_movimiento_global);
    const gchar *ubicacion_str = get_entry_text(entry_inventario_ubicacion_global);
    const gchar *stock_minimo_str = get_entry_text(entry_inventario_stock_minimo_global);
    const gchar *costo_str = get_entry_text(entry_inventario_costo_global);

    if (!codigo_str || strlen(codigo_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Campo Requerido", "Por favor, ingrese un codigo en el campo 'Codigo' para modificar el producto.");
        return;
    }
    // Copiar los datos de los GtkEntry a la estructura Producto para la modificacion
    strcpy(producto_modificado.codigo, codigo_str);
    strcpy(producto_modificado.nombre, nombre_str ? nombre_str : "");
    strcpy(producto_modificado.categoria, categoria_str ? categoria_str : "");
    producto_modificado.cantidad_disponible = atoi(cantidad_str ? cantidad_str : "0");
    producto_modificado.precio_venta_bs = atof(precio_bs_str ? precio_bs_str : "0.0");
    strcpy(producto_modificado.estado_fisico, estado_str ? estado_str : "");

    // Copiar los nuevos campos
    strcpy(producto_modificado.fecha_adquisicion, fecha_adquisicion_str ? fecha_adquisicion_str : "");
    strcpy(producto_modificado.movimiento_inventario, movimiento_str ? movimiento_str : "");
    strcpy(producto_modificado.ubicacion, ubicacion_str ? ubicacion_str : "");
    producto_modificado.nivel_stock_minimo = atoi(stock_minimo_str ? stock_minimo_str : "0");
    producto_modificado.costo = atof(costo_str ? costo_str : "0.0");

    // Asignar valores por defecto para campos no expuestos en la UI simple de este ejemplo,
    // o conservar los valores originales si no se modifican a traves de la UI.
    // Esto es crucial para no sobrescribir datos no visibles con valores vacios/cero.
    Producto *original_producto = find_producto(codigo_str);
    if (original_producto) {
        // Solo copiamos si no hay entrada directa en la UI o queremos mantener el valor
        if (strlen(producto_modificado.descripcion) == 0) strcpy(producto_modificado.descripcion, original_producto->descripcion);
        if (strlen(producto_modificado.proveedor) == 0) strcpy(producto_modificado.proveedor, original_producto->proveedor);
        // ... y para otros campos numericos/string que no se editen directamente
        producto_modificado.precio_venta_cop = original_producto->precio_venta_cop;
        producto_modificado.precio_venta_usd = original_producto->precio_venta_usd;
        producto_modificado.precio_venta_eur = original_producto->precio_venta_eur;
    } else {
        // En caso de que no se encuentre el original (lo que indicaria un error de flujo), usar valores por defecto
        strcpy(producto_modificado.descripcion, "Sin descripcion (modificado)");
        strcpy(producto_modificado.proveedor, "Proveedor General (modificado)");
        producto_modificado.precio_venta_cop = producto_modificado.precio_venta_bs * 100;
        producto_modificado.precio_venta_usd = producto_modificado.precio_venta_bs / 40;
        producto_modificado.precio_venta_eur = producto_modificado.precio_venta_usd * 0.9;
    }


    if (update_producto(&producto_modificado)) { // Llamar a la funcion de actualizacion del data_manager
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Modificacion Exitosa", "Producto modificado exitosamente.");
        clear_inventory_entry_fields(); // Limpiar campos despues de modificar
        update_product_tree_view();     // Actualizar la lista mostrada en el TreeView de Inventario
        update_productos_page_tree_view(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(productos_category_combo_global))); // Actualiza pagina Productos con filtro actual
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Modificacion", "Fallo al modificar el producto. El codigo no fue encontrado.");
    }
}

/**
 * @brief Maneja el clic del boton "Eliminar Producto".
 * Obtiene el codigo del producto seleccionado en el GtkTreeView y lo elimina.
 */
void on_inventario_eliminar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Eliminar Producto' presionado.\n");

    if (!inventory_tree_view_global) {
        g_printerr("[CALLBACKS] Error: GtkTreeView de inventario no esta inicializado.\n");
        return;
    }

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(inventory_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Intentar obtener la seleccion actual
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *codigo_producto;
        // Obtener el codigo del producto de la columna 0 de la fila seleccionada
        gtk_tree_model_get(model, &iter, 0, &codigo_producto, -1);

        if (codigo_producto) {
            g_print("[CALLBACKS] Intento eliminar producto con codigo: %s\n", codigo_producto);
            // Confirmacion antes de eliminar
            GtkWidget *dialog = gtk_message_dialog_new(main_app_window,
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_QUESTION,
                                                       GTK_BUTTONS_YES_NO,
                                                       "Esta seguro que desea eliminar el producto '%s'?", codigo_producto);
            gtk_window_set_title(GTK_WINDOW(dialog), "Confirmar Eliminacion");
            gint response = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            if (response == GTK_RESPONSE_YES) {
                if (delete_producto(codigo_producto)) { // Llamar a la funcion de eliminacion del data_manager
                    show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Eliminacion Exitosa", "Producto eliminado exitosamente.");
                    gtk_list_store_remove(GTK_LIST_STORE(model), &iter); // Eliminar la fila del GtkListStore
                    clear_inventory_entry_fields(); // Limpiar campos despues de eliminar
                    update_product_tree_view();     // Actualizar la lista mostrada en el TreeView de Inventario
                    update_productos_page_tree_view("Todos"); // Tambien actualizar la pagina de Productos
                } else {
                    show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Eliminacion", "Fallo al eliminar el producto. No se encontro en la lista de datos.");
                }
            }
            g_free(codigo_producto); // Liberar la cadena obtenida con gtk_tree_model_get
        } else {
            g_printerr("[CALLBACKS] Error: No se pudo obtener el codigo del producto de la seleccion.\n");
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Seleccion", "No se pudo obtener el codigo del producto seleccionado.");
        }
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Por favor, seleccione un producto de la lista para marcar como vendido.");
    }
}


// =========================================================
// Implementacion de callbacks de Productos
// =========================================================

/**
 * @brief Callback for the category filter ComboBox on the Products page.
 * Updates the product list based on the selected category.
 * @param combo_box The GtkComboBox that emitted the signal.
 * @param user_data User data (not used).
 */
void on_productos_category_changed(GtkComboBox *combo_box, gpointer user_data) {
    g_print("[CALLBACKS] Filtro de categoria en Productos ha cambiado.\n");
    const gchar *selected_category = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
    if (selected_category) {
        update_productos_page_tree_view(selected_category);
        clear_productos_detail_labels(); // Clear previous details
        g_free((gchar*)selected_category); // Free the string returned by get_active_text
    }
}

/**
 * @brief Callback for selection change in the Products page TreeView.
 * Displays details of the selected product in the detail labels.
 * @param selection The GtkTreeSelection object of the TreeView.
 * @param user_data User data (not used).
 */
void on_productos_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data) {
    g_print("[CALLBACKS] Seleccion de TreeView de Productos ha cambiado.\n");
    GtkTreeModel *model;
    GtkTreeIter iter;

    clear_productos_detail_labels(); // Clear previous details

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *codigo_producto_selected;
        gtk_tree_model_get(model, &iter, 0, &codigo_producto_selected, -1); // Get code from selected row

        if (codigo_producto_selected) {
            Producto *p = find_producto(codigo_producto_selected); // Find full product data
            if (p) {
                // Populate all detail labels
                gtk_label_set_text(GTK_LABEL(label_productos_codigo_global), p->codigo);
                gtk_label_set_text(GTK_LABEL(label_productos_nombre_global), p->nombre);
                gtk_label_set_text(GTK_LABEL(label_productos_categoria_global), p->categoria);

                char buffer[64];
                sprintf(buffer, "%d", p->cantidad_disponible);
                gtk_label_set_text(GTK_LABEL(label_productos_cantidad_global), buffer);

                sprintf(buffer, "%.2f", p->precio_venta_bs);
                gtk_label_set_text(GTK_LABEL(label_productos_precio_bs_global), buffer);

                gtk_label_set_text(GTK_LABEL(label_productos_estado_global), p->estado_fisico);
                gtk_label_set_text(GTK_LABEL(label_productos_fecha_adq_global), p->fecha_adquisicion);
                gtk_label_set_text(GTK_LABEL(label_productos_movimiento_global), p->movimiento_inventario);
                gtk_label_set_text(GTK_LABEL(label_productos_ubicacion_global), p->ubicacion);
                sprintf(buffer, "%d", p->nivel_stock_minimo);
                gtk_label_set_text(GTK_LABEL(label_productos_stock_min_global), buffer);
                sprintf(buffer, "%.2f", p->costo);
                gtk_label_set_text(GTK_LABEL(label_productos_costo_adq_global), buffer);

                g_print("[CALLBACKS] Detalles del producto '%s' mostrados.\n", p->nombre);
            } else {
                g_printerr("[CALLBACKS] Error: Producto con codigo '%s' no encontrado en el data_manager al seleccionar en Productos.\n", codigo_producto_selected);
            }
            g_free(codigo_producto_selected);
        }
    }
}

/**
 * @brief Callback for the "Marcar como Vendido" button on the Products page.
 * Prompts for quantity sold and updates the product's available quantity.
 * @param button The GtkButton that emitted the signal.
 * @param user_data User data (not used).
 */
void on_productos_marcar_vendido_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Marcar como Vendido' presionado.\n");

    if (!productos_tree_view_global) {
        g_printerr("[CALLBACKS] Error: GtkTreeView de Productos no esta inicializado.\n");
        return;
    }

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(productos_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *codigo_producto_selected;
        gint cantidad_actual;
        gtk_tree_model_get(model, &iter, 0, &codigo_producto_selected, 3, &cantidad_actual, -1); // Get code and current quantity

        Producto *p_actual = find_producto(codigo_producto_selected);
        if (!p_actual) {
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Venta", "Producto no encontrado en el inventario.");
            g_free(codigo_producto_selected);
            return;
        }

        // Crear un dialogo para solicitar la cantidad a vender
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Registrar Venta",
                                                        main_app_window,
                                                        GTK_DIALOG_MODAL,
                                                        "_Vender", GTK_RESPONSE_OK,
                                                        "_Cancelar", GTK_RESPONSE_CANCEL,
                                                        NULL);
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *label = gtk_label_new("Cantidad a vender:");
        GtkWidget *entry_cantidad_venta = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry_cantidad_venta), "1"); // Valor por defecto
        gtk_box_pack_start(GTK_BOX(content_area), label, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(content_area), entry_cantidad_venta, FALSE, FALSE, 5);
        gtk_widget_show_all(dialog);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gint cantidad_vendida = -1;

        if (response == GTK_RESPONSE_OK) {
            const gchar *cantidad_str = gtk_entry_get_text(GTK_ENTRY(entry_cantidad_venta));
            cantidad_vendida = atoi(cantidad_str);
            if (cantidad_vendida <= 0) {
                show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Cantidad Invalida", "La cantidad a vender debe ser un numero positivo.");
                cantidad_vendida = -1; // Invalid quantity
            }
        }
        gtk_widget_destroy(dialog); // Destruir el dialogo despues de usarlo

        if (cantidad_vendida > 0) {
            if (p_actual->cantidad_disponible >= cantidad_vendida) {
                p_actual->cantidad_disponible -= cantidad_vendida;
                // Actualizar el producto en el data_manager (re-usa la funcion de modificacion)
                if (update_producto(p_actual)) {
                    show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Venta Exitosa", "Producto vendido exitosamente. Cantidad disponible actualizada.");

                    // Actualizar AMBAS tablas para reflejar los cambios
                    update_product_tree_view(); // Tabla de Inventario
                    const gchar *current_filter = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(productos_category_combo_global));
                    update_productos_page_tree_view(current_filter ? current_filter : "Todos"); // Tabla de Productos
                    if (current_filter) g_free((gchar*)current_filter);
                    clear_productos_detail_labels(); // Limpiar detalles
                } else {
                    show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Actualizacion", "No se pudo actualizar la cantidad del producto en el inventario.");
                }
            } else {
                show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Stock Insuficiente", "No hay suficiente cantidad disponible para la venta.");
            }
        }
        g_free(codigo_producto_selected);
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Por favor, seleccione un producto de la lista para marcar como vendido.");
    }
}

// =========================================================
// Implementacion de callbacks de Clientes
// =========================================================

/**
 * @brief Maneja el clic del boton "Crear Cliente".
 * Obtiene los datos de los campos de entrada y anade un nuevo cliente.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_clientes_crear_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Crear Cliente' presionado.\n");
    Cliente nuevo_cliente;
    const gchar *cedula_str = get_entry_text(entry_clientes_cedula_global);
    const gchar *nombre_str = get_entry_text(entry_clientes_nombre_global);
    const gchar *apellido_str = get_entry_text(entry_clientes_apellido_global);
    const gchar *telefono_str = get_entry_text(entry_clientes_telefono_global);
    const gchar *direccion_str = get_entry_text(entry_clientes_direccion_global);

    if (!cedula_str || strlen(cedula_str) == 0 ||
        !nombre_str || strlen(nombre_str) == 0 ||
        !apellido_str || strlen(apellido_str) == 0)
    {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Creacion", "Por favor, complete los campos obligatorios: Cedula, Nombre, Apellido.");
        return;
    }

    strcpy(nuevo_cliente.cedula, cedula_str);
    strcpy(nuevo_cliente.nombre, nombre_str);
    strcpy(nuevo_cliente.apellido, apellido_str);
    strcpy(nuevo_cliente.telefono, telefono_str ? telefono_str : "");
    strcpy(nuevo_cliente.direccion, direccion_str ? direccion_str : "");

    if (add_cliente(&nuevo_cliente)) {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Creacion Exitosa", "Cliente creado exitosamente.");
        clear_clientes_entry_fields();
        update_clientes_tree_view();
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Creacion", "Fallo al crear el cliente. Posiblemente un cliente con esa cedula ya existe.");
    }
}

/**
 * @brief Maneja el clic del boton "Buscar Cliente".
 * Obtiene la cedula del campo de entrada, busca el cliente y rellena todos los campos si lo encuentra.
 * Tambien selecciona la fila correspondiente en el TreeView.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_clientes_buscar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Buscar Cliente' presionado.\n");
    const gchar *cedula_str = get_entry_text(entry_clientes_cedula_global);

    if (!cedula_str || strlen(cedula_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Campo Vacio", "Por favor, ingrese una cedula para buscar.");
        return;
    }

    Cliente *c = find_cliente(cedula_str);
    if (c) {
        g_print("[CALLBACKS] Cliente encontrado: %s %s (Cedula: %s).\n", c->nombre, c->apellido, c->cedula);
        gtk_entry_set_text(GTK_ENTRY(entry_clientes_cedula_global), c->cedula);
        gtk_entry_set_text(GTK_ENTRY(entry_clientes_nombre_global), c->nombre);
        gtk_entry_set_text(GTK_ENTRY(entry_clientes_apellido_global), c->apellido);
        gtk_entry_set_text(GTK_ENTRY(entry_clientes_telefono_global), c->telefono);
        gtk_entry_set_text(GTK_ENTRY(entry_clientes_direccion_global), c->direccion);

        // Opcional: Seleccionar la fila correspondiente en el TreeView
        if (clientes_tree_view_global && clientes_list_store) {
            GtkTreePath *path = NULL;
            GtkTreeIter iter;
            gboolean found = FALSE;

            if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(clientes_list_store), &iter)) {
                do {
                    gchar *current_cedula;
                    gtk_tree_model_get(GTK_TREE_MODEL(clientes_list_store), &iter, 0, &current_cedula, -1);
                    if (current_cedula && strcmp(current_cedula, cedula_str) == 0) {
                        path = gtk_tree_model_get_path(GTK_TREE_MODEL(clientes_list_store), &iter);
                        found = TRUE;
                        g_free(current_cedula);
                        break;
                    }
                    g_free(current_cedula);
                } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(clientes_list_store), &iter));
            }

            if (found && path) {
                GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clientes_tree_view_global));
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(clientes_tree_view_global), path, NULL, FALSE, 0.0, 0.0);
                gtk_tree_path_free(path);
            }
        }

    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Cliente No Encontrado", "Cliente con la cedula especificada no encontrado.");
        clear_clientes_entry_fields();
        gtk_entry_set_text(GTK_ENTRY(entry_clientes_cedula_global), cedula_str);
        if (clientes_tree_view_global) {
            GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clientes_tree_view_global));
            gtk_tree_selection_unselect_all(selection);
        }
    }
}

/**
 * @brief Maneja el clic del boton "Modificar Cliente".
 * Obtiene los datos actualizados de los campos de entrada y modifica el cliente existente.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_clientes_modificar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Modificar Cliente' presionado.\n");
    Cliente cliente_modificado;
    const gchar *cedula_str = get_entry_text(entry_clientes_cedula_global);
    const gchar *nombre_str = get_entry_text(entry_clientes_nombre_global);
    const gchar *apellido_str = get_entry_text(entry_clientes_apellido_global);
    const gchar *telefono_str = get_entry_text(entry_clientes_telefono_global);
    const gchar *direccion_str = get_entry_text(entry_clientes_direccion_global);

    if (!cedula_str || strlen(cedula_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Campo Requerido", "Por favor, ingrese la cedula del cliente a modificar.");
        return;
    }

    strcpy(cliente_modificado.cedula, cedula_str);
    strcpy(cliente_modificado.nombre, nombre_str ? nombre_str : "");
    strcpy(cliente_modificado.apellido, apellido_str ? apellido_str : "");
    strcpy(cliente_modificado.telefono, telefono_str ? telefono_str : "");
    strcpy(cliente_modificado.direccion, direccion_str ? direccion_str : "");

    if (update_cliente(&cliente_modificado)) {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Modificacion Exitosa", "Cliente modificado exitosamente.");
        clear_clientes_entry_fields();
        update_clientes_tree_view();
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Modificacion", "Fallo al modificar el cliente. Cedula no encontrada.");
    }
}

/**
 * @brief Maneja el clic del boton "Eliminar Cliente".
 * Obtiene la cedula del cliente seleccionado en el GtkTreeView y lo elimina.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_clientes_eliminar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Eliminar Cliente' presionado.\n");

    if (!clientes_tree_view_global) {
        g_printerr("[CALLBACKS] Error: GtkTreeView de clientes no esta inicializado.\n");
        return;
    }

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clientes_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *cedula_cliente;
        gtk_tree_model_get(model, &iter, 0, &cedula_cliente, -1);

        if (cedula_cliente) {
            GtkWidget *dialog = gtk_message_dialog_new(main_app_window,
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_QUESTION,
                                                       GTK_BUTTONS_YES_NO,
                                                       "Esta seguro que desea eliminar el cliente con cedula '%s'?", cedula_cliente);
            gtk_window_set_title(GTK_WINDOW(dialog), "Confirmar Eliminacion");
            gint response = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            if (response == GTK_RESPONSE_YES) {
                if (delete_cliente(cedula_cliente)) {
                    show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Eliminacion Exitosa", "Cliente eliminado exitosamente.");
                    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
                    clear_clientes_entry_fields();
                } else {
                    show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Eliminacion", "Fallo al eliminar el cliente. No se encontro en la lista de datos.");
                }
            }
            g_free(cedula_cliente);
        } else {
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Seleccion", "No se pudo obtener la cedula del cliente seleccionado.");
        }
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Por favor, seleccione un cliente de la lista para eliminar.");
    }
}

/**
 * @brief Callback que se activa cuando la seleccion de fila en el TreeView de clientes cambia.
 * Rellena los campos de entrada con los datos del cliente seleccionado.
 * @param selection El objeto GtkTreeSelection del TreeView.
 * @param user_data Datos de usuario (no usados).
 */
void on_clientes_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data) {
    g_print("[CALLBACKS] Seleccion de TreeView de Clientes ha cambiado.\n");
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *cedula, *nombre, *apellido, *telefono, *direccion;

        gtk_tree_model_get(model, &iter,
                           0, &cedula,
                           1, &nombre,
                           2, &apellido,
                           3, &telefono,
                           4, &direccion,
                           -1);

        if (entry_clientes_cedula_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_cedula_global), cedula);
        if (entry_clientes_nombre_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_nombre_global), nombre);
        if (entry_clientes_apellido_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_apellido_global), apellido);
        if (entry_clientes_telefono_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_telefono_global), telefono);
        if (entry_clientes_direccion_global) gtk_entry_set_text(GTK_ENTRY(entry_clientes_direccion_global), direccion);

        g_free(cedula);
        g_free(nombre);
        g_free(apellido);
        g_free(telefono);
        g_free(direccion);
    } else {
        g_print("[CALLBACKS] No hay seleccion en el TreeView de Clientes. Limpiando campos de entrada.\n");
        clear_clientes_entry_fields();
    }
}


// =========================================================
// Implementacion de callbacks de Facturas
// =========================================================

/**
 * @brief Maneja el clic del boton "Nueva Factura".
 * Limpia todos los campos de entrada y la tabla de detalles, y genera un nuevo numero de factura.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_nueva_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Nueva Factura' presionado.\n");
    clear_facturas_page_fields(); // Limpiar todos los campos de la factura actual
    // Generar un nuevo numero de factura y fecha
    gchar *new_invoice_num = get_next_invoice_number();
    gtk_label_set_text(GTK_LABEL(label_factura_numero_global), new_invoice_num);
    g_free(new_invoice_num);

    time_t rawtime;
    struct tm *info;
    char date_buffer[11];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", info);
    gtk_label_set_text(GTK_LABEL(label_factura_fecha_global), date_buffer);
}

/**
 * @brief Maneja el clic del boton "Buscar Cliente" en la pagina de Facturas.
 * Busca el cliente por cedula y rellena los campos de datos del cliente en la factura.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_buscar_cliente_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Buscar Cliente' (Facturas) presionado.\n");
    const gchar *cedula_str = get_entry_text(entry_factura_cliente_cedula_global);

    if (!cedula_str || strlen(cedula_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Campo Vacio", "Por favor, ingrese una cedula en el campo 'Cedula Cliente' para buscar.");
        // Limpiar los labels de cliente si no hay cedula
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_nombre_global), "");
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_telefono_global), "");
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_direccion_global), "");
        return;
    }

    Cliente *c = find_cliente(cedula_str);
    if (c) {
        g_print("[CALLBACKS] Cliente '%s %s' encontrado para la factura.\n", c->nombre, c->apellido);
        char full_name[256];
        snprintf(full_name, sizeof(full_name), "%s %s", c->nombre, c->apellido);
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_nombre_global), full_name);
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_telefono_global), c->telefono);
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_direccion_global), c->direccion);
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Cliente No Encontrado", "Cliente con la cedula especificada no encontrado. Por favor, registralo primero en la seccion de Clientes.");
        // Limpiar los labels de cliente si no se encuentra
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_nombre_global), "");
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_telefono_global), "");
        gtk_label_set_text(GTK_LABEL(label_factura_cliente_direccion_global), "");
    }
}

/**
 * @brief Maneja el clic del boton "Anadir Producto" a la factura actual.
 * Busca el producto, valida la cantidad y lo anade a la lista temporal de detalles de factura.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_anadir_producto_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Anadir Producto a Factura' presionado.\n");
    const gchar *codigo_prod_str = get_entry_text(entry_factura_producto_codigo_global);
    const gchar *cantidad_prod_str = get_entry_text(entry_factura_producto_cantidad_global);

    if (!codigo_prod_str || strlen(codigo_prod_str) == 0 ||
        !cantidad_prod_str || strlen(cantidad_prod_str) == 0)
    {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Datos Incompletos", "Por favor, ingrese el Codigo y la Cantidad del producto.");
        return;
    }

    int cantidad_a_anadir = atoi(cantidad_prod_str);
    if (cantidad_a_anadir <= 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Cantidad Invalida", "La cantidad debe ser un numero positivo.");
        return;
    }

    Producto *producto_inventario = find_producto(codigo_prod_str);
    if (!producto_inventario) {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Producto No Encontrado", "Producto no encontrado en el inventario. Verifique el codigo.");
        return;
    }

    if (producto_inventario->cantidad_disponible < cantidad_a_anadir) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Cantidad insuficiente en inventario. Solo hay %d unidades de '%s' disponibles.",
                 producto_inventario->cantidad_disponible, producto_inventario->nombre);
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Stock Insuficiente", msg);
        return;
    }

    // Comprobar si el producto ya esta en la lista de detalles de la factura actual
    GList *iter;
    gboolean found_in_invoice_details = FALSE;
    for (iter = current_invoice_details; iter != NULL; iter = iter->next) {
        DetalleFactura *detalle_existente = (DetalleFactura*)iter->data;
        if (strcmp(detalle_existente->codigo_producto, codigo_prod_str) == 0) {
            // Producto ya existe, actualizar cantidad
            if (producto_inventario->cantidad_disponible < (detalle_existente->cantidad + cantidad_a_anadir)) {
                 char msg[256];
                 snprintf(msg, sizeof(msg), "No se puede anadir mas de %d unidades de '%s'. Stock limitado. Actualmente ya tienes %d unidades en esta factura.",
                          producto_inventario->cantidad_disponible - detalle_existente->cantidad, producto_inventario->nombre, detalle_existente->cantidad);
                 show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Stock Insuficiente", msg);
                 return;
            }
            detalle_existente->cantidad += cantidad_a_anadir;
            detalle_existente->subtotal_detalle = detalle_existente->cantidad * detalle_existente->precio_unitario;
            found_in_invoice_details = TRUE;
            g_print("[CALLBACKS] Cantidad de producto '%s' actualizada en detalles de factura.\n", producto_inventario->nombre);
            break;
        }
    }

    if (!found_in_invoice_details) {
        // Si el producto no estaba en la factura, anadirlo como nuevo detalle
        DetalleFactura *nuevo_detalle = (DetalleFactura*)malloc(sizeof(DetalleFactura));
        if (!nuevo_detalle) {
            g_printerr("[CALLBACKS] Error: Fallo al asignar memoria para nuevo detalle de factura.\n");
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Memoria", "No se pudo asignar memoria para el detalle del producto.");
            return;
        }

        strcpy(nuevo_detalle->codigo_producto, producto_inventario->codigo);
        strcpy(nuevo_detalle->nombre_producto, producto_inventario->nombre);
        strcpy(nuevo_detalle->tipo_zapato, producto_inventario->categoria);
        nuevo_detalle->cantidad = cantidad_a_anadir;
        nuevo_detalle->precio_unitario = producto_inventario->precio_venta_bs;
        nuevo_detalle->subtotal_detalle = (double)nuevo_detalle->cantidad * nuevo_detalle->precio_unitario;

        current_invoice_details = g_list_append(current_invoice_details, nuevo_detalle);
        g_print("[CALLBACKS] Nuevo producto '%s' anadido a detalles de factura.\n", producto_inventario->nombre);
    }

    // Actualizar la tabla de detalles de la factura actual
    gtk_list_store_clear(factura_detalles_list_store);
    for (iter = current_invoice_details; iter != NULL; iter = iter->next) {
        DetalleFactura *detalle = (DetalleFactura*)iter->data;
        GtkTreeIter list_iter_gtk; // Renombrado para evitar conflicto con 'iter' de GList
        gtk_list_store_append(factura_detalles_list_store, &list_iter_gtk); // Anadir nueva fila
        gtk_list_store_set(factura_detalles_list_store, &list_iter_gtk,
                           0, detalle->codigo_producto,
                           1, detalle->nombre_producto,
                           2, detalle->cantidad,
                           3, detalle->precio_unitario,
                           4, detalle->subtotal_detalle,
                           -1);
    }

    // Limpiar campos de entrada de producto y actualizar totales
    gtk_entry_set_text(GTK_ENTRY(entry_factura_producto_codigo_global), "");
    gtk_entry_set_text(GTK_ENTRY(entry_factura_producto_cantidad_global), "");
    update_factura_totals_labels();
}

/**
 * @brief Maneja el clic del boton "Eliminar Producto de Factura" (de la factura actual).
 * Elimina el producto seleccionado de la lista temporal de detalles de factura.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_eliminar_detalle_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Eliminar Producto de Factura' presionado.\n");
    if (!factura_detalles_tree_view_global) {
        g_printerr("[CALLBACKS] Error: TreeView de detalles de factura no esta inicializado.\n");
        return;
    }

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(factura_detalles_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter_gtk; // Iterador para el GtkTreeView

    if (gtk_tree_selection_get_selected(selection, &model, &iter_gtk)) {
        gchar *codigo_detalle_ui; // Codigo del producto seleccionado en la UI
        gtk_tree_model_get(model, &iter_gtk, 0, &codigo_detalle_ui, -1);

        if (codigo_detalle_ui) {
            GList *list_iter = current_invoice_details;
            GList *prev_iter = NULL;
            gboolean removed = FALSE;

            while (list_iter != NULL) {
                DetalleFactura *detalle = (DetalleFactura*)list_iter->data;
                if (strcmp(detalle->codigo_producto, codigo_detalle_ui) == 0) {
                    if (prev_iter) {
                        current_invoice_details = g_list_remove_link(current_invoice_details, list_iter);
                        g_list_free_1(list_iter); // Liberar el enlace de la lista
                    } else {
                        current_invoice_details = g_list_remove_link(current_invoice_details, list_iter);
                        g_list_free_1(list_iter); // Liberar el enlace de la lista
                    }
                    g_free(detalle); // Liberar la memoria del DetalleFactura
                    removed = TRUE;
                    show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Detalle Eliminado", "Producto eliminado de la factura actual.");
                    break;
                }
                prev_iter = list_iter;
                list_iter = list_iter->next;
            }
            g_free(codigo_detalle_ui);

            if (removed) {
                // Actualizar la tabla de detalles de la factura actual y los totales
                gtk_list_store_clear(factura_detalles_list_store);
                GList *temp_iter;
                for (temp_iter = current_invoice_details; temp_iter != NULL; temp_iter = temp_iter->next) {
                    DetalleFactura *detalle = (DetalleFactura*)temp_iter->data;
                    GtkTreeIter list_row_iter;
                    gtk_list_store_append(factura_detalles_list_store, &list_row_iter);
                    gtk_list_store_set(factura_detalles_list_store, &list_row_iter,
                                    0, detalle->codigo_producto,
                                    1, detalle->nombre_producto,
                                    2, detalle->cantidad,
                                    3, detalle->precio_unitario,
                                    4, detalle->subtotal_detalle,
                                    -1);
                }
                update_factura_totals_labels();
            } else {
                show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error", "No se pudo encontrar el producto en los detalles de la factura actual.");
            }
        } else {
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error", "No se pudo obtener el codigo del producto seleccionado.");
        }
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Seleccione un producto de la tabla de detalles de la factura para eliminarlo.");
    }
}


/**
 * @brief Maneja el clic del boton "Guardar Factura".
 * Recopila todos los datos de la factura actual y la guarda en el sistema.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_guardar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Guardar Factura' presionado.\n");
    Factura nueva_factura;

    const gchar *numero_factura_str = gtk_label_get_text(GTK_LABEL(label_factura_numero_global));
    const gchar *fecha_str = gtk_label_get_text(GTK_LABEL(label_factura_fecha_global));
    const gchar *cedula_cliente_str = get_entry_text(entry_factura_cliente_cedula_global);
    const gchar *nombre_cliente_lbl_str = gtk_label_get_text(GTK_LABEL(label_factura_cliente_nombre_global));
    const gchar *telefono_cliente_str = gtk_label_get_text(GTK_LABEL(label_factura_cliente_telefono_global));
    const gchar *direccion_cliente_str = gtk_label_get_text(GTK_LABEL(label_factura_cliente_direccion_global));
    const gchar *total_lbl_str = gtk_label_get_text(GTK_LABEL(label_factura_total_global)); // Cambio: Usar directamente la etiqueta del total

    // Validaciones basicas
    if (strlen(cedula_cliente_str) == 0 || strlen(nombre_cliente_lbl_str) == 0 || current_invoice_details == NULL) {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Datos Incompletos", "Por favor, busque un cliente y anada al menos un producto a la factura.");
        return;
    }

    // Rellenar la estructura Factura
    strcpy(nueva_factura.numero_factura, numero_factura_str);
    strcpy(nueva_factura.id_tienda, HEADER_TITLE); // "Zapateria Cachamay"
    strcpy(nueva_factura.eslogan_tienda, SLOGAN);   // "Fluye con cada paso"
    strcpy(nueva_factura.fecha, fecha_str);
    strcpy(nueva_factura.cedula_cliente, cedula_cliente_str);

    // Intentar separar el nombre completo en nombre y apellido de la etiqueta.
    // Esto es una simplificacion. En un sistema robusto, los campos nombre y apellido
    // se rellenarian directamente al buscar el cliente.
    char temp_name_buffer[100];
    char temp_lastname_buffer[100];
    strcpy(temp_name_buffer, "");
    strcpy(temp_lastname_buffer, "");

    char *space_pos = strchr(nombre_cliente_lbl_str, ' ');
    if (space_pos) {
        strncpy(temp_name_buffer, nombre_cliente_lbl_str, space_pos - nombre_cliente_lbl_str);
        temp_name_buffer[space_pos - nombre_cliente_lbl_str] = '\0';
        strcpy(temp_lastname_buffer, space_pos + 1);
    } else {
        strcpy(temp_name_buffer, nombre_cliente_lbl_str);
        // temp_lastname_buffer se queda vacio si no hay espacio
    }

    strcpy(nueva_factura.nombre_cliente, temp_name_buffer);
    strcpy(nueva_factura.apellido_cliente, temp_lastname_buffer);

    strcpy(nueva_factura.telefono_cliente, telefono_cliente_str);
    strcpy(nueva_factura.direccion_cliente, direccion_cliente_str);

    nueva_factura.detalles_productos = current_invoice_details; // Asigna la lista temporal
    nueva_factura.porcentaje_impuesto = 0.013; // 1.3%

    // Convertir el total de la etiqueta (ej. "123.45 Bs") a un double
    // Usar directamente el valor de la etiqueta de total
    // Se extrae solo el número de la cadena "X.YY Bs"
    sscanf(total_lbl_str, "%lf", &nueva_factura.monto_total);

    if (add_factura(&nueva_factura)) {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Factura Guardada", "Factura guardada exitosamente.");
        clear_facturas_page_fields(); // Limpiar y preparar para nueva factura
        update_facturas_tree_view(); // Actualizar la lista de facturas registradas

        // Resetear current_invoice_details a NULL ya que la lista ahora es propiedad de la factura guardada
        // Esto es crucial para evitar doble free o que se siga modificando la lista de la factura guardada
        current_invoice_details = NULL;

        // Generar un nuevo numero de factura y fecha para la siguiente
        gchar *new_invoice_num = get_next_invoice_number();
        gtk_label_set_text(GTK_LABEL(label_factura_numero_global), new_invoice_num);
        g_free(new_invoice_num);

        time_t rawtime;
        struct tm *info;
        char date_buffer[11];
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", info);
        gtk_label_set_text(GTK_LABEL(label_factura_fecha_global), date_buffer);

    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error al Guardar", "Fallo al guardar la factura. Puede que el numero de factura ya exista.");
    }
}

/**
 * @brief Maneja el clic del boton "Modificar Factura".
 * Carga los datos de la factura seleccionada en los campos de edicion.
 * NOTA: La modificacion de detalles de una factura ya guardada puede ser compleja.
 * Para simplificar, esta funcion cargara la factura para edicion.
 * La logica de actualizacion en `data_manager.c` ya maneja la liberacion de los detalles antiguos.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_modificar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Modificar Factura' presionado.\n");
    if (!facturas_tree_view_global) return;

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(facturas_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter_selected;

    if (gtk_tree_selection_get_selected(selection, &model, &iter_selected)) {
        gchar *numero_factura_str;
        gtk_tree_model_get(model, &iter_selected, 0, &numero_factura_str, -1);

        Factura *factura_a_modificar = find_factura(numero_factura_str);
        if (factura_a_modificar) {
            // Limpiar la interfaz primero
            clear_facturas_page_fields();

            // Rellenar campos de la factura principal
            gtk_label_set_text(GTK_LABEL(label_factura_numero_global), factura_a_modificar->numero_factura);
            gtk_label_set_text(GTK_LABEL(label_factura_fecha_global), factura_a_modificar->fecha);
            gtk_entry_set_text(GTK_ENTRY(entry_factura_cliente_cedula_global), factura_a_modificar->cedula_cliente);

            char full_name[256];
            snprintf(full_name, sizeof(full_name), "%s %s", factura_a_modificar->nombre_cliente, factura_a_modificar->apellido_cliente);
            gtk_label_set_text(GTK_LABEL(label_factura_cliente_nombre_global), full_name);
            gtk_label_set_text(GTK_LABEL(label_factura_cliente_telefono_global), factura_a_modificar->telefono_cliente);
            gtk_label_set_text(GTK_LABEL(label_factura_cliente_direccion_global), factura_a_modificar->direccion_cliente);

            // Copiar los detalles de productos de la factura existente a la lista temporal
            if (current_invoice_details) { // Limpiar por si acaso
                g_list_free_full(current_invoice_details, g_free);
                current_invoice_details = NULL;
            }
            GList *detail_iter;
            for (detail_iter = factura_a_modificar->detalles_productos; detail_iter != NULL; detail_iter = detail_iter->next) {
                DetalleFactura *original_detalle = (DetalleFactura*)detail_iter->data;
                // Asignar memoria y copiar para que la lista temporal sea independiente de la lista de la factura guardada
                DetalleFactura *copy_detalle = (DetalleFactura*)malloc(sizeof(DetalleFactura));
                if (copy_detalle) {
                    memcpy(copy_detalle, original_detalle, sizeof(DetalleFactura));
                    current_invoice_details = g_list_append(current_invoice_details, copy_detalle);
                }
            }

            // Rellenar la tabla de detalles de la factura actual
            gtk_list_store_clear(factura_detalles_list_store);
            for (detail_iter = current_invoice_details; detail_iter != NULL; detail_iter = detail_iter->next) {
                DetalleFactura *detalle = (DetalleFactura*)detail_iter->data;
                GtkTreeIter list_row_iter;
                gtk_list_store_append(factura_detalles_list_store, &list_row_iter);
                gtk_list_store_set(factura_detalles_list_store, &list_row_iter,
                                   0, detalle->codigo_producto,
                                   1, detalle->nombre_producto,
                                   2, detalle->cantidad,
                                   3, detalle->precio_unitario,
                                   4, detalle->subtotal_detalle,
                                   -1);
            }
            update_factura_totals_labels(); // Recalcular y mostrar totales

            show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Factura Cargada", "Factura cargada para edicion.");
        } else {
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error", "Factura no encontrada para modificar.");
        }
        g_free(numero_factura_str);
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Seleccione una factura de la lista para modificar.");
    }
}

/**
 * @brief Maneja el clic del boton "Eliminar Factura".
 * Elimina la factura seleccionada de la lista de facturas registradas.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_eliminar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Eliminar Factura' presionado.\n");
    if (!facturas_tree_view_global) return;

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(facturas_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *numero_factura_str;
        gtk_tree_model_get(model, &iter, 0, &numero_factura_str, -1);

        if (numero_factura_str) {
            GtkWidget *dialog = gtk_message_dialog_new(main_app_window,
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_QUESTION,
                                                       GTK_BUTTONS_YES_NO,
                                                       "Esta seguro que desea eliminar la factura numero '%s'?", numero_factura_str);
            gtk_window_set_title(GTK_WINDOW(dialog), "Confirmar Eliminacion");
            gint response = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            if (response == GTK_RESPONSE_YES) {
                if (delete_factura(numero_factura_str)) {
                    show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Eliminacion Exitosa", "Factura eliminada exitosamente.");
                    gtk_list_store_remove(GTK_LIST_STORE(model), &iter); // Eliminar de la UI
                    // No limpiar los campos de la factura actual a menos que la factura eliminada fuera la que se estaba editando
                    clear_facturas_page_fields(); // Limpiar por seguridad o si es la activa
                } else {
                    show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Eliminacion", "Fallo al eliminar la factura. No se encontro.");
                }
            }
            g_free(numero_factura_str);
        }
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Seleccione una factura de la lista para eliminarla.");
    }
}

/**
 * @brief Callback que se activa cuando la seleccion de fila en el TreeView de facturas cambia.
 * Rellena los campos de visualizacion de la factura con los datos de la factura seleccionada.
 * Esto es para VER los datos, no necesariamente para cargar para edicion.
 * @param selection El objeto GtkTreeSelection del TreeView.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data) {
    g_print("[CALLBACKS] Seleccion de TreeView de Facturas ha cambiado.\n");
    GtkTreeModel *model;
    GtkTreeIter iter;

    // Al seleccionar una factura, limpiar la seccion de edicion de "factura actual"
    // y solo mostrar los datos de la factura seleccionada en los campos de arriba.
    clear_facturas_page_fields(); // Limpia los campos de entrada y la tabla de detalles

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *numero_factura, *fecha, *cedula_cliente, *nombre_cliente, *apellido_cliente, *monto_total_str;
        gtk_tree_model_get(model, &iter,
                           0, &numero_factura,
                           1, &fecha,
                           2, &cedula_cliente,
                           3, &nombre_cliente,
                           4, &apellido_cliente,
                           5, &monto_total_str,
                           -1);

        // Rellenar los labels principales de la factura con los datos de la seleccion
        if (label_factura_numero_global) gtk_label_set_text(GTK_LABEL(label_factura_numero_global), numero_factura);
        if (label_factura_fecha_global) gtk_label_set_text(GTK_LABEL(label_factura_fecha_global), fecha);
        if (entry_factura_cliente_cedula_global) gtk_entry_set_text(GTK_ENTRY(entry_factura_cliente_cedula_global), cedula_cliente);

        char full_name[256];
        snprintf(full_name, sizeof(full_name), "%s %s", nombre_cliente, apellido_cliente);
        if (label_factura_cliente_nombre_global) gtk_label_set_text(GTK_LABEL(label_factura_cliente_nombre_global), full_name);

        // Necesitamos el resto de los datos del cliente y los detalles de los productos
        // para rellenar los otros campos y la tabla de detalles.
        Factura *selected_factura = find_factura(numero_factura);
        if (selected_factura) {
            if (label_factura_cliente_telefono_global) gtk_label_set_text(GTK_LABEL(label_factura_cliente_telefono_global), selected_factura->telefono_cliente);
            if (label_factura_cliente_direccion_global) gtk_label_set_text(GTK_LABEL(label_factura_cliente_direccion_global), selected_factura->direccion_cliente);

            // Rellenar la tabla de detalles de la factura actual (solo visualizacion)
            gtk_list_store_clear(factura_detalles_list_store);
            GList *detail_iter;
            for (detail_iter = selected_factura->detalles_productos; detail_iter != NULL; detail_iter = detail_iter->next) {
                DetalleFactura *detalle = (DetalleFactura*)detail_iter->data;
                GtkTreeIter list_row_iter;
                gtk_list_store_append(factura_detalles_list_store, &list_row_iter);
                gtk_list_store_set(factura_detalles_list_store, &list_row_iter,
                                   0, detalle->codigo_producto,
                                   1, detalle->nombre_producto,
                                   2, detalle->cantidad,
                                   3, detalle->precio_unitario,
                                   4, detalle->subtotal_detalle,
                                   -1);
            }
            // Recalcular y mostrar los totales de esta factura
            // No podemos usar update_factura_totals_labels() directamente porque se basa en current_invoice_details
            // que esta vacio. Recalculamos directamente con los valores de la factura seleccionada.
            double subtotal_calc = 0.0;
            for (detail_iter = selected_factura->detalles_productos; detail_iter != NULL; detail_iter = detail_iter->next) {
                DetalleFactura *detalle = (DetalleFactura*)detail_iter->data;
                subtotal_calc += detalle->subtotal_detalle;
            }
            double iva_rate = selected_factura->porcentaje_impuesto;
            double iva_amount = subtotal_calc * iva_rate;
            double total = subtotal_calc + iva_amount;

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%.2f Bs", subtotal_calc);
            gtk_label_set_text(GTK_LABEL(label_factura_subtotal_global), buffer);

            snprintf(buffer, sizeof(buffer), "%.2f Bs", iva_amount);
            gtk_label_set_text(GTK_LABEL(label_factura_iva_global), buffer);

            snprintf(buffer, sizeof(buffer), "%.2f Bs", total);
            gtk_label_set_text(GTK_LABEL(label_factura_total_global), buffer);

            g_print("[CALLBACKS] Detalles de la factura '%s' mostrados.\n", selected_factura->numero_factura);

        } else {
            g_printerr("[CALLBACKS] Error: Factura con numero '%s' no encontrada en data_manager al seleccionar TreeView.\n", numero_factura);
        }

        g_free(numero_factura);
        g_free(fecha);
        g_free(cedula_cliente);
        g_free(nombre_cliente);
        g_free(apellido_cliente);
        g_free(monto_total_str);
    } else {
        g_print("[CALLBACKS] No hay seleccion en el TreeView de Facturas. Limpiando campos.\n");
        clear_facturas_page_fields(); // Ya se llama al inicio para resetear siempre
    }
}

/**
 * @brief Maneja el clic del boton "Mostrar Factura".
 * Abre una nueva ventana para mostrar los detalles completos de la factura seleccionada.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_facturas_mostrar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Mostrar Factura' presionado.\n");
    if (!facturas_tree_view_global) return;

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(facturas_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *numero_factura_str;
        gtk_tree_model_get(model, &iter, 0, &numero_factura_str, -1);

        Factura *factura_a_mostrar = find_factura(numero_factura_str);
        if (factura_a_mostrar) {
            GtkWidget *dialog_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_title(GTK_WINDOW(dialog_window), "Detalle de Factura");
            gtk_window_set_default_size(GTK_WINDOW(dialog_window), 600, 700);
            gtk_window_set_transient_for(GTK_WINDOW(dialog_window), main_app_window);
            gtk_window_set_modal(GTK_WINDOW(dialog_window), TRUE);
            gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog_window), TRUE);

            GtkWidget *dialog_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
            gtk_container_set_border_width(GTK_CONTAINER(dialog_vbox), 20);
            gtk_container_add(GTK_CONTAINER(dialog_window), dialog_vbox);

            // Encabezado de la Factura
            GtkWidget *header_label = gtk_label_new(NULL);
            gchar *header_text = g_strdup_printf(
                "<span font_desc='Sans Bold 16'>%s</span>\n"
                "<span font_desc='Sans Italic 10'>%s</span>\n\n"
                "<span font_desc='Sans Bold 12'>FACTURA NUMERO: %s</span>\n"
                "<span font_desc='Sans Bold 12'>FECHA:          %s</span>",
                factura_a_mostrar->id_tienda,
                factura_a_mostrar->eslogan_tienda,
                factura_a_mostrar->numero_factura,
                factura_a_mostrar->fecha
            );
            gtk_label_set_markup(GTK_LABEL(header_label), header_text);
            g_free(header_text);
            gtk_box_pack_start(GTK_BOX(dialog_vbox), header_label, FALSE, FALSE, 0);

            // Datos del Cliente
            GtkWidget *client_frame = gtk_frame_new("Datos del Cliente");
            GtkWidget *client_grid = gtk_grid_new();
            gtk_grid_set_row_spacing(GTK_GRID(client_grid), 5);
            gtk_grid_set_column_spacing(GTK_GRID(client_grid), 10);
            gtk_container_set_border_width(GTK_CONTAINER(client_grid), 5);
            gtk_container_add(GTK_CONTAINER(client_frame), client_grid);
            gtk_box_pack_start(GTK_BOX(dialog_vbox), client_frame, FALSE, FALSE, 10);

            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new("Cedula:"), 0, 0, 1, 1);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new(factura_a_mostrar->cedula_cliente), 1, 0, 1, 1);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new("Nombre:"), 0, 1, 1, 1);
            gchar *client_name = g_strdup_printf("%s %s", factura_a_mostrar->nombre_cliente, factura_a_mostrar->apellido_cliente);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new(client_name), 1, 1, 1, 1);
            g_free(client_name);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new("Telefono:"), 0, 2, 1, 1);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new(factura_a_mostrar->telefono_cliente), 1, 2, 1, 1);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new("Direccion:"), 0, 3, 1, 1);
            gtk_grid_attach(GTK_GRID(client_grid), gtk_label_new(factura_a_mostrar->direccion_cliente), 1, 3, 1, 1);

            // Detalles de Productos
            GtkWidget *details_frame = gtk_frame_new("Detalles de Productos");
            GtkWidget *details_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
            gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(details_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
            gtk_widget_set_vexpand(details_scrolled_window, TRUE);
            gtk_container_add(GTK_CONTAINER(details_frame), details_scrolled_window);
            gtk_box_pack_start(GTK_BOX(dialog_vbox), details_frame, TRUE, TRUE, 10);

            // Usar un GtkTextView para mostrar los detalles como texto preformateado
            GtkWidget *text_view = gtk_text_view_new();
            gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
            gtk_container_add(GTK_CONTAINER(details_scrolled_window), text_view);

            GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
            gchar *details_text = g_strdup_printf("%-15s %-30s %-8s %-12s %-12s\n"
                                                  "--------------------------------------------------------------------------------\n",
                                                  "Codigo", "Descripcion", "Cant.", "P. Unitario", "Subtotal");
            gtk_text_buffer_set_text(buffer, details_text, -1);
            g_free(details_text);

            GList *detail_iter;
            for (detail_iter = factura_a_mostrar->detalles_productos; detail_iter != NULL; detail_iter = detail_iter->next) {
                DetalleFactura *detalle = (DetalleFactura*)detail_iter->data;
                gchar *line = g_strdup_printf("%-15s %-30s %-8d %-12.2f %-12.2f\n",
                                              detalle->codigo_producto,
                                              detalle->nombre_producto,
                                              detalle->cantidad,
                                              detalle->precio_unitario,
                                              detalle->subtotal_detalle);
                gtk_text_buffer_insert_at_cursor(buffer, line, -1);
                g_free(line);
            }
            gtk_text_buffer_insert_at_cursor(buffer, "--------------------------------------------------------------------------------\n", -1);


            // Totales
            GtkWidget *totals_grid_display = gtk_grid_new();
            gtk_grid_set_row_spacing(GTK_GRID(totals_grid_display), 5);
            gtk_grid_set_column_spacing(GTK_GRID(totals_grid_display), 10);
            gtk_box_pack_start(GTK_BOX(dialog_vbox), totals_grid_display, FALSE, FALSE, 0);

            double subtotal_calc = factura_a_mostrar->monto_total / (1 + factura_a_mostrar->porcentaje_impuesto);
            double iva_amount = factura_a_mostrar->monto_total - subtotal_calc;

            gtk_grid_attach(GTK_GRID(totals_grid_display), gtk_label_new("SUBTOTAL:"), 0, 0, 1, 1);
            gchar *subtotal_str = g_strdup_printf("%.2f Bs", subtotal_calc);
            gtk_grid_attach(GTK_GRID(totals_grid_display), gtk_label_new(subtotal_str), 1, 0, 1, 1);
            g_free(subtotal_str);

            gtk_grid_attach(GTK_GRID(totals_grid_display), gtk_label_new(g_strdup_printf("IVA (%.1f%%):", factura_a_mostrar->porcentaje_impuesto * 100)), 0, 1, 1, 1);
            gchar *iva_str = g_strdup_printf("%.2f Bs", iva_amount);
            gtk_grid_attach(GTK_GRID(totals_grid_display), gtk_label_new(iva_str), 1, 1, 1, 1);
            g_free(iva_str);

            gtk_grid_attach(GTK_GRID(totals_grid_display), gtk_label_new("TOTAL A PAGAR:"), 0, 2, 1, 1);
            gchar *total_str = g_strdup_printf("%.2f Bs", factura_a_mostrar->monto_total);
            gtk_grid_attach(GTK_GRID(totals_grid_display), gtk_label_new(total_str), 1, 2, 1, 1);
            g_free(total_str);


            gtk_widget_show_all(dialog_window);

        } else {
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error", "Factura no encontrada para mostrar.");
        }
        g_free(numero_factura_str);
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Seleccione una factura de la lista para mostrarla.");
    }
}

// =========================================================
// Implementacion de callbacks de Trabajadores
// =========================================================

/**
 * @brief Maneja el clic del boton "Crear Trabajador".
 * Obtiene los datos de los campos de entrada y anade un nuevo trabajador.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_trabajadores_crear_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Crear Trabajador' presionado.\n");
    Trabajador nuevo_trabajador;
    const gchar *cedula_str = get_entry_text(entry_trabajadores_cedula_global);
    const gchar *nombre_str = get_entry_text(entry_trabajadores_nombre_global);
    const gchar *apellido_str = get_entry_text(entry_trabajadores_apellido_global);
    const gchar *numero_empleado_str = get_entry_text(entry_trabajadores_numero_empleado_global);
    const gchar *telefono_str = get_entry_text(entry_trabajadores_telefono_global);
    const gchar *direccion_str = get_entry_text(entry_trabajadores_direccion_global);

    if (!cedula_str || strlen(cedula_str) == 0 ||
        !nombre_str || strlen(nombre_str) == 0 ||
        !apellido_str || strlen(apellido_str) == 0 ||
        !numero_empleado_str || strlen(numero_empleado_str) == 0)
    {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Creacion", "Por favor, complete los campos obligatorios: Cedula, Nombre, Apellido, Numero de Empleado.");
        return;
    }

    strcpy(nuevo_trabajador.cedula, cedula_str);
    strcpy(nuevo_trabajador.nombre, nombre_str);
    strcpy(nuevo_trabajador.apellido, apellido_str);
    strcpy(nuevo_trabajador.numero_empleado, numero_empleado_str);
    strcpy(nuevo_trabajador.telefono, telefono_str ? telefono_str : "");
    strcpy(nuevo_trabajador.direccion, direccion_str ? direccion_str : "");

    if (add_trabajador(&nuevo_trabajador)) {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Creacion Exitosa", "Trabajador creado exitosamente.");
        clear_trabajadores_entry_fields();
        update_trabajadores_tree_view();
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Creacion", "Fallo al crear el trabajador. Posiblemente un trabajador con esa cedula o numero de empleado ya existe.");
    }
}

/**
 * @brief Maneja el clic del boton "Buscar Trabajador".
 * Obtiene la cedula o numero de empleado del campo de entrada, busca el trabajador y rellena todos los campos si lo encuentra.
 * Tambien selecciona la fila correspondiente en el TreeView.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_trabajadores_buscar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Buscar Trabajador' presionado.\n");
    const gchar *cedula_o_numero_empleado_str = get_entry_text(entry_trabajadores_cedula_global); // Usar cedula para la busqueda inicial

    if (!cedula_o_numero_empleado_str || strlen(cedula_o_numero_empleado_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Campo Vacio", "Por favor, ingrese una cedula o numero de empleado para buscar.");
        return;
    }

    Trabajador *t = find_trabajador(cedula_o_numero_empleado_str);
    if (t) {
        g_print("[CALLBACKS] Trabajador encontrado: %s %s (Cedula: %s, Num. Empleado: %s).\n", t->nombre, t->apellido, t->cedula, t->numero_empleado);
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_cedula_global), t->cedula);
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_nombre_global), t->nombre);
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_apellido_global), t->apellido);
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_numero_empleado_global), t->numero_empleado);
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_telefono_global), t->telefono);
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_direccion_global), t->direccion);

        // Opcional: Seleccionar la fila correspondiente en el TreeView
        if (trabajadores_tree_view_global && trabajadores_list_store) {
            GtkTreePath *path = NULL;
            GtkTreeIter iter;
            gboolean found = FALSE;

            if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(trabajadores_list_store), &iter)) {
                do {
                    gchar *current_cedula;
                    gtk_tree_model_get(GTK_TREE_MODEL(trabajadores_list_store), &iter, 0, &current_cedula, -1);
                    if (current_cedula && strcmp(current_cedula, cedula_o_numero_empleado_str) == 0) {
                        path = gtk_tree_model_get_path(GTK_TREE_MODEL(trabajadores_list_store), &iter);
                        found = TRUE;
                        g_free(current_cedula);
                        break;
                    }
                    g_free(current_cedula);
                } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(trabajadores_list_store), &iter));
            }

            if (found && path) {
                GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(trabajadores_tree_view_global));
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(trabajadores_tree_view_global), path, NULL, FALSE, 0.0, 0.0);
                gtk_tree_path_free(path);
            }
        }

    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Trabajador No Encontrado", "Trabajador con la cedula o numero de empleado especificado no encontrado.");
        clear_trabajadores_entry_fields();
        gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_cedula_global), cedula_o_numero_empleado_str);
        if (trabajadores_tree_view_global) {
            GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(trabajadores_tree_view_global));
            gtk_tree_selection_unselect_all(selection);
        }
    }
}

/**
 * @brief Maneja el clic del boton "Modificar Trabajador".
 * Obtiene los datos actualizados de los campos de entrada y modifica el trabajador existente.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_trabajadores_modificar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Modificar Trabajador' presionado.\n");
    Trabajador trabajador_modificado;
    const gchar *cedula_str = get_entry_text(entry_trabajadores_cedula_global);
    const gchar *nombre_str = get_entry_text(entry_trabajadores_nombre_global);
    const gchar *apellido_str = get_entry_text(entry_trabajadores_apellido_global);
    const gchar *numero_empleado_str = get_entry_text(entry_trabajadores_numero_empleado_global);
    const gchar *telefono_str = get_entry_text(entry_trabajadores_telefono_global);
    const gchar *direccion_str = get_entry_text(entry_trabajadores_direccion_global);

    if (!cedula_str || strlen(cedula_str) == 0) {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Campo Requerido", "Por favor, ingrese la cedula del trabajador a modificar.");
        return;
    }

    strcpy(trabajador_modificado.cedula, cedula_str);
    strcpy(trabajador_modificado.nombre, nombre_str ? nombre_str : "");
    strcpy(trabajador_modificado.apellido, apellido_str ? apellido_str : "");
    strcpy(trabajador_modificado.numero_empleado, numero_empleado_str ? numero_empleado_str : "");
    strcpy(trabajador_modificado.telefono, telefono_str ? telefono_str : "");
    strcpy(trabajador_modificado.direccion, direccion_str ? direccion_str : "");

    if (update_trabajador(&trabajador_modificado)) {
        show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Modificacion Exitosa", "Trabajador modificado exitosamente.");
        clear_trabajadores_entry_fields();
        update_trabajadores_tree_view();
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Modificacion", "Fallo al modificar el trabajador. Cedula o Numero de Empleado no encontrado o duplicado.");
    }
}

/**
 * @brief Maneja el clic del boton "Eliminar Trabajador".
 * Obtiene la cedula o numero de empleado del trabajador seleccionado en el GtkTreeView y lo elimina.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_trabajadores_eliminar_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Eliminar Trabajador' presionado.\n");

    if (!trabajadores_tree_view_global) {
        g_printerr("[CALLBACKS] Error: GtkTreeView de trabajadores no esta inicializado.\n");
        return;
    }

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(trabajadores_tree_view_global));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *cedula_trabajador;
        gtk_tree_model_get(model, &iter, 0, &cedula_trabajador, -1); // Obtener la cedula de la columna 0

        if (cedula_trabajador) {
            GtkWidget *dialog = gtk_message_dialog_new(main_app_window,
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_QUESTION,
                                                       GTK_BUTTONS_YES_NO,
                                                       "Esta seguro que desea eliminar el trabajador con cedula '%s'?", cedula_trabajador);
            gtk_window_set_title(GTK_WINDOW(dialog), "Confirmar Eliminacion");
            gint response = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            if (response == GTK_RESPONSE_YES) {
                if (delete_trabajador(cedula_trabajador)) { // Eliminar por cedula
                    show_message_dialog(main_app_window, GTK_MESSAGE_INFO, "Eliminacion Exitosa", "Trabajador eliminado exitosamente.");
                    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
                    clear_trabajadores_entry_fields();
                } else {
                    show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Eliminacion", "Fallo al eliminar el trabajador. No se encontro en la lista de datos.");
                }
            }
            g_free(cedula_trabajador);
        } else {
            show_message_dialog(main_app_window, GTK_MESSAGE_ERROR, "Error de Seleccion", "No se pudo obtener la cedula del trabajador seleccionado.");
        }
    } else {
        show_message_dialog(main_app_window, GTK_MESSAGE_WARNING, "Sin Seleccion", "Por favor, seleccione un trabajador de la lista para eliminar.");
    }
}

/**
 * @brief Callback que se activa cuando la seleccion de fila en el TreeView de trabajadores cambia.
 * Rellena los campos de entrada con los datos del trabajador seleccionado.
 * @param selection El objeto GtkTreeSelection del TreeView.
 * @param user_data Datos de usuario (no usados).
 */
void on_trabajadores_tree_view_selection_changed(GtkTreeSelection *selection, gpointer user_data) {
    g_print("[CALLBACKS] Seleccion de TreeView de Trabajadores ha cambiado.\n");
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *cedula, *nombre, *apellido, *numero_empleado, *telefono, *direccion;

        gtk_tree_model_get(model, &iter,
                           0, &cedula,
                           1, &nombre,
                           2, &apellido,
                           3, &numero_empleado,
                           4, &telefono,
                           5, &direccion,
                           -1);

        if (entry_trabajadores_cedula_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_cedula_global), cedula);
        if (entry_trabajadores_nombre_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_nombre_global), nombre);
        if (entry_trabajadores_apellido_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_apellido_global), apellido);
        if (entry_trabajadores_numero_empleado_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_numero_empleado_global), numero_empleado);
        if (entry_trabajadores_telefono_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_telefono_global), telefono);
        if (entry_trabajadores_direccion_global) gtk_entry_set_text(GTK_ENTRY(entry_trabajadores_direccion_global), direccion);

        g_free(cedula);
        g_free(nombre);
        g_free(apellido);
        g_free(numero_empleado);
        g_free(telefono);
        g_free(direccion);
    } else {
        g_print("[CALLBACKS] No hay seleccion en el TreeView de Trabajadores. Limpiando campos de entrada.\n");
        clear_trabajadores_entry_fields();
    }
}

// =========================================================
// Implementacion de callbacks de Reportes
// =========================================================

/**
 * @brief Callback para el boton "Reporte de Inventario".
 * Muestra el reporte de inventario en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_inventario_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte de Inventario' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_inventory(buffer); // Llama a la funcion del data_manager para generar el reporte
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Costo vs Venta".
 * Muestra el reporte de relacion costo-venta en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_rentabilidad_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Costo vs Venta' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_cost_sale_ratio(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Productos Danados".
 * Muestra el reporte de productos dañados en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_productos_danados_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Productos Dañados' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_damaged_products(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte de Proveedores".
 * Muestra el reporte de proveedores en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_proveedores_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte de Proveedores' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_suppliers(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Stock Bajo".
 * Muestra el reporte de productos con stock bajo en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_stock_bajo_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Stock Bajo' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_low_stock_products(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Cantidad Trabajadores".
 * Muestra el reporte de cantidad de trabajadores en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_conteo_trabajadores_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Cantidad Trabajadores' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_worker_count(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Compras Clientes".
 * Muestra el reporte de clientes con compras en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_compras_clientes_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Compras Clientes' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_customer_purchases(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Mas/Menos Vendido".
 * Muestra el reporte de tipo de zapato mas y menos vendido en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_mas_menos_vendidos_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Mas/Menos Vendido' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_most_least_sold_shoe_type(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Ventas Diarias".
 * Muestra el reporte de ventas diarias por tipo de zapato en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_ventas_diarias_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Ventas Diarias' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_daily_sales_by_shoe_type(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}

/**
 * @brief Callback para el boton "Reporte Ultimas 7 Facturas".
 * Muestra los detalles de las ultimas 7 facturas en el GtkTextView de reportes.
 * @param button El boton que emitio la senal.
 * @param user_data Datos de usuario (no usados).
 */
void on_reportes_ultimas_facturas_clicked(GtkButton *button, gpointer user_data) {
    g_print("[CALLBACKS] Boton 'Reporte Ultimas 7 Facturas' presionado.\n");
    if (text_view_reportes_output_global) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_reportes_output_global));
        dm_show_last_seven_invoices(buffer);
    } else {
        g_printerr("[CALLBACKS] Error: text_view_reportes_output_global es NULL.\n");
    }
}
