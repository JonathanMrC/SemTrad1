#include <bits/stdc++.h>

using namespace std;
/*
Jonathan Martin Civrian
se ingresa la direccion del archivo y listo
Leemos un archivo .asm
*/
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

bool esHex(string s){
    set<char>hex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'f'};
    for(char a : s){
        if(hex.find(tolower(a)) == hex.end())
            return false;
    }
    return true;
}

int StringtoInt(string s){
    int tmp = 0;
    for(int i = 0; i < s.size();++i){
        if(!esNum(s[i]))
            return 1e5;
        tmp*=10;
        tmp+=s[i]-'0';
    }
    return tmp;
}

string FDIR(string &s){//dar formato correcto a una direccion
    string direccion = "";
    for(int i = 0; i < s.size();++i){//darle el formato correcto a la direccion
        if(s[i] == '\\')
            direccion+="\\\\";
        else direccion+=s[i];
    }
    return direccion;
}

string StringtoUpper(string s){//cambia una string de minusculas a mayusculas
    for(int i = 0; i < s.size();++i)
        s[i] = toupper(s[i]);
    return s;
}

void AgregarMnemonicos(map<string, string> &mnemonicos){//agrega los mnemonicos de un txt
    string s, t;
    ifstream archivo("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practica1_SemTraductores\\mnemonicos.txt");
    //el archivo tiene los mnemonicos en mayusculas
    if(!archivo.good())
        cout << "No se encontro el archivo de mnemonicos\n";
    else
        while(getline(archivo, s, ',')){
            getline(archivo, t);
            mnemonicos[s] = t;
        }
    archivo.close();
    return;
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

bool esInstruccion(string s, map<string, string> &mnemonicos){
    int pos = 0, i, dir;
    string mnemonico;
    for(; pos < s.size() && s[pos] == ' ';++pos);//avanzamos en la cadena hasta encontrar algo diferente a espacio
    for(i = pos; i < s.size() && s[i] != ' ';++i);//obtenemos el final de la cadena
    mnemonico = s.substr(pos, i-pos);
    if(mnemonicos.find(StringtoUpper(mnemonico)) != mnemonicos.end()){//es un mnemonico valido por lo que ahora se debe validar si su  modo de direccionamiento es correcto
        cout << "Mnemonico: " << mnemonico << "operando del mnemonico: " << s.substr(i, s.size()-i) << endl;
        return true;
    }
    return false;
}

bool OchoBit(string s){
    if(esNum(s[0])){//esta en base decimal
        int tmp = StringtoInt(s);
        if(tmp <= 256 && tmp >= 0)
            return true;
        return false;
    }
    else if(s[0] == '$'){//es hexadecimal
        if(esHex(s) && s.size() == 3)
            return true;
        return false;
    }
    else if(s[0] == '@'){//octal
        return true;
    }
    else{
        for(char a : s){
            if(a != '1' && a != '0')
                return false;
        }
        return s.size() == 9;
    }
}

bool Diezy6Bit(string s){
    if(esNum(s[0])){//esta en base decimal
        int tmp = StringtoInt(s);
        if(tmp <= 65536 && tmp >= 0)
            return true;
        return false;
    }
    else if(s[0] == '$'){//es hexadecimal
        if(esHex(s) && s.size() == 5)
            return true;
        return false;
    }
    else if(s[0] == '@'){//octal
        return true;
    }
    else{
        for(char a : s){
            if(a != '1' && a != '0')
                return false;
        }
        return s.size() == 17;
    }
}


//C:\Users\jonat\Desktop\Seminario Traductores\Practica1_SemTraductores\pruebas.asm       direccion del archivo

int main()
{
    map<string, string> mnemonicos;
    AgregarMnemonicos(mnemonicos);
    /*for(auto a : mnemonicos)
        cout << a.first << "\t" << a.second <<endl;
        */
    ifstream archivo;
    string tmp, comentario;
    int pos;
    cout << "Ingrese la direccion del archivo:\n";
    getline(cin, tmp);
    cout << endl;
    archivo.open(FDIR(tmp), ios::in);                           //abrir el archivo en modo lectura

    if(!archivo.good())                                         //si el archivo no esta bien
        cout << "No se encontro el archivo\n";
    else{
        while(getline(archivo, tmp)){                           //leo una linea del archivo .asm
            pos = tmp.find(';');
            if(pos != -1){                                      //si pos es != de -1 hay un comentario
                                        /*me falta saber que hacer con el cometario*/
                comentario = tmp.substr(pos, tmp.size()-pos);   //guardo el cometario por si acaso
                tmp.erase(pos, tmp.size()-pos);                 //elimino el cometario de la cadena
            }
            if(tmp[0] != ' '){                                  //es etiqueta si no hay un espacio al principio de la cadena
                string etiqueta;
                pos = tmp.find(' ');                            //regresa la posicion donde aparece el primer espacio por lo tanto termina la etiqueta
                if(tmp.substr(0, pos) == "")                    //si es una linea con un enter, saltamos a la siguiente linea
                    continue;
                etiqueta = tmp.substr(0, pos);
                cout << "Etiqueta: " << etiqueta << endl;       //guardo la etiqueta
                tmp.erase(0, pos);                              //borro la etiqeuta de la linea
                pos = 0;
                int fin = 0;
                string mnemonico;
                for(; pos < tmp.size() && tmp[pos] == ' ';++pos);//avanzamos en la cadena hasta encontrar algo diferente a espacio
                for(fin = pos; fin < tmp.size() && tmp[fin] != ' ';++fin);//avanzamos en la cadena hasta encontrar algo diferente a espacio
                mnemonico = tmp.substr(0, fin-pos+1);
                if(mnemonico.size() == tmp.size())
                    continue;
                tmp.erase(0, fin);
                cout << "Mnemonico: " << mnemonico << "\tOperando: " << tmp << endl;

            }
            else{                                                //debe ser una instruccion
                pos = 0;
                int fin = 0;
                string mnemonico;
                cout << "Lo que queda: " << tmp << endl;
                for(; pos < tmp.size() && tmp[pos] == ' ';++pos);//avanzamos en la cadena hasta encontrar algo diferente a espacio
                for(fin = pos; fin < tmp.size() && tmp[fin] != ' ';++fin);//avanzamos en la cadena hasta encontrar algo diferente a espacio
                mnemonico = tmp.substr(pos, fin-pos+1);
                if(mnemonico.size() == tmp.size())
                    continue;
                tmp.erase(0, fin);
                cout << "Mnemonico: " << mnemonico << "\tOperando: " << tmp << endl;
            }
            cout << "Comentario: " << comentario << endl << endl;
            //al final del ciclo debo guardar la desicion a la que llegue con el analisis en el archivo con nombre igual, pero terminacion .lst
        }
        archivo.close();
    }
    return 0;
}
