/*
 * ImagenEstereo.hpp
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
 * Clase que captura dos imagenes de un arreglo estereo,
 * genera un mapa de profundidad y lo analiza en busca de 
 * objetos cercanos a las cámaras
 * 
 * DECLARACIÓN  
 * 
*/

#ifndef IMAGENESTEREO_HPP
#define IMAGENESTEREO_HPP

class ImagenEstereo
{
	private:
	/**
	 * Atributos Privados de la clase ImagenEstereo.
	 * */
	/*
	 * Datos Tipo Imagen.
	 * */
	//Imagenes donde se genera el mapa de correspondencia.
	Mat correspondencia;
	Mat correspondenciaReal;
	Mat mapaProfundidad;	
	
	//Imagenes capturadas desde el arreglo de cámaras estereos.
	Mat imagenIzq;
	Mat imagenDer;
	//Imagenes capturadas luego de la aplicación de la matriz 
	//de rectificación
	Mat imagenIzqRemap;
	Mat imagenDerRemap;
	
	//Rectangulos utilizados por la clase bloque	
	Rect roi1;
	Rect roi2;
		
	/*
	 * Datos Tipo Matriz.
	 * */
	
	//Matriz de Reproyección.
	Mat matrizReproyeccion;
	//Mapas sin distorsión de la cámara izquierda.
	Mat mapaIzqX;
	Mat mapaIzqY;
	//Mapas sin distorsión de la cámara derecha.
	Mat mapaDerX;
	Mat mapaDerY;
	
	//Clase que determina los datos para generar el mapa
	//de profundidad.
	StereoSGBM bloque;
	
	//escalar utilizados para acceder y obtener el color de cada 
	//pixel del mapa de correspondencia	
	CvScalar escalarPixel;
		
	public:
	
	/**
	 * Métodos Publicos de la clase Imagenestereo.
	 * */
	
	//Constructor por defecto.
	ImagenEstereo();
	//Metodo de captura de imagenes.
	int CapturarImagenes( VideoCapture camaraIzqCaptura , VideoCapture camaraDerCaptura );
	
	
	//Metodo que carga los mapas de desdistorción generados por 
	//la clase Camara.
	int CargarDatosCalibracion();
	
	
	//Genera un mapa de correspondencia en bruto.
	int BuscarCorrespondencia();
	
	
	//Genera el mapa de correspondencia Real dividiendo entre 16
	//el valorde cada pixel
	int CalcularProfundidad();
	
	
	//Libera las matrices que ya no se vana utilizar
	int LiberarDatos();
	
	
	//Muestra el mapa de correspondencia
	int MostrarCorrespondencia();
	
	
	//Recorre cada pixel y calcula el numero de ellos cuyo color sea
	//??? y los suma, los suma y verifica si representan o no un 
	//obstaculo
	bool VerificarExistenciaObstaculos();
	


};
#endif
