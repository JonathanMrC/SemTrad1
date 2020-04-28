#include <bits/stdc++.h>
#include"Mnemonico.h"
using namespace std;

void InsertarMn(vector<Mnemonico> &instrucciones){
    ifstream archivo;
    string cadena, aux, instruccion;
    unsigned int pos;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica2_ST\\TABOP.txt", ios::in);
    if(archivo.fail())
        cout << "No se encontro el archivo de los mnemonicos en la carpeta del proyecto\nDireccion:\tC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practica2_ST\\TABOP.txt";
    else{
        while(getline(archivo, cadena)){
            //cout << endl << cadena << endl;
            vector<string> modos;
            instruccion = "";
            for(pos = 0; pos < cadena.size() && cadena[pos] != '|';++pos){
                instruccion += cadena[pos];
            }
            pos++;
            //cout << "INSTRUCCION NUEVA:\t" << instruccion << endl << "MODOS:\n";
            aux = "";
            for(; pos < cadena.size();++pos){
                if(cadena[pos] != '|'){
                   aux+=cadena[pos];
                   //cout << cadena[pos];
                }
                else{
                    //cout << " = "<< aux << endl;
                    modos.push_back(aux);
                    aux = "";
                }
            }
            /*cout << "dEBUG\n";
            for(auto modo : modos)
                cout << modo << ',';*/
            Mnemonico nuevo(instruccion, modos);
            instrucciones.push_back(nuevo);
        }
    }
    return;
}

bool esLetra(char &c, int modo){//modo 0 = mayusculas, modo = 1 minusculas, modo = 2 letra
    bool mayus = false, minusc = false;
    if(c >= 'A' && c <= 'Z')
        mayus = true;
    if( c >= 'a' && c <= 'z')
        minusc = true;
    if((modo == 0) && mayus)
        return true;
    if((modo == 1) && minusc)
        return true;
    if((modo == 2) && minusc || mayus)
        return true;
    return false;
}

bool esNum(char &c){//es para saber si un char es un numero
    if(c >= '0' && c <= '9')
        return true;
    return false;
}



string StringtoUpper(string s){//cambia una string de minusculas a mayusculas
    for(int i = 0; i < s.size();++i)
        s[i] = toupper(s[i]);
    return s;
}

void GuardarLst(string s){
    ofstream archivo;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica2_ST\\P2.lst", ios::app);
    archivo<<s;
    archivo.close();
}


bool esEtiqueta(string s){
    char c;
    bool b = false;//bandera para saber si ya hubo un ':' en la cadena
    if(!esLetra(s[0], 2) && s[0] != '_')//si el primer caracter no es ni letra ni '_' no es etiqueta
        return false;
    for(int i = 1; i < s.size();++i){
        c = s[i];
        if(!b){//si no ha aparecido el ':' entonces todavia es etiqueta
            if(!esLetra(c, 2) && !esNum(c) && c != '_'){//si no es letra, numero o guion bajo o es ':' o no es posible
                if(c == ':')
                    b = true;
                else return false;
            }
        }
        else return false;//si ya hubo un ':' entonces termino el label por lo que no es posible
    }
    return true;
}


string esInstruccion(string instruccion_candidato, string operando, vector<Mnemonico> &instrucciones){
    for(auto instruccion : instrucciones){
        if(instruccion.getNombre() == StringtoUpper(instruccion_candidato)){//si en los mnemonicos hay una instruccion con la misma cadena
            //cout << "La instruccion:\t" << instruccion_candidato << " = " << instruccion.getNombre();
            if(instruccion.esINH(operando)){
                //cout << "La instruccion es INH\n";
                return "INH";
            }
            else if(instruccion.esIMM(operando)){
                //cout << "La instruccion es IMM\n";
                return "IMM";
            }
            else if(instruccion.esDIR(operando)){
                //cout << "La instruccion es DIR\n";
                return "DIR";
            }
            else if(instruccion.esEXT(operando)){
                //cout << "La instruccion es EXT\n";
                return "EXT";
            }
            else return "nel";
        }
    }
    return "nel";
}


int main()
{
    string tmp, comentario, copia, instruccion, operando, modo_dir;
    vector<Mnemonico> instrucciones;
    int pos;
    bool valido = true;
    InsertarMn(instrucciones);/*
    for( int i = 0; i < instrucciones.size();++i){
        cout << instrucciones[i].getNombre() << endl << "Modos\n";
        for( int j = 0; j < instrucciones[i].getModos().size();++j)
            cout << "\t" << instrucciones[i].getModos()[j] << endl;}*/
    ifstream archivo;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica2_ST\\P2.asm", ios::in);
//    ofstream archivol;
//    archivol.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practica2_ST\\P2.lst", ios::trunc);
//    archivo.close();
    if(!archivo.good())                                         //si el archivo no esta bien
        cout << "No se encontro el archivo\nEn la ruta:\nC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practica2_ST\\P2.asm";
    else{
        while(getline(archivo, tmp)){                           //leo una linea del archivo .asm
            copia = tmp;
            valido= true;
            pos = tmp.find(';');
            if(pos != -1){                                      //si pos es != de -1 hay un comentario
                comentario = tmp.substr(pos, tmp.size()-pos);   //guardo el cometario por si acaso
                tmp.erase(pos, tmp.size()-pos);                 //elimino el cometario de la cadena
            }
            if(tmp[0] != ' '){                                  //es etiqueta si no hay un espacio al principio de la cadena
                string candidato;
                pos = tmp.find(' ');                            //regresa la posicion donde aparece el primer espacio por lo tanto termina la etiqueta
                if(tmp.substr(0, pos) == "")                    //si es una linea con un enter, saltamos a la siguiente linea
                    continue;
                candidato = tmp.substr(0, pos);
                if(!esEtiqueta(candidato)){
                    valido = false;
                    cout << "La etiqueta no es valida\t"<<candidato<< endl;
                }
                else{
                    tmp.erase(0, pos);                              //borro la etiqeuta de la linea
                    for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //avanzo hasta encontrar el inicio de la instruccion
                    tmp.erase(0, pos-1);                                        //borro esa parte inservible
                    pos = tmp.find(' ');                                        //encuentro el fin de la instruccion
                    instruccion = tmp.substr(0, pos);                           //guardo la instruccion
                    tmp.erase(0, pos-1);                                        //elimino la instruccion de la cadena tmp
                    for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //busco el inicio del operando
                    tmp.erase(0, pos-1);                                        //elimino la basura
                    pos = tmp.find(' ');                                        //encuentro el final del operando
                    operando = tmp.substr(0, pos-1);                            //guardo el operando
                    modo_dir = esInstruccion(instruccion, operando, instrucciones);
                    if(modo_dir == "nel")
                        valido = false;
                }
            }
            else{                                                //debe ser una instruccion
                //cout << "Cadena inicial:" << tmp << endl;
                for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //avanzo hasta encontrar el inicio de la instruccion
                tmp.erase(0, pos);                                        //borro esa parte inservible
                //cout << "Cadena despues de borrar el espacio inicial:" << tmp << endl;
                pos = tmp.find(' ');                                        //encuentro el fin de la instruccion
                instruccion = tmp.substr(0, pos);                           //guardo la instruccion
                tmp.erase(0, pos);                                        //elimino la instruccion de la cadena tmp
                //cout << "Cadena despues de borrar la instruccion:" << tmp << endl;
                for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //busco el inicio del operando
                tmp.erase(0, pos);                                        //elimino la basura
                pos = tmp.find(' ');                                        //encuentro el final del operando
                operando = tmp.substr(0, pos);                            //guardo el operando
                //cout << "INSTRUCCION:\t"<<instruccion << ", " << operando<< endl;
                modo_dir = esInstruccion(instruccion, operando, instrucciones);
                //cout << modo_dir << endl;
                if(modo_dir == "nel")
                    valido = false;
            }
            if(!valido)
                copia+=" ; invalido\n";
            else{
                copia+="; ";
                copia+= modo_dir;
                copia+="\n";
            }
            GuardarLst(copia);
            //al final del ciclo debo guardar la desicion a la que llegue con el analisis en el archivo con nombre igual, pero terminacion .lst
        }
        archivo.close();
    }
    return 0;
}
