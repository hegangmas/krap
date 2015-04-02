//Avtor:Uroš Golob
//Naloga je zahtevala:
//Napiši program za množenje in seštevanje dveh matrik z dvodimenzionalnimi polji

#include<iostream>
#include<stdlib.h>
#include<time.h>

# define Velikost_polja 50
using namespace std;
    class MATRIKE{
    	private:
    		float matrika1[Velikost_polja][Velikost_polja],matrika2[Velikost_polja][Velikost_polja];
    		float mZmnozek[Velikost_polja][Velikost_polja],mSestevek[Velikost_polja][Velikost_polja];
    		int vrstic1,stolpcev1,vrstic2,stolpcev2;
    	public:
    		MATRIKE(){}
    		void rocnoPrva();
			void samodejnoPrva();
    		void prikaziPrva();
    		void rocnoDruga();
			void samodejnoDruga();
    		void prikaziDruga();
    		void Zmnozek();
    		void prikaziZmnozek();
			void Sestevek ();
			void prikaziSestevek ();
			void Meni ();
};

void MATRIKE::prikaziPrva () 
{
    std:: cout<<"\nElementi prve matrike so: \n\n";
			for (int i=0; i<vrstic1; i++)
			{
				for (int j=0; j<stolpcev1; j++)
				{
                std::  cout<<"\t"<<matrika1[i][j]<<"\t";
           	}
            std::cout<<std::endl<<std::endl;
            }
}

void MATRIKE::prikaziDruga () 
{                            
	std::cout<<"\nElementi druge matrike so :\n\n";    
	
	for (int i=0; i<vrstic2; i++)
	{                                    				
		for (int j=0; j<stolpcev2; j++)
		{                                    					
			std::cout<<"\t"<<matrika2[i][j]<<"\t";                                    			
		}                                    	
		std::cout<<endl<<endl;                                    			
	}                             
}

void MATRIKE::rocnoPrva()
{
    	std::cout<<"\nVnesi stevilo vrstic prve matrike = ";
    	std::cin>>vrstic1;
    	std::cout<<"\nVnesi stevilo stolpcev prve matike = ";
    	std::cin>>stolpcev1;
    	std::cout<<"\nVnesi "<<vrstic1*stolpcev1<<" elementov prve matrike ;\n";


     for (int i=0; i<vrstic1; i++)
	 {
		for (int j=0; j<stolpcev1; j++)
		{
cout<<"Vnesi skalar, ki ima kordinate ["<<i+1<<","<<j+1<<"]\t:";
           std::cin>>matrika1[i][j];
        }
     }
}

void MATRIKE::samodejnoPrva()
{srand(time(NULL));
    	std::cout<<"\nVnesi stevilo vrstic prve matrike = ";
    	std::cin>>vrstic1;
    	std::cout<<"\nVnesi stevilo stolpcev prve matike = ";
    std::	cin>>stolpcev1;
cout<<"\t!!!SAMODEJNO VNASANJE SKALARJEV!!!\n";
     for (int i=0; i<vrstic1; i++)
	 {
		for (int j=0; j<stolpcev1; j++)
		{
           matrika1[i][j]=rand()%10+1;
        }
     }
}     
          
void MATRIKE::rocnoDruga()
{
    	std::cout<<"\nVnesi stevilo vrstic druge matrike = ";
       std:: cin>>vrstic2;
       std:: cout<<"\nVnesi stevilo stolpcev prve matike = ";
       std:: cin>>stolpcev2;
       std::cout<<"\nVnesi "<<vrstic2*stolpcev2<<" elementov druge matrike ;\n";
	for (int i=0; i<vrstic2; i++)
		{
			for (int j=0; j<stolpcev2; j++)
				{
cout<<"Vnesi skalar, ki ima kordinate ["<<i+1<<","<<j+1<<"]\t:";
                   	std::cin>>matrika2[i][j];
              	}
		}
}

void MATRIKE::samodejnoDruga()
{srand(time(NULL));
    	std::cout<<"\nVnesi stevilo vrstic druge matrike = ";
        std::cin>>vrstic2;
        std::cout<<"\nVnesi stevilo stolpcev prve matike = ";
        std::cin>>stolpcev2;
       std:: cout<<"\t!!!SAMODEJNO VNASANJE SKALARJEV!!!\n";
	for (int i=0; i<vrstic2; i++)
		{
			for (int j=0; j<stolpcev2; j++)
				{
                   	matrika2[i][j]=rand()%10+1;;
              	}
		}
}

void MATRIKE::Zmnozek () 
{
    if (stolpcev1==vrstic2)
	{
	for (int i=0; i<vrstic1; i++)
		{
		for (int j=0; j<stolpcev2; j++)
			{
			mZmnozek[i][j]=0;
				for (int k=0; k<vrstic2; k++)
					{
                       	mZmnozek[i][j]+=matrika1[i][k]*matrika2[k][j];
                    }
           	}
        }
     }
   	else std::cerr<<"\aStevilo stolpcev prve natrike ni enako stevilu vrstic druge (mnozenje teh dveh matrik ni definirano)\n";
}


void MATRIKE::Sestevek ()
{
	if (stolpcev1==stolpcev2 && vrstic1==vrstic2)
	{
		for (int i=0; i<vrstic1; i++)
		{
		for (int j=0; j<stolpcev1; j++)
			{
			mSestevek[i][j]=matrika1[i][j] + matrika2[i][j];
			}
		}
	}
	else std::cerr<<"\aMatriki nista enakih dimenzij (sestevanje ni definirano)\n";
}

void MATRIKE::prikaziZmnozek () 
{
	if (stolpcev1==vrstic2)
	{
   	std::cout<<"Zmnozek matrik je:\n\n";
	
	for (int i=0; i<vrstic1; i++) 
		{
		for (int j=0; j<stolpcev2; j++) 
			{
			std::cout<<"\t"<<mZmnozek[i][j]<<"\t";
			}
   	std::cout<<endl<<endl;
   		} 
	}
	
}

void MATRIKE::prikaziSestevek () 
{
	if (stolpcev1==stolpcev2 && vrstic1==vrstic2)
	{
   	std::cout<<"Sestevek matrik je:\n\n";
	
	for (int i=0; i<vrstic1; i++) 
		{
		for (int j=0; j<stolpcev2; j++) 
			{
			std::cout<<"\t"<<mSestevek[i][j]<<"\t";
			}
   	std::cout<<std::endl<<std::endl;
   		} 
	}
	
}

void MATRIKE::Meni()
{	std::cout<<"***********************************************\n";
	std::cout<<"*\t1. ROCNI VNOS MATRIK                  *\n";
	std::cout<<"*\t2. SAMODEJNI VNOS MATRIK              *\n";
	std::cout<<"*\t3. IZBRIS ZASLONA POVNI IZPIS MENIJA  *\n";
	std::cout<<"***********************************************\n";
	std::cout<<"*\t0. IZHOD IZ PROGRAMA                  *\n";	
	std::cout<<"***********************************************\n\n";
}

int main () 
{                                                              
 MATRIKE mm;
 int izbira;

do {
	mm.Meni();
std::cin>> izbira;
switch (izbira)
{
		case 1:
	{
		mm.rocnoPrva();
		mm.rocnoDruga();
		mm.prikaziPrva();
		mm.prikaziDruga();		
		mm.Zmnozek ();
		mm.prikaziZmnozek();
		mm.Sestevek ();
		mm.prikaziSestevek ();
		break;
	}

		case 2:
	{
		mm.samodejnoPrva();	
		mm.samodejnoDruga();		
		mm.prikaziPrva();
		mm.prikaziDruga();
		mm.Zmnozek ();
		mm.prikaziZmnozek();
		mm.Sestevek ();
		mm.prikaziSestevek () ;
		break;
	}

		case 3:{system("cls");break;}
}
		
}while (izbira!=0);
return 0;
}
