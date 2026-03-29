#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Soy un Log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config

	// Le pedimos al config que nos dé los valores que leyó del archivo
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de la CLAVE para comprobar que lo leyó bien
	log_info(logger, "Leí la clave: %s", valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "CLIENTE", 1, LOG_LEVEL_INFO);
      //verifico si hubo un error al crearlo
	  if(nuevo_logger== NULL){
		printf("¡No se pudo crear el logger!\n");
		abort();
	  }
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
// config_create lee el archivo y lo carga en memoria
	nuevo_config = config_create("cliente.config");

	// Verificamos si hubo un error al leerlo (ej: si el archivo no existe o le pusimos mal el nombre)
	if (nuevo_config == NULL) {
		printf("¡No se pudo crear el config!\n");
		abort();
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
    // ¡No te olvides de liberar las lineas antes de regresar!

// Mientras lo que escribimos NO sea un texto vacío ("")
	while (strcmp(leido, "") != 0) {
		
		// Logueamos lo que el usuario escribió
		log_info(logger, "Ingresaste: %s", leido);

		// Devolvemos la memoria de esta línea que ya usamos
		free(leido);

		// Volvemos a leer otra línea para la próxima vuelta del ciclo
		leido = readline("> ");
	}

	// Si salimos del "while", significa que el usuario apretó Enter sin escribir nada.
	// Esa línea vacía también ocupó memoria, así que la liberamos
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
paquete=crear_paquete();
leido = readline("> ");
while (strcmp(leido, "") != 0) {
	
		
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);

		free(leido);

		//  Volvemos a leer adentro del ciclo para que la variable 'leido' cambie
		// Si no ponemos esto, el while evalúa siempre la primera palabra y nunca termina.
		leido = readline("> "); 

	}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	//Enviamos el paquete entero al servidor
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	// Cerramos la conexión (colgamos el teléfono)
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  if (logger != NULL) {
		log_destroy(logger);
	}
	// AHORA: Destruimos el config
	if (config != NULL) {
		config_destroy(config);
	}
}
