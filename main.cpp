#include <iostream>
#include <string>
#include <cstdio>
#include <set>
#include <map>
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::set<int> Casilla;	//conjunto de simbolos generadores de la casilla
typedef std::map<int,Casilla> Fila;	
typedef std::map<int, Fila> Tabla;

typedef enum{S=83,R=82,A=65,B=66,C=67,D=68} Generadores; //Simbolos generadores  /*simbolo=valor ASCII*/ 
typedef enum{l=108,x=120,c=99,t=116} Terminales; //Simbolos terminales  /*simbolo=valor ASCII*/	

Generadores generdores;
Terminales terminales;

class Proyeccion
{
	public:
		int generador;
		int primero;
		int segundo;
};

typedef std::vector<Proyeccion> Gramatica;

Tabla tabla;

Gramatica gramatica;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Proyeccion proyeccion(int a, int b, int c=0) //(generador, primero, segundo) si no hay segundo se omite y solo se colocan generador y primero
{
	Proyeccion proyeccion;
	
	proyeccion.generador=a;
	proyeccion.primero=b;
	proyeccion.segundo=c;

	return proyeccion;
}

void definirGramatica()
{						   	   
	gramatica.push_back(proyeccion(S,A,C));	// S->AC
	gramatica.push_back(proyeccion(C,S,R));	// C->SR
	gramatica.push_back(proyeccion(R,B,D));	// R->BD
	gramatica.push_back(proyeccion(D,S,R));	// D->SR
	gramatica.push_back(proyeccion(A,l));	// A->l
	gramatica.push_back(proyeccion(S,x));	// S->x
	gramatica.push_back(proyeccion(B,c));	// B->c
	gramatica.push_back(proyeccion(R,t));	// R->t
}

void imprimirGramatica(Gramatica &g) //Imprime la gramatica
{
	Gramatica::iterator itr;
	for(itr=g.begin(); itr!=g.end(); ++itr)
	{
		printf("%c->%c%c\n", itr->generador, itr->primero, itr->segundo);
	}
}

void reiniciar()	//limpia el contenido de la tabla
{
	for(int i=0;i<tabla.size();i++)
	{
		for(int j=0;j<tabla.size(); j++)
		{
			tabla[i][j].clear();
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insertarConjunto(Casilla &c, int a) //inserta el generador de a en el conjunto c // a es terminal //primer columna
{
	for(int i=0; i<gramatica.size(); i++)
	{
		if(gramatica[i].primero==a)
		{
			//printf("gen: %c \n",gramatica[i].generador);
			c.insert(gramatica[i].generador);
		}
	}
	//cout<<"++++++"<<endl;
}

void insertarConjunto(Casilla &c, int a, int b) //inserta el generador de a y b en el conjunto c // a y b son generadores
{
	for(int i=0; i<gramatica.size(); i++)
	{
		if(gramatica[i].primero==a && gramatica[i].segundo==b) 
		{
			//printf("gen: %c %i \n",gramatica[i].generador, i);
			c.insert(gramatica[i].generador);
		}
	}
	//cout<<"++++++"<<endl;
}

void unionConjuntos(Casilla &c, Casilla &c1, Casilla &c2) //une c con los generadores que deriven a la union compuesta por los generadores de c1 y c2 
{
	Casilla::iterator itr1;
	Casilla::iterator itr2;
	
	for(itr1=c1.begin(); itr1!=c1.end(); itr1++)
	{
		for(itr2=c2.begin(); itr2!=c2.end(); itr2++)
		{
			//printf("pri: %c seg: %c \n", *itr1, *itr2);
			insertarConjunto(c, *itr1, *itr2);
		}
	}
	//cout<<"-------"<<endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool cyk(Gramatica &g, string &w) 
{
	int n=w.length();

	for(int i=1; i<=n; i++) //primer columna 
	{
		insertarConjunto(tabla[i][1],w[i-1]);
	}
	
	for(int j=2; j<=n; j++)
	{
		for(int i=1; i<=n-j+1; i++)
		{
			for(int k=1; k<=j-1; k++)
			{
				unionConjuntos(tabla[i][j], tabla[i][k], tabla[i + k][j - k]);
			}
		}
	}

	return tabla[1][n].find(S)!=tabla[1][n].end();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	definirGramatica();
	imprimirGramatica(gramatica);
	
	string w;	//Cadena a evaluar
	
	bool loop=true;
	
	do
	{
		cout<<"Cadena: ";cin>>w;
			
		if(cyk(gramatica, w))
		{
			cout<<"aceptada"<<endl;
		}
		else
		{
			cout<<"rechazada"<<endl;
		}
		
		cout<<endl<<"Repetir(1/0): ";cin>>loop;cout<<endl;
		reiniciar();
	}while(loop);
	
	cout<<endl;
	system("PAUSE");
	return 0;
}
