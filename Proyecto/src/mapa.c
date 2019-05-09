#include "mapa.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "info_nave.h"
char symbol_equipos[MAX_EQUIPOS] = {'A','B','C','D'};

tipo_mapa * mapa_create() {
	tipo_mapa  * new_mapa;
	new_mapa = (tipo_mapa *)malloc(sizeof(tipo_mapa));

	// inicializa info_naves
	for (int i = 0; i < N_EQUIPOS; i++){
		for (int j = 0; j < N_NAVES; j++) {
			info_nave * info = info_nave_create(i, j);
			new_mapa->info_naves[i][j] = *info;
			info_nave_destroy(info);
		}
	}


	
	for (int i = 0; i < N_EQUIPOS; i++) {
		new_mapa->num_naves[i] = N_NAVES;
	}

	for (int i = 0; i < MAPA_MAXX; i++) {
		for (int j = 0; j < MAPA_MAXY; j++) {
			mapa_clean_casilla(new_mapa, j, i);
		}
	}

	// Inicializa las casillas del mapa
	for (int i = 0; i < N_EQUIPOS; i++) {
		for (int j = 0; j < N_NAVES; j++) {
			info_nave nave = new_mapa->info_naves[i][j];
			new_mapa->casillas[nave.posy][nave.posx].equipo=nave.equipo;
			new_mapa->casillas[nave.posy][nave.posx].num_nave=nave.num;
			new_mapa->casillas[nave.posy][nave.posx].simbolo=symbol_equipos[nave.equipo];
		}
	}

	
	return new_mapa;
}

void mapa_destroy(tipo_mapa * mapa) {
	free(mapa);
}


void mapa_clean_casilla(tipo_mapa *mapa, int posy, int posx) {
	mapa->casillas[posy][posx].equipo=-1;
	mapa->casillas[posy][posx].num_nave=-1;
	mapa->casillas[posy][posx].simbolo=SYMB_VACIO;
}

tipo_casilla mapa_get_casilla(tipo_mapa *mapa, int posy, int posx) {
	return mapa->casillas[posy][posx];
}

int mapa_get_distancia(tipo_mapa *mapa, int oriy,int orix,int targety,int targetx) {
	int distx,disty;

	distx=abs(targetx - orix);
	disty=abs(targety - oriy);
	return (distx > disty)? distx:disty;
}

info_nave mapa_get_nave(tipo_mapa *mapa, int equipo, int num_nave) {
	return mapa->info_naves[equipo][num_nave];
}

int mapa_get_num_naves(tipo_mapa *mapa, int equipo) {
	return mapa->num_naves[equipo];
}

char mapa_get_symbol(tipo_mapa *mapa, int posy, int posx) {
	return mapa->casillas[posy][posx].simbolo;
}

bool mapa_is_casilla_vacia(tipo_mapa *mapa, int posy, int posx) {
	return (mapa->casillas[posy][posx].equipo < 0);
}

void mapa_restore(tipo_mapa *mapa) {
	int i,j;

	for(j=0;j<MAPA_MAXY;j++) {
		for(i=0;i<MAPA_MAXX;i++) {
			tipo_casilla cas = mapa_get_casilla(mapa,j, i);
			if (cas.equipo < 0) {
				mapa_set_symbol(mapa,j, i, SYMB_VACIO);
			}
			else {
				mapa_set_symbol(mapa, j, i,symbol_equipos[cas.equipo]);
			}
		}
	}
}

void mapa_set_symbol(tipo_mapa *mapa, int posy, int posx, char symbol) {
	mapa->casillas[posy][posx].simbolo=symbol;
}


bool mapa_set_nave(tipo_mapa *mapa, info_nave nave) {
	if (nave.equipo >= N_EQUIPOS) return false;
	if (nave.num >= N_NAVES) return false;
	mapa->info_naves[nave.equipo][nave.num]=nave;
	if (nave.vida >= 0) { // !!!!!!!!!!!!! cambiado, antes ponia if (nave.viva)
		mapa->casillas[nave.posy][nave.posx].equipo=nave.equipo;
		mapa->casillas[nave.posy][nave.posx].num_nave=nave.num;
		mapa->casillas[nave.posy][nave.posx].simbolo=symbol_equipos[nave.equipo];
	}
	else {
		mapa_clean_casilla(mapa,nave.posy, nave.posx);
	}
	return true;
}

void mapa_set_num_naves(tipo_mapa *mapa, int equipo, int numNaves) {
	mapa->num_naves[equipo]=numNaves;
}

void mapa_send_misil(tipo_mapa *mapa, int origeny, int origenx, int targety, int targetx) {
	int px=origenx;
	int py=origeny;
	int tx=targetx;
	int ty=targety;
	char ps = mapa_get_symbol(mapa,py, px);
	int nextx, nexty;
	char nexts;

	int run = tx - origenx;
	int rise = ty - origeny;
	float m = ((float) rise) / ((float) run);
	float b = origeny - (m * origenx);
	int inc = (origenx < tx)? 1:-1;

	for (nextx = origenx; (( origenx < tx) && (nextx <= tx)) || (( origenx > tx) && (nextx >= tx)); nextx+=inc)
	{
		// solve for y
		float y = (m * nextx) + b;

		// round to nearest int
		nexty = (y > 0.0) ? floor(y + 0.5) : ceil(y - 0.5);

		if ((nexty < 0) || (nexty >= MAPA_MAXY)) {
			continue;
		}
		nexts = mapa_get_symbol(mapa,nexty, nextx);
		mapa_set_symbol(mapa,nexty,nextx,'*');
		mapa_set_symbol(mapa,py,px,ps);
		usleep(50000);
		px = nextx;
		py = nexty;
		ps = nexts;
	}

	mapa_set_symbol(mapa,py, px,ps);
}


// retorna -1 si error, 0 si no. pos[2] es un argumento de salida, un buffer de coordenadas
int mapa_generate_pos_nave(int equipo, int num_nave, int pos[2]) {
	if (equipo >= MAX_EQUIPOS)
		return -1;
	if (num_nave >= MAX_NAVES)
		return -1;
	
	// Establece en que cuadrante del tablero comenzará el equipo
	int offsetx;
	int offsety;

	switch (equipo)
	{
		case 1:
			offsetx = (int)ceil(MAPA_MAXX/2*1.0) - 1;
			offsety = (int)ceil(MAPA_MAXY/2*1.0) - 1;
			break;
		case 2: 
			offsetx = (int)ceil(MAPA_MAXX/2*1.0) - 1;
			offsety = 0;
			break;
		case 3:
			offsetx = 0;
			offsety = (int)ceil(MAPA_MAXY/2*1.0) - 1;
			break;
		case 0:
		default:
			offsetx = 0;
			offsety = 0;
			break;
	}

	int off_rel_x;
	int off_rel_y;


	// Establece en que sub-cuadrante del cuadrante comenzará cada nave del equipo
	switch (num_nave)
	{

		case 1:
			off_rel_x = (int)ceil(MAPA_MAXX/4*1.0) - 1;
			off_rel_y = (int)ceil(MAPA_MAXY/4*1.0) - 1;
			break;

		case 2:
			off_rel_x = (int)ceil(MAPA_MAXX/4*1.0) - 1;
			off_rel_y = 0;
			break;

		case 3:
			off_rel_x = 0;
			off_rel_y = (int)ceil(MAPA_MAXY/4*1.0) - 1;			
			break;

		case 0:
		default:
			off_rel_x = 0;
			off_rel_y = 0;

			break;
	}


			
	pos[0] = 1 + (rand() % ((int)ceil(MAPA_MAXX/4*1.0) - 1));
	pos[1] = 1 + (rand() % ((int)ceil(MAPA_MAXY/4*1.0) - 1));

	pos[0] += (offsetx +  off_rel_x);
	pos[1] += (offsety + off_rel_y);



	return 0;
}



int mapa_get_pos_nave_enemiga_cercana (tipo_mapa * mapa, int equipo, int origenx, int origeny, int *posx, int *posy) {
	int curr_dist = -1;
	for (int i = 0; i < N_EQUIPOS; i++) {
		for (int j = 0; j < N_NAVES; j++) {
			if (mapa->info_naves[i][j].vida > 0 && mapa->info_naves[i][j].equipo != equipo) {
				int daux = mapa_get_distancia(mapa, origeny, origenx, mapa->info_naves[i][j].posy, mapa->info_naves[i][j].posx);
				if (curr_dist == -1 || curr_dist > daux) {
					curr_dist = daux;
					*posx = mapa->info_naves[i][j].posx;
					*posy = mapa->info_naves[i][j].posy;
				}
			}

		}
	}
	return curr_dist; 

}