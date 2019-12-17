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
		cout << "O ficheiro existe. Deseja continuar?" << '\n' << endl;
		cout << "não -> 0" << '\n' << "sim -> 1" << endl;
		cin >> resp;
		return resp;
	}
	else{
		cout << "O ficheiro não existe" << endl;
	}

	return resp;

}

/*Class de Node_xml importante para a leitura do trajeto */
class Node_xml{
	private:
		string id; //id do Node_xml
		double u, v, angle; //coordenadas do Node_xml e o angulo

	public:
		Node_xml(string id, double u, double v, double angle){
			this->id=id;
			this->u=u;
			this->v=v;
			this->angle=angle;
		}

		string Getid(){
			return this->id;
		}
		double Getu(){
			return this->u;
		}
		double Getv(){
			return this->v;
		}
		double Getangle(){
			return this->angle;
		}
};

class Link_xml{
	private:
		string n_start, n_stop;
		double c1_u, c1_v;
		double c2_u, c2_v;

	public:
		Link_xml(string n_start, string n_stop, double c1_u, double c1_v, double c2_u, double c2_v){
			this->n_start=n_start;
			this->n_stop=n_stop;
			this->c1_u=c1_u;
			this->c1_v=c1_v;
			this->c2_u=c2_u;
			this->c2_v=c2_v;
		}

		string GetStart(){
			return this->n_start;
		}
		string GetStop(){
			return this->n_stop;
		}
		double getC1U(){
			return this->c1_u;
		}
		double getC1V(){
			return this->c1_v;
		}
		double getC2U(){
			return this->c2_u;
		}
		double getC2V(){
			return this->c2_v;
		}
};

void writelist(list<Node_xml> Nodes, list<Link_xml> Links){
	for(Node_xml & node : Nodes){
		cout<<node.Getid()<<endl;
	}
	for(Link_xml & link : Links){
		cout<<link.GetStart()<<endl;
	}
}


void OrganizeTrajectory(list<Node_xml> Nodes, list<Link_xml> Links)
{
	//list<Path> trajectory;

     for ( Link_xml & Link_xml : Links) {
         for ( Node_xml & node1 : Nodes) {
        	 if(!((Link_xml.GetStart()).compare(node1.Getid()))){
        		 for( Node_xml & node2: Nodes){
        			 if(!(Link_xml.GetStop().compare(node2.Getid()))){
        				 cout << node1.Getangle();
        			 }
        		 }
        	 }
         }
      }

      /*while(it!=Links.end()){
    	  while(it2!=Nodes.end()){
    		  if(!(it->GetStart().compare(it2->Getid()))){
    			  while(it3!=Nodes.end()){
    				  if(!(it->GetStop().compare(it2->Getid()))){
    					  Path path(*it2, *it3, *it);
    					  trajectory.push_back(path);
    				  }
    				  it3++;
    			  }
    		  }
    		  it2++;
    	  }
    	  it++;
      }*/

     /* list<Path>::iterator it4 = trajectory.begin();
      while(it4!=trajectory.end()){
    	  cout << it4->GetStart() << " " << it4->GetStop() << endl;
    	  it4++;
      }*/
}


void read_xml(string adr){

		CMarkup xml;

		list<Node_xml> Nodes;
		list<Link_xml> Links;

		xml.Load(adr);

		double u=0, v=0, angle=0, c1_u = 0, c1_v = 0, c2_u = 0, c2_v = 0 ;
		string name = "", str_aux = "", id = "", n_start = "", n_stop = "";

		xml.ResetPos(); //Reposicionamento
		//xml.FindElem("Path");
		while(xml.FindElem("graph")){
			xml.IntoElem(); //Entrar no graph
			name = xml.GetAttrib("name");
			cout<<name<<endl;

			xml.FindElem("nodes"); //find the element of the nodes
			xml.IntoElem(); //Enter in the Node_xml department
				while(xml.FindElem("node")){

					id = xml.GetAttrib("id"); //tirar o id
					//cout << id <<endl;


					str_aux = xml.GetAttrib("u"); //tirar a coordenada u
					u = stod(str_aux);
					//cout<<u<<endl;

					str_aux = xml.GetAttrib("v"); //tirar a coordenada v
					v = stod(str_aux);
					//cout<<v<<endl;

					str_aux = xml.GetAttrib("angle"); //tirar a coordenada u
					angle = stod(str_aux);
					//cout<<angle<<endl;

					Node_xml Node_xml(id, u, v, angle);
					Nodes.push_back(Node_xml);
				}
			xml.OutOfElem(); //exist Node_xml department

			xml.FindElem("links"); //find the element of the links
			xml.IntoElem(); //Enter in the Link_xml department
				while(xml.FindElem("link")){

					n_start = xml.GetAttrib("node_start"); //tirar o Node_xml de partida
					//cout<<n_start<<endl;

					n_stop = xml.GetAttrib("node_stop"); //tirar o Node_xml de chegada
					//cout<<n_stop<<endl;

					str_aux = xml.GetAttrib("c1_u");
					c1_u = stod(str_aux);
					//cout<<c1_u<<endl;

					str_aux = xml.GetAttrib("c1_v");
					c1_v = stod(str_aux);
					//cout<<c1_v<<endl;

					str_aux = xml.GetAttrib("c2_u");
					c2_u = stod(str_aux);
					//cout<<c2_u<<endl;

					str_aux = xml.GetAttrib("c2_v");
					c2_v = stod(str_aux);
					//cout<<c2_v<<endl;

					Link_xml Link_xml(n_start, n_stop, c1_u, c1_v, c2_u, c2_v);
					Links.push_back(Link_xml);
				}
			xml.OutOfElem(); //exist links department
		}

		xml.OutOfElem(); //exist links department

	writelist(Nodes, Links);

}

void render_xml(string adr){

}



int main(){

	string adr = "";
	while(1){
		if(state==0){

			adr.clear();
			cout << "------XML parser interface------" << endl;
			cout << "1 -> Read" << endl;
			cout << "2 -> Render" << endl;
			cout << "3 -> QUIT" << endl;
			cin >> state;
		}
		else if(state==1){

			adr = get_name(); //Get the address of the file
			if(info_file(adr)){
				read_xml(adr);
				state = 0;
			}
			else{
				adr.clear();
				state = 0;
			}
		}

		else if(state == 2){

			adr = get_name();
			if(info_file(adr)){
				render_xml(adr);
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




