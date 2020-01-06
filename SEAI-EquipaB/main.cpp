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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/constants/constants.hpp>
#include <fstream>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>


using boost::multiprecision::cpp_dec_float_50;
using namespace boost::numeric::ublas;
using namespace std;


//The window we'll be rendering to
	SDL_Window* window = NULL;

//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	SDL_Renderer* renderer = NULL;

static string ADR_XML =  "/home/mariano/Documents/SEAI/SEAI/SEAI-EquipaB/XML_SCRIPTS/";//Endereço fixo dos cripts de xml
const char *path = "/home/mariano/Documents/SEAI/SEAI/SEAI-EquipaB/Coordenadas";

int state = 0, sub_state=0;

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const double Init_OrigX = 561.699000886475;
const double Init_OrigY = 335.436643180684;

const double RX = 0.03721763077543844890923335868867;
const double RY = 0.037494504574287688152908912010387;

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
		Node_xml(){

		}
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

		void operator =(Node_xml &node){
			this->id = node.id;
			this->u = node.u;
			this->v = node.v;
			this->angle = node.angle;
		}
};

class Link_xml{
	private:
		string n_start, n_stop;
		double c1_u, c1_v;
		double c2_u, c2_v;

	public:
		Link_xml(){

		}
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

		void operator =(Link_xml &link){
			this->n_start = link.n_start;
			this->n_stop = link.n_stop;
			this->c1_u = link.c1_u;
			this->c1_v = link.c1_v;
			this->c2_u = link.c2_u;
			this->c2_v = link.c2_v;
		}
};

class Path{

	private:
		Node_xml start, stop;
		Link_xml link_of_nodes;

	public:
		Path(Node_xml start, Node_xml stop, Link_xml link_of_nodes){
			this->start = start;
			this->stop = stop;
			this->link_of_nodes = link_of_nodes;
		}

		double getUStart(){
			return this->start.Getu();
		}
		double getVStart(){
			return this->start.Getv();
		}
		double getUStop(){
			return this->stop.Getu();
		}
		double getVStop(){
			return this->stop.Getv();
		}
		double getC1U(){
			return this->link_of_nodes.getC1U();
		}
		double getC1V(){
			return this->link_of_nodes.getC1V();
		}
		double getC2U(){
			return this->link_of_nodes.getC2U();
		}
		double getC2V(){
			return this->link_of_nodes.getC2V();
		}

};

class Ponto{

	private:
		double x, y;
	public:
		Ponto(double x, double y){
			this->x = x;
			this->y = y;
		}

		double getX(){
			return x;
		}

		double getY(){
			return y;
		}
};


void funcValuesSimtow(list<Path> *trajectory, list<Ponto> *trajectoryXY){

	double x=0.0, y=0.0, t=0.0;

	matrix<double> Init_frame(3,1);
	Init_frame(2,0) = 0;

	matrix<double> R(3,3);
			R(0,0) = 0;
			R(0,1) = -1;
			R(0,2) = 0;
			R(1,0) = -1;
			R(1,1) = 0;
			R(1,2) = 0;
			R(2,0) = 0;
			R(2,1) = 0;
			R(2,2) = -1;


	matrix<double> result(3,1);

	for(Path & path : *trajectory){

		for(t = 0.0; t <= 1.0; t += 0.1) {

			x = pow(1-t,3)*path.getUStart()+ 3*t*pow(1-t,2)*path.getC1U() + 3*pow(t,2)*(1-t)*path.getC2U() + pow(t,3)*path.getUStop();
			y = pow(1-t,3)*path.getVStart() + 3*t*pow(1-t,2)*path.getC1V() + 3*pow(t,2)*(1-t)*path.getC2V() + pow(t,3)*path.getVStop();

			Init_frame(0,0) = x - Init_OrigX;
			Init_frame(1,0) = y - Init_OrigY;

			result = prod(R, Init_frame);

			//x_coordinate << result(0,0) * RX ;
			//y_coordinate << result(1,0) * RY ;

			Ponto ponto(result(0,0) * RX, result(1,0) * RY);
			trajectoryXY->push_back(ponto);

			//x_coordinate << result(0,0) * RX << " ";
			//y_coordinate << result(1,0) * RY << " ";

		}
	}


	fstream x_coordinates, y_coordinates;

	x_coordinates.open("/home/mariano/Documents/SEAI/SEAI/SEAI-EquipaB/Coordenadas/x_coordinates.txt", ios::out);

	y_coordinates.open("/home/mariano/Documents/SEAI/SEAI/SEAI-EquipaB/Coordenadas/y_coordinates.txt", ios::out);

	for(Ponto & ponto : *trajectoryXY){
		x_coordinates << ponto.getX() << '\n';
		y_coordinates << ponto.getY() << '\n';
	}

	x_coordinates.close();
	y_coordinates.close();

}


void writelist(list<Ponto> trajectory){
	for(Ponto & ponto : trajectory){
			cout<<ponto.getX()<<endl;
			cout<<ponto.getY()<<endl;
			cout<<'\n'<<endl;
		}

}
void writelist(list<Path> trajectory){
	for(Path & path : trajectory){
			cout<<path.getUStart()<<endl;
			cout<<path.getVStart()<<endl;
			cout<<'\n'<<endl;
		}

}

void render_xml(list<Path> *trajectory){

	double t = 0.0, x = 0.0, y = 0.0;

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
		/*Initialization of SDL*/
		if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) == 0){
			SDL_bool done = SDL_FALSE;

			while(!done){
				SDL_Event event;

				list<Path> aux = *trajectory;


				/*set background color to black*/
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);

				/*set draw color to white*/
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

				for(Path & path : *trajectory){

					for(t = 0.0; t <= 1.0; t += 0.0001) {
						x = pow(1-t,3)*path.getUStart()+ 3*t*pow(1-t,2)*path.getC1U() + 3*pow(t,2)*(1-t)*path.getC2U() + pow(t,3)*path.getUStop();
						y = pow(1-t,3)*path.getVStart() + 3*t*pow(1-t,2)*path.getC1V() + 3*pow(t,2)*(1-t)*path.getC2V() + pow(t,3)*path.getVStop();
						SDL_RenderDrawPoint(renderer , (int)x , (int)y) ;

					}

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


void read_xml(string adr, list<Path> *trajectory){

		CMarkup xml;

		list<Node_xml> Nodes;
		list<Link_xml> Links;

		xml.Load(adr);

		double u=0, v=0, angle=0, c1_u = 0, c1_v = 0, c2_u = 0, c2_v = 0;
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

					str_aux = xml.GetAttrib("u"); //tirar a coordenada u
					u = stod(str_aux);

					str_aux = xml.GetAttrib("v"); //tirar a coordenada v
					v = stod(str_aux);

					str_aux = xml.GetAttrib("angle"); //tirar a coordenada u
					angle = stod(str_aux);

					Node_xml Node_xml(id, u, v, angle);
					Nodes.push_back(Node_xml);
				}
			xml.OutOfElem(); //exist Node_xml department

			xml.FindElem("links"); //find the element of the links
			xml.IntoElem(); //Enter in the Link_xml department
				while(xml.FindElem("link")){

					n_start = xml.GetAttrib("node_start"); //tirar o Node_xml de partida

					n_stop = xml.GetAttrib("node_stop"); //tirar o Node_xml de chegada

					str_aux = xml.GetAttrib("c1_u");
					c1_u = stod(str_aux);

					str_aux = xml.GetAttrib("c1_v");
					c1_v = stod(str_aux);

					str_aux = xml.GetAttrib("c2_u");
					c2_u = stod(str_aux);

					str_aux = xml.GetAttrib("c2_v");
					c2_v = stod(str_aux);

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
		        				 Path path(node1, node2, link);
		        				 trajectory->push_back(path);
		        			 }
		        		 }
		        	 }
		         }
		      }

}




int main(){

	string adr = "";
	list<Path> trajectory;
	list<Ponto> trajectoryXY;


	while(1){
		if(state==0){

			//writelist(trajectory);
			adr.clear();
			cout << "------XML parser interface------" << endl;
			cout << "1 -> Read" << endl;
			cout << "2 -> Render" << endl;
			cout << "3 -> Inicializar" << endl;
			cout << "4 -> SimTwo Values" << endl;
			cout << "5 -> Quit" << endl;
			cin >> state;
		}
		else if(state==1){

			adr = get_name(); //Get the address of the file
			if(info_file(adr)){
				read_xml(adr, &trajectory);
				writelist(trajectory);
				state = 0;
			}
			else{
				adr.clear();
				state = 0;
			}
		}

		else if(state == 2){
			if(trajectory.size()!=0){
				render_xml(&trajectory);
				state = 0;
			}
			else{
				state = 0;
			}
		}

		else if(state == 3){
			trajectory.clear();
			trajectoryXY.clear();
			adr.clear();
			state = 0;
		}

		else if(state == 4){
			if(trajectory.size()!=0){
				funcValuesSimtow(&trajectory, &trajectoryXY);
				writelist(trajectoryXY);
				state = 0;
			}
			else
				state = 0;
		}

		else if(state == 5){
			state = 0;
			break;
		}

	}

}




