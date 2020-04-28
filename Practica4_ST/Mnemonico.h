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

struct ModoDir{
    string nombre, cop;
    int li;
    ModoDir(){};
    ModoDir(string a, string b, int c){
        nombre = a;
        cop = b;
        li = c;
    }
    void setNombre(string a){
        nombre = a;
    }
    void setCop(string a){
        cop = a;
    }
    void setli(int a){
        li = a;
    }

};

class Mnemonico{
    string instruccion;
    vector<ModoDir> modo_dir;

public:
    Mnemonico();
    Mnemonico(string nombre);
    Mnemonico(string instruccion, vector<ModoDir> modos);
    string getNombre();
    vector<ModoDir> getModos();
    void Add_MD(vector<ModoDir> modo_dir);
    void Add_VMD(ModoDir modo);
    //utilidades para los modos de direccionamiento
    ModoDir existeMD(string MD);       //la instruccion puede esar este modo de direccionamiento?
    int is8or16(int valor);         //si es un entero de 8 bits regresa 1, si es de 16 bit regresa 2, sino -1
    int htod(string valor);         //convierte de hexa a decimal
    int sdtod(string valor);        //convirte de stringdecimal a decimal
    int otod(string valor);         //convierte de octal a decimal
    int btod(string valor);         //convierte de binario a decimal
    //validar los modos de dir
    ModoDir esINH(string operando);
    pair<ModoDir, int> esIMM(string operando);
    ModoDir esDIR(string operando);
    ModoDir esEXT(string operando);

};

Mnemonico::Mnemonico(){};
Mnemonico::Mnemonico(string nombre){
    instruccion = nombre;
};
Mnemonico::Mnemonico(string instruccion, vector<ModoDir> modos){
    this->instruccion = instruccion;
    this->modo_dir = modos;
}
string Mnemonico:: getNombre(){
    return this->instruccion;
}
vector<ModoDir> Mnemonico:: getModos(){
    return modo_dir;
}

void Mnemonico:: Add_MD(vector<ModoDir> modos){
    this->modo_dir = modos;
}
void Mnemonico:: Add_VMD(ModoDir modo){
    this->modo_dir.push_back(modo);
}
//*************************************************************

ModoDir Mnemonico:: existeMD(string MD){
    for(auto modo : this->modo_dir){
        if(modo.nombre == MD)
            return modo;
    }
    ModoDir aux;
    aux.nombre = "-";
    return aux;
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
ModoDir Mnemonico:: esINH(string operando){
    /*en inherente no es necesario ningun operando
      por lo que el operando debe estar vacio*/
    ModoDir aux = existeMD("INH"), falso;
    falso.nombre = "-";
    if(aux.nombre != "-"){
        for(auto caracter : operando){
            if( caracter != ' ')        //si hay algo que no sea un espacio, no puede ser inh
                return falso;
        }
        return aux;
    }
    return aux;
}

pair<ModoDir, int> Mnemonico:: esIMM(string operando){
    ModoDir aux = existeMD("IMM");
    if(aux.nombre != "-"){
        if(operando[0] == '#')
        {
            operando.erase(0, 1);
            char c = operando[0];
            if(c >= '0' && c <= '9'){//es decimal
                return {aux, is8or16(sdtod(operando))};
            }
            operando.erase(0, 1);
            if(c == '$'){                //es hexa
                return {aux, is8or16(htod(operando))};      //si es de 8 bits retorna 1
            }
            else if(c == '@'){                //es oct
                return {aux, is8or16(otod(operando))};         //si es de 8 bits retorna 1
            }
            else if(c == '%'){                //es binario
                return {aux, is8or16(btod(operando))};        //si es de 8 bits retorna 1
            }
        }
    }
    return {aux, -1};
}
ModoDir Mnemonico:: esDIR(string operando){
    ModoDir aux = existeMD("DIR"), falso;
    falso.nombre = "-";
    if(aux.nombre != "-"){
        char c = operando[0];
        if(c >= '0' && c <= '9'){//es decimal
            if(is8or16(sdtod(operando)) == 1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
        operando.erase(0, 1);
        if(c == '$'){                //es hexa
            if(is8or16(htod(operando)) == 1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
        else if(c == '@'){                //es oct
            if(is8or16(otod(operando)) == 1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
        else if(c == '%'){                //es binario
            if(is8or16(btod(operando)) == 1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
    }
    return aux;
}
ModoDir Mnemonico:: esEXT(string operando){
    ModoDir aux = existeMD("EXT"), falso;
    falso.nombre = "-";
    if(aux.nombre != "-"){
        char c = operando[0];
        if(c >= '0' && c <= '9'){//es decimal
            if(is8or16(sdtod(operando)) != -1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
        operando.erase(0, 1);
        if(c == '$'){                //es hexa
            if(is8or16(htod(operando)) != -1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
        else if(c == '@'){                //es oct
            if(is8or16(otod(operando)) != -1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
        else if(c == '%'){                //es binario
            if(is8or16(btod(operando)) != -1)        //si es de 8 bits return true
                return aux;
            return falso;
        }
    }
    return aux;
}






#endif // MNEMONICO_H_INCLUDED
