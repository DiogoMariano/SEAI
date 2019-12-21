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
#include <math.h>
#include <SDL2/SDL.h>


using namespace std;

//The window we'll be rendering to
	SDL_Window* window = NULL;

//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	SDL_Renderer* renderer = NULL;

static string ADR_XML =  "/home/mariano/Documents/SEAI/SEAI/SEAI-EquipaB/XML_SCRIPTS/";//Endereço fixo dos cripts de xml

int state = 0, sub_state=0;

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


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
		return false;
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

class Path{
	private:
		Node_xml *start, *stop;
		Link_xml *link_of_nodes;

	public:
		Path(Node_xml *start, Node_xml *stop, Link_xml *link_of_nodes){
			this->start = start;
			this->stop = stop;
			this->link_of_nodes = link_of_nodes;
		}

		double getUStart(){
			return this->start->Getu();
		}

		double getVStart(){
			return this->start->Getv();
		}

		double getAngStart(){
			return this->start->Getangle();
		}

		double getUStop(){
			return this->stop->Getu();
		}

		double getVStop(){
			return this->stop->Getv();
		}

		double getAngStop(){
			return this->stop->Getangle();
		}

		double getC1U(){
			return this->link_of_nodes->getC1U();
		}

		double getC1V(){
			return this->link_of_nodes->getC1V();
		}

		double getC2U(){
			return this->link_of_nodes->getC2U();
		}

		double getC2V(){
			return this->link_of_nodes->getC2V();
		}

};

void writelist(list<Path> trajectory){
	for(Path & path : trajectory){
			cout<<path.getUStart()<<endl;
			cout<<path.getC1U()<<endl;
		}

}

void render_xml(list<Path> trajectory){

	double t = 0.0, x = 0.0, y = 0.0;

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
		/*Initialization of SDL*/
		if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) == 0){
			SDL_bool done = SDL_FALSE;

			while(!done){
				SDL_Event event;

				/*set background color to black*/
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);

				/*set draw color to white*/
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

				for(Path & path : trajectory){

					for(t = 0.0; t <= 1.0; t += 0.0001) {

						x = pow(1-t,3)*path.getUStart() + 3*t*pow(1-t,2)*path.getC1U() + 3*pow(t,2)*(1-t)*path.getC2U() + pow(t,3)*path.getUStop();
						y = pow(1-t,3)*path.getVStart() + 3*t*pow(1-t,2)*path.getC1V() + 3*pow(t,2)*(1-t)*path.getC2V() + pow(t,3)*path.getVStop();
						SDL_RenderDrawPoint(renderer , (int)x , (int)y) ;
					}
					/*Red Line between control Point P0 & P1*/
					//SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
					//SDL_RenderDrawLine(renderer , path.getUStart() , path.getVStart() , path.getC1U(), path.getC1V()) ;


					/*Line between control Point P1 & P2*/
					//SDL_RenderDrawLine(renderer , path.getC1U(), path.getC1V(), path.getC2U(), path.getC2V()) ;


					/*Line between control Point P2 & P3*/
					//SDL_RenderDrawLine(renderer , path.getC2U(), path.getC2V() , path.getUStop() , path.getVStop()) ;
				}

				if (SDL_PollEvent(&event)) {
					/* if window cross button clicked then quit from window */
					if (event.type == SDL_QUIT) {
						done = SDL_TRUE;
					}
				}

				/*show the window*/
				SDL_RenderPresent(renderer);

			}
		}
			/*Destroy the renderer and window*/
			if (renderer) {
				SDL_DestroyRenderer(renderer);
			}
			if (window) {
				SDL_DestroyWindow(window);
			}
	}

	/*clean up SDL*/
	SDL_Quit();
}


void read_xml(string adr){

		CMarkup xml;

		list<Node_xml> Nodes;
		list<Link_xml> Links;

		list<Path> trajectory;

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

		for ( Link_xml & link : Links) {
		         for ( Node_xml & node1 : Nodes) {
		        	 if(!((link.GetStart()).compare(node1.Getid()))){
		        		 for( Node_xml & node2: Nodes){
		        			 if(!(link.GetStop().compare(node2.Getid()))){
		        				 Path path(&node1, &node2, &link);
		        				 trajectory.push_back(path);
		        			 }
		        		 }
		        	 }
		         }
		      }


		render_xml(trajectory);
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

			/*if(trajectory.size()!=0){
				render_xml(trajectory);
				writelist(trajectory);
				state = 0;
			}
			else{
				state = 0;
			}*/
		}

		else if(state == 3){
			state = 0;
			break;
		}



	}

}




