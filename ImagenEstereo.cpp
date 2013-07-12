/*
 * ImagenEstereo.cpp
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
 *  Clase que captura dos imagenes de un arreglo estereo,
 * genera un mapa de profundidad y lo analiza en busca de 
 * objetos cercanos a las cámaras
 * 
 * IMPLEMENTACIÓN
 * 
 */

#include "ImagenEstereo.hpp"

ImagenEstereo::ImagenEstereo ( )
{	
	mapaIzqX.create( 320, 240, CV_16SC2 );
	mapaIzqY.create( 320, 240, CV_16UC1 );
	mapaDerX.create( 320, 240, CV_16SC2 );
	mapaDerY.create( 320, 240, CV_16UC1 );
	matrizReproyeccion.create( 4, 4, CV_64F );
	
	//bloque.init(0, 160, 11);
	
	bloque.minDisparity = 0;
	bloque.numberOfDisparities = 96;
	bloque.SADWindowSize = 5;
	bloque.preFilterCap = 21;
    bloque.P1 = 8*4*bloque.SADWindowSize*bloque.SADWindowSize;
    bloque.P2 = 32*4*bloque.SADWindowSize*bloque.SADWindowSize;
    bloque.uniquenessRatio = 10;
    bloque.speckleWindowSize = 70;
    bloque.speckleRange = 32;
    bloque.disp12MaxDiff = 1;
    bloque.fullDP = false;
	

}

int ImagenEstereo::CargarDatosCalibracion ( )
{
	  //cout<<"carga"<<endl;
	  
	  FileStorage mapaXIzq ("mapaXIzq.xml", FileStorage::READ);
	  FileStorage mapaYIzq ("mapaYIzq.xml", FileStorage::READ);
	  FileStorage mapaXDer ("mapaXDer.xml", FileStorage::READ);
	  FileStorage mapaYDer ("mapaYDer.xml", FileStorage::READ);
	  FileStorage Reproyeccion ("Reproyeccion.xml", FileStorage::READ);
	  
	  mapaXIzq["mapaIzqX"] >> mapaIzqX;
	  mapaYIzq["mapaIzqY"] >> mapaIzqY;
	  mapaXDer["mapaDerX"] >> mapaDerX;
	  mapaYDer["mapaDerY"] >> mapaDerY;
	  Reproyeccion["matrizReproyeccion"] >> matrizReproyeccion; 
	  	  
	  mapaXIzq.release();
	  mapaYIzq.release();
	  mapaXDer.release();
	  mapaYDer.release();
	  Reproyeccion.release();
			  
	  return 0;
  }

int ImagenEstereo::CapturarImagenes ( VideoCapture camaraIzqCaptura , VideoCapture camaraDerCaptura )
{
	cout<<"Capturando"<<endl;
	camaraIzqCaptura >> imagenIzq;
	cout<<"Capturando"<<endl;
	camaraDerCaptura >> imagenDer;
	cout<<"Fin Captura"<<endl;
	
	
	cout<<"Remapeando"<<endl;
    remap ( imagenIzq, imagenIzqRemap, mapaIzqX, mapaIzqY, INTER_LINEAR );
	cout<<"Remapeando"<<endl;
	remap ( imagenDer, imagenDerRemap, mapaDerX, mapaDerY, INTER_LINEAR  );
    cout<<"Fin Remapeo"<<endl;
    
	return 0;	
}

int ImagenEstereo::BuscarCorrespondencia ( )
{
	bloque (imagenIzqRemap, imagenDerRemap, correspondencia);
	
	correspondencia.convertTo (correspondenciaReal, CV_8U, 255/(bloque.numberOfDisparities*16.));
	
	return 0;	
}

int ImagenEstereo::CalcularProfundidad ( )
{
	

	return 0;
}

int ImagenEstereo::MostrarCorrespondencia ()
{	
	namedWindow ("Bloque");
	//namedWindow ("Derecha");
	//namedWindow ("Izquierda");
	
	imshow( "Bloque", correspondenciaReal);
	//imshow( "Derecha", imagenDer);
	//imshow( "Izquierda", imagenIzq);
	
	return 0;
}

bool ImagenEstereo::VerificarExistenciaObstaculos ( )
{
	int contadorPixel = 0;
	
	MatIterator_<uchar> iterador = correspondenciaReal.begin<uchar>();
	MatIterator_<uchar> finIterador = correspondenciaReal.end<uchar>();
	
	for ( ; iterador!= finIterador; ++iterador)
	{
		int colorPixel = 0;
			
		colorPixel = (*iterador);
		
		if (colorPixel >= 100)
		{
			contadorPixel++;
		}
	}

	if (contadorPixel >=4000)
	{
		return true;
	}
	
	return false;
}

int ImagenEstereo::LiberarDatos ( )
{
	matrizReproyeccion.release();
	mapaIzqX.release();
	mapaIzqY.release();
	mapaDerX.release();
	mapaDerY.release();
	
	return 0;
}
