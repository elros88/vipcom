/*
 * Vibrador.cpp
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


#include <libusb.h>
#include "SDL/SDL.h"
#include "Vibrador.hpp"

Vibrador::Vibrador ()
{
	sesion = NULL;
	salir = false;	
	alertaObstaculo = 0;
    alertaDespeje = 0;	
}

int Vibrador::IniciarUSB ()
{
	int error;
	
	error = libusb_init (&sesion); //Iniciar libusb (si regresa 0 no hubo errores).
	
	if (error != 0)
	{
		return 1;		
	}
	
	libusb_set_debug (sesion, 3); //Determinar el valor de verbosidad a 3.
	
	return 0;	
}

int Vibrador::ListarDispositivos ()
{
	numeroDispositivos = libusb_get_device_list(sesion, &dispositivos); 
	
	if(numeroDispositivos < 0) 
	{
		return 1;
	}
	
	return 0;
}

int Vibrador::AbrirPinguino ()
{
	manejador = libusb_open_device_with_vid_pid(sesion, 0x4D8, 0xFEAA);
	
	if (manejador == NULL)
	{
		return 1;
	}
	
	
	libusb_free_device_list(dispositivos, 1);
	
	if (libusb_kernel_driver_active (manejador, 0) == 1)
	{
		cout<<"Driver del kernel para el dispositivo Activo"<<endl;
		
		if (libusb_detach_kernel_driver(manejador, 0) == 0)
		{
			cout<<"Driver del kernel para el dispositivo Desactivado"<<endl;
		}
		else
		{
			return 1;
		}
	}
	
	if (libusb_set_configuration (manejador, 3) != 0)
	{
		cout<<"Error al configurar dispositivo"<<endl;
		return 1;
	}
	
	if (libusb_claim_interface(manejador, 0) < 0)
	{
		return 1;
	}
	
	
	return 0;
}

int Vibrador::ComunicacionPinguino ()
{
	unsigned char *dato = &valor;
	int error;
	int bits;
	
	cout<<"Transmitiendo: "<<*dato<<endl;
	
	error = libusb_bulk_transfer (manejador, 0x01, dato, 1, &bits, 0);
	
	cout<<error<<endl;
	
	if (error == 0 && bits == 1)
	{
		cout<<"Transmicion Exitosa"<<endl;
	}
	
	else 
	{
		return 1;
	}
	
	return 0;
}

int Vibrador::CerrarPinguino ()
{
	if (libusb_release_interface(manejador, 0) != 0)
	{
		cout<<"No se pudo liberar el dispositivo"<<endl;
		return 1;
	}
	
	return 0;
}

int Vibrador::CerrarUSB ()
{
	libusb_close(manejador);
	
	libusb_exit(sesion);
	
	return 0;
}


void Vibrador::VerificarObstaculo (bool obstaculo)
{
	if(obstaculo)
    {
        alertaObstaculo ++;
        alertaDespeje = 0;

        if(alertaObstaculo == 1)
        {
            cout<<"Existe un Obstaculo"<<endl;
            valor = 'o';
            ComunicacionPinguino ( );
        }
    }

    else
    {
        alertaObstaculo = 0;
        alertaDespeje ++;

        if(alertaDespeje == 1)
        {
            cout<<"Obstaculo Removido"<<endl;
            valor = 'l';
            ComunicacionPinguino ( );
        }
    }
	
	
}
