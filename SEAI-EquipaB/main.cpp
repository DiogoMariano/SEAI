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
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

using namespace std;

static string ADR_XML =  "/home/mariano/Documents/SEAI/SEAI/SEAI-EquipaB/XML_SCRIPTS/";//Endereço fixo dos cripts de xml

int state = 0, sub_state=0;

/*Função
 *
 * to_xml: passa o parametro (string name) para um endereço de directorio
 * exists: analisa se o ficheiro xml que peço existe ou não
 * */

string to_xml(string name){
	return ADR_XML + name + ".xml";
}

inline bool exists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

string get_name(){
	string adr = "";
	cout << "NOME: ";
	cin >> adr;
	return to_xml(adr);
}

bool info_file(string adr){

	bool resp;

	if(exists(adr)){
		cout << "O ficheiro já existe deseja deseja continuar?" << '\n' << "!Atenção que se tencionar escrever o ficheiro será totalmente apagado primeiro" << endl;
		cout << "não -> 0" << '\n' << "sim -> 1" << endl;
		cin >> resp;
		return resp;
	}
	else{
		cout << "O ficheiro não existe deseja criar e editar?" << endl;
		cout << "não -> 0" << '\n' << "sim -> 1" << endl;
		cin >> resp;
		return resp;
	}


}

/*Class de Path importante para a leitura do trajeto */
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

void showTheContent(list<Path> trajecto)
{
     list<Path>::iterator it = trajecto.begin();
      while(it!=trajecto.end()){
    	  cout << it->number << "+" << it->type << "+" << it->x_f << "+" << it->y_f << endl;
    	  it++;
      }
}


void read_xml(string adr){

		CMarkup xml;
		list<Path> trajecto;
		xml.Load(adr);

		int number=0, x_f=0, y_f=0;
		string type ="", str_aux="";

		xml.ResetPos(); //Reposicionamento
		//xml.FindElem("Path");
		while(xml.FindElem("Path")){


			str_aux = xml.GetAttrib("number");
			number = stoi(str_aux);
			xml.IntoElem(); //Entra no Path


			xml.FindElem("type"); //Number type
			type = xml.GetData();
			//cout << type << endl;

			xml.FindElem("to_x"); //Number data
			str_aux = xml.GetData();
			x_f = stoi(str_aux);
			//cout << x_f << endl;

			xml.FindElem("to_y"); //Number data
			str_aux = xml.GetData();
			y_f = stoi(str_aux);
			//cout << y_f << endl;

			xml.OutOfElem();

			Path path(number, x_f, y_f, type);
			trajecto.push_back(path);
		}

		showTheContent(trajecto);

}

void write_xml(string adr){
	bool resp;
	string type;
	int x_f, y_f, number = 0;

	CMarkup xml;

	while(1){

		if(sub_state == 0){
			cout << "------XML writer------" << endl;
			cout << "1 -> Add Path" << endl;
			cout << "3 -> Quit" << endl;
			cin >> sub_state;

		}

		else if(sub_state == 1){


			cout << "Coordenadas finais X: ";
			cin >> x_f;
			cout << "Coordenadas finais Y: ";
			cin >> y_f;
			cout << "tipo de trajetoria: ";
			cin >> type;

			cout << type << ", " << "X = " << x_f << "Y = " << y_f << endl;
			cout << "1-confirmação" << '\n' << "0-cancelamento" << endl;
			cin >> resp;

			if(resp){
				sub_state = 2;
			}
			else{
				type.clear();
				x_f = 0;
				y_f = 0;
				sub_state = 0;
			}
		}

		else if(sub_state==2){

			xml.AddElem( "Path" );
			xml.SetAttrib( "number", number );
			xml.IntoElem();
			xml.AddElem( "type", type );
			xml.AddElem( "to_x", x_f );
			xml.AddElem( "to_y", y_f );
			xml.OutOfElem();
			/*acrescentar info para circle ou line*/
			xml.Save(adr);

			number++;
			sub_state = 0;
		}

		else if(sub_state == 3){
			state = 0;
			sub_state = 0;
			break;
		}

	}
}




int main(){

	string adr = "";
	while(1){
		if(state==0){

			adr.clear();
			cout << "------XML parser interface------" << endl;
			cout << "1 -> Write" << endl;
			cout << "2 -> Read" << endl;
			cout << "3 -> QUIT" << endl;
			cin >> state;
		}
		else if(state==1){

			adr = get_name();
			if(info_file(adr)){
				write_xml(adr);
			}
			else{
				adr.clear();
				state = 0;
			}
		}

		else if(state == 2){

			adr = get_name();
			if(info_file(adr)){
				read_xml(adr);
				state = 0;
			}
			else{
				state = 0;
			}
		}

		else if(state == 3){
			state = 0;
			break;
		}



	}

}




