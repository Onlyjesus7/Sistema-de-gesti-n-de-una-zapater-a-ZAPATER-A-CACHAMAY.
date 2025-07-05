// =========================================================
// Archivo: ui_elements.c
// Descripcion: Implementacion de funciones para crear elementos de la interfaz.
//              Contiene la logica para construir la interfaz de usuario programmaticamente.
// =========================================================
#include "ui_elements.h"
#include <stdio.h>   // Para g_print (para imprimir en consola)
#include <string.h>  // Para strlen (para trabajar con cadenas de texto)
#include <stdlib.h> // Para atoi, atof
#include <time.h>    // Para time, localtime, strftime (para fechas)

// =========================================================
// DEFINICIONES DE VARIABLES GLOBALES DE WIDGETS (PROPIAS DE UI_ELEMENTS.C)
// Estas variables se INICIALIZAN AQUI (a NULL) y se les asignara el widget real
// cuando se creen en las funciones de creacion de UI. Son 'extern' en los .h.
// =========================================================
GtkListStore *product_list_store = NULL; // Modelo de datos para la tabla de productos (Inventario)

// Variables globales para los GtkEntry de la pagina de Inventario
GtkWidget *entry_inventario_codigo_global = NULL;
GtkWidget *entry_inventario_nombre_global = NULL;
GtkWidget *entry_inventario_categoria_global = NULL;
GtkWidget *entry_inventario_cantidad_global = NULL;
GtkWidget *entry_inventario_precio_bs_global = NULL;
GtkWidget *entry_inventario_estado_global = NULL;

// Punteros globales para los nuevos campos de entrada de Inventario
GtkWidget *entry_inventario_fecha_adquisicion_global = NULL;
GtkWidget *entry_inventario_movimiento_global = NULL;
GtkWidget *entry_inventario_ubicacion_global = NULL;
GtkWidget *entry_inventario_stock_minimo_global = NULL;
GtkWidget *entry_inventario_costo_global = NULL;

// Puntero global al GtkTreeView de Inventario
GtkWidget *inventory_tree_view_global = NULL;


// Definiciones para la pagina de Productos
GtkListStore *productos_list_store = NULL; // Modelo de datos para la tabla de Productos
GtkWidget *productos_tree_view_global = NULL; // Tabla para la pagina de Productos
GtkWidget *productos_category_combo_global = NULL; // Combobox para filtrar por categoria

// Labels de solo lectura para mostrar detalles del producto seleccionado en pagina de Productos
GtkWidget *label_productos_codigo_global = NULL;
GtkWidget *label_productos_nombre_global = NULL;
GtkWidget *label_productos_categoria_global = NULL;
GtkWidget *label_productos_cantidad_global = NULL;
GtkWidget *label_productos_precio_bs_global = NULL;
GtkWidget *label_productos_estado_global = NULL;
GtkWidget *label_productos_fecha_adq_global = NULL;
GtkWidget *label_productos_movimiento_global = NULL;
GtkWidget *label_productos_ubicacion_global = NULL;
GtkWidget *label_productos_stock_min_global = NULL;
GtkWidget *label_productos_costo_adq_global = NULL;

// Definiciones para la pagina de Clientes
GtkListStore *clientes_list_store = NULL; // Modelo para la tabla de clientes
GtkWidget *clientes_tree_view_global = NULL; // TreeView para la pagina de clientes

GtkWidget *entry_clientes_cedula_global = NULL;
GtkWidget *entry_clientes_nombre_global = NULL;
GtkWidget *entry_clientes_apellido_global = NULL;
GtkWidget *entry_clientes_telefono_global = NULL;
GtkWidget *entry_clientes_direccion_global = NULL;

// Definiciones para la pagina de Facturas
GtkListStore *facturas_list_store = NULL; // Modelo para la tabla de facturas
GtkWidget *facturas_tree_view_global = NULL; // TreeView para la pagina de facturas

// Campos de entrada y visualizacion de la factura actual
GtkWidget *label_factura_numero_global = NULL;
GtkWidget *label_factura_fecha_global = NULL;
GtkWidget *entry_factura_cliente_cedula_global = NULL;
GtkWidget *label_factura_cliente_nombre_global = NULL;
GtkWidget *label_factura_cliente_telefono_global = NULL;
GtkWidget *label_factura_cliente_direccion_global = NULL;

GtkWidget *entry_factura_producto_codigo_global = NULL;
GtkWidget *entry_factura_producto_cantidad_global = NULL;
GtkListStore *factura_detalles_list_store = NULL; // Modelo para la tabla de detalles de la factura actual
GtkWidget *factura_detalles_tree_view_global = NULL; // TreeView para los detalles de la factura actual

GtkWidget *label_factura_subtotal_global = NULL;
GtkWidget *label_factura_iva_global = NULL;
GtkWidget *label_factura_total_global = NULL;

// DEFINICIONES PARA LA PAGINA DE TRABAJADORES
GtkListStore *trabajadores_list_store = NULL; // Modelo para la tabla de trabajadores
GtkWidget *trabajadores_tree_view_global = NULL; // TreeView para la pagina de trabajadores

GtkWidget *entry_trabajadores_cedula_global = NULL;
GtkWidget *entry_trabajadores_nombre_global = NULL;
GtkWidget *entry_trabajadores_apellido_global = NULL;
GtkWidget *entry_trabajadores_numero_empleado_global = NULL;
GtkWidget *entry_trabajadores_telefono_global = NULL;
GtkWidget *entry_trabajadores_direccion_global = NULL;

// DEFINICIONES PARA LA PAGINA DE REPORTES
GtkWidget *text_view_reportes_output_global = NULL; // TextView donde se mostraran los reportes


// =========================================================
// DECLARACIONES EXTERNAS DE VARIABLES GLOBALES (DEFINIDAS EN MAIN.C)
// Estas variables se DEFINEN en main.c, aqui solo las declaramos para poder usarlas.
// =========================================================
extern GtkWindow *main_app_window;
extern GtkStack *main_content_stack;


// =========================================================
// FUNCIONES AUXILIARES PARA LA CREACION DE UI
// =========================================================

/**
 * @brief Crea un boton de navegacion estandar para la barra lateral.
 * @param label_text El texto a mostrar en el boton.
 * @param handler_func La funcion de callback a ejecutar cuando se haga clic.
 * @return Un nuevo GtkButton.
 */
static GtkWidget* create_nav_button(const gchar *label_text, GCallback handler_func) {
    GtkWidget *button = gtk_button_new_with_label(label_text);
    // Establecer un tamano minimo para que los botones de navegacion se vean bien
    gtk_widget_set_size_request(button, 150, 40);
    // Conectar la senal "clicked" del boton a la funcion de callback proporcionada
    g_signal_connect(button, "clicked", handler_func, NULL);
    return button;
}

/**
 * @brief Crea una pagina base para el GtkStack con un color de fondo especifico.
 * Use esta funcion para construir las paginas individuales de su aplicacion.
 * @param label_text El texto inicial para una etiqueta de bienvenida en la pagina.
 * @param r Componente rojo del color de fondo (0.0 a 1.0).
 * @param g Componente verde del color de fondo (0.0 a 1.0).
 * @param b Componente azul del color de fondo (0.0 a 1.0).
 * @return Un GtkWidget (GtkBox) que representa la pagina con el color de fondo.
 */
static GtkWidget* create_colored_page(const gchar *label_text, gdouble r, gdouble g, gdouble b) {
    // Crear una caja vertical para organizar el contenido de la pagina
    GtkWidget *page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    // Anadir un margen alrededor del contenido de la pagina
    gtk_container_set_border_width(GTK_CONTAINER(page_box), 20);
    // Asegurarse de que la pagina se expanda para llenar todo el espacio disponible
    gtk_widget_set_hexpand(page_box, TRUE);
    gtk_widget_set_vexpand(page_box, TRUE);

    // Definir el color de fondo usando la estructura GdkRGBA
    GdkRGBA color = { r, g, b, 1.0 }; // El ultimo valor (1.0) es la opacidad (completamente opaco)
    // Aplicar el color de fondo a la GtkBox en su estado normal
    gtk_widget_override_background_color(page_box, GTK_STATE_NORMAL, &color);

    // Anadir una etiqueta de bienvenida a la pagina
    GtkWidget *label = gtk_label_new(label_text);
    gtk_box_pack_start(GTK_BOX(page_box), label, FALSE, FALSE, 0); // Empaquetar la etiqueta en la caja

    g_print("[UI] Pagina '%s' creada con color de fondo (R:%.1f G:%.1f B:%.1f).\n", label_text, r, b, b);
    return page_box;
}

/**
 * @brief Configura y anade una columna de texto a un GtkTreeView.
 * @param tree_view El GtkTreeView al que se anadira la columna.
 * @param column_idx El indice de la columna en el modelo de datos (GtkListStore).
 * @param title El titulo visible de la columna.
 */
static void add_tree_view_column(GtkTreeView *tree_view, gint column_idx, const gchar *title) {
    // Un GtkCellRendererText es responsable de dibujar texto en la columna
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    // Crear la columna con el titulo y asociarla con el renderizador y el indice de columna en el modelo
    GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(title, renderer, "text", column_idx, NULL);
    // Anadir la columna al GtkTreeView
    gtk_tree_view_append_column(tree_view, col);
}

/**
 * @brief Actualiza los datos mostrados en el GtkTreeView de productos (pagina Inventario).
 * Esta funcion consulta el `data_manager` y llena el `product_list_store`.
 */
void update_product_tree_view(void) {
    g_print("[UI] Actualizando TreeView de productos (pagina Inventario)...\n");
    if (!product_list_store) {
        g_printerr("[UI] Error: product_list_store no inicializado. No se puede actualizar el TreeView.\n");
        return;
    }

    gtk_list_store_clear(product_list_store); // Limpiar datos existentes en el TreeView

    GList *productos = NULL;
    get_all_productos(&productos); // Obtener todos los productos del gestor de datos (data_manager.c)

    // Iterar sobre la lista de productos y anadirlos al GtkListStore
    GList *iter;
    for (iter = productos; iter != NULL; iter = iter->next) {
        Producto *p = (Producto*)iter->data; // Obtener el puntero al Producto
        GtkTreeIter list_iter; // Iterador para anadir una nueva fila al ListStore
        gtk_list_store_append(product_list_store, &list_iter); // Anadir una nueva fila
        // Establecer los valores de cada columna en la nueva fila
        gtk_list_store_set(product_list_store, &list_iter,
                           0, p->codigo,           // Col 0: Codigo
                           1, p->nombre,           // Col 1: Nombre
                           2, p->categoria,        // Col 2: Categoria
                           3, p->cantidad_disponible, // Col 3: Cantidad
                           4, p->precio_venta_bs,  // Col 4: Precio Bs
                           5, p->estado_fisico,    // Col 5: Estado Fisico
                           6, p->fecha_adquisicion,    // Col 6
                           7, p->movimiento_inventario, // Col 7
                           8, p->ubicacion,             // Col 8
                           9, p->nivel_stock_minimo,   // Col 9
                           10, p->costo,                // Col 10: Costo Adquisicion
                           -1); // Indicador de fin de argumentos para gtk_list_store_set
    }
    g_list_free(productos); // Liberar la copia de la GList (NO los datos de los productos, que son gestionados por data_manager)
    g_print("[UI] TreeView de productos (Inventario) actualizado. Total: %d productos en memoria.\n", g_list_length(lista_productos));
}


/**
 * @brief Actualiza los datos mostrados en el GtkTreeView de productos (pagina Productos).
 * Esta funcion consulta el `data_manager` y llena el `productos_list_store`,
 * aplicando un filtro basado en categoria.
 * @param filter_category La categoria a filtrar (ej., "Deportivos", "Casuales", "Vestir", "Todos").
 */
void update_productos_page_tree_view(const gchar *filter_category) {
    g_print("[UI] Actualizando TreeView de productos (pagina Productos) con filtro: %s...\n", filter_category);
    if (!productos_list_store) {
        g_printerr("[UI] Error: productos_list_store no inicializado. No se puede actualizar el TreeView.\n");
        return;
    }

    gtk_list_store_clear(productos_list_store); // Limpiar datos existentes

    GList *productos = NULL;
    get_all_productos(&productos); // Obtener todos los productos del gestor de datos

    GList *iter;
    for (iter = productos; iter != NULL; iter = iter->next) {
        Producto *p = (Producto*)iter->data;
        // Aplicar filtro
        if (strcmp(filter_category, "Todos") == 0 || strcmp(p->categoria, filter_category) == 0) {
            GtkTreeIter list_iter;
            gtk_list_store_append(productos_list_store, &list_iter);
            // Mostrar solo algunas columnas en la tabla de Productos para mayor legibilidad
            gtk_list_store_set(productos_list_store, &list_iter,
                               0, p->codigo,
                               1, p->nombre,
                               2, p->categoria,
                               3, p->cantidad_disponible,
                               4, p->precio_venta_bs,
                               5, p->estado_fisico,
                               -1);
        }
    }
    g_list_free(productos);
    g_print("[UI] TreeView de productos (Productos) actualizado.\n");
}


/**
 * @brief Actualiza los datos mostrados en el GtkTreeView de clientes (pagina Clientes).
 * Esta funcion consulta el `data_manager` y llena el `clientes_list_store`.
 */
void update_clientes_tree_view(void) {
    g_print("[UI] Actualizando TreeView de clientes (pagina Clientes)...\n");
    if (!clientes_list_store) {
        g_printerr("[UI] Error: clientes_list_store no inicializado. No se puede actualizar el TreeView.\n");
        return;
    }

    gtk_list_store_clear(clientes_list_store); // Limpiar datos existentes

    GList *clientes = NULL;
    get_all_clientes(&clientes); // Obtener todos los clientes del gestor de datos

    GList *iter;
    for (iter = clientes; iter != NULL; iter = iter->next) {
        Cliente *c = (Cliente*)iter->data;
        GtkTreeIter list_iter;
        gtk_list_store_append(clientes_list_store, &list_iter);
        gtk_list_store_set(clientes_list_store, &list_iter,
                           0, c->cedula,
                           1, c->nombre,
                           2, c->apellido,
                           3, c->telefono,
                           4, c->direccion,
                           -1);
    }
    g_list_free(clientes);
    g_print("[UI] TreeView de clientes (Clientes) actualizado. Total: %d clientes en memoria.\n", g_list_length(lista_clientes));
}


/**
 * @brief Actualiza los datos mostrados en el GtkTreeView de facturas (pagina Facturas).
 * Esta funcion consulta el `data_manager` y llena el `facturas_list_store`.
 */
void update_facturas_tree_view(void) {
    g_print("[UI] Actualizando TreeView de facturas (pagina Facturas)...\n");
    if (!facturas_list_store) {
        g_printerr("[UI] Error: facturas_list_store no inicializado. No se puede actualizar el TreeView.\n");
        return;
    }

    gtk_list_store_clear(facturas_list_store); // Limpiar datos existentes

    GList *facturas = NULL;
    get_all_facturas(&facturas); // Obtener todas las facturas del gestor de datos

    GList *iter;
    for (iter = facturas; iter != NULL; iter = iter->next) {
        Factura *f = (Factura*)iter->data;
        GtkTreeIter list_iter;
        gtk_list_store_append(facturas_list_store, &list_iter);

        // Formatear el monto total a string para la visualizacion
        char monto_total_str[64];
        // Corregido: Usar f->monto_total directamente, ya que Factura ahora tiene ese miembro
        snprintf(monto_total_str, sizeof(monto_total_str), "%.2f Bs", f->monto_total);

        gtk_list_store_set(facturas_list_store, &list_iter,
                           0, f->numero_factura,
                           1, f->fecha,
                           2, f->cedula_cliente,
                           3, f->nombre_cliente, // Nombre del cliente en la tabla
                           4, f->apellido_cliente, // Apellido del cliente en la tabla
                           5, monto_total_str,
                           -1);
    }
    g_list_free(facturas);
    g_print("[UI] TreeView de facturas (Facturas) actualizado. Total: %d facturas en memoria.\n", g_list_length(lista_facturas));
}

/**
 * @brief Actualiza los datos mostrados en el GtkTreeView de trabajadores (pagina Trabajadores).
 * Esta funcion consulta el `data_manager` y llena el `trabajadores_list_store`.
 */
void update_trabajadores_tree_view(void) {
    g_print("[UI] Actualizando TreeView de trabajadores (pagina Trabajadores)...\n");
    if (!trabajadores_list_store) {
        g_printerr("[UI] Error: trabajadores_list_store no inicializado. No se puede actualizar el TreeView.\n");
        return;
    }

    gtk_list_store_clear(trabajadores_list_store); // Limpiar datos existentes

    GList *trabajadores = NULL;
    get_all_trabajadores(&trabajadores); // Obtener todos los trabajadores del gestor de datos

    GList *iter;
    for (iter = trabajadores; iter != NULL; iter = iter->next) {
        Trabajador *t = (Trabajador*)iter->data;
        GtkTreeIter list_iter;
        gtk_list_store_append(trabajadores_list_store, &list_iter);
        gtk_list_store_set(trabajadores_list_store, &list_iter,
                           0, t->cedula,
                           1, t->nombre,
                           2, t->apellido,
                           3, t->numero_empleado,
                           4, t->telefono,
                           5, t->direccion,
                           -1);
    }
    g_list_free(trabajadores);
    g_print("[UI] TreeView de trabajadores (Trabajadores) actualizado. Total: %d trabajadores en memoria.\n", g_list_length(lista_trabajadores));
}

/**
 * @brief Actualiza los datos mostrados en el GtkTreeView de detalles de factura.
 * Esta funcion llena el `factura_detalles_list_store` con los detalles de la factura actual.
 * @param detalles_productos La GList de DetalleFactura a mostrar.
 */
void update_factura_detalle_tree_view(GList *detalles_productos) {
    g_print("[UI] Actualizando TreeView de detalles de factura...\n");
    if (!factura_detalles_list_store) {
        g_printerr("[UI] Error: factura_detalles_list_store no inicializado. No se puede actualizar el TreeView.\n");
        return;
    }

    gtk_list_store_clear(factura_detalles_list_store); // Limpiar datos existentes

    GList *iter;
    for (iter = detalles_productos; iter != NULL; iter = iter->next) {
        DetalleFactura *detalle = (DetalleFactura*)iter->data; // Corregido: Usar DetalleFactura
        GtkTreeIter list_iter;
        gtk_list_store_append(factura_detalles_list_store, &list_iter);
        gtk_list_store_set(factura_detalles_list_store, &list_iter,
                           0, detalle->codigo_producto,
                           1, detalle->nombre_producto,
                           2, detalle->cantidad,
                           3, detalle->precio_unitario, // Corregido: usar precio_unitario
                           4, detalle->subtotal_detalle, // Corregido: usar subtotal_detalle
                           -1);
    }
    g_print("[UI] TreeView de detalles de factura actualizado.\n");
}


// =========================================================
// FUNCIONES PARA CREAR WIDGETS PRINCIPALES Y PAGINAS DE GTKSTACK
// =========================================================

/**
 * @brief Crea la ventana principal de la aplicacion con su diseno base.
 * Esta funcion es responsable de construir toda la jerarquia de la UI y asignar
 * los widgets principales a sus variables globales correspondientes.
 * @return La GtkWindow principal.
 */
GtkWidget* create_main_window(void) {
    GtkWidget *window;
    GtkWidget *main_vbox;       // Caja vertical principal
    GtkWidget *header_bar;      // Barra de encabezado
    GtkWidget *main_grid;       // Cuadricula principal (para barra lateral y stack)
    GtkWidget *sidebar_vbox;    // Caja vertical para botones de navegacion

    // 1. Crear la ventana principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    main_app_window = GTK_WINDOW(window); // Asignar a la variable global main_app_window (definida en main.c)
    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    // Establecer un tamano predeterminado mas pequeno y equilibrado (ancho 700, alto 500)
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // Centrar ventana
    g_signal_connect(window, "destroy", G_CALLBACK(on_main_window_destroy), NULL); // Conectar senal de cierre

    // ** CONFIGURACION DE BOTONES ESTANDAR DE VENTANA (Cerrar, Minimizar, Maximizar) **
    // Asegurarse de que sea TRUE para que los botones del sistema operativo se muestren.
    gtk_window_set_decorated(GTK_WINDOW(window), TRUE);
    // Asegurarse de que sea TRUE para permitir al usuario redimensionar la ventana.
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    g_print("[UI] Ventana principal creada con decoraciones estandar y es redimensionable.\n");

    // 2. Crear la caja vertical principal (para HeaderBar + Grid)
    main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    g_print("[UI] Caja principal (VBox) anadida a la ventana.\n");

    // 3. Crear y configurar la barra de encabezado
    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), HEADER_TITLE);
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(header_bar), SLOGAN);
    // Aumentar el margen superior/inferior para hacer la cabecera mas alta
    gtk_container_set_border_width(GTK_CONTAINER(header_bar), 10); // Aumenta el padding a 10px en todos los lados
    // Si la ventana ya tiene decoraciones (TRUE en gtk_window_set_decorated),
    // no es necesario mostrar el boton de cierre de la HeaderBar, el del SO ya lo hace.
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), FALSE);
    gtk_box_pack_start(GTK_BOX(main_vbox), header_bar, FALSE, FALSE, 0); // Empaquetar en la caja principal
    g_print("[UI] HeaderBar creada y anadida.\n");

    // 4. Crear la cuadricula principal (para barra lateral y contenido)
    main_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(main_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(main_grid), 5);
    gtk_container_set_border_width(GTK_CONTAINER(main_grid), 5);
    gtk_box_pack_start(GTK_BOX(main_vbox), main_grid, TRUE, TRUE, 0); // Empaquetar en la caja principal
    g_print("[UI] Cuadricula principal creada y anadida.\n");

    // 5. Crear la caja vertical para la barra lateral
    sidebar_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Espaciado entre botones
    gtk_container_set_border_width(GTK_CONTAINER(sidebar_vbox), 5);
    gtk_grid_attach(GTK_GRID(main_grid), sidebar_vbox, 0, 0, 1, 1); // Adjuntar a la columna 0, fila 0 de la cuadricula
    g_print("[UI] Barra lateral (VBox) creada y anadida a la Cuadricula.\n");

    // 6. Anadir botones de navegacion a la barra lateral
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), create_nav_button("Inventario", G_CALLBACK(on_btn_inventario_clicked)), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), create_nav_button("Productos", G_CALLBACK(on_btn_productos_clicked)), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), create_nav_button("Clientes", G_CALLBACK(on_btn_clientes_clicked)), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), create_nav_button("Facturas", G_CALLBACK(on_btn_facturas_clicked)), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), create_nav_button("Trabajadores", G_CALLBACK(on_btn_trabajadores_clicked)), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_vbox), create_nav_button("Reportes", G_CALLBACK(on_btn_reportes_clicked)), FALSE, FALSE, 0);
    g_print("[UI] Botones de navegacion anadidos a la Barra lateral.\n");

    // 7. Crear el GtkStack para contenido dinamico
    main_content_stack = GTK_STACK(gtk_stack_new()); // Asignar a la variable global main_content_stack (definida en main.c)
    gtk_grid_attach(GTK_GRID(main_grid), GTK_WIDGET(main_content_stack), 1, 0, 1, 1); // Adjuntar a la columna 1, fila 0 de la cuadricula
    gtk_widget_set_hexpand(GTK_WIDGET(main_content_stack), TRUE); // Expandir horizontalmente
    gtk_widget_set_vexpand(GTK_WIDGET(main_content_stack), TRUE); // Expandir verticalmente
    g_print("[UI] GtkStack principal creado y anadido a la Cuadricula.\n");

    // 8. Anadir las paginas al GtkStack
    gtk_stack_add_named(main_content_stack, create_inventario_page(), PAGE_INVENTARIO);
    gtk_stack_add_named(main_content_stack, create_productos_page(), PAGE_PRODUCTOS);
    gtk_stack_add_named(main_content_stack, create_clientes_page(), PAGE_CLIENTES);
    gtk_stack_add_named(main_content_stack, create_facturas_page(), PAGE_FACTURAS);
    gtk_stack_add_named(main_content_stack, create_trabajadores_page(), PAGE_TRABAJADORES);
    gtk_stack_add_named(main_content_stack, create_reportes_page(), PAGE_REPORTES);
    g_print("[UI] Paginas de contenido anadidas al GtkStack.\n");

    // Mostrar la primera pagina por defecto
    gtk_stack_set_visible_child_name(main_content_stack, PAGE_INVENTARIO);

    return window;
}

/**
 * @brief Crea la pagina de Inventario para el GtkStack.
 * Esta pagina contendra los campos para el CRUD de productos y un GtkTreeView.
 * @return Un GtkWidget que representa la pagina de Inventario.
 */
GtkWidget* create_inventario_page(void) {
    GtkWidget *page_vbox = create_colored_page("Gestion de Inventario (Productos)", 0.9, 0.9, 1.0); // Azul claro

    // Contenedor para el contenido real de la pagina, que ira dentro de la ventana con desplazamiento
    GtkWidget *content_area_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(content_area_vbox, TRUE);
    gtk_widget_set_vexpand(content_area_vbox, FALSE); // No expandir, el desplazamiento se encargara

    // Ventana con desplazamiento para envolver el contenido de la pagina
    GtkWidget *scrolled_window_page = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_page),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_page), content_area_vbox);
    gtk_box_pack_start(GTK_BOX(page_vbox), scrolled_window_page, TRUE, TRUE, 0); // Empaquetar la ventana con desplazamiento en la pagina principal

    // Formulario para la entrada de productos (GtkGrid para organizacion estructurada)
    GtkWidget *form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 5); // Espaciado entre filas
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 10); // Espaciado entre columnas
    gtk_grid_set_row_homogeneous(GTK_GRID(form_grid), FALSE); // Las filas no tienen la misma altura forzada
    gtk_grid_set_column_homogeneous(GTK_GRID(form_grid), FALSE); // Las columnas no tienen el mismo ancho forzado

    // Organizar campos en 2 columnas y 6 filas
    // Fila 0
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Codigo:"), 0, 0, 1, 1);
    GtkWidget *entry_codigo = gtk_entry_new();
    entry_inventario_codigo_global = entry_codigo;
    gtk_grid_attach(GTK_GRID(form_grid), entry_codigo, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Fecha Adquisicion (YYYY-MM-DD):"), 2, 0, 1, 1);
    GtkWidget *entry_fecha_adquisicion = gtk_entry_new();
    entry_inventario_fecha_adquisicion_global = entry_fecha_adquisicion;
    gtk_grid_attach(GTK_GRID(form_grid), entry_fecha_adquisicion, 3, 0, 1, 1);

    // Fila 1
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Nombre:"), 0, 1, 1, 1);
    GtkWidget *entry_nombre = gtk_entry_new();
    entry_inventario_nombre_global = entry_nombre;
    gtk_grid_attach(GTK_GRID(form_grid), entry_nombre, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Movimiento Inventario:"), 2, 1, 1, 1);
    GtkWidget *entry_movimiento = gtk_entry_new();
    entry_inventario_movimiento_global = entry_movimiento;
    gtk_grid_attach(GTK_GRID(form_grid), entry_movimiento, 3, 1, 1, 1);

    // Fila 2
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Categoria:"), 0, 2, 1, 1);
    GtkWidget *entry_categoria = gtk_entry_new();
    entry_inventario_categoria_global = entry_categoria;
    gtk_grid_attach(GTK_GRID(form_grid), entry_categoria, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Ubicacion:"), 2, 2, 1, 1);
    GtkWidget *entry_ubicacion = gtk_entry_new();
    entry_inventario_ubicacion_global = entry_ubicacion;
    gtk_grid_attach(GTK_GRID(form_grid), entry_ubicacion, 3, 2, 1, 1);

    // Fila 3
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Cantidad:"), 0, 3, 1, 1);
    GtkWidget *entry_cantidad = gtk_entry_new();
    entry_inventario_cantidad_global = entry_cantidad;
    gtk_grid_attach(GTK_GRID(form_grid), entry_cantidad, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Stock Minimo:"), 2, 3, 1, 1);
    GtkWidget *entry_stock_minimo = gtk_entry_new();
    entry_inventario_stock_minimo_global = entry_stock_minimo;
    gtk_grid_attach(GTK_GRID(form_grid), entry_stock_minimo, 3, 3, 1, 1);

    // Fila 4
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Precio (Bs):"), 0, 4, 1, 1);
    GtkWidget *entry_precio_bs = gtk_entry_new();
    entry_inventario_precio_bs_global = entry_precio_bs;
    gtk_grid_attach(GTK_GRID(form_grid), entry_precio_bs, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Costo Adquisicion:"), 2, 4, 1, 1);
    GtkWidget *entry_costo = gtk_entry_new();
    entry_inventario_costo_global = entry_costo;
    gtk_grid_attach(GTK_GRID(form_grid), entry_costo, 3, 4, 1, 1);

    // Fila 5
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Estado Fisico:"), 0, 5, 1, 1);
    GtkWidget *entry_estado = gtk_entry_new();
    entry_inventario_estado_global = entry_estado;
    gtk_grid_attach(GTK_GRID(form_grid), entry_estado, 1, 5, 1, 1);

    // Empaquetar el formulario en la caja de contenido
    gtk_box_pack_start(GTK_BOX(content_area_vbox), form_grid, FALSE, FALSE, 10);

    // Botones de accion (CRUD: Crear, Leer, Actualizar, Eliminar)
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    // Hacer que los botones en esta caja tengan el mismo ancho
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);

    GtkWidget *btn_crear = gtk_button_new_with_label("Crear Producto");
    g_signal_connect(btn_crear, "clicked", G_CALLBACK(on_inventario_crear_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_crear, TRUE, TRUE, 0);

    GtkWidget *btn_leer = gtk_button_new_with_label("Buscar Producto");
    g_signal_connect(btn_leer, "clicked", G_CALLBACK(on_inventario_buscar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_leer, TRUE, TRUE, 0);

    GtkWidget *btn_modificar = gtk_button_new_with_label("Modificar Producto");
    g_signal_connect(btn_modificar, "clicked", G_CALLBACK(on_inventario_modificar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_modificar, TRUE, TRUE, 0);

    GtkWidget *btn_eliminar = gtk_button_new_with_label("Eliminar Producto");
    g_signal_connect(btn_eliminar, "clicked", G_CALLBACK(on_inventario_eliminar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_eliminar, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(content_area_vbox), button_box, FALSE, FALSE, 10);

    // GtkTreeView para mostrar la lista de productos
    GtkWidget *scrolled_window_tree = gtk_scrolled_window_new(NULL, NULL); // Renombrado para evitar conflicto
    // Configurar la politica de la barra de desplazamiento (aparecer automaticamente si el contenido excede el tamano)
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_tree),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    // Establecer un tipo de sombra para el borde de la ventana de desplazamiento
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window_tree), GTK_SHADOW_ETCHED_IN);
    // Asegurarse de que la ventana de desplazamiento se expanda
    gtk_widget_set_hexpand(scrolled_window_tree, TRUE);
    gtk_widget_set_vexpand(scrolled_window_tree, TRUE);

    // Crear el GtkListStore (el modelo de datos que usa TreeView)
    // Se define con 11 columnas y sus respectivos tipos de datos
    product_list_store = gtk_list_store_new(11,           // Numero total de columnas en el modelo
                                           G_TYPE_STRING, // Col 0: Codigo
                                           G_TYPE_STRING, // Col 1: Nombre
                                           G_TYPE_STRING, // Col 2: Categoria
                                           G_TYPE_INT,    // Col 3: Cantidad
                                           G_TYPE_DOUBLE, // Col 4: Precio Bs
                                           G_TYPE_STRING, // Col 5: Estado Fisico
                                           G_TYPE_STRING, // Col 6: Fecha Adquisicion
                                           G_TYPE_STRING, // Col 7: Movimiento Inventario
                                           G_TYPE_STRING, // Col 8: Ubicacion
                                           G_TYPE_INT,    // Col 9: Nivel Stock Minimo
                                           G_TYPE_DOUBLE  // Col 10: Costo Adquisicion
                                           );

    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(product_list_store));
    // Asignar el puntero GtkTreeView a la variable global
    inventory_tree_view_global = tree_view;

    // Obtener la seleccion del TreeView
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    // Conectar la senal "changed" de la seleccion a nuestro callback
    g_signal_connect(selection, "changed", G_CALLBACK(on_inventario_tree_view_selection_changed), NULL); // CORREGIDO

    // Hacer visibles los encabezados de las columnas
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);
    // Mostrar lineas de cuadricula en el TreeView
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(tree_view), GTK_TREE_VIEW_GRID_LINES_BOTH);
    // No activar la fila con un solo clic (requiere doble clic o un boton explicito)
    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(tree_view), FALSE);

    // Anadir columnas visibles al TreeView (AHORA SE ANADEN LAS 11 COLUMNAS)
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 0, "Codigo");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 1, "Nombre");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 2, "Categoria");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 3, "Cantidad");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 4, "Precio (Bs)");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 5, "Estado");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 6, "Fecha Adq."); // Abreviado para espacio
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 7, "Movimiento");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 8, "Ubicacion");
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 9, "Stock Min."); // Abreviado
    add_tree_view_column(GTK_TREE_VIEW(tree_view), 10, "Costo Adq."); // Abreviado


    // Anadir el TreeView a la ventana con desplazamiento
    gtk_container_add(GTK_CONTAINER(scrolled_window_tree), tree_view);
    // Empaquetar la ventana con desplazamiento en la caja vertical del area de contenido
    gtk_box_pack_start(GTK_BOX(content_area_vbox), scrolled_window_tree, TRUE, TRUE, 10);

    // Inicializar el TreeView con los productos existentes al crear la pagina
    update_product_tree_view();

    g_print("[UI] Pagina de Inventario (CRUD de Productos) creada.\n");
    return page_vbox;
}

/**
 * @brief Crea la pagina de Productos para el GtkStack.
 * Esta seccion gestionara los productos vendidos, la cantidad disponible y los detalles del zapato por clase.
 * @return Un GtkWidget que representa la pagina de Productos.
 */
GtkWidget* create_productos_page(void) {
    GtkWidget *page_vbox = create_colored_page("Gestion de Productos Vendidos y Detalles", 0.8, 1.0, 0.8); // Color verde claro

    // Contenedor para el contenido real de la pagina, que ira dentro de la ventana con desplazamiento
    GtkWidget *content_area_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(content_area_vbox, TRUE);
    gtk_widget_set_vexpand(content_area_vbox, FALSE);

    // Ventana con desplazamiento para envolver el contenido de la pagina
    GtkWidget *scrolled_window_page = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_page),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_page), content_area_vbox);
    gtk_box_pack_start(GTK_BOX(page_vbox), scrolled_window_page, TRUE, TRUE, 0);

    // Cuadricula para el filtro (combo de categoria) y la lista de productos
    GtkWidget *top_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(top_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(top_grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), top_grid, FALSE, FALSE, 10);

    // ComboBox de filtro por categoria
    gtk_grid_attach(GTK_GRID(top_grid), gtk_label_new("Filtrar por Categoria:"), 0, 0, 1, 1);
    productos_category_combo_global = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(productos_category_combo_global), "Todos");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(productos_category_combo_global), "Deportivo"); // Corregido: "Deportivo" singular
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(productos_category_combo_global), "Casual");    // Corregido: "Casual" singular
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(productos_category_combo_global), "Vestir");    // Corregido: "Vestir" singular
    gtk_combo_box_set_active(GTK_COMBO_BOX(productos_category_combo_global), 0); // Seleccionar "Todos" por defecto
    g_signal_connect(productos_category_combo_global, "changed", G_CALLBACK(on_productos_category_changed), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(top_grid), productos_category_combo_global, 1, 0, 1, 1);

    // Ventana con desplazamiento para el GtkTreeView de la lista de productos
    GtkWidget *scrolled_window_tree = gtk_scrolled_window_new(NULL, NULL); // Renombrado
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_tree),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window_tree), GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_hexpand(scrolled_window_tree, TRUE);
    gtk_widget_set_vexpand(scrolled_window_tree, TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), scrolled_window_tree, TRUE, TRUE, 10); // Empaquetar debajo del filtro

    // GtkTreeView de la lista de productos
    productos_list_store = gtk_list_store_new(6,           // Solo mostrando detalles clave en esta tabla
                                             G_TYPE_STRING, // 0: Codigo
                                             G_TYPE_STRING, // 1: Nombre
                                             G_TYPE_STRING, // 2: Categoria
                                             G_TYPE_INT,    // 3: Cantidad Disponible
                                             G_TYPE_DOUBLE, // 4: Precio Venta Bs
                                             G_TYPE_STRING  // 5: Estado Fisico
                                             );

    productos_tree_view_global = gtk_tree_view_new_with_model(GTK_TREE_MODEL(productos_list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(productos_tree_view_global), TRUE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(productos_tree_view_global), GTK_TREE_VIEW_GRID_LINES_BOTH);
    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(productos_tree_view_global), FALSE);

    // Columnas para el TreeView de Productos (mostrando menos columnas que el inventario para una vista mas limpia)
    add_tree_view_column(GTK_TREE_VIEW(productos_tree_view_global), 0, "Codigo");
    add_tree_view_column(GTK_TREE_VIEW(productos_tree_view_global), 1, "Nombre");
    add_tree_view_column(GTK_TREE_VIEW(productos_tree_view_global), 2, "Categoria");
    add_tree_view_column(GTK_TREE_VIEW(productos_tree_view_global), 3, "Cant. Disp.");
    add_tree_view_column(GTK_TREE_VIEW(productos_tree_view_global), 4, "Precio (Bs)");
    add_tree_view_column(GTK_TREE_VIEW(productos_tree_view_global), 5, "Estado");

    gtk_container_add(GTK_CONTAINER(scrolled_window_tree), productos_tree_view_global);

    // Conectar la senal de cambio de seleccion para mostrar los detalles del producto
    GtkTreeSelection *selection_productos = gtk_tree_view_get_selection(GTK_TREE_VIEW(productos_tree_view_global));
    g_signal_connect(selection_productos, "changed", G_CALLBACK(on_productos_tree_view_selection_changed), NULL);

    // Area de visualizacion de detalles del producto
    GtkWidget *details_frame = gtk_frame_new("Detalles del Producto Seleccionado");
    gtk_container_set_border_width(GTK_CONTAINER(details_frame), 5);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), details_frame, FALSE, FALSE, 10);

    GtkWidget *details_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(details_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(details_grid), 10);
    gtk_container_add(GTK_CONTAINER(details_frame), details_grid);

    // Etiquetas para los detalles del producto (solo lectura) - organizadas en 2 columnas
    // Fila 0
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Codigo:"), 0, 0, 1, 1);
    label_productos_codigo_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_codigo_global, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Fecha Adquisicion:"), 2, 0, 1, 1);
    label_productos_fecha_adq_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_fecha_adq_global, 3, 0, 1, 1);

    // Fila 1
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Nombre:"), 0, 1, 1, 1);
    label_productos_nombre_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_nombre_global, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Movimiento Inventario:"), 2, 1, 1, 1);
    label_productos_movimiento_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_movimiento_global, 3, 1, 1, 1);

    // Fila 2
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Categoria:"), 0, 2, 1, 1);
    label_productos_categoria_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_categoria_global, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Ubicacion:"), 2, 2, 1, 1);
    label_productos_ubicacion_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_ubicacion_global, 3, 2, 1, 1);

    // Fila 3
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Cantidad Disponible:"), 0, 3, 1, 1);
    label_productos_cantidad_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_cantidad_global, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Stock Minimo:"), 2, 3, 1, 1);
    label_productos_stock_min_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_stock_min_global, 3, 3, 1, 1);

    // Fila 4
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Precio Venta (Bs):"), 0, 4, 1, 1);
    label_productos_precio_bs_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_precio_bs_global, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Costo Adquisicion:"), 2, 4, 1, 1);
    label_productos_costo_adq_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_costo_adq_global, 3, 4, 1, 1);

    // Fila 5
    gtk_grid_attach(GTK_GRID(details_grid), gtk_label_new("Estado Fisico:"), 0, 5, 1, 1);
    label_productos_estado_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(details_grid), label_productos_estado_global, 1, 5, 1, 1);

    // Boton para marcar como vendido
    GtkWidget *btn_marcar_vendido = gtk_button_new_with_label("Marcar como Vendido");
    g_signal_connect(btn_marcar_vendido, "clicked", G_CALLBACK(on_productos_marcar_vendido_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), btn_marcar_vendido, FALSE, FALSE, 10);

    g_print("[UI] Pagina de Productos creada.\n");
    return page_vbox;
}

/**
 * @brief Crea la pagina de Clientes para el GtkStack.
 * Esta pagina permitira gestionar la informacion del cliente (anadir, modificar, eliminar, ver).
 * @return Un GtkWidget que representa la pagina de Clientes.
 */
GtkWidget* create_clientes_page(void) {
    GtkWidget *page_vbox = create_colored_page("Gestion de Clientes", 0.8, 0.9, 1.0); // Azul claro para Clientes

    // Contenedor para el contenido real de la pagina, que ira dentro de la ventana con desplazamiento
    GtkWidget *content_area_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(content_area_vbox, TRUE);
    gtk_widget_set_vexpand(content_area_vbox, FALSE);

    // Ventana con desplazamiento para envolver el contenido de la pagina
    GtkWidget *scrolled_window_page = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_page),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_page), content_area_vbox);
    gtk_box_pack_start(GTK_BOX(page_vbox), scrolled_window_page, TRUE, TRUE, 0);

    // Formulario de entrada de clientes
    GtkWidget *form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), form_grid, FALSE, FALSE, 10);

    // Campos de entrada para Cliente
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Cedula:"), 0, 0, 1, 1);
    entry_clientes_cedula_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_clientes_cedula_global, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Nombre:"), 0, 1, 1, 1);
    entry_clientes_nombre_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_clientes_nombre_global, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Apellido:"), 0, 2, 1, 1);
    entry_clientes_apellido_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_clientes_apellido_global, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Telefono:"), 0, 3, 1, 1);
    entry_clientes_telefono_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_clientes_telefono_global, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Direccion:"), 0, 4, 1, 1);
    entry_clientes_direccion_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_clientes_direccion_global, 1, 4, 1, 1);

    // Botones de accion (CRUD) para Cliente
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), button_box, FALSE, FALSE, 10);

    GtkWidget *btn_crear_cliente = gtk_button_new_with_label("Crear Cliente");
    g_signal_connect(btn_crear_cliente, "clicked", G_CALLBACK(on_clientes_crear_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_crear_cliente, TRUE, TRUE, 0);

    GtkWidget *btn_buscar_cliente = gtk_button_new_with_label("Buscar Cliente");
    g_signal_connect(btn_buscar_cliente, "clicked", G_CALLBACK(on_clientes_buscar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_buscar_cliente, TRUE, TRUE, 0);

    GtkWidget *btn_modificar_cliente = gtk_button_new_with_label("Modificar Cliente");
    g_signal_connect(btn_modificar_cliente, "clicked", G_CALLBACK(on_clientes_modificar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_modificar_cliente, TRUE, TRUE, 0);

    GtkWidget *btn_eliminar_cliente = gtk_button_new_with_label("Eliminar Cliente");
    g_signal_connect(btn_eliminar_cliente, "clicked", G_CALLBACK(on_clientes_eliminar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_eliminar_cliente, TRUE, TRUE, 0);

    // TreeView para mostrar la lista de clientes
    GtkWidget *scrolled_window_tree = gtk_scrolled_window_new(NULL, NULL); // Renombrado
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_tree),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window_tree), GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_hexpand(scrolled_window_tree, TRUE);
    gtk_widget_set_vexpand(scrolled_window_tree, TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), scrolled_window_tree, TRUE, TRUE, 10);

    clientes_list_store = gtk_list_store_new(5,           // Numero de columnas para Cliente
                                            G_TYPE_STRING, // 0: Cedula
                                            G_TYPE_STRING, // 1: Nombre
                                            G_TYPE_STRING, // 2: Apellido
                                            G_TYPE_STRING, // 3: Telefono
                                            G_TYPE_STRING  // 4: Direccion
                                            );

    clientes_tree_view_global = gtk_tree_view_new_with_model(GTK_TREE_MODEL(clientes_list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clientes_tree_view_global), TRUE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(clientes_tree_view_global), GTK_TREE_VIEW_GRID_LINES_BOTH);
    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(clientes_tree_view_global), FALSE);

    add_tree_view_column(GTK_TREE_VIEW(clientes_tree_view_global), 0, "Cedula");
    add_tree_view_column(GTK_TREE_VIEW(clientes_tree_view_global), 1, "Nombre");
    add_tree_view_column(GTK_TREE_VIEW(clientes_tree_view_global), 2, "Apellido");
    add_tree_view_column(GTK_TREE_VIEW(clientes_tree_view_global), 3, "Telefono");
    add_tree_view_column(GTK_TREE_VIEW(clientes_tree_view_global), 4, "Direccion");

    gtk_container_add(GTK_CONTAINER(scrolled_window_tree), clientes_tree_view_global);

    // Conectar la senal de seleccion del TreeView de clientes
    GtkTreeSelection *selection_clientes = gtk_tree_view_get_selection(GTK_TREE_VIEW(clientes_tree_view_global));
    g_signal_connect(selection_clientes, "changed", G_CALLBACK(on_clientes_tree_view_selection_changed), NULL);

    g_print("[UI] Pagina de Clientes creada.\n");
    return page_vbox;
}

/**
 * @brief Crea la pagina de Facturas para el GtkStack.
 * Esta pagina manejara la gestion diaria de facturas (crear, eliminar, modificar, ver, imprimir/exportar).
 * @return Un GtkWidget que representa la pagina de Facturas.
 */
GtkWidget* create_facturas_page(void) {
    GtkWidget *page_vbox = create_colored_page("Gestion de Facturas", 1.0, 0.9, 0.8); // Naranja claro para Facturas

    // Contenedor para el contenido real de la pagina, que ira dentro de la ventana con desplazamiento
    GtkWidget *content_area_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(content_area_vbox, TRUE);
    gtk_widget_set_vexpand(content_area_vbox, FALSE);

    // Ventana con desplazamiento para envolver el contenido de la pagina
    GtkWidget *scrolled_window_page = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_page),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_page), content_area_vbox);
    gtk_box_pack_start(GTK_BOX(page_vbox), scrolled_window_page, TRUE, TRUE, 0);

    // Cuadricula principal para la entrada de facturas y detalles
    GtkWidget *main_invoice_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(main_invoice_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(main_invoice_grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), main_invoice_grid, FALSE, FALSE, 10);

    // --- Seccion de Datos de la Factura y Cliente ---
    GtkWidget *invoice_info_frame = gtk_frame_new("Datos de la Factura y Cliente");
    gtk_container_set_border_width(GTK_CONTAINER(invoice_info_frame), 5);
    gtk_grid_attach(GTK_GRID(main_invoice_grid), invoice_info_frame, 0, 0, 2, 1); // Abarca 2 columnas

    GtkWidget *invoice_info_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(invoice_info_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(invoice_info_grid), 10);
    gtk_container_add(GTK_CONTAINER(invoice_info_frame), invoice_info_grid);

    // Fila 0: Numero de Factura y Fecha
    gtk_grid_attach(GTK_GRID(invoice_info_grid), gtk_label_new("Numero Factura:"), 0, 0, 1, 1);
    label_factura_numero_global = gtk_label_new("AUTO GENERADO"); // Se actualizara automaticamente
    gtk_grid_attach(GTK_GRID(invoice_info_grid), label_factura_numero_global, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(invoice_info_grid), gtk_label_new("Fecha:"), 2, 0, 1, 1);
    label_factura_fecha_global = gtk_label_new(""); // Se actualizara automaticamente
    gtk_grid_attach(GTK_GRID(invoice_info_grid), label_factura_fecha_global, 3, 0, 1, 1);

    // Fila 1: Cedula del Cliente y Boton Buscar Cliente
    gtk_grid_attach(GTK_GRID(invoice_info_grid), gtk_label_new("Cedula Cliente:"), 0, 1, 1, 1);
    entry_factura_cliente_cedula_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(invoice_info_grid), entry_factura_cliente_cedula_global, 1, 1, 1, 1);
    GtkWidget *btn_buscar_cliente_factura = gtk_button_new_with_label("Buscar Cliente");
    g_signal_connect(btn_buscar_cliente_factura, "clicked", G_CALLBACK(on_facturas_buscar_cliente_clicked), NULL);
    gtk_grid_attach(GTK_GRID(invoice_info_grid), btn_buscar_cliente_factura, 2, 1, 2, 1); // Abarca 2 columnas

    // Fila 2-4: Datos del Cliente (solo visualizacion)
    gtk_grid_attach(GTK_GRID(invoice_info_grid), gtk_label_new("Nombre Cliente:"), 0, 2, 1, 1);
    label_factura_cliente_nombre_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(invoice_info_grid), label_factura_cliente_nombre_global, 1, 2, 3, 1); // Abarca 3 columnas

    gtk_grid_attach(GTK_GRID(invoice_info_grid), gtk_label_new("Telefono Cliente:"), 0, 3, 1, 1);
    label_factura_cliente_telefono_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(invoice_info_grid), label_factura_cliente_telefono_global, 1, 3, 3, 1);

    gtk_grid_attach(GTK_GRID(invoice_info_grid), gtk_label_new("Direccion Cliente:"), 0, 4, 1, 1);
    label_factura_cliente_direccion_global = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(invoice_info_grid), label_factura_cliente_direccion_global, 1, 4, 3, 1);

    // --- Seccion de Anadir Productos a la Factura Actual ---
    GtkWidget *add_product_frame = gtk_frame_new("Anadir Productos a la Factura Actual");
    gtk_container_set_border_width(GTK_CONTAINER(add_product_frame), 5);
    gtk_grid_attach(GTK_GRID(main_invoice_grid), add_product_frame, 0, 1, 2, 1);

    GtkWidget *add_product_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(add_product_frame), add_product_vbox);

    GtkWidget *add_product_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(add_product_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(add_product_grid), 10);
    gtk_box_pack_start(GTK_BOX(add_product_vbox), add_product_grid, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(add_product_grid), gtk_label_new("Codigo Producto:"), 0, 0, 1, 1);
    entry_factura_producto_codigo_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(add_product_grid), entry_factura_producto_codigo_global, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(add_product_grid), gtk_label_new("Cantidad:"), 2, 0, 1, 1);
    entry_factura_producto_cantidad_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(add_product_grid), entry_factura_producto_cantidad_global, 3, 0, 1, 1);

    GtkWidget *btn_add_product_to_invoice = gtk_button_new_with_label("Anadir Producto");
    g_signal_connect(btn_add_product_to_invoice, "clicked", G_CALLBACK(on_facturas_anadir_producto_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(add_product_vbox), btn_add_product_to_invoice, FALSE, FALSE, 0);

    // Tabla para los detalles de la factura actual (productos agregados)
    GtkWidget *scrolled_window_detalles = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_detalles),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled_window_detalles, TRUE); // Esto hace que la tabla de detalles se expanda
    gtk_box_pack_start(GTK_BOX(add_product_vbox), scrolled_window_detalles, TRUE, TRUE, 5);

    factura_detalles_list_store = gtk_list_store_new(5,           // Columnas para DetalleFactura
                                                    G_TYPE_STRING, // 0: Codigo Producto
                                                    G_TYPE_STRING, // 1: Descripcion (Nombre Producto)
                                                    G_TYPE_INT,    // 2: Cantidad
                                                    G_TYPE_DOUBLE, // 3: Precio Unitario
                                                    G_TYPE_DOUBLE  // 4: Subtotal
                                                    );
    factura_detalles_tree_view_global = gtk_tree_view_new_with_model(GTK_TREE_MODEL(factura_detalles_list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(factura_detalles_tree_view_global), TRUE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(factura_detalles_tree_view_global), GTK_TREE_VIEW_GRID_LINES_BOTH);

    add_tree_view_column(GTK_TREE_VIEW(factura_detalles_tree_view_global), 0, "Cod. Prod.");
    add_tree_view_column(GTK_TREE_VIEW(factura_detalles_tree_view_global), 1, "Descripcion");
    add_tree_view_column(GTK_TREE_VIEW(factura_detalles_tree_view_global), 2, "Cant.");
    add_tree_view_column(GTK_TREE_VIEW(factura_detalles_tree_view_global), 3, "P. Unit.");
    add_tree_view_column(GTK_TREE_VIEW(factura_detalles_tree_view_global), 4, "Subtotal");

    gtk_container_add(GTK_CONTAINER(scrolled_window_detalles), factura_detalles_tree_view_global);

    // Boton para eliminar producto de la factura actual (si esta seleccionado)
    GtkWidget *btn_remove_product_from_invoice = gtk_button_new_with_label("Eliminar Producto de Factura");
    g_signal_connect(btn_remove_product_from_invoice, "clicked", G_CALLBACK(on_facturas_eliminar_detalle_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(add_product_vbox), btn_remove_product_from_invoice, FALSE, FALSE, 0);


    // --- Seccion de Totales ---
    GtkWidget *totals_frame = gtk_frame_new("Totales");
    gtk_container_set_border_width(GTK_CONTAINER(totals_frame), 5);
    gtk_grid_attach(GTK_GRID(main_invoice_grid), totals_frame, 0, 2, 2, 1); // Abarca 2 columnas

    GtkWidget *totals_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(totals_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(totals_grid), 10);
    gtk_container_add(GTK_CONTAINER(totals_frame), totals_grid);

    gtk_grid_attach(GTK_GRID(totals_grid), gtk_label_new("Subtotal:"), 0, 0, 1, 1);
    label_factura_subtotal_global = gtk_label_new("0.00 Bs");
    gtk_grid_attach(GTK_GRID(totals_grid), label_factura_subtotal_global, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(totals_grid), gtk_label_new("IVA (1.3%):"), 0, 1, 1, 1);
    label_factura_iva_global = gtk_label_new("0.00 Bs");
    gtk_grid_attach(GTK_GRID(totals_grid), label_factura_iva_global, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(totals_grid), gtk_label_new("Total a Pagar:"), 0, 2, 1, 1);
    label_factura_total_global = gtk_label_new("0.00 Bs");
    gtk_grid_attach(GTK_GRID(totals_grid), label_factura_total_global, 1, 2, 1, 1);


    // --- Botones de Accion para la Factura Actual ---
    GtkWidget *action_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(action_buttons_box), TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), action_buttons_box, FALSE, FALSE, 10);

    GtkWidget *btn_nueva_factura = gtk_button_new_with_label("Nueva Factura");
    g_signal_connect(btn_nueva_factura, "clicked", G_CALLBACK(on_facturas_nueva_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(action_buttons_box), btn_nueva_factura, TRUE, TRUE, 0);

    GtkWidget *btn_guardar_factura = gtk_button_new_with_label("Guardar Factura");
    g_signal_connect(btn_guardar_factura, "clicked", G_CALLBACK(on_facturas_guardar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(action_buttons_box), btn_guardar_factura, TRUE, TRUE, 0);

    GtkWidget *btn_mostrar_factura = gtk_button_new_with_label("Mostrar Factura");
    g_signal_connect(btn_mostrar_factura, "clicked", G_CALLBACK(on_facturas_mostrar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(action_buttons_box), btn_mostrar_factura, TRUE, TRUE, 0);


    // --- Tabla de Facturas Registradas ---
    GtkWidget *registered_invoices_frame = gtk_frame_new("Facturas Registradas");
    gtk_container_set_border_width(GTK_CONTAINER(registered_invoices_frame), 5);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), registered_invoices_frame, TRUE, TRUE, 10);

    GtkWidget *scrolled_window_facturas = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_facturas),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window_facturas), GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_hexpand(scrolled_window_facturas, TRUE);
    // Ajustar el tamano vertical de la tabla de facturas registradas
    gtk_widget_set_vexpand(scrolled_window_facturas, FALSE); // No expandir verticalmente al maximo
    gtk_widget_set_size_request(scrolled_window_facturas, -1, 150); // Establecer una altura fija de 150px


    gtk_container_add(GTK_CONTAINER(registered_invoices_frame), scrolled_window_facturas);

    facturas_list_store = gtk_list_store_new(6,           // Columnas para Facturas
                                            G_TYPE_STRING, // 0: Numero Factura
                                            G_TYPE_STRING, // 1: Fecha
                                            G_TYPE_STRING, // 2: Cedula Cliente
                                            G_TYPE_STRING, // 3: Nombre Cliente
                                            G_TYPE_STRING, // 4: Apellido Cliente
                                            G_TYPE_STRING  // 5: Monto Total (como string formateado)
                                            );
    facturas_tree_view_global = gtk_tree_view_new_with_model(GTK_TREE_MODEL(facturas_list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(facturas_tree_view_global), TRUE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(facturas_tree_view_global), GTK_TREE_VIEW_GRID_LINES_BOTH);
    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(facturas_tree_view_global), FALSE);

    add_tree_view_column(GTK_TREE_VIEW(facturas_tree_view_global), 0, "Num. Factura");
    add_tree_view_column(GTK_TREE_VIEW(facturas_tree_view_global), 1, "Fecha");
    add_tree_view_column(GTK_TREE_VIEW(facturas_tree_view_global), 2, "Cedula Cliente");
    add_tree_view_column(GTK_TREE_VIEW(facturas_tree_view_global), 3, "Nombre Cliente");
    add_tree_view_column(GTK_TREE_VIEW(facturas_tree_view_global), 4, "Apellido Cliente");
    add_tree_view_column(GTK_TREE_VIEW(facturas_tree_view_global), 5, "Total");

    gtk_container_add(GTK_CONTAINER(scrolled_window_facturas), facturas_tree_view_global);

    // Conectar la senal de seleccion del TreeView de facturas
    GtkTreeSelection *selection_facturas = gtk_tree_view_get_selection(GTK_TREE_VIEW(facturas_tree_view_global));
    g_signal_connect(selection_facturas, "changed", G_CALLBACK(on_facturas_tree_view_selection_changed), NULL);

    // Botones de accion para facturas registradas (modificar, eliminar)
    GtkWidget *registered_invoice_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(registered_invoice_buttons_box), TRUE);
    gtk_box_pack_start(GTK_BOX(registered_invoices_frame), registered_invoice_buttons_box, FALSE, FALSE, 10);

    GtkWidget *btn_modificar_factura = gtk_button_new_with_label("Modificar Factura");
    g_signal_connect(btn_modificar_factura, "clicked", G_CALLBACK(on_facturas_modificar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(registered_invoice_buttons_box), btn_modificar_factura, TRUE, TRUE, 0);

    GtkWidget *btn_eliminar_factura = gtk_button_new_with_label("Eliminar Factura");
    g_signal_connect(btn_eliminar_factura, "clicked", G_CALLBACK(on_facturas_eliminar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(registered_invoice_buttons_box), btn_eliminar_factura, TRUE, TRUE, 0);


    g_print("[UI] Pagina de Facturas creada.\n");
    return page_vbox;
}

/**
 * @brief Crea la pagina de Trabajadores para el GtkStack.
 * @return Un GtkWidget que representa la pagina de Trabajadores.
 */
GtkWidget* create_trabajadores_page(void) {
    GtkWidget *page_vbox = create_colored_page("Gestion de Trabajadores", 1.0, 1.0, 0.8); // Amarillo claro para Trabajadores

    // Contenedor para el contenido real de la pagina, que ira dentro de la ventana con desplazamiento
    GtkWidget *content_area_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(content_area_vbox, TRUE);
    gtk_widget_set_vexpand(content_area_vbox, FALSE);

    // Ventana con desplazamiento para envolver el contenido de la pagina
    GtkWidget *scrolled_window_page = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_page),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_page), content_area_vbox);
    gtk_box_pack_start(GTK_BOX(page_vbox), scrolled_window_page, TRUE, TRUE, 0);


    // Formulario de entrada de trabajadores
    GtkWidget *form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), form_grid, FALSE, FALSE, 10);

    // Campos de entrada para Trabajador
    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Cedula:"), 0, 0, 1, 1);
    entry_trabajadores_cedula_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_trabajadores_cedula_global, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Numero Empleado:"), 2, 0, 1, 1);
    entry_trabajadores_numero_empleado_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_trabajadores_numero_empleado_global, 3, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Nombre:"), 0, 1, 1, 1);
    entry_trabajadores_nombre_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_trabajadores_nombre_global, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Apellido:"), 0, 2, 1, 1);
    entry_trabajadores_apellido_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_trabajadores_apellido_global, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Telefono:"), 0, 3, 1, 1);
    entry_trabajadores_telefono_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_trabajadores_telefono_global, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(form_grid), gtk_label_new("Direccion:"), 0, 4, 1, 1);
    entry_trabajadores_direccion_global = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form_grid), entry_trabajadores_direccion_global, 1, 4, 1, 1);

    // Botones de accion (CRUD) para Trabajador
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), button_box, FALSE, FALSE, 10);

    GtkWidget *btn_crear_trabajador = gtk_button_new_with_label("Crear Trabajador");
    g_signal_connect(btn_crear_trabajador, "clicked", G_CALLBACK(on_trabajadores_crear_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_crear_trabajador, TRUE, TRUE, 0);

    GtkWidget *btn_buscar_trabajador = gtk_button_new_with_label("Buscar Trabajador");
    g_signal_connect(btn_buscar_trabajador, "clicked", G_CALLBACK(on_trabajadores_buscar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_buscar_trabajador, TRUE, TRUE, 0);

    GtkWidget *btn_modificar_trabajador = gtk_button_new_with_label("Modificar Trabajador");
    g_signal_connect(btn_modificar_trabajador, "clicked", G_CALLBACK(on_trabajadores_modificar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_modificar_trabajador, TRUE, TRUE, 0);

    GtkWidget *btn_eliminar_trabajador = gtk_button_new_with_label("Eliminar Trabajador");
    g_signal_connect(btn_eliminar_trabajador, "clicked", G_CALLBACK(on_trabajadores_eliminar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), btn_eliminar_trabajador, TRUE, TRUE, 0);

    // TreeView para mostrar la lista de trabajadores
    GtkWidget *scrolled_window_tree = gtk_scrolled_window_new(NULL, NULL); // Renombrado
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_tree),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window_tree), GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_hexpand(scrolled_window_tree, TRUE);
    gtk_widget_set_vexpand(scrolled_window_tree, TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), scrolled_window_tree, TRUE, TRUE, 10);

    trabajadores_list_store = gtk_list_store_new(6,           // Numero de columnas para Trabajador
                                                G_TYPE_STRING, // 0: Cedula
                                                G_TYPE_STRING, // 1: Nombre
                                                G_TYPE_STRING, // 2: Apellido
                                                G_TYPE_STRING, // 3: Numero Empleado
                                                G_TYPE_STRING, // 4: Telefono
                                                G_TYPE_STRING  // 5: Direccion
                                                );

    trabajadores_tree_view_global = gtk_tree_view_new_with_model(GTK_TREE_MODEL(trabajadores_list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(trabajadores_tree_view_global), TRUE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(trabajadores_tree_view_global), GTK_TREE_VIEW_GRID_LINES_BOTH);
    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(trabajadores_tree_view_global), FALSE);

    add_tree_view_column(GTK_TREE_VIEW(trabajadores_tree_view_global), 0, "Cedula");
    add_tree_view_column(GTK_TREE_VIEW(trabajadores_tree_view_global), 1, "Nombre");
    add_tree_view_column(GTK_TREE_VIEW(trabajadores_tree_view_global), 2, "Apellido");
    add_tree_view_column(GTK_TREE_VIEW(trabajadores_tree_view_global), 3, "Num. Empleado");
    add_tree_view_column(GTK_TREE_VIEW(trabajadores_tree_view_global), 4, "Telefono");
    add_tree_view_column(GTK_TREE_VIEW(trabajadores_tree_view_global), 5, "Direccion");

    gtk_container_add(GTK_CONTAINER(scrolled_window_tree), trabajadores_tree_view_global);

    // Conectar la senal de seleccion del TreeView de trabajadores
    GtkTreeSelection *selection_trabajadores = gtk_tree_view_get_selection(GTK_TREE_VIEW(trabajadores_tree_view_global));
    g_signal_connect(selection_trabajadores, "changed", G_CALLBACK(on_trabajadores_tree_view_selection_changed), NULL);

    g_print("[UI] Pagina de Trabajadores creada.\n");
    return page_vbox;
}

/**
 * @brief Crea la pagina de Reportes para el GtkStack.
 * @return Un GtkWidget que representa la pagina de Reportes.
 */
GtkWidget* create_reportes_page(void) {
    GtkWidget *page_vbox = create_colored_page("Contenido de Reportes - Aqui podras generar informes y estadisticas.", 1.0, 0.8, 1.0); // Color rosa claro

    // Contenedor para el contenido real de la pagina, que ira dentro de la ventana con desplazamiento
    GtkWidget *content_area_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(content_area_vbox, TRUE);
    gtk_widget_set_vexpand(content_area_vbox, FALSE);

    // Ventana con desplazamiento para envolver el contenido de la pagina
    GtkWidget *scrolled_window_page = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_page),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_page), content_area_vbox);
    gtk_box_pack_start(GTK_BOX(page_vbox), scrolled_window_page, TRUE, TRUE, 0);

    // Crear un GtkGrid para organizar los botones de reportes
    GtkWidget *report_buttons_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(report_buttons_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(report_buttons_grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(report_buttons_grid), 10);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), report_buttons_grid, FALSE, FALSE, 0);

    // Botones de Reportes
    GtkWidget *btn_report_inventory = gtk_button_new_with_label("Reporte de Inventario");
    g_signal_connect(btn_report_inventory, "clicked", G_CALLBACK(on_reportes_inventario_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_inventory, 0, 0, 1, 1);

    GtkWidget *btn_report_cost_sale_ratio = gtk_button_new_with_label("Reporte Costo vs Venta");
    g_signal_connect(btn_report_cost_sale_ratio, "clicked", G_CALLBACK(on_reportes_rentabilidad_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_cost_sale_ratio, 1, 0, 1, 1);

    GtkWidget *btn_report_damaged_products = gtk_button_new_with_label("Reporte Productos Danados");
    g_signal_connect(btn_report_damaged_products, "clicked", G_CALLBACK(on_reportes_productos_danados_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_damaged_products, 0, 1, 1, 1);

    GtkWidget *btn_report_suppliers = gtk_button_new_with_label("Reporte de Proveedores");
    g_signal_connect(btn_report_suppliers, "clicked", G_CALLBACK(on_reportes_proveedores_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_suppliers, 1, 1, 1, 1);

    GtkWidget *btn_report_low_stock = gtk_button_new_with_label("Reporte Stock Bajo");
    g_signal_connect(btn_report_low_stock, "clicked", G_CALLBACK(on_reportes_stock_bajo_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_low_stock, 0, 2, 1, 1);

    GtkWidget *btn_report_worker_count = gtk_button_new_with_label("Reporte Cantidad Trabajadores");
    g_signal_connect(btn_report_worker_count, "clicked", G_CALLBACK(on_reportes_conteo_trabajadores_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_worker_count, 1, 2, 1, 1);

    GtkWidget *btn_report_customer_purchases = gtk_button_new_with_label("Reporte Compras Clientes");
    g_signal_connect(btn_report_customer_purchases, "clicked", G_CALLBACK(on_reportes_compras_clientes_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_customer_purchases, 0, 3, 1, 1);

    GtkWidget *btn_report_most_least_sold = gtk_button_new_with_label("Reporte Mas/Menos Vendido");
    g_signal_connect(btn_report_most_least_sold, "clicked", G_CALLBACK(on_reportes_mas_menos_vendidos_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_most_least_sold, 1, 3, 1, 1);

    GtkWidget *btn_report_daily_sales = gtk_button_new_with_label("Reporte Ventas Diarias");
    g_signal_connect(btn_report_daily_sales, "clicked", G_CALLBACK(on_reportes_ventas_diarias_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_daily_sales, 0, 4, 1, 1);

    GtkWidget *btn_report_last_seven_invoices = gtk_button_new_with_label("Reporte Ultimas 7 Facturas");
    g_signal_connect(btn_report_last_seven_invoices, "clicked", G_CALLBACK(on_reportes_ultimas_facturas_clicked), NULL); // CORREGIDO
    gtk_grid_attach(GTK_GRID(report_buttons_grid), btn_report_last_seven_invoices, 1, 4, 1, 1);

    // Area para mostrar el output del reporte
    GtkWidget *scrolled_window_report_output = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_report_output),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(scrolled_window_report_output, TRUE);
    gtk_widget_set_vexpand(scrolled_window_report_output, TRUE);
    gtk_box_pack_start(GTK_BOX(content_area_vbox), scrolled_window_report_output, TRUE, TRUE, 10);

    text_view_reportes_output_global = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_reportes_output_global), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view_reportes_output_global), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window_report_output), text_view_reportes_output_global);


    g_print("[UI] Pagina de Reportes creada.\n");
    return page_vbox;
}
