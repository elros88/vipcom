/*
 * Vibrador.hpp
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


#ifndef VIBRADOR_HPP
#define VIBRADOR_HPP

class Vibrador
{
	private:
	
	//Variables de acceso al puerto USB
	
	libusb_device **dispositivos; //Lista de dispositivos.
	
	libusb_context *sesion; //Sesion de libusb.
	
	libusb_device_handle *manejador; //Manejador de dispositivos.
	
	size_t numeroDispositivos; //Valor que guarda el numero de dispositivos encontrados.
	
	//Variable de salida
	
	bool salir;
	
	unsigned char valor;
	
	// Variables que controlan que Archivo de Alarma utilizar
	int alertaObstaculo;
	int alertaDespeje;
	
	public:
	
	Vibrador ();
	
	int IniciarUSB ();
	int ListarDispositivos ();
	int AbrirPinguino ();
	int ComunicacionPinguino ();
	int CerrarPinguino ();
	int CerrarUSB ();
	void VerificarObstaculo (bool obstaculo);
};

#endif
