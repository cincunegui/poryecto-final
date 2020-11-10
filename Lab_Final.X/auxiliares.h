
#define random rand() & ((1 << 3)-1)

typedef enum{
    inicial,
    jugando, 
    fin_juego,
    } estado_juego ;
    
typedef struct {
    int color; //Corregir esto. Color no va a ser un entero 
    int posicion; 
} pelota;

typedef struct {
    
    int num_Partida; // Numero de la partida que se esta jugando
    uint8_t tiempo; // Tiempo que demoro la partida
    uint8_t Puntuacion;
    
} estadisticas_partida;

typedef enum{
        UI_OPTION_NUM,
        UI_OPTION_ALPHANUM,
        UI_OPTION_ALPHA,
} ui_options_t;
    
typedef enum{
        UI_MENU_STATE_INIT_SHOW, // Mostrar Menu
        UI_MENU_STATE_INIT_WAIT, // Esperar entrada
        UI_MENU_STATE_OPTIONS_SHOW, // Mostrar opciones
        UI_MENU_STATE_OPTIONS_WAIT, // Espera recibir datos
        UI_MENU_STATE_OPTIONS_CHECK,// Chequeo de datos recibidos
        UI_MENU_STATE_SET_TIMEDATE, // Setear hora
        UI_MENU_STATE_GET_LOG, //Desplegar registro
        UI_MENU_STATE_PLAY, //Jugar
                
        UI_MENU_STATE_OPTIONS_ENTRADA,
}ui_menu_states_t;

void Interfaz_Usuario(void) ;

void finalizar_juego (estado_juego *partida);

void mostrar_estadisticas(void);

void tablero(pelota *pelota_azul, pelota *pelota_roja);

int mover_pelota(pelota *pelota_azul);

void juego(void);

void jugar(void);

void inicio_juego(pelota *pelota_roja, pelota *pelota_azul, estado_juego *partida);

int sensibilidad(void);

void Set_Date(void);

void mostrar_estadisticas(void);

void finalizar_juego(estado_juego *partida);

bool UI_checkValidOption(uint8_t *p_src, ui_options_t p_type, uint32_t p_max, uint32_t p_min);

bool UI_waitForInput( uint8_t *p_dest );