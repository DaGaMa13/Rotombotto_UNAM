/************************************************************
*	Garcés Marín Daniel 		
*	TESIS <Construcción de una plataforma robótica abierta para pruebas de desempeño de componentes y algoritmos>
*
*		Programa de Arduino enfocado en dos objetivos principales:
*			-Recolectar información de los sensores:
*       > Sensores de luminicencia TEMT6000 [4]
*       > Sensor de temperatura LM45  [1]
*     -Control de señales luminosas al usuario por medio de led's
*
*   Ultima versión: 4 de Marzo del 2019
*********************************************************************/
//DaGaMa_jû-san 

//LIBRERÍAS
#include <ros.h>
#include <std_msgs/Float32MultiArray.h> 
//-----------------------------------------------

//ASIGNACIÓN DE LOS PINES DEL ARDUINO MEGA

  //Entradas Analogicas
int sen_L[4]= {A0,A1,A2,A3};
int sen_T=A4;

//----------------------------------------------------------------------------------

//VARIABLES GLOBALES


  //Variables Topicos
int valor_SL[4]={0,0,0,0};  //Almacenamiento de valores sensados de intensidad luminosa
int valor_Temp=0; //Alemacenamiento de los valores sensados de temperatura
	
  //Almacenamiento de los datos que se enviarán a la Raspberry
float data_arduino[5];
  // [4]>Luminicencia + [1]>Temperatura

//----------------------------------------------------------------------------------------

  //ROS

std_msgs::Float32MultiArray d_robot;//Crea un arreglo de tipo multif32 de la libreria std_msgs
ros::NodeHandle n;//Crea un nodo en ros llamado n

  //Se subscribe al topico publicado en la raspbery para el control de los motores
 //Publica los valores registrados por el arduino para ser leidos po un nodo el la Raspberry
ros::Publisher data_robot("/hardware/arduino/data", &d_robot);

//____________________________________________________________________________________________________________________

	//SETUP
void setup(){ 

  n.getHardware()->setBaud(500000);
  
  //Iniciar nodo en ros
  n.initNode(); 
  
  n.advertise(data_robot); 
  d_robot.data_length = 5; //Declara tamaño del arreglo a publicar
  
} //Fin del SETUP

//_____________________________________________________________________________________________________________________
//>>>>> LOOP
void loop(){

  //TOMA DE DATOS DE LOS SENSORES

    //Datos de los sensores de luz
    for(int i=0;i<4;i++){
       valor_SL[i]=analogRead(sen_L[i]);
        data_arduino[i]=valor_SL[i];  //Asignacin al arreglo del topico a publicar
     }//Fin de las lecturas de los fotoresistores
        
  //Datos de la temperatura   
      data_arduino[4]=analogRead(sen_T);  //Asignacin al arreglo del topico a publicar


  //PROCESO DE LA PUBLICACION DE TOPICO      
    d_robot.data=data_arduino;//Se le asignan los datos recolectados al topico que se va a publicar

  //Publicación de la DATA general del arduino

    data_robot.publish(&d_robot);
    n.spinOnce();  //Publica los datos de mi arreglo a mi topico


}//Fin del LOOP
