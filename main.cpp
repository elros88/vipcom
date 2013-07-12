/*
 * main.cpp
 * 
 * Copyright 2013 Antonio José Del Valle Ortiz Lira <antonio.ortiz.988@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

/*
 * Software de Vision Estereoscopica que determina la presencia
 * de objetos cercanos.
 * Version 1.0.
 */

//Inclusion de Librerias:
//- OpenCV:

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;

//- C++ ANSI/ISO

#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::fstream;
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>


// - Generacion de mapa de profundidad

#include "ImagenEstereo.cpp"

// - Control de Pinguino

#include "Vibrador.cpp"


// Instanciacion de objetos de los modulos del software

ImagenEstereo imagen;
Vibrador vibrador;


//Creacion de flujos de capturas desde las camaras

VideoCapture camaraIzqCaptura ( 1 );
VideoCapture camaraDerCaptura ( 2 );

//Iniciar Modulo USB

int IniciarModuloUSB ( )
{
	vibrador.IniciarUSB ();
	vibrador.ListarDispositivos ();
	vibrador.AbrirPinguino ();
	
	return 0;
}

// Llamado del modulo de Generacion del mapa deprofundidad

bool GenerarImagenEstereo ( )
{
	imagen.CapturarImagenes ( camaraIzqCaptura , camaraDerCaptura );
	imagen.BuscarCorrespondencia ( );
	imagen.MostrarCorrespondencia ( );
	return imagen.VerificarExistenciaObstaculos ( );
}

// Funcion Principal

int main()
{	
	IniciarModuloUSB ( );
	
	cout<<"Configurando Video"<<endl;
	
	camaraIzqCaptura.set( CV_CAP_PROP_FRAME_WIDTH , 320 );
	camaraIzqCaptura.set( CV_CAP_PROP_FRAME_HEIGHT , 240 );
	camaraIzqCaptura.set( CV_CAP_PROP_FPS , 3 );
	
	cout<<"Configurando Video"<<endl;
	
	camaraDerCaptura.set( CV_CAP_PROP_FRAME_WIDTH , 320 );
	camaraDerCaptura.set( CV_CAP_PROP_FRAME_HEIGHT , 240 );
	camaraDerCaptura.set( CV_CAP_PROP_FPS , 3 );
	
	cout<<"FIN Configurando Video"<<endl;
	
	char salida;
	
	
	bool obstaculo = false;
		
	
	// Carga de datos de calibracion en el modulo de generacion del 
	//mapa de profundidad
	imagen.CargarDatosCalibracion ( );
	
	// Ciclo Principal
	while ( salida != 27)
	{
		obstaculo = GenerarImagenEstereo ( );
		
		vibrador.VerificarObstaculo ( obstaculo );
		
		salida = cvWaitKey (33);
	}
	
	//Liberacion de imagenes de la memoria RAM
	imagen.LiberarDatos ( );
	
	return 0;
}
