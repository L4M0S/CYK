#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::set<int> conjunto;
typedef std::map<int,conjunto> fila;
typedef std::map<int, fila> Tabla;

typedef enum{S=83,R=82,A=65,B=66,C=67,D=68} generadores; //Simbolos generadores  /*simbolo=valor ASCII*/ 
typedef enum{l=108,x=120,c=99,t=116} terminales; //Simbolos terminales  /*simbolo=valor ASCII*/	

Tabla tabla;

class Regla
{
	public:
		int generador;
		int primero;
		int segundo;
};

typedef std::vector<Regla> Gramatica;
Gramatica gramatica;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Regla crearRegla(int a, int b, int c=0) /*(generador, primero, segundo) si no hay segundo se omite y solo se colocan generador y primero*/
{
	Regla regla;
	
	regla.generador=a;
	regla.primero=b;
	regla.segundo=c;

	return regla;
}

void inicializarGramatica() 
{						   	   
	gramatica.push_back(crearRegla(S,A,C));// S->AC
	gramatica.push_back(crearRegla(C,S,R));// C->SR
	gramatica.push_back(crearRegla(R,B,D));// R->BD
	gramatica.push_back(crearRegla(D,S,R));// D->SR
	gramatica.push_back(crearRegla(A,l));// A->l
	gramatica.push_back(crearRegla(S,x));// S->x
	gramatica.push_back(crearRegla(B,c));// B->c
	gramatica.push_back(crearRegla(R,t));// R->t
}

void imprimirGramatica(Gramatica &s) //Imprime la gramatica
{
	Gramatica::iterator itr;
	for (itr=s.begin(); itr!=s.end(); ++itr)
	{
		printf("%c -> %c%c \n", itr->generador, itr->primero, itr->segundo);
	}
}

bool esMiembro(conjunto &s, int a) //Retorna true si a pertenece al conjunto s
{
	return s.find(a)!=s.end();
}

void insertarEnConjunto(conjunto &s, int a) //inserta a en el conjunto s
{
	for (int i=0; i<gramatica.size(); i++)
	{
		if (gramatica[i].primero==a || gramatica[i].segundo==a) 
		{
			s.insert(gramatica[i].generador);
		}
	}
}

void insertarEnConjunto(conjunto &s, int a, int b) //inserta a y b en el conjunto s
{
	for (int i=0; i<gramatica.size(); i++)
	{
		if (gramatica[i].primero==a && gramatica[i].segundo==b) 
		{
			s.insert(gramatica[i].generador);
		}
	}
}

void unirConjuntos(conjunto &s, conjunto &s1,conjunto &s2) //une el conjunto s con los conjuntos s1 y s2 
{
	conjunto::iterator itr1;
	conjunto::iterator itr2;
	
	for (itr1=s1.begin(); itr1!=s1.end(); itr1++)
	{
		for (itr2=s2.begin(); itr2!=s2.end(); itr2++)
		{
			insertarEnConjunto(s, *itr1, *itr2);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool cyk(Gramatica &g, string &w) 
{
	int n=w.length();

	for (int i=1; i<=n; i++)
	{
		insertarEnConjunto(tabla[i][1],w[i-1]);
	}
	
	for (int j=2; j<=n; j++)
	{
		for (int i=1; i<=n-j+1; i++)
		{
			for (int k=1; k<=j-1; k++)
			{
				unirConjuntos(tabla[i][j], tabla[i][k], tabla[i + k][j - k]);
			}
		}
	}

	if (esMiembro(tabla[1][n], S)) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	inicializarGramatica();
	imprimirGramatica(gramatica);
	string w;	//Cadena a evaluar
	cin>>w;
		
	if (cyk(gramatica, w))
	{
		cout<<"aceptada"<<endl;
	}
	else
	{
		cout<<"rechazada"<<endl;
	}

	system("PAUSE");
	return 0;
}
