#ifndef MNEMONICO_H_INCLUDED
#define MNEMONICO_H_INCLUDED
#include<bits/stdc++.h>

using namespace std;

long long int potencia(int base,int pot){
    long long int res = 1;
    for(int i = 0; i < pot;++i)
        res*=base;
    return res;
}

class Mnemonico{
    string instruccion;
    vector<string> modo_dir;
    int long_instruccion;

public:
    Mnemonico();
    Mnemonico(string instruccion, vector<string> modos);
    Mnemonico(string instruccion, vector<string> modos, int li);
    string getNombre();
    vector<string> getModos();
    void Add_MD(vector<string> modos);
    void Add_VMD(string modo);
    //utilidades para los modos de direccionamiento
    bool existeMD(string MD);       //la instruccion puede esar este modo de direccionamiento?
    int is8or16(int valor);         //si es un entero de 8 bits regresa 1, si es de 16 bit regresa 2, sino -1
    int htod(string valor);         //convierte de hexa a decimal
    int sdtod(string valor);        //convirte de stringdecimal a decimal
    int otod(string valor);         //convierte de octal a decimal
    int btod(string valor);         //convierte de binario a decimal
    //validar los modos de dir
    bool esINH(string operando);
    bool esIMM(string operando);
    bool esDIR(string operando);
    bool esEXT(string operando);

};

Mnemonico::Mnemonico(){};
Mnemonico::Mnemonico(string instruccion, vector<string> modos){
    this->instruccion = instruccion;
    this->modo_dir = modos;
}
Mnemonico::Mnemonico(string instruccion, vector<string> modos, int li){
    this->instruccion = instruccion;
    this->modo_dir = modos;
    this->long_instruccion = li;
}
string Mnemonico:: getNombre(){
    return this->instruccion;
}
vector<string> Mnemonico:: getModos(){
    return modo_dir;
}

void Mnemonico:: Add_MD(vector<string> modos){
    this->modo_dir = modos;
}
void Mnemonico:: Add_VMD(string modo){
    this->modo_dir.push_back(modo);
}
//*************************************************************

bool Mnemonico:: existeMD(string MD){
    for(auto modo : this->modo_dir){
        if(modo == MD)
            return true;
    }
    return false;
}

int Mnemonico:: is8or16(int valor){
    if(valor <= 255 && valor >= 0)
        return 1;
    else if(valor <= 65535 && valor >= 0)
        return 2;
    return -1;
}

int Mnemonico:: htod(string valor){
    reverse(valor.begin(), valor.end());
    int val = 0;
    char c;
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '9')
            val+=(c-'0')*(pow(16, i));
        else if(c >= 'a' && c <= 'f')
            val+=(10+ c-'0')*(pow(16, i));
        else return 1e7;
    }
    return val;
}

int Mnemonico:: sdtod(string valor){
    reverse(valor.begin(), valor.end());
    int val = 0, pot;
    for(unsigned int i = 0; i < valor.size();++i){
        if(valor[i] >= '0' && valor[i] <= '9'){
            pot = potencia(10, i);
            val+=(valor[i]-'0')*pot;
        }
        else return 1e7;
    }
    return val;
}

int Mnemonico:: otod(string valor){
    reverse(valor.begin(), valor.end());
    int val = 0;
    char c;
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '8')
            val+=(c-'0')*(pow(8, i));
        else return 1e7;
    }
    return val;
}
int Mnemonico:: btod(string valor){
    reverse(valor.begin(), valor.end());
    int val = 0;
    char c;
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '1')
            val+=(c-'0')*(pow(2, i));
        else return 1e7;
    }
    return val;
}
//******************************************************************
bool Mnemonico:: esINH(string operando){
    /*en inherente no es necesario ningun operando
      por lo que el operando debe estar vacio*/
    if(existeMD("INH")){
        for(auto caracter : operando){
            if( caracter != ' ')        //si hay algo que no sea un espacio, no puede ser inh
                return false;
        }
        return true;
    }
    return false;
}

bool Mnemonico:: esIMM(string operando){
    if(existeMD("IMM")){
        if(operando[0] == '#')
        {
            operando.erase(0, 1);
            char c = operando[0];
            if(c >= '0' && c <= '9'){//es decimal
                if(is8or16(sdtod(operando)) == 1)        //si es de 8 bits return true
                    return true;
                return false;
            }
            operando.erase(0, 1);
            if(c == '#'){                //es hexa
                if(is8or16(htod(operando)) == 1)        //si es de 8 bits return true
                    return true;
                return false;
            }
            else if(c == '@'){                //es oct
                if(is8or16(otod(operando)) == 1)        //si es de 8 bits return true
                    return true;
                return false;
            }
            else if(c == '%'){                //es binario
                if(is8or16(btod(operando)) == 1)        //si es de 8 bits return true
                    return true;
                return false;
            }
        }
    }
    return false;
}
bool Mnemonico:: esDIR(string operando){
    if(existeMD("DIR")){
        char c = operando[0];
        if(c >= '0' && c <= '9'){//es decimal
            if(is8or16(sdtod(operando)) == 1)        //si es de 8 bits return true
                return true;
            return false;
        }
        operando.erase(0, 1);
        if(c == '#'){                //es hexa
            if(is8or16(htod(operando)) == 1)        //si es de 8 bits return true
                return true;
            return false;
        }
        else if(c == '@'){                //es oct
            if(is8or16(otod(operando)) == 1)        //si es de 8 bits return true
                return true;
            return false;
        }
        else if(c == '%'){                //es binario
            if(is8or16(btod(operando)) == 1)        //si es de 8 bits return true
                return true;
            return false;
        }
    }
    return false;
}
bool Mnemonico:: esEXT(string operando){
    if(existeMD("EXT")){
        char c = operando[0];
        if(c >= '0' && c <= '9'){//es decimal
            if(is8or16(sdtod(operando)) != -1)        //si es de 8 bits return true
                return true;
            return false;
        }
        operando.erase(0, 1);
        if(c == '#'){                //es hexa
            if(is8or16(htod(operando)) != -1)        //si es de 8 bits return true
                return true;
            return false;
        }
        else if(c == '@'){                //es oct
            if(is8or16(otod(operando)) != -1)        //si es de 8 bits return true
                return true;
            return false;
        }
        else if(c == '%'){                //es binario
            if(is8or16(btod(operando)) != -1)        //si es de 8 bits return true
                return true;
            return false;
        }
    }
    return false;
}






#endif // MNEMONICO_H_INCLUDED
