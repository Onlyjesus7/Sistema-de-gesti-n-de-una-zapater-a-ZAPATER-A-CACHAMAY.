# Zapatería Guayana - Sistema de Gestión

Este es un sistema de gestión de inventario para una zapatería, desarrollado en C con la biblioteca GTK para la interfaz gráfica. Permite a los usuarios administrar productos, realizar ventas y llevar un registro del inventario.

## Características Principales

El sistema ofrece una solución integral para la gestión de una zapatería, organizada en varios módulos accesibles desde una barra de navegación lateral.

### Módulo de Inventario
*   **Gestión Completa de Productos:** Realiza operaciones CRUD (Crear, Leer, Modificar, Eliminar) para los productos.
*   **Formulario Detallado:** Añade y edita productos con información completa: código, nombre, categoría, cantidad, precio, fecha de adquisición, ubicación en la tienda, stock mínimo, costo y estado físico.
*   **Búsqueda Rápida:** Encuentra productos instantáneamente por su código.
*   **Visualización de Inventario:** Muestra todos los productos en una tabla clara y organizada.

### Módulo de Productos
*   **Vista de Catálogo:** Visualiza los productos disponibles para la venta.
*   **Filtro por Categoría:** Filtra fácilmente los productos por categorías (Deportivo, Casual, Vestir) para una navegación más rápida.
*   **Detalles al Instante:** Selecciona un producto de la lista para ver todos sus detalles en un panel dedicado.
*   **Registro de Ventas Simplificado:** Marca un producto como vendido directamente desde esta vista, descontando automáticamente la cantidad del stock.

### Módulo de Clientes
*   **Administración de Clientes:** Realiza operaciones CRUD completas para la base de datos de clientes.
*   **Información de Contacto:** Almacena datos esenciales como cédula, nombre, apellido, teléfono y dirección.
*   **Búsqueda por Cédula:** Localiza a cualquier cliente de forma rápida usando su número de cédula.

### Módulo de Facturación
*   **Creación de Facturas:** Genera nuevas facturas con un número único y la fecha actual de forma automática.
*   **Integración con Clientes:** Busca y asocia un cliente existente a una factura con solo introducir su cédula.
*   **Carrito de Compras Dinámico:** Añade múltiples productos a una factura, especificando la cantidad de cada uno. El sistema valida la disponibilidad en el inventario en tiempo real.
*   **Cálculo Automático de Totales:** Calcula el subtotal, el IVA (1.3%) y el monto total de la factura de forma automática.
*   **Gestión de Facturas Emitidas:** Visualiza una lista de todas las facturas guardadas. Puedes seleccionarlas para modificarlas, eliminarlas o ver un detalle completo en una ventana emergente.

### Módulo de Trabajadores
*   **Gestión de Personal:** Realiza operaciones CRUD para los empleados de la tienda.
*   **Datos del Empleado:** Almacena información clave como cédula, nombre, apellido, número de empleado, teléfono y dirección.
*   **Búsqueda de Personal:** Encuentra a un trabajador por su cédula o número de empleado.

### Módulo de Reportes
*   **Visión General del Negocio:** Genera una variedad de reportes de texto para analizar el estado de la tienda:
    *   **Inventario General:** Lista completa de todos los productos.
    *   **Rentabilidad:** Relación entre el costo y el precio de venta de cada producto.
    *   **Productos Dañados:** Informe de productos marcados como "Dañado".
    *   **Proveedores:** Lista de los proveedores registrados.
    *   **Stock Bajo:** Productos cuya cantidad es igual o inferior al stock mínimo definido.
    *   **Lista de Trabajadores:** Conteo y lista del personal.
    *   **Clientes con Compras:** Relación de clientes que han realizado al menos una compra.
    *   **Más y Menos Vendidos:** Identifica el tipo de zapato más y menos popular.
    *   **Ventas Diarias:** Desglose de las ventas totales por tipo de zapato para cada día.
    *   **Últimas 7 Facturas:** Un detalle completo de las últimas siete facturas emitidas.

## Empezando

Estas instrucciones te guiarán para obtener una copia del proyecto en funcionamiento en tu máquina local para desarrollo y pruebas.

### Prerrequisitos

Necesitarás tener instalado el compilador GCC y la biblioteca de desarrollo de GTK.

#### En sistemas basados en Debian/Ubuntu (Linux)

```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev
```

#### En Windows

La forma más sencilla de obtener un entorno de desarrollo C con GTK en Windows es usando **MSYS2**.

1.  **Instalar MSYS2:** Descarga e instala MSYS2 desde [su web oficial](https://www.msys2.org/). Sigue las instrucciones de instalación, que normalmente incluyen la actualización de los paquetes base con `pacman -Syu`.
2.  **Instalar GCC y GTK3:** Abre una terminal de MSYS2 (MinGW 64-bit) y ejecuta el siguiente comando para instalar las herramientas de desarrollo y GTK3:
    ```bash
    pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-gtk3
    ```

### Instalación

1.  Clona el repositorio a tu máquina local:
    ```bash
    git clone https://github.com/tu_usuario/Zapateria_Guayana_code.git
    ```
2.  Navega al directorio del proyecto:
    ```bash
    cd Zapateria_Guayana_code
    ```
3.  Compila el proyecto. Puedes usar el archivo de proyecto de Code::Blocks (`Zapateria_Guayana_code.cbp`) o compilarlo manualmente con GCC:
    ```bash
    gcc -o mi_programa main.c callbacks.c data_manager.c ui_elements.c `pkg-config --cflags --libs gtk+-3.0`
    ```

## Uso

Una vez compilado, puedes ejecutar el programa con:

```bash
./mi_programa
```

La aplicación se iniciará, mostrando la ventana principal con la lista de zapatos. Desde allí, podrás utilizar los botones para añadir, modificar, eliminar y vender productos.

## Almacenamiento de Datos

Los datos de los zapatos se almacenan en un archivo binario llamado `zapatos.dat` en el mismo directorio que el ejecutable.

## Arquitectura y Modelo de Datos

El programa está estructurado en varios ficheros para separar las funcionalidades:

*   `main.c`: Contiene la función `main` que inicializa GTK, crea la ventana principal y carga los datos iniciales.
*   `ui_elements.c`: Se encarga de la creación de todos los widgets de la interfaz gráfica, como la vista de árbol (`GtkTreeView`), las ventanas de diálogo para añadir/modificar productos y los botones.
*   `callbacks.c`: Define todas las funciones de callback que se conectan a las señales de los widgets (por ejemplo, `on_btn_add_clicked`). Estas funciones manejan la interacción del usuario con la interfaz.
*   `data_manager.c`: Gestiona la lógica de negocio y la manipulación de datos. Contiene las funciones para leer y escribir los datos de los zapatos en el archivo `zapatos.dat`, así como las funciones para añadir, modificar y eliminar productos de la lista en memoria.
*   `data_types.h`: Define la estructura `Zapato` que se utiliza para almacenar la información de cada producto.
*   `constants.h`: Define constantes utilizadas en el programa, como el nombre del fichero de datos.

La persistencia de los datos se consigue guardando un array de estructuras `Zapato` directamente en un fichero binario (`zapatos.dat`). El programa carga este array en memoria al arrancar y lo vuelve a guardar en el fichero cada vez que se realiza un cambio (añadir, modificar o eliminar un producto).

### Estructuras de Datos Clave

Para manejar la información, el proyecto se basa en las siguientes estructuras (definidas en `data_types.h`):

*   **`Producto`**: Es la plantilla para cada zapato o artículo en la tienda. Contiene todos sus detalles, desde el código y nombre hasta el costo, precio de venta y cantidad en stock. Se utiliza principalmente en el **Módulo de Inventario** para la gestión de productos y en el **Módulo de Productos** para mostrarlos.

*   **`Cliente`**: Almacena la información de contacto de cada cliente que compra en la tienda. Es la base del **Módulo de Clientes** y se utiliza en el **Módulo de Facturación** para asociar una venta a una persona.

*   **`Trabajador`**: Guarda los datos de los empleados de la tienda. Toda la gestión de esta estructura se realiza en el **Módulo de Trabajadores**.

*   **`Factura`**: Representa una venta completa. Contiene los datos del cliente, la fecha, el monto total y, lo más importante, una lista de los productos vendidos en esa transacción.

*   **`DetalleFactura`**: Es una línea dentro de una factura. Especifica qué producto se vendió, cuántas unidades y a qué precio. Una `Factura` puede tener múltiples `DetalleFactura`.

En la versión actual, todos estos datos se gestionan en memoria mediante listas enlazadas de GLib (`GList`) y se inicializan con datos de ejemplo al arrancar la aplicación. No hay persistencia en disco.

## Desglose Detallado de Funciones

### `main.c`

*   `main()`: Punto de entrada de la aplicación. Inicializa GTK, el gestor de datos (`data_manager`), crea la ventana principal y todos los elementos de la UI a través de `create_main_window()`, muestra la ventana y comienza el bucle principal de GTK.

### `ui_elements.c`

*   `create_main_window()`: Construye la ventana principal de la aplicación, incluyendo la barra de cabecera, la barra lateral de navegación y el `GtkStack` que contiene todas las páginas.
*   `create_inventario_page()`: Crea la página de "Inventario". Esta página contiene un formulario para el CRUD (Crear, Leer, Modificar, Eliminar) de productos y un `GtkTreeView` para mostrar todos los productos del inventario.
*   `create_productos_page()`: Crea la página de "Productos". Esta página permite visualizar los productos, filtrarlos por categoría y marcarlos como vendidos.
*   `create_clientes_page()`: Crea la página de "Clientes" para la gestión de la información de los clientes (CRUD).
*   `create_facturas_page()`: Crea la página de "Facturas". Permite crear nuevas facturas, añadir productos, buscar clientes y ver una lista de las facturas ya generadas.
*   `create_trabajadores_page()`: Crea la página de "Trabajadores" para la gestión del personal (CRUD).
*   `create_reportes_page()`: Crea la página de "Reportes", que contiene botones para generar diferentes tipos de informes.
*   `update_product_tree_view()`: Actualiza la tabla de productos en la página de "Inventario".
*   `update_productos_page_tree_view()`: Actualiza la tabla de productos en la página de "Productos", aplicando el filtro de categoría seleccionado.
*   `update_clientes_tree_view()`: Actualiza la tabla de clientes.
*   `update_facturas_tree_view()`: Actualiza la tabla de facturas.
*   `update_trabajadores_tree_view()`: Actualiza la tabla de trabajadores.
*   `update_factura_detalle_tree_view()`: Actualiza la tabla de detalles de la factura que se está creando.
*   `add_tree_view_column()`: Función de utilidad para añadir columnas a los `GtkTreeView`.

### `callbacks.c`

*   `on_main_window_destroy()`: Cierra la aplicación y libera los recursos.
*   `on_btn_inventario_clicked()`, `on_btn_productos_clicked()`, etc.: Callbacks para los botones de la barra lateral que cambian la página visible en el `GtkStack`.
*   `on_inventario_crear_clicked()`, `on_inventario_buscar_clicked()`, etc.: Callbacks para los botones de la página de "Inventario" que realizan las operaciones CRUD sobre los productos.
*   `on_inventario_tree_view_selection_changed()`: Rellena los campos del formulario de inventario cuando se selecciona un producto en la tabla.
*   `on_productos_category_changed()`: Filtra la lista de productos en la página de "Productos" cuando se cambia la categoría en el ComboBox.
*   `on_productos_marcar_vendido_clicked()`: Marca un producto como vendido, actualizando su stock.
*   `on_clientes_crear_clicked()`, `on_clientes_buscar_clicked()`, etc.: Callbacks para el CRUD de clientes.
*   `on_facturas_nueva_clicked()`: Limpia el formulario de facturas para crear una nueva.
*   `on_facturas_buscar_cliente_clicked()`: Busca un cliente por su cédula y rellena sus datos en el formulario de la factura.
*   `on_facturas_anadir_producto_clicked()`: Añade un producto a la factura que se está creando.
*   `on_facturas_guardar_clicked()`: Guarda la factura actual.
*   `on_facturas_mostrar_clicked()`: Muestra una ventana con el detalle de la factura seleccionada.
*   `on_trabajadores_crear_clicked()`, `on_trabajadores_buscar_clicked()`, etc.: Callbacks para el CRUD de trabajadores.
*   `on_reportes_inventario_clicked()`, `on_reportes_rentabilidad_clicked()`, etc.: Callbacks para los botones de la página de "Reportes" que llaman a las funciones correspondientes del `data_manager` para generar y mostrar los informes.

### `data_manager.c`

*   `init_data_manager()`: Inicializa las listas en memoria para productos, clientes, etc., y las llena con datos de ejemplo.
*   `cleanup_data_manager()`: Libera toda la memoria utilizada por las listas de datos.
*   `add_producto()`, `find_producto()`, `update_producto()`, `delete_producto()`: Funciones CRUD para la gestión de productos en memoria.
*   `add_cliente()`, `find_cliente()`, `update_cliente()`, `delete_cliente()`: Funciones CRUD para la gestión de clientes.
*   `add_factura()`, `find_factura()`, `update_factura()`, `delete_factura()`: Funciones CRUD para la gestión de facturas.
*   `add_trabajador()`, `find_trabajador()`, `update_trabajador()`, `delete_trabajador()`: Funciones CRUD para la gestión de trabajadores.
*   `get_all_productos()`, `get_all_clientes()`, etc.: Funciones que devuelven una copia de las listas de datos.
*   `get_next_invoice_number()`: Genera un número de factura único y secuencial.
*   `dm_show_inventory()`, `dm_show_cost_sale_ratio()`, etc.: Funciones que generan el texto de los diferentes reportes y lo insertan en un `GtkTextBuffer` para su visualización.

## Contribuciones

Las contribuciones son bienvenidas. Si deseas contribuir, por favor, haz un fork del repositorio y crea un pull request con tus cambios.

