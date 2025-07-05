// =========================================================
// Archivo: data_manager.c
// Descripcion: Implementacion de las funciones de gestion de datos.
// =========================================================
#include "data_manager.h"
#include <stdio.h>   // Para g_print, snprintf
#include <stdlib.h>  // Para malloc, free
#include <string.h>  // Para strcpy, strcmp, memcpy
#include <time.h>    // Para time, localtime, strftime (para fechas y numeros de factura)
#include <gtk/gtk.h> // Necesario para GtkTextBuffer

// ** DEFINICION UNICA DE VARIABLES GLOBALES EN data_manager.c **
// Estas listas se definen AQUI y SOLO AQUI en todo el proyecto.
// En otros archivos, se DECLARAN como 'extern'.
GList *lista_productos = NULL;
GList *lista_clientes = NULL;
GList *lista_trabajadores = NULL;
GList *lista_facturas = NULL;
GList *lista_proveedores = NULL; // Mantener por si se usa mas adelante

// Variable global interna para el proximo numero de factura (incremental)
static int next_invoice_sequential_number = 1;


/**
 * @brief Funcion de destruccion para liberar la memoria de un Producto.
 * @param data Puntero al Producto a liberar.
 */
static void free_producto_data(gpointer data) {
    if (data) {
        free(data); // Asumimos que Producto fue asignado con malloc
    }
}

/**
 * @brief Funcion de destruccion para liberar la memoria de un Cliente.
 * @param data Puntero al Cliente a liberar.
 */
static void free_cliente_data(gpointer data) {
    if (data) {
        free(data); // Asumimos que Cliente fue asignado con malloc
    }
}

/**
 * @brief Funcion de destruccion para liberar la memoria de un Trabajador.
 * @param data Puntero al Trabajador a liberar.
 */
static void free_trabajador_data(gpointer data) {
    if (data) {
        free(data); // Asumimos que Trabajador fue asignado con malloc
    }
}

/**
 * @brief Funcion de destruccion para liberar la memoria de un DetalleFactura.
 * @param data Puntero al DetalleFactura a liberar.
 */
static void free_detalle_factura_data(gpointer data) {
    if (data) {
        free(data); // Asumimos que DetalleFactura fue asignado con malloc
    }
}

/**
 * @brief Funcion de destruccion para liberar la memoria de una Factura y sus detalles.
 * @param data Puntero a la Factura a liberar.
 */
static void free_factura_data(gpointer data) {
    if (data) {
        Factura *f = (Factura*)data;
        if (f->detalles_productos) { // Corregido: usar detalles_productos
            g_list_free_full(f->detalles_productos, free_detalle_factura_data);
            f->detalles_productos = NULL;
        }
        free(f);
    }
}


/**
 * @brief Inicializa el gestor de datos. Crea las listas vacias.
 */
void init_data_manager(void) {
    g_print("[DATA] Inicializando gestor de datos...\n");
    // Aseguramos que las listas estan limpias al inicio
    if (lista_productos != NULL) {
        g_list_free_full(lista_productos, free_producto_data);
        lista_productos = NULL;
    }
    if (lista_clientes != NULL) {
        g_list_free_full(lista_clientes, free_cliente_data);
        lista_clientes = NULL;
    }
    if (lista_trabajadores != NULL) {
        g_list_free_full(lista_trabajadores, free_trabajador_data); // Usar free_trabajador_data
        lista_trabajadores = NULL;
    }
    if (lista_facturas != NULL) {
        g_list_free_full(lista_facturas, free_factura_data);
        lista_facturas = NULL;
    }
    if (lista_proveedores != NULL) {
        g_list_free_full(lista_proveedores, g_free); // Asumimos que Proveedor es freeable directamente o no es necesario
        lista_proveedores = NULL;
    }

    // Inicializar el contador de facturas (podria cargar desde archivo en una app real)
    next_invoice_sequential_number = 1;

    // Datos de ejemplo
    Producto *p1 = malloc(sizeof(Producto));
    if (p1) {
        strcpy(p1->codigo, "PROD001");
        strcpy(p1->nombre, "Zapatillas Deportivas X");
        strcpy(p1->descripcion, "Zapatillas de running de alto rendimiento");
        strcpy(p1->categoria, "Deportivo");
        p1->cantidad_disponible = 50;
        strcpy(p1->ubicacion, "Estante A1");
        strcpy(p1->proveedor, "SportCorp");
        strcpy(p1->fecha_adquisicion, "2023-01-15");
        p1->costo = 45.00;
        p1->precio_venta_bs = 80.00;
        p1->precio_venta_cop = 120000.00;
        p1->precio_venta_usd = 20.00;
        p1->precio_venta_eur = 18.00;
        p1->nivel_stock_minimo = 10;
        strcpy(p1->estado_fisico, "Nuevo");
        strcpy(p1->movimiento_inventario, "Entrada inicial");
        lista_productos = g_list_append(lista_productos, p1);
    }

    Producto *p2 = malloc(sizeof(Producto));
    if (p2) {
        strcpy(p2->codigo, "PROD002");
        strcpy(p2->nombre, "Zapatos Casuales Urban");
        strcpy(p2->descripcion, "Calzado comodo para uso diario");
        strcpy(p2->categoria, "Casual");
        p2->cantidad_disponible = 30;
        strcpy(p2->ubicacion, "Estante B2");
        strcpy(p2->proveedor, "FootStyle");
        strcpy(p2->fecha_adquisicion, "2023-02-01");
        p2->costo = 30.00;
        p2->precio_venta_bs = 60.00;
        p2->precio_venta_cop = 90000.00;
        p2->precio_venta_usd = 15.00;
        p2->precio_venta_eur = 13.00;
        p2->nivel_stock_minimo = 5;
        strcpy(p2->estado_fisico, "Nuevo");
        strcpy(p2->movimiento_inventario, "Entrada inicial");
        lista_productos = g_list_append(lista_productos, p2);
    }

    Producto *p3 = malloc(sizeof(Producto));
    if (p3) {
        strcpy(p3->codigo, "PROD003");
        strcpy(p3->nombre, "Botas de Vestir Elegance");
        strcpy(p3->descripcion, "Botas de cuero para ocasiones formales");
        strcpy(p3->categoria, "Vestir");
        p3->cantidad_disponible = 20;
        strcpy(p3->ubicacion, "Vitrina 1");
        strcpy(p3->proveedor, "ElegantSteps");
        strcpy(p3->fecha_adquisicion, "2023-03-10");
        p3->costo = 70.00;
        p3->precio_venta_bs = 120.00;
        p3->precio_venta_cop = 180000.00;
        p3->precio_venta_usd = 30.00;
        p3->precio_venta_eur = 27.00;
        p3->nivel_stock_minimo = 8;
        strcpy(p3->estado_fisico, "Dañado"); // Ejemplo de producto dañado
        strcpy(p3->movimiento_inventario, "Entrada inicial");
        lista_productos = g_list_append(lista_productos, p3);
    }

    // Ejemplo de datos de cliente
    Cliente *c1 = malloc(sizeof(Cliente));
    if (c1) {
        strcpy(c1->cedula, "12345678");
        strcpy(c1->nombre, "Ana");
        strcpy(c1->apellido, "Garcia");
        strcpy(c1->telefono, "0412-1234567");
        strcpy(c1->direccion, "Av. Siempre Viva 123");
        lista_clientes = g_list_append(lista_clientes, c1);
    }

    Cliente *c2 = malloc(sizeof(Cliente));
    if (c2) {
        strcpy(c2->cedula, "87654321");
        strcpy(c2->nombre, "Luis");
        strcpy(c2->apellido, "Perez");
        strcpy(c2->telefono, "0424-7654321");
        strcpy(c2->direccion, "Calle Luna, Calle Sol");
        lista_clientes = g_list_append(lista_clientes, c2);
    }

    // Ejemplo de datos de trabajador
    Trabajador *t1 = malloc(sizeof(Trabajador));
    if (t1) {
        strcpy(t1->cedula, "V-10203040");
        strcpy(t1->nombre, "Maria");
        strcpy(t1->apellido, "Fernandez");
        strcpy(t1->numero_empleado, "EMP001");
        strcpy(t1->telefono, "0416-1112233");
        strcpy(t1->direccion, "Urb. El Bosque, Casa 5");
        lista_trabajadores = g_list_append(lista_trabajadores, t1);
    }
    Trabajador *t2 = malloc(sizeof(Trabajador));
    if (t2) {
        strcpy(t2->cedula, "V-20304050");
        strcpy(t2->nombre, "Carlos");
        strcpy(t2->apellido, "Rodriguez");
        strcpy(t2->numero_empleado, "EMP002");
        strcpy(t2->telefono, "0414-4445566");
        strcpy(t2->direccion, "Res. La Cascada, Apt 10A");
        lista_trabajadores = g_list_append(lista_trabajadores, t2);
    }
    // Inicializar facturas de ejemplo para los reportes
    Factura *f1 = malloc(sizeof(Factura));
    if (f1) {
        strcpy(f1->numero_factura, "FC-2024010001");
        strcpy(f1->fecha, "2024-01-01");
        strcpy(f1->cedula_cliente, "12345678");
        strcpy(f1->nombre_cliente, "Ana Garcia");
        strcpy(f1->telefono_cliente, "0412-1234567");
        strcpy(f1->direccion_cliente, "Av. Siempre Viva 123");
        f1->detalles_productos = NULL; // Corregido: usar detalles_productos
        DetalleFactura *fd1_1 = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
        strcpy(fd1_1->codigo_producto, "PROD001");
        strcpy(fd1_1->nombre_producto, "Zapatillas Deportivas X");
        strcpy(fd1_1->tipo_zapato, "Deportivo"); // Añadido tipo_zapato
        fd1_1->cantidad = 2;
        fd1_1->precio_unitario = 80.00; // Corregido: usar precio_unitario
        fd1_1->subtotal_detalle = fd1_1->cantidad * fd1_1->precio_unitario; // Corregido: usar subtotal_detalle
        f1->detalles_productos = g_list_append(f1->detalles_productos, fd1_1); // Corregido: usar detalles_productos
        f1->monto_total = fd1_1->subtotal_detalle; // Corregido: usar monto_total y subtotal_detalle
        f1->porcentaje_impuesto = 0.16;
        f1->monto_total = f1->monto_total * (1 + f1->porcentaje_impuesto); // Corregido: usar monto_total
        lista_facturas = g_list_append(lista_facturas, f1);
    }

    Factura *f2 = malloc(sizeof(Factura));
    if (f2) {
        strcpy(f2->numero_factura, "FC-2024010002");
        strcpy(f2->fecha, "2024-01-02");
        strcpy(f2->cedula_cliente, "87654321");
        strcpy(f2->nombre_cliente, "Luis Perez");
        strcpy(f2->telefono_cliente, "0424-7654321");
        strcpy(f2->direccion_cliente, "Calle Luna, Calle Sol");
        f2->detalles_productos = NULL; // Corregido: usar detalles_productos
        DetalleFactura *fd2_1 = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
        strcpy(fd2_1->codigo_producto, "PROD002");
        strcpy(fd2_1->nombre_producto, "Zapatos Casuales Urban");
        strcpy(fd2_1->tipo_zapato, "Casual"); // Añadido tipo_zapato
        fd2_1->cantidad = 1;
        fd2_1->precio_unitario = 60.00; // Corregido: usar precio_unitario
        fd2_1->subtotal_detalle = fd2_1->cantidad * fd2_1->precio_unitario; // Corregido: usar subtotal_detalle
        f2->detalles_productos = g_list_append(f2->detalles_productos, fd2_1); // Corregido: usar detalles_productos

        DetalleFactura *fd2_2 = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
        strcpy(fd2_2->codigo_producto, "PROD001");
        strcpy(fd2_2->nombre_producto, "Zapatillas Deportivas X");
        strcpy(fd2_2->tipo_zapato, "Deportivo"); // Añadido tipo_zapato
        fd2_2->cantidad = 1;
        fd2_2->precio_unitario = 80.00; // Corregido: usar precio_unitario
        fd2_2->subtotal_detalle = fd2_2->cantidad * fd2_2->precio_unitario; // Corregido: usar subtotal_detalle
        f2->detalles_productos = g_list_append(f2->detalles_productos, fd2_2); // Corregido: usar detalles_productos

        f2->monto_total = fd2_1->subtotal_detalle + fd2_2->subtotal_detalle; // Corregido: usar monto_total y subtotal_detalle
        f2->porcentaje_impuesto = 0.16;
        f2->monto_total = f2->monto_total * (1 + f2->porcentaje_impuesto); // Corregido: usar monto_total
        lista_facturas = g_list_append(lista_facturas, f2);
    }
    Factura *f3 = malloc(sizeof(Factura));
    if (f3) {
        strcpy(f3->numero_factura, "FC-2024010003");
        strcpy(f3->fecha, "2024-01-02"); // Mismo dia que f2 para reporte diario
        strcpy(f3->cedula_cliente, "12345678");
        strcpy(f3->nombre_cliente, "Ana Garcia");
        strcpy(f3->telefono_cliente, "0412-1234567");
        strcpy(f3->direccion_cliente, "Av. Siempre Viva 123");
        f3->detalles_productos = NULL; // Corregido: usar detalles_productos
        DetalleFactura *fd3_1 = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
        strcpy(fd3_1->codigo_producto, "PROD003");
        strcpy(fd3_1->nombre_producto, "Botas de Vestir Elegance");
        strcpy(fd3_1->tipo_zapato, "Vestir"); // Añadido tipo_zapato
        fd3_1->cantidad = 1;
        fd3_1->precio_unitario = 120.00; // Corregido: usar precio_unitario
        fd3_1->subtotal_detalle = fd3_1->cantidad * fd3_1->precio_unitario; // Corregido: usar subtotal_detalle
        f3->detalles_productos = g_list_append(f3->detalles_productos, fd3_1); // Corregido: usar detalles_productos
        f3->monto_total = fd3_1->subtotal_detalle; // Corregido: usar monto_total y subtotal_detalle
        f3->porcentaje_impuesto = 0.16;
        f3->monto_total = f3->monto_total * (1 + f3->porcentaje_impuesto); // Corregido: usar monto_total
        lista_facturas = g_list_append(lista_facturas, f3);
    }
    // Add more dummy invoices for testing get_last_n_facturas and other reports
    for (int i = 4; i <= 10; ++i) {
        Factura *f = malloc(sizeof(Factura));
        if (f) {
            snprintf(f->numero_factura, sizeof(f->numero_factura), "FC-202401000%d", i);
            snprintf(f->fecha, sizeof(f->fecha), "2024-01-%02d", i);
            strcpy(f->cedula_cliente, (i % 2 == 0) ? "12345678" : "87654321");
            strcpy(f->nombre_cliente, (i % 2 == 0) ? "Ana Garcia" : "Luis Perez");
            strcpy(f->telefono_cliente, (i % 2 == 0) ? "0412-1234567" : "0424-7654321");
            strcpy(f->direccion_cliente, (i % 2 == 0) ? "Av. Siempre Viva 123" : "Calle Luna, Calle Sol");
            f->detalles_productos = NULL; // Corregido: usar detalles_productos

            DetalleFactura *fd = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
            if (fd) {
                // Alternate between products for variety
                const char* prod_code = (i % 3 == 0) ? "PROD003" : ((i % 2 == 0) ? "PROD001" : "PROD002");
                const char* prod_name;
                const char* prod_type; // Variable para el tipo de zapato
                double unit_price;
                if (strcmp(prod_code, "PROD001") == 0) {
                    prod_name = "Zapatillas Deportivas X";
                    prod_type = "Deportivo";
                    unit_price = 80.00;
                } else if (strcmp(prod_code, "PROD002") == 0) {
                    prod_name = "Zapatos Casuales Urban";
                    prod_type = "Casual";
                    unit_price = 60.00;
                } else { // PROD003
                    prod_name = "Botas de Vestir Elegance";
                    prod_type = "Vestir";
                    unit_price = 120.00;
                }

                strcpy(fd->codigo_producto, prod_code);
                strcpy(fd->nombre_producto, prod_name);
                strcpy(fd->tipo_zapato, prod_type); // Asignar tipo de zapato
                fd->cantidad = (i % 2 == 0) ? 1 : 2; // Vary quantity
                fd->precio_unitario = unit_price; // Corregido: usar precio_unitario
                fd->subtotal_detalle = fd->cantidad * fd->precio_unitario; // Corregido: usar subtotal_detalle
                f->detalles_productos = g_list_append(f->detalles_productos, fd); // Corregido: usar detalles_productos
            }
            f->monto_total = (fd ? fd->subtotal_detalle : 0.0); // Corregido: usar monto_total y subtotal_detalle
            f->porcentaje_impuesto = 0.16;
            f->monto_total = f->monto_total * (1 + f->porcentaje_impuesto); // Corregido: usar monto_total
            lista_facturas = g_list_append(lista_facturas, f);
        }
    }


    g_print("[DATA] Gestor de datos inicializado con datos de ejemplo.\\n");
}

/**
 * @brief Libera el gestor de datos liberando toda la memoria asignada.
 */
void cleanup_data_manager(void) {
    g_print("[DATA] Limpiando gestor de datos...\\n");
    g_list_free_full(lista_productos, free_producto_data);
    lista_productos = NULL;
    g_list_free_full(lista_clientes, free_cliente_data);
    lista_clientes = NULL;
    g_list_free_full(lista_trabajadores, free_trabajador_data);
    lista_trabajadores = NULL;
    g_list_free_full(lista_facturas, free_factura_data);
    lista_facturas = NULL;
    g_print("[DATA] Memoria liberada.\\n");
}


// =========================================================
// Funciones CRUD para PRODUCTOS
// =========================================================

/**
 * @brief Agrega un nuevo producto a la lista.
 * @param nuevo_producto Puntero al Producto a agregar.
 * @return TRUE si se agrego con exito, FALSE si ya existe un producto con el mismo codigo.
 */
gboolean add_producto(Producto *nuevo_producto) {
    if (find_producto(nuevo_producto->codigo) != NULL) {
        g_printerr("[DATA] Error: Ya existe un producto con el codigo '%s'.\\n", nuevo_producto->codigo);
        return FALSE;
    }
    Producto *p_copy = malloc(sizeof(Producto));
    if (!p_copy) return FALSE;
    memcpy(p_copy, nuevo_producto, sizeof(Producto));
    lista_productos = g_list_append(lista_productos, p_copy);
    g_print("[DATA] Producto '%s' agregado.\\n", nuevo_producto->nombre);
    return TRUE;
}

/**
 * @brief Busca un producto por su codigo.
 * @param codigo El codigo del producto a buscar.
 * @return Puntero al Producto encontrado, o NULL si no se encuentra.
 */
Producto* find_producto(const gchar *codigo) {
    GList *iter;
    for (iter = lista_productos; iter != NULL; iter = iter->next) {
        Producto *p = (Producto*) iter->data;
        if (strcmp(p->codigo, codigo) == 0) {
            return p;
        }
    }
    return NULL;
}

/**
 * @brief Actualiza un producto existente.
 * @param producto_actualizado Puntero al Producto con los datos actualizados.
 * @return TRUE si se actualizo con exito, FALSE si el producto no se encontro.
 */
gboolean update_producto(Producto *producto_actualizado) {
    Producto *p_existente = find_producto(producto_actualizado->codigo);
    if (p_existente) {
        memcpy(p_existente, producto_actualizado, sizeof(Producto)); // Copiar todos los datos
        g_print("[DATA] Producto '%s' actualizado.\\n", producto_actualizado->nombre);
        return TRUE;
    }
    g_printerr("[DATA] Error: Producto con codigo '%s' no encontrado para actualizar.\\n", producto_actualizado->codigo);
    return FALSE;
}

/**
 * @brief Elimina un producto por su codigo.
 * @param codigo El codigo del producto a eliminar.
 * @return TRUE si se elimino con exito, FALSE si no se encontro.
 */
gboolean delete_producto(const gchar *codigo) {
    GList *iter;
    for (iter = lista_productos; iter != NULL; iter = iter->next) {
        Producto *p = (Producto*) iter->data;
        if (strcmp(p->codigo, codigo) == 0) {
            lista_productos = g_list_remove_link(lista_productos, iter);
            free_producto_data(p); // Liberar la memoria del producto eliminado
            g_list_free_1(iter);   // Liberar el enlace de la lista
            g_print("[DATA] Producto con codigo '%s' eliminado.\\n", codigo);
            return TRUE;
        }
    }
    g_printerr("[DATA] Error: Producto con codigo '%s' no encontrado para eliminar.\\n", codigo);
    return FALSE;
}

/**
 * @brief Obtiene una copia de la lista actual de productos.
 * La funcion que llama es responsable de liberar la GList retornada
 * (pero NO los datos internos de los productos, que son manejados por data_manager).
 * @param productos_out Puntero a la GList donde se copiaran los punteros de productos.
 */
void get_all_productos(GList **productos_out) {
    *productos_out = g_list_copy(lista_productos);
}


// =========================================================
// Funciones CRUD para CLIENTES
// =========================================================

/**
 * @brief Agrega un nuevo cliente a la lista.
 * @param nuevo_cliente Puntero al Cliente a agregar.
 * @return TRUE si se agrego con exito, FALSE si ya existe un cliente con la misma cedula.
 */
gboolean add_cliente(Cliente *nuevo_cliente) {
    if (find_cliente(nuevo_cliente->cedula) != NULL) {
        g_printerr("[DATA] Error: Ya existe un cliente con la cedula '%s'.\\n", nuevo_cliente->cedula);
        return FALSE;
    }
    Cliente *c_copy = malloc(sizeof(Cliente));
    if (!c_copy) return FALSE;
    memcpy(c_copy, nuevo_cliente, sizeof(Cliente));
    lista_clientes = g_list_append(lista_clientes, c_copy);
    g_print("[DATA] Cliente '%s %s' agregado.\\n", nuevo_cliente->nombre, nuevo_cliente->apellido);
    return TRUE;
}

/**
 * @brief Busca un cliente por su cedula.
 * @param cedula La cedula del cliente a buscar.
 * @return Puntero al Cliente encontrado, o NULL si no se encuentra.
 */
Cliente* find_cliente(const gchar *cedula) {
    GList *iter;
    for (iter = lista_clientes; iter != NULL; iter = iter->next) {
        Cliente *c = (Cliente*) iter->data;
        if (strcmp(c->cedula, cedula) == 0) {
            return c;
        }
    }
    return NULL;
}

/**
 * @brief Actualiza un cliente existente.
 * @param cliente_actualizado Puntero al Cliente con los datos actualizados.
 * @return TRUE si se actualizo con exito, FALSE si el cliente no se encontro.
 */
gboolean update_cliente(Cliente *cliente_actualizado) {
    Cliente *c_existente = find_cliente(cliente_actualizado->cedula);
    if (c_existente) {
        memcpy(c_existente, cliente_actualizado, sizeof(Cliente));
        g_print("[DATA] Cliente '%s %s' actualizado.\\n", cliente_actualizado->nombre, cliente_actualizado->apellido);
        return TRUE;
    }
    g_printerr("[DATA] Error: Cliente con cedula '%s' no encontrado para actualizar.\\n", cliente_actualizado->cedula);
    return FALSE;
}

/**
 * @brief Elimina un cliente por su cedula.
 * @param cedula La cedula del cliente a eliminar.
 * @return TRUE si se elimino con exito, FALSE si no se encontro.
 */
gboolean delete_cliente(const gchar *cedula) {
    GList *iter;
    for (iter = lista_clientes; iter != NULL; iter = iter->next) {
        Cliente *c = (Cliente*) iter->data;
        if (strcmp(c->cedula, cedula) == 0) {
            lista_clientes = g_list_remove_link(lista_clientes, iter);
            free_cliente_data(c); // Liberar la memoria del cliente eliminado
            g_list_free_1(iter);   // Liberar el enlace de la lista
            g_print("[DATA] Cliente con cedula '%s' eliminado.\\n", cedula);
            return TRUE;
        }
    }
    g_printerr("[DATA] Error: Cliente con cedula '%s' no encontrado para eliminar.\\n", cedula);
    return FALSE;
}

/**
 * @brief Obtiene una copia de la lista actual de clientes.
 * La funcion que llama es responsable de liberar la GList retornada
 * (pero NO los datos internos de los clientes, que son manejados por data_manager).
 * @param clientes_out Puntero a la GList donde se copiaran los punteros de clientes.
 */
void get_all_clientes(GList **clientes_out) {
    *clientes_out = g_list_copy(lista_clientes);
}


// =========================================================
// Funciones CRUD para FACTURAS
// =========================================================

/**
 * @brief Genera el proximo numero de factura en formato FC-YYYYMMNNNN.
 * @return Una cadena de caracteres con el proximo numero de factura.
 * Debe ser liberada con g_free() despues de su uso.
 */
gchar* get_next_invoice_number(void) {
    time_t rawtime;
    struct tm *info;
    char date_buffer[9]; //YYYYMMDD\0

    time(&rawtime);
    info = localtime(&rawtime);

    strftime(date_buffer, sizeof(date_buffer), "%Y%m%d", info);

    gchar *invoice_num = g_strdup_printf("FC-%s%04d", date_buffer, next_invoice_sequential_number++);
    return invoice_num;
}

/**
 * @brief Agrega una nueva factura a la lista.
 * @param nueva_factura Puntero a la Factura a agregar.
 * @return TRUE si se agrego con exito, FALSE si ya existe una factura con el mismo numero.
 */
gboolean add_factura(Factura *nueva_factura) {
    if (find_factura(nueva_factura->numero_factura) != NULL) {
        g_printerr("[DATA] Error: Ya existe una factura con el numero '%s'.\\n", nueva_factura->numero_factura);
        return FALSE;
    }
    Factura *f_copy = malloc(sizeof(Factura));
    if (!f_copy) return FALSE;
    memcpy(f_copy, nueva_factura, sizeof(Factura));
    // Los detalles tambien deben copiarse profundamente
    f_copy->detalles_productos = NULL; // Corregido: usar detalles_productos
    for (GList *l = nueva_factura->detalles_productos; l != NULL; l = l->next) { // Corregido: usar detalles_productos
        DetalleFactura *fd_original = (DetalleFactura*)l->data; // Corregido: usar DetalleFactura
        DetalleFactura *fd_copy = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
        if (fd_copy) {
            memcpy(fd_copy, fd_original, sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
            f_copy->detalles_productos = g_list_append(f_copy->detalles_productos, fd_copy); // Corregido: usar detalles_productos
        }
    }

    lista_facturas = g_list_append(lista_facturas, f_copy);
    g_print("[DATA] Factura '%s' agregada.\\n", nueva_factura->numero_factura);
    return TRUE;
}

/**
 * @brief Busca una factura por su numero.
 * @param numero_factura El numero de la factura a buscar.
 * @return Puntero a la Factura encontrada, o NULL si no se encuentra.
 */
Factura* find_factura(const gchar *numero_factura) {
    GList *iter;
    for (iter = lista_facturas; iter != NULL; iter = iter->next) {
        Factura *f = (Factura*) iter->data;
        if (strcmp(f->numero_factura, numero_factura) == 0) {
            return f;
        }
    }
    return NULL;
}

/**
 * @brief Actualiza una factura existente.
 * @param factura_actualizada Puntero a la Factura con los datos actualizados.
 * @return TRUE si se actualizo con exito, FALSE si la factura no se encontro.
 */
gboolean update_factura(Factura *factura_actualizada) {
    Factura *f_existente = find_factura(factura_actualizada->numero_factura);
    if (f_existente) {
        // Liberar detalles antiguos
        g_list_free_full(f_existente->detalles_productos, g_free); // Corregido: usar detalles_productos
        f_existente->detalles_productos = NULL; // Corregido: usar detalles_productos

        // Copiar datos generales
        memcpy(f_existente, factura_actualizada, sizeof(Factura));

        // Copiar detalles nuevos profundamente
        for (GList *l = factura_actualizada->detalles_productos; l != NULL; l = l->next) { // Corregido: usar detalles_productos
            DetalleFactura *fd_original = (DetalleFactura*)l->data; // Corregido: usar DetalleFactura
            DetalleFactura *fd_copy = malloc(sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
            if (fd_copy) {
                memcpy(fd_copy, fd_original, sizeof(DetalleFactura)); // Corregido: usar DetalleFactura
                f_existente->detalles_productos = g_list_append(f_existente->detalles_productos, fd_copy); // Corregido: usar detalles_productos
            }
        }
        g_print("[DATA] Factura '%s' actualizada.\\n", factura_actualizada->numero_factura);
        return TRUE;
    }
    g_printerr("[DATA] Error: Factura con numero '%s' no encontrada para actualizar.\\n", factura_actualizada->numero_factura);
    return FALSE;
}

/**
 * @brief Elimina una factura por su numero.
 * @param numero_factura El numero de la factura a eliminar.
 * @return TRUE si se elimino con exito, FALSE si no se encontro.
 */
gboolean delete_factura(const gchar *numero_factura) {
    GList *iter;
    for (iter = lista_facturas; iter != NULL; iter = iter->next) {
        Factura *f = (Factura*) iter->data;
        if (strcmp(f->numero_factura, numero_factura) == 0) {
            lista_facturas = g_list_remove_link(lista_facturas, iter);
            free_factura_data(f); // Liberar la memoria de la factura y sus detalles
            g_list_free_1(iter);   // Liberar el enlace de la lista
            g_print("[DATA] Factura con numero '%s' eliminada.\\n", numero_factura);
            return TRUE;
        }
    }
    g_printerr("[DATA] Error: Factura con numero '%s' no encontrada para eliminar.\\n", numero_factura);
    return FALSE;
}

/**
 * @brief Obtiene una copia de la lista actual de facturas.
 * La funcion que llama es responsable de liberar la GList retornada
 * (pero NO los datos internos de las facturas, que son manejados por data_manager).
 * @param facturas_out Puntero a la GList donde se copiaran los punteros de facturas.
 */
void get_all_facturas(GList **facturas_out) {
    *facturas_out = g_list_copy(lista_facturas);
}

/**
 * @brief Obtiene una copia de las ultimas N facturas de la lista.
 * La funcion que llama es responsable de liberar la GList retornada
 * (pero NO los datos internos de las facturas, que son manejados por data_manager).
 * @param n El numero de facturas mas recientes a obtener.
 * @param facturas_out Puntero a la GList donde se copiaran los punteros de facturas.
 */
void get_last_n_facturas(int n, GList **facturas_out) {
    *facturas_out = NULL;
    if (lista_facturas == NULL || n <= 0) {
        return;
    }

    // g_list_nth_data es O(N), pero para listas pequeñas esta bien.
    // Para listas muy grandes, se podria mantener un puntero al final o usar g_list_reverse y tomar los primeros n.
    int total_facturas = g_list_length(lista_facturas);
    int start_index = total_facturas - n;
    if (start_index < 0) {
        start_index = 0; // Si hay menos de N facturas, toma todas
    }

    GList *current = g_list_nth(lista_facturas, start_index);
    while (current != NULL) {
        *facturas_out = g_list_append(*facturas_out, current->data);
        current = current->next;
    }
    // NOTA: Se copia solo los punteros a las facturas existentes, no se hace deep copy.
    // Las facturas y sus detalles son gestionados por data_manager.
}


// =========================================================
// Funciones CRUD para TRABAJADORES
// =========================================================

/**
 * @brief Agrega un nuevo trabajador a la lista.
 * @param nuevo_trabajador Puntero al Trabajador a agregar.
 * @return TRUE si se agrego con exito, FALSE si ya existe un trabajador con la misma cedula o numero de empleado.
 */
gboolean add_trabajador(Trabajador *nuevo_trabajador) {
    if (find_trabajador(nuevo_trabajador->cedula) != NULL || find_trabajador(nuevo_trabajador->numero_empleado) != NULL) {
        g_printerr("[DATA] Error: Ya existe un trabajador con la cedula '%s' o numero de empleado '%s'.\\n",
                   nuevo_trabajador->cedula, nuevo_trabajador->numero_empleado);
        return FALSE;
    }
    Trabajador *t_copy = malloc(sizeof(Trabajador));
    if (!t_copy) return FALSE;
    memcpy(t_copy, nuevo_trabajador, sizeof(Trabajador));
    lista_trabajadores = g_list_append(lista_trabajadores, t_copy);
    g_print("[DATA] Trabajador '%s %s' agregado.\\n", nuevo_trabajador->nombre, nuevo_trabajador->apellido);
    return TRUE;
}

/**
 * @brief Busca un trabajador por su cedula o numero de empleado.
 * @param cedula_o_numero_empleado La cedula o numero de empleado del trabajador a buscar.
 * @return Puntero al Trabajador encontrado, o NULL si no se encuentra.
 */
Trabajador* find_trabajador(const gchar *cedula_o_numero_empleado) {
    GList *iter;
    for (iter = lista_trabajadores; iter != NULL; iter = iter->next) {
        Trabajador *t = (Trabajador*) iter->data;
        if (strcmp(t->cedula, cedula_o_numero_empleado) == 0 || strcmp(t->numero_empleado, cedula_o_numero_empleado) == 0) {
            return t;
        }
    }
    return NULL;
}

/**
 * @brief Actualiza un trabajador existente.
 * @param trabajador_actualizado Puntero al Trabajador con los datos actualizados.
 * @return TRUE si se actualizo con exito, FALSE si el trabajador no se encontro.
 */
gboolean update_trabajador(Trabajador *trabajador_actualizado) {
    Trabajador *t_existente = find_trabajador(trabajador_actualizado->cedula); // Buscar por cedula
    if (!t_existente) {
        t_existente = find_trabajador(trabajador_actualizado->numero_empleado); // Si no se encuentra, buscar por numero de empleado
    }

    if (t_existente) {
        memcpy(t_existente, trabajador_actualizado, sizeof(Trabajador));
        g_print("[DATA] Trabajador '%s %s' actualizado.\\n", trabajador_actualizado->nombre, trabajador_actualizado->apellido);
        return TRUE;
    }
    g_printerr("[DATA] Error: Trabajador con cedula/numero '%s/%s' no encontrado para actualizar.\\n",
               trabajador_actualizado->cedula, trabajador_actualizado->numero_empleado);
    return FALSE;
}

/**
 * @brief Elimina un trabajador por su cedula o numero de empleado.
 * @param cedula_o_numero_empleado La cedula o numero de empleado del trabajador a eliminar.
 * @return TRUE si el trabajador se elimino con exito, FALSE si no se encontro.
 */
gboolean delete_trabajador(const gchar *cedula_o_numero_empleado) {
    GList *iter;
    for (iter = lista_trabajadores; iter != NULL; iter = iter->next) {
        Trabajador *t = (Trabajador*) iter->data;
        if (strcmp(t->cedula, cedula_o_numero_empleado) == 0 || strcmp(t->numero_empleado, cedula_o_numero_empleado) == 0) {
            lista_trabajadores = g_list_remove_link(lista_trabajadores, iter);
            free_trabajador_data(t); // Liberar la memoria del trabajador eliminado
            g_list_free_1(iter);   // Liberar el enlace de la lista
            g_print("[DATA] Trabajador con identificador '%s' eliminado.\\n", cedula_o_numero_empleado);
            return TRUE;
        }
    }
    g_printerr("[DATA] Error: Trabajador con identificador '%s' no encontrado para eliminar.\\n", cedula_o_numero_empleado);
    return FALSE;
}

/**
 * @brief Obtiene una copia de la lista actual de trabajadores.
 * La funcion que llama es responsable de liberar la GList retornada
 * (pero NO los datos internos de los trabajadores, que son manejados por data_manager).
 * @param trabajadores_out Puntero a la GList donde se copiaran los punteros de trabajadores.
 */
void get_all_trabajadores(GList **trabajadores_out) {
    *trabajadores_out = g_list_copy(lista_trabajadores);
}


// =========================================================
// Funciones para generar Reportes (Implementacion Basica)
// =========================================================

/**
 * @brief Genera el reporte de Inventario de la Tienda.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_inventory(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte de Inventario de la Tienda ---\n\n");
    GList *productos_list = NULL;
    get_all_productos(&productos_list);

    if (productos_list == NULL) {
        g_string_append(report_str, "No hay productos en el inventario.\n");
    } else {
        g_string_append(report_str, "Código\tNombre\t\tCantidad\tUbicación\tEstado\n");
        g_string_append(report_str, "-----------------------------------------------------------------------\n");
        for (GList *iter = productos_list; iter != NULL; iter = g_list_next(iter)) {
            Producto *p = (Producto*)iter->data;
            g_string_append_printf(report_str, "%s\t%s\t\t%d\t\t%s\t\t%s\n",
                                   p->codigo, p->nombre, p->cantidad_disponible, p->ubicacion, p->estado_fisico);
        }
    }
    g_list_free(productos_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Relacion de Valor de Costo y Valor de Venta.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_cost_sale_ratio(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Relación Costo y Venta ---\n\n");
    GList *productos_list = NULL;
    get_all_productos(&productos_list);

    if (productos_list == NULL) {
        g_string_append(report_str, "No hay productos registrados.\n");
    } else {
        g_string_append(report_str, "Código\tNombre\t\tCosto (Bs)\tPrecio Venta (Bs)\tGanancia (Bs)\tMargen (%)\n");
        g_string_append(report_str, "---------------------------------------------------------------------------------------------------\n");
        for (GList *iter = productos_list; iter != NULL; iter = g_list_next(iter)) {
            Producto *p = (Producto*)iter->data;
            double ganancia = p->precio_venta_bs - p->costo;
            double margen = (p->precio_venta_bs > 0) ? (ganancia / p->precio_venta_bs) * 100 : 0.0;
            g_string_append_printf(report_str, "%s\t%s\t\t%.2f\t\t%.2f\t\t\t%.2f\t\t%.2f\n",
                                   p->codigo, p->nombre, p->costo, p->precio_venta_bs, ganancia, margen);
        }
    }
    g_list_free(productos_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Productos Dañados.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_damaged_products(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Productos Dañados ---\n\n");
    GList *productos_list = NULL;
    get_all_productos(&productos_list);

    gboolean found_damaged = FALSE;
    g_string_append(report_str, "Código\tNombre\t\tCantidad\tUbicación\n");
    g_string_append(report_str, "-------------------------------------------------------\n");
    for (GList *iter = productos_list; iter != NULL; iter = g_list_next(iter)) {
        Producto *p = (Producto*)iter->data;
        if (strcmp(p->estado_fisico, "Dañado") == 0) {
            g_string_append_printf(report_str, "%s\t%s\t\t%d\t\t%s\n",
                                   p->codigo, p->nombre, p->cantidad_disponible, p->ubicacion);
            found_damaged = TRUE;
        }
    }
    if (!found_damaged) {
        g_string_append(report_str, "No se encontraron productos con estado 'Dañado'.\n");
    }
    g_list_free(productos_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Relacion de Proveedores.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_suppliers(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Relación de Proveedores ---\n\n");
    GList *productos_list = NULL;
    get_all_productos(&productos_list);

    GList *suppliers = NULL; // Usaremos una GList para almacenar nombres de proveedores unicos

    for (GList *iter = productos_list; iter != NULL; iter = g_list_next(iter)) {
        Producto *p = (Producto*)iter->data;
        // Comprobar si el proveedor ya esta en la lista
        gboolean found = FALSE;
        for (GList *s_iter = suppliers; s_iter != NULL; s_iter = g_list_next(s_iter)) {
            if (strcmp((gchar*)s_iter->data, p->proveedor) == 0) {
                found = TRUE;
                break;
            }
        }
        if (!found && strlen(p->proveedor) > 0 && strcmp(p->proveedor, "Desconocido") != 0) {
            suppliers = g_list_append(suppliers, g_strdup(p->proveedor)); // Anadir una copia del nombre del proveedor
        }
    }

    if (suppliers == NULL) {
        g_string_append(report_str, "No se encontraron proveedores registrados en los productos.\n");
    } else {
        g_string_append(report_str, "Proveedores Registrados:\n");
        g_string_append(report_str, "------------------------\n");
        for (GList *s_iter = suppliers; s_iter != NULL; s_iter = g_list_next(s_iter)) {
            g_string_append_printf(report_str, "- %s\n", (gchar*)s_iter->data);
        }
    }
    g_list_free_full(suppliers, g_free); // Liberar la lista de proveedores y sus cadenas
    g_list_free(productos_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Nivel de Stock Minimo.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_low_stock_products(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Productos con Nivel de Stock Mínimo ---\n\n");
    GList *productos_list = NULL;
    get_all_productos(&productos_list);

    gboolean found_low_stock = FALSE;
    g_string_append(report_str, "Código\tNombre\t\tCantidad Actual\tStock Mínimo\tEstado\n");
    g_string_append(report_str, "---------------------------------------------------------------------------------\n");
    for (GList *iter = productos_list; iter != NULL; iter = g_list_next(iter)) {
        Producto *p = (Producto*)iter->data;
        if (p->cantidad_disponible <= p->nivel_stock_minimo) {
            g_string_append_printf(report_str, "%s\t%s\t\t%d\t\t%d\t\t%s\n",
                                   p->codigo, p->nombre, p->cantidad_disponible, p->nivel_stock_minimo, p->estado_fisico);
            found_low_stock = TRUE;
        }
    }
    if (!found_low_stock) {
        g_string_append(report_str, "Todos los productos están por encima de su nivel de stock mínimo.\n");
    }
    g_list_free(productos_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Cantidad de Trabajadores de la Tienda.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_worker_count(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Cantidad de Trabajadores ---\n\n");
    GList *trabajadores_list = NULL;
    get_all_trabajadores(&trabajadores_list);

    int count = g_list_length(trabajadores_list);
    g_string_append_printf(report_str, "Total de trabajadores registrados: %d\n\n", count);

    if (count > 0) {
        g_string_append(report_str, "Cédula\tNombre\t\tApellido\tNro. Empleado\n");
        g_string_append(report_str, "-----------------------------------------------------------------------\n");
        for (GList *iter = trabajadores_list; iter != NULL; iter = g_list_next(iter)) {
            Trabajador *t = (Trabajador*)iter->data;
            g_string_append_printf(report_str, "%s\t%s\t\t%s\t\t%s\n",
                                   t->cedula, t->nombre, t->apellido, t->numero_empleado);
        }
    }
    g_list_free(trabajadores_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Relacion de Clientes que realizaron compras.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_customer_purchases(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Clientes con Compras Registradas ---\n\n");
    GList *facturas_list = NULL;
    get_all_facturas(&facturas_list);

    GList *clientes_con_compras = NULL; // Lista para almacenar cédulas de clientes únicos

    for (GList *iter = facturas_list; iter != NULL; iter = g_list_next(iter)) {
        Factura *f = (Factura*)iter->data;
        // Verificar si la cédula del cliente ya está en la lista de clientes con compras
        gboolean found = FALSE;
        for (GList *c_iter = clientes_con_compras; c_iter != NULL; c_iter = g_list_next(c_iter)) {
            if (strcmp((gchar*)c_iter->data, f->cedula_cliente) == 0) {
                found = TRUE;
                break;
            }
        }
        if (!found && strlen(f->cedula_cliente) > 0) {
            clientes_con_compras = g_list_append(clientes_con_compras, g_strdup(f->cedula_cliente));
        }
    }

    if (clientes_con_compras == NULL) {
        g_string_append(report_str, "No se encontraron clientes con compras registradas.\n");
    } else {
        g_string_append(report_str, "Cédula\tNombre\t\tApellido\tTeléfono\n");
        g_string_append(report_str, "-----------------------------------------------------------------------\n");
        for (GList *c_iter = clientes_con_compras; c_iter != NULL; c_iter = g_list_next(c_iter)) {
            const gchar *cedula = (gchar*)c_iter->data;
            Cliente *c = find_cliente(cedula); // Buscar los detalles completos del cliente
            if (c) {
                g_string_append_printf(report_str, "%s\t%s\t\t%s\t\t%s\n",
                                       c->cedula, c->nombre, c->apellido, c->telefono);
            } else {
                g_string_append_printf(report_str, "%s\t(Cliente no encontrado en la base de datos)\n", cedula);
            }
        }
    }
    g_list_free_full(clientes_con_compras, g_free); // Liberar la lista de cédulas y sus cadenas
    g_list_free(facturas_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Tipo de zapato mas vendido y menos vendido.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_most_least_sold_shoe_type(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Tipo de Zapato Más y Menos Vendido ---\n\n");
    GList *facturas_list = NULL;
    get_all_facturas(&facturas_list);

    GHashTable *shoe_type_sales = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    if (facturas_list == NULL) {
        g_string_append(report_str, "No hay ventas registradas.\n");
    } else {
        for (GList *f_iter = facturas_list; f_iter != NULL; f_iter = g_list_next(f_iter)) {
            Factura *f = (Factura*)f_iter->data;
            for (GList *d_iter = f->detalles_productos; d_iter != NULL; d_iter = g_list_next(d_iter)) { // Corregido: detalles_productos
                DetalleFactura *detalle = (DetalleFactura*)d_iter->data; // Corregido: DetalleFactura
                gchar *type = g_strdup(detalle->tipo_zapato);
                gint *current_sales = (gint*)g_hash_table_lookup(shoe_type_sales, type);
                if (current_sales) {
                    *current_sales += detalle->cantidad;
                } else {
                    gint *new_sales = g_malloc(sizeof(gint));
                    *new_sales = detalle->cantidad;
                    g_hash_table_insert(shoe_type_sales, type, new_sales);
                }
            }
        }

        if (g_hash_table_size(shoe_type_sales) == 0) {
            g_string_append(report_str, "No hay datos de ventas por tipo de zapato.\n");
        } else {
            gchar *most_sold_type = NULL;
            gint max_sales = -1;
            gchar *least_sold_type = NULL;
            gint min_sales = G_MAXINT;

            GHashTableIter hash_iter;
            gpointer key, value;
            g_hash_table_iter_init(&hash_iter, shoe_type_sales);
            while (g_hash_table_iter_next(&hash_iter, &key, &value)) {
                gchar *type = (gchar*)key;
                gint sales = *(gint*)value;

                if (sales > max_sales) {
                    max_sales = sales;
                    most_sold_type = type;
                }
                if (sales < min_sales) {
                    min_sales = sales;
                    least_sold_type = type;
                }
            }
            g_string_append_printf(report_str, "Tipo de zapato más vendido: %s (Cantidad: %d)\n", most_sold_type, max_sales);
            g_string_append_printf(report_str, "Tipo de zapato menos vendido: %s (Cantidad: %d)\n", least_sold_type, min_sales);
        }
    }
    g_hash_table_destroy(shoe_type_sales);
    g_list_free(facturas_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Totales de ventas diarias por tipo de zapato.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_daily_sales_by_shoe_type(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Ventas Diarias por Tipo de Zapato ---\n\n");
    GList *facturas_list = NULL;
    get_all_facturas(&facturas_list);

    // Key: date (char*), Value: GHashTable (Key: shoe_type (char*), Value: total_sales (double*))
    GHashTable *daily_sales_by_type = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)g_hash_table_destroy);

    if (facturas_list == NULL) {
        g_string_append(report_str, "No hay ventas registradas.\n");
    } else {
        for (GList *f_iter = facturas_list; f_iter != NULL; f_iter = g_list_next(f_iter)) {
            Factura *f = (Factura*)f_iter->data;
            gchar *date_copy = g_strdup(f->fecha);

            GHashTable *type_sales_for_day = (GHashTable*)g_hash_table_lookup(daily_sales_by_type, date_copy);
            if (!type_sales_for_day) {
                type_sales_for_day = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
                g_hash_table_insert(daily_sales_by_type, date_copy, type_sales_for_day);
            } else {
                g_free(date_copy); // Liberar la copia si ya existe la clave
            }

            for (GList *d_iter = f->detalles_productos; d_iter != NULL; d_iter = g_list_next(d_iter)) { // Corregido: detalles_productos
                DetalleFactura *detalle = (DetalleFactura*)d_iter->data; // Corregido: DetalleFactura
                gchar *type_copy = g_strdup(detalle->tipo_zapato);
                gdouble *current_total = (gdouble*)g_hash_table_lookup(type_sales_for_day, type_copy);
                if (current_total) {
                    *current_total += detalle->subtotal_detalle; // Corregido: subtotal_detalle
                } else {
                    gdouble *new_total = g_malloc(sizeof(gdouble));
                    *new_total = detalle->subtotal_detalle; // Corregido: subtotal_detalle
                    g_hash_table_insert(type_sales_for_day, type_copy, new_total);
                }
            }
        }

        if (g_hash_table_size(daily_sales_by_type) == 0) {
            g_string_append(report_str, "No hay datos de ventas diarias por tipo de zapato.\n");
        } else {
            // Obtener y ordenar las fechas
            GList *dates = g_hash_table_get_keys(daily_sales_by_type);
            dates = g_list_sort(dates, (GCompareFunc)strcmp); // Ordenar fechas alfabeticamente (que funciona para YYYY-MM-DD)

            for (GList *date_iter = dates; date_iter != NULL; date_iter = g_list_next(date_iter)) {
                gchar *date = (gchar*)date_iter->data;
                GHashTable *type_sales_for_day = (GHashTable*)g_hash_table_lookup(daily_sales_by_type, date);

                g_string_append_printf(report_str, "Fecha: %s\n", date);
                g_string_append(report_str, "  Tipo de Zapato\tVentas Totales (Bs)\n");
                g_string_append(report_str, "  -------------------------------------\n");

                // Obtener y ordenar los tipos de zapato para el dia
                GList *shoe_types = g_hash_table_get_keys(type_sales_for_day);
                shoe_types = g_list_sort(shoe_types, (GCompareFunc)strcmp);

                for (GList *type_iter = shoe_types; type_iter != NULL; type_iter = g_list_next(type_iter)) {
                    gchar *type = (gchar*)type_iter->data;
                    gdouble *total_sales = (gdouble*)g_hash_table_lookup(type_sales_for_day, type);
                    g_string_append_printf(report_str, "  %s\t\t%.2f\n", type, *total_sales);
                }
                g_list_free(shoe_types); // Liberar la lista de tipos de zapato (no las cadenas)
                g_string_append(report_str, "\n");
            }
            g_list_free(dates); // Liberar la lista de fechas (no las cadenas)
        }
    }
    g_hash_table_destroy(daily_sales_by_type);
    g_list_free(facturas_list);
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}

/**
 * @brief Genera el reporte de Detalle de las ultimas N facturas.
 * @param buffer El GtkTextBuffer donde se escribira el reporte.
 */
void dm_show_last_seven_invoices(GtkTextBuffer *buffer) {
    GString *report_str = g_string_new("--- Reporte: Detalles de las Últimas 7 Facturas ---\n\n");
    GList *last_invoices = NULL;
    get_last_n_facturas(7, &last_invoices); // Obtener las ultimas 7 facturas

    if (last_invoices == NULL) {
        g_string_append(report_str, "No hay facturas registradas.\n");
    } else {
        for (GList *iter = last_invoices; iter != NULL; iter = g_list_next(iter)) {
            Factura *f = (Factura*)iter->data;
            g_string_append_printf(report_str, "-------------------------------------------------------\n");
            g_string_append_printf(report_str, "Factura Nro: %s\n", f->numero_factura);
            g_string_append_printf(report_str, "Fecha: %s\n", f->fecha);
            g_string_append_printf(report_str, "Cliente: %s %s (C.I.: %s)\n", f->nombre_cliente, f->apellido_cliente, f->cedula_cliente);
            g_string_append_printf(report_str, "Teléfono: %s, Dirección: %s\n", f->telefono_cliente, f->direccion_cliente);
            g_string_append(report_str, "  Productos:\n");
            g_string_append(report_str, "    Código\tProducto\t\tCantidad\tP.Unitario\tSubtotal\n");
            g_string_append(report_str, "    -------------------------------------------------------------------\n");

            if (f->detalles_productos == NULL) { // Corregido: detalles_productos
                g_string_append(report_str, "    (Sin productos en esta factura)\n");
            } else {
                for (GList *d_iter = f->detalles_productos; d_iter != NULL; d_iter = g_list_next(d_iter)) { // Corregido: detalles_productos
                    DetalleFactura *detalle = (DetalleFactura*)d_iter->data; // Corregido: DetalleFactura
                    g_string_append_printf(report_str, "    %s\t%s\t\t%d\t\t%.2f\t\t%.2f\n",
                                           detalle->codigo_producto, detalle->nombre_producto,
                                           detalle->cantidad, detalle->precio_unitario, detalle->subtotal_detalle); // Corregido: nombres de miembros
                }
            }
            g_string_append_printf(report_str, "  Subtotal: %.2f Bs\n", f->monto_total / (1 + f->porcentaje_impuesto)); // Recalcular subtotal
            g_string_append_printf(report_str, "  IVA (%.1f%%): %.2f Bs\n", f->porcentaje_impuesto * 100, f->monto_total - (f->monto_total / (1 + f->porcentaje_impuesto)));
            g_string_append_printf(report_str, "  Total Factura: %.2f Bs\n", f->monto_total);
            g_string_append(report_str, "\n");
        }
    }
    g_list_free(last_invoices); // Liberar la copia de la lista (no los datos de las facturas)
    gtk_text_buffer_set_text(buffer, report_str->str, -1);
    g_string_free(report_str, TRUE);
}
