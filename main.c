
// =========================================================
// Archivo: main.c
// Descripci�n: Punto de entrada principal de la aplicaci�n GTK3 para Zapater�a Cachamay.
//              Inicializa GTK, crea la interfaz y gestiona el ciclo de vida.
// =========================================================

#include <gtk/gtk.h>       // Librer�a principal de GTK
#include "constants.h"     // Constantes del proyecto
#include "data_manager.h"  // Para inicializar y limpiar los datos (extern GList, etc.)
#include "ui_elements.h"   // Para crear los elementos de la interfaz (contiene create_main_window)
#include "callbacks.h"     // Para que las declaraciones extern de widgets globales sean visibles

// ** DEFINICI�N �NICA DE VARIABLES GLOBALES EN main.c **
// Estas variables se definen AQUI (se les asigna memoria y se inicializan a NULL)
// y SOLO AQUI en todo el proyecto. En otros archivos, se DECLARAN como 'extern'.
GtkWindow *main_app_window = NULL;
GtkStack *main_content_stack = NULL;


/**
 * @brief Funci�n principal de la aplicaci�n.
 * @param argc N�mero de argumentos de la l�nea de comandos.
 * @param argv Array de cadenas de argumentos de la l�nea de comandos.
 * @return C�digo de estado de la aplicaci�n.
 */
int main(int argc, char *argv[]) {
    // 1. Inicializar la librer�a GTK.
    gtk_init(&argc, &argv);
    g_print("[MAIN] GTK inicializado.\n");

    // 2. Inicializar el gestor de datos.
    init_data_manager();

    // 3. Crear la ventana principal y su contenido.
    // create_main_window() construye la UI y ASIGNA los punteros a
    // main_app_window y main_content_stack, que est�n definidos globalmente aqu�.
    create_main_window();

    // Como main_app_window ya se asigna dentro de create_main_window(),
    // solo necesitamos comprobar que se haya creado correctamente.
    if (main_app_window == NULL) {
        g_printerr("[MAIN] Error: La ventana principal no se pudo crear (main_app_window es NULL).\n");
        cleanup_data_manager();
        return 1;
    }
    g_print("[MAIN] Interfaz de usuario creada.\n");

    // 4. Muestra la ventana y todos los widgets que contiene.
    gtk_widget_show_all(GTK_WIDGET(main_app_window));

    // 5. Inicia el bucle principal de GTK.
    gtk_main();

    // 6. Limpiar recursos al finalizar.
    cleanup_data_manager();
    g_print("[MAIN] Aplicaci�n finalizada y recursos limpiados.\n");

    return 0; // Salida exitosa
}
