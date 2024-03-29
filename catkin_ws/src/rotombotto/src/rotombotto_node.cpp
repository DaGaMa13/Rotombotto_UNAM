/**********************************************************************
*	Garcés Marín Daniel 		
*	TESIS <Construcción de una plataforma robótica abierta para pruebas de desempeño de componentes y algoritmos>
*
*	<ROTOMBOTTO> Nodo para prueba del software
*		El principal objetivo de este nodo es el dar una revisión de los elementos que componen el robot y publicar todos los
*		datos a disposición con el fin de revisar el estado del equipo y localizar discrepancias o problemas con el hardware.
*
*   Ultima versión: 8 de Mayo del 2019
**********************************************************************/

//>>>>>>>> LIBRERÍAS <<<<<<<<<<<<<
// ダ・ガ・マ・jû-san
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <termio.h>
#include <sys/ioctl.h>
#include <string>
#include <iostream>
using namespace std;
//----------------------------------

//>>> VARAIBLES GLOBALES

	//Variables de apoyo
int i=0,j=0; //varaibles contadores
int temp=0,cont=0; //Varaibles para la dirección de prueba de los motores, función pruebaMotores()
int num_fot=0; //Variable para la función valorFoto()
float valor_foto=0.0; //Variable para la función valorFoto()

	//Variables paraa guardar información de los tópicos
float datos_Foto[8]={0,0,0,0,0,0,0,0}; //Variable para la función valorFoto()
float datos_Tempt=0; //Variables para la funcion valorTempt()
float datos_joy[3]= {0,0,0}; //Variables para el tópico de información del joystick
float dirMotor[2]={0,0}; //Variable para indicar los valores de dirección y velocidadesa los motores

//_____________________________________________________________________________________________________________________

//>>>>>>>>>>> FUNCIONES <<<<<<<<<<<

	//Obtención de los valores de los fotoresistores
void valorFoto(const std_msgs::Float32MultiArray::ConstPtr& dFoto){

	for(i=0;i<4;i++){ //Vaciado de los valores fotoresitores
		datos_Foto[i]=dFoto->data[i];
		std::cout<<">>Dato publicado Fotoresitor["<<i<<"]:_ "<<datos_Foto[i]<<std::endl;	}//Fin Vaciado de los valores fotoresitores

	//Tratamiento de los datos:: Obteniedo valor más alto;
	for(i=0;i<4;i++){ //comparación de todos los valores prra saber el más alto

		if(valor_foto<=datos_Foto[i]){ //valor actual mayor
			num_fot=i;
			valor_foto=datos_Foto[i]; }	} //Fin de la busqueda mayor

	std::cout<<"Fuente de luz ubicada en fotoresitor["<<num_fot<<"]:: "<<valor_foto<<std::endl;

	std::cout<<" --"<<std::endl;									}//Fin de valorFoto
//----------------------------------------------------------------------------------------

	//Obtención de la temperatura registrada en el robot
void valorTempt(const std_msgs::Float32MultiArray::ConstPtr& dTempt){

	std::cout<<">>Dato publicado Temperatura::";
		datos_Tempt = dTempt->data[0];
		std::cout<<"["<<datos_Tempt<<"]";  //Fin del vaciado de los encoders

	std::cout<<" --"<<std::endl;  								}//Fin de valorEnc
//-----------------------------------------------------------------------------------------

	//Obtención del la dirección del nodo SMART THINGS
void dataJoy(const std_msgs::Float32MultiArray::ConstPtr& dataJ){

	datos_joy[0] = dataJ->data[0]; //Boton B
	datos_joy[1] = dataJ->data[1]; //BOTON DERECHO
	datos_joy[2] = dataJ->data[2]; //BOTON Izquierdo

	std::cout<<"Datos de los botones recibidos:: B:"<<datos_joy[0]<<" B_DER:"<<datos_joy[1]<<" B_IZQ:"<<datos_joy[2]<<std::endl;	}//Fin de dirObtenida
//-----------------------------------------------------------------------------------------

	//Función para la pruba de los motores
void pruebaMotores(const std_msgs::Float32MultiArray::ConstPtr& motorD){

	dirMotor[0] = motorD->data[0]; //Velocidad motor Derecho
	dirMotor[1] = motorD->data[1]; //Velocidad motor Izquiero

	std::cout<<"Datos de los botones recibidos:: VEL_DER:"<<dirMotor[0]<<" VEL_IZQ:"<<dirMotor[0]<<std::endl;

	if (dirMotor[0]>0 && dirMotor[1]>0)
		std::cout<<" MOTORES AVANZANDO"<<std::endl;

	else if (dirMotor[0]<0 && dirMotor[1]<0)
		std::cout<<" MOTORES RETROCESO"<<std::endl;

	else if (dirMotor[0]>0 && dirMotor[1]<0)
		std::cout<<" MOTORES GIRO IZQUIERDA"<<std::endl;

	else if (dirMotor[0]<0 && dirMotor[1]>0)
		std::cout<<" MOTORES GIRO DERECHA"<<std::endl;

	else if (dirMotor[0]==0 && dirMotor[1]==0)
		std::cout<<" MOTORES ALTO"<<std::endl;

	else
		std::cout<<" MOTORES DESCONOCIDO"<<std::endl;

}//Fin de prueba motores


//_______________________________________________________________________________________________________________
//Función principal
int main(int  argc, char** argv){

	std::cout<<">>>>>ROTOMBOTTO EN LÍNEA<<<<<<"<<std::endl;
	ros::init(argc,argv,"ROTOMBOTTO");
	ros:: NodeHandle n;

    //Obtención de los datos transmitidos por los diferentes nodos 
 	ros::Subscriber subFoto = n.subscribe("/hardware/sensors/luz",1000,valorFoto); //Nodo Sensors/Fotoresistores
 	ros::Subscriber subTempt = n.subscribe("/hardware/sensors/tempt",1000,valorTempt); //Nodo Sensors/Temperatura
 	ros::Subscriber subJoy = n.subscribe("/hardware/joystick/data",1000,dataJoy); //Nodo Hardware/joy
 	ros::Subscriber subMotorD = n.subscribe("/hardware/motors/speeds",1000,pruebaMotores); //Nodo Hardware/joystick


	ros::Rate loop(1);
    ros::Rate r(10);
	
	while(ros::ok()){

		ros::spinOnce();
		loop.sleep();
        std::cout<<""<<std::endl;

	} //Fin del while(ROS)

return 0;
}//Fin del main principal