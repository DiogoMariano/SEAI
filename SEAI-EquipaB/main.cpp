/*
 * main.cpp
 *
 *  Created on: 27/11/2019
 *      Author: mariano
 */
/*Version 1.0
 *
 * XML PARSER SEAI Equipa B
 * 1 - Write XML
 * 2 - Read XML*/

#include <iostream>
#include <string>
#include <list>
#include "Markup.h"



class Path{
	public:
		std::string type; //Line or circle
		int number; //Identificação do path
		int x_f, y_f; //destino

		Path(int number, int x_f, int y_f, std::string type){
			this->number=number;
			this->type=type;
			this->x_f=x_f;
			this->y_f=y_f;
		}
};

void showTheContent(std::list<Path> trajecto)
{
     std::list<Path>::iterator it = trajecto.begin();
      while(it!=trajecto.end()){
    	  std::cout << it->number << std::endl;
    	  it++;
      }
}

int main(){

	CMarkup xml;
	std::list<Path> trajecto;


	xml.Load("/home/mariano/Documents/SEAI/XMLProgram/xml_example.xml");

	int number=0, x_f=0, y_f=0;
	std::string type ="", str_aux="";

	xml.ResetPos(); //Reposicionamento
	//xml.FindElem("Path");
	while(xml.FindElem("Path")){

		//xml.FindElem("Path");
		xml.IntoElem(); //Entra no Path

		xml.FindElem("number"); //Number data
		str_aux = xml.GetData();
		number = stoi(str_aux);
		//std::cout << number << std::endl;

		xml.FindElem("type"); //Number type
		type = xml.GetData();
		//std::cout << type << std::endl;

		xml.FindElem("to_x"); //Number data
		str_aux = xml.GetData();
		x_f = stoi(str_aux);
		//std::cout << x_f << std::endl;

		xml.FindElem("to_y"); //Number data
		str_aux = xml.GetData();
		y_f = stoi(str_aux);
		//std::cout << y_f << std::endl;

		Path path_1(number, x_f, y_f, type);
		trajecto.push_back(path_1);
		xml.OutOfElem();

	}

	showTheContent(trajecto);







}




