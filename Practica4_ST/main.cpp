#include <bits/stdc++.h>
#include"Mnemonico.h"
using namespace std;

bool esNum(char &c){//es para saber si un char es un numero
    if(c >= '0' && c <= '9')
        return true;
    return false;
}

int sdtod(string valor){
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

int HextoDec(string valor){
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


char AumentarCharEnHex(char a){
    if(a <= '8' && a >= '0')
        a++;
    else if(a == '9')
        a = 'a';
    else if(a >= 'a' && a <= 'e')
        a++;
    else
        a = '0';
    return a;
}


void aumentarhexa8(string &cadena, int a){
    char uno, dos = cadena[0];
    while(a--){
        uno = AumentarCharEnHex(cadena[1]);
        if(uno == '0'){
            dos = AumentarCharEnHex(cadena[0]);
        }
        cadena[0] = dos;
        cadena[1] = uno;
        //cout << "CL++ = " << CL << endl;
    }
}

void Hexpp(string &CL, int a){      //formato de cadena++ en hexadecimal
    char uno, dos = CL[2], tres = CL[1], cuatro = CL[0];
    while(a--){
        uno = AumentarCharEnHex(CL[3]);
        if(uno == '0'){
            dos = AumentarCharEnHex(CL[2]);
            if(dos == '0'){
                tres = AumentarCharEnHex(CL[1]);
                if(tres == '0'){
                    cuatro = AumentarCharEnHex(CL[0]);
                }
            }
        }
        CL[0] = cuatro;
        CL[1] = tres;
        CL[2] = dos;
        CL[3] = uno;
        //cout << "CL++ = " << CL << endl;
    }
}

int OcttoDec(string valor){
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

int BintoDec(string valor){
    reverse(valor.begin(), valor.end());
    int val = 0;
    char c;
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '1')
            val+=(c-'0')*(potencia(2, i));
        else return 1e7;
    }
    return val;
}

string ConvertirAHex(string valor){         //convierte de cualquier base a hexadecimal
    if(!valor.size()) return "";
    char c = valor[0];
    if(c >= '0' && c <= '9'){               //es decimal
        string formato = "0000";
        Hexpp(formato, sdtod(valor));
        return formato;
    }
    if(c == '#'){
        valor.erase(0, 1);
    }
    c = valor[0];
    if(!esNum(c)){
        valor.erase(0, 1);
    }
    string formato = "0000";
    if(c >= '0' && c <= '9'){               //es decimal
        Hexpp(formato, sdtod(valor));
        return formato;
    }
    if(c == '$'){               //es hexa
        for(int i = 0; valor.size() < 4;++i)
            valor = "0"+valor;
        return valor;
    }
    if(c == '@'){                //es oct
        Hexpp(formato, OcttoDec(valor));
        return formato;
    }
    if(c == '%'){                //es binario
        Hexpp(formato, BintoDec(valor));
        return formato;
    }
}

void InsertarMn(vector<Mnemonico> &instrucciones){
    ifstream archivo;
    string cadena, instruccion;
    unsigned int pos;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\TABOP.txt", ios::in);
    if(archivo.fail())
        cout << "No se encontro el archivo de los mnemonicos en la carpeta del proyecto\nDireccion:\tC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\TABOP.txt";
    else{
        while(getline(archivo, cadena)){
            //cout << endl << cadena << endl;
            vector<ModoDir> modos;
            string modo = "", cop = "", cli = "", instruccion = "";
            int li;
            for(pos = 0; pos < cadena.size() && cadena[pos] != '|';++pos){
                instruccion += cadena[pos];
            }
            pos++;
            //cout << "INSTRUCCION NUEVA:\t" << instruccion << endl;
            for(; pos < cadena.size();++pos){
                //cout << "Modo:";
                for(; cadena[pos] != '%' && pos < cadena.size(); ++pos){
                    modo+=cadena[pos];
                    //cout << cadena[pos];
                }
                for(pos+=1; cadena[pos] != '$' && pos < cadena.size(); ++pos){
                    cop+=cadena[pos];
                    //cout << cadena[pos];
                }
                //cout << endl << "COP:";
                for(pos+=1;cadena[pos] != '|' && pos < cadena.size();++pos){
                    cli+=cadena[pos];
                    //cout << cadena[pos];
                }
                li = sdtod(cli);
                //cout << "\tResultado final:\nModo:"<< aux.first << "\tCOP:" << aux.second << endl;
                ModoDir md(modo, cop, li);
                modos.push_back(md);
                modo = "";
                cop = "";
                cli = "";
            }
//            cout << "\nMD de:" << instruccion << "\n";
//            for(auto modo : modos)
//                cout <<modo.nombre << " COP:" << modo.cop<< " li:" << modo.li<< endl;
//            cout << endl;
            Mnemonico nuevo(instruccion, modos);
            instrucciones.push_back(nuevo);
        }
    }
    return;
}

bool esLetra(char c, int modo){//modo 0 = mayusculas, modo = 1 minusculas, modo = 2 letra
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


string StringtoUpper(string s){//cambia una string de minusculas a mayusculas
    for(int i = 0; i < s.size();++i)
        s[i] = toupper(s[i]);
    return s;
}

void GuardarEt(map<string, string> etiquetas){
    ofstream archivo;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\TABSIN.txt", ios::out);
    for(auto it : etiquetas)
        archivo << it.first << "\t" << it.second << endl;
    archivo.close();
}

void GuardarLst(string s){
    ofstream archivo;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\P4.lst", ios::app);
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


ModoDir esInstruccion(string instruccion_candidato, string operando, vector<Mnemonico> &instrucciones){
    ModoDir aux;
    for(auto instruccion : instrucciones){
        if(instruccion.getNombre() == StringtoUpper(instruccion_candidato)){//si en los mnemonicos hay una instruccion con la misma cadena
            //cout << "La instruccion:\t" << instruccion_candidato << " = " << instruccion.getNombre();
            aux = instruccion.esINH(operando);
            if(aux.nombre != "-"){
                //cout << "La instruccion es INH\n";
                return aux;
            }
            pair<ModoDir, int> par;
            par = instruccion.esIMM(operando);
            if(par.first.nombre != "-"){
                //cout << "La instruccion es IMM\n";
                if(par.first.nombre == "ADDD" && par.second != 2){
                    aux.nombre = "-";
                    return aux;
                }
                else if(par.first.nombre != "ADDD" && par.second == 1)
                    return par.first;
            }
            aux = instruccion.esDIR(operando);
            //cout << aux.nombre << ' ' << aux.cop << ' ' << aux.li << endl;
            if(aux.nombre != "-"){
                //cout << "La instruccion es DIR\n";
                return aux;
            }
            aux = instruccion.esEXT(operando);
            if(aux.nombre != "-"){
                //cout << "La instruccion es EXT\n";
                return aux;
            }
            aux.nombre = "-";            return aux;
        }
    }
    aux.nombre = "-";
    return aux;
}


int main()
{
    //reinicio el .lst y el tabsim
    ofstream archivol, TABSIN;
    ifstream archivo;
    archivol.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\P4.lst", ios::trunc);
    archivol.close();
    TABSIN.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\TABSIN.txt", ios::trunc);
    TABSIN.close();
    //estructuras de datos
    vector<Mnemonico> instrucciones;
    map<string, string> etiquetas;
    //variables
    string tmp, comentario, copia, instruccion, operando, modo_dir, CL = "0000", copiacl, cadena_directiva = "";
    int pos;
    bool valido = true, banderaI, fin_asm;
    //Inicializaciones
    InsertarMn(instrucciones);

    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\P4.asm", ios::in);

    if(!archivo.good())                                         //si el archivo no esta bien
        cout << "No se encontro el archivo\nEn la ruta:\nC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica4_ST\\P4.asm";
    else{
        while(getline(archivo, tmp)){                           //leo una linea del archivo .asm
            ModoDir aux;
            copiacl = CL;
            copia = "\t"+tmp;
            valido = true;
            banderaI = fin_asm = false;
            pair<string, int> modo_dir = {"", 0};
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
                tmp.erase(0, pos);                              //borro la etiqeuta de la linea
                if(!esEtiqueta(candidato)){
                    valido = false;
                    cout << "La etiqueta no es valida\t"<<candidato<< endl;
                }
                else{
                    //cout << "Es etiqueta:" << candidato << endl;
                    etiquetas[candidato] = CL;
                    for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //avanzo hasta encontrar el inicio de la instruccion
                    tmp.erase(0, pos);                                        //borro esa parte inservible
                    pos = tmp.find(' ');                                        //encuentro el fin de la instruccion
                    instruccion = tmp.substr(0, pos);                           //guardo la instruccion
                    tmp.erase(0, pos);                                        //elimino la instruccion de la cadena tmp
                    for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //busco el inicio del operando
                    tmp.erase(0, pos);                                        //elimino la basura
                    pos = tmp.find(' ');                                        //encuentro el final del operando
                    operando = tmp.substr(0, pos-1);                            //guardo el operando
                    //cout << "\nInstruccion o directiva|" << instruccion << "|operando|" << operando << endl;
                    for(auto inst: instrucciones){                              //verifico si es instruccion o directiva
                        if(inst.getNombre() == instruccion){
                            banderaI = true;
                        }
                    }
                    if(banderaI)
                    {
                        aux = esInstruccion(instruccion, operando, instrucciones);
                        if(aux.nombre == "-")
                            valido = false;
                        else
                            Hexpp(CL, aux.li);
                    }
                    else{                                                           //es una directiva
                        if(instruccion == "ORG"){
                            CL = ConvertirAHex(operando);
                        }
                        else if(instruccion == "START"){
                            CL = "0000";
                        }
                        else if(instruccion == "END"){
                            fin_asm = true;
                        }
                        else if(instruccion == "EQU"){
                            etiquetas[candidato] = ConvertirAHex(operando);
                        }
                        else if(instruccion == "DC.B"){
                            if(operando == " "){
                                Hexpp(CL, 1);
                                cadena_directiva +="00 ";
                            }
                            else{
                                string s = "";
                                int pos = 0;
                                for(pos = 0; pos < operando.size();++pos){
                                    s = "";
                                    for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                    for(;operando[pos] != ',' && pos < operando.size();++pos)
                                        s+=operando[pos];
                                    //falta validar los char
                                    s = ConvertirAHex(s);
                                    Hexpp(CL, 1);
                                    cadena_directiva +=s +' ';
                                }
                            }
                        }
                        else if(instruccion == "DC.W"){
                            if(operando.size() == 0){
                                Hexpp(CL, 2);
                                cadena_directiva ="0000 ";
                            }
                            else{
                                string s = "";
                                int pos = 0;
                                for(pos = 0; pos < operando.size();++pos){
                                    s = "";
                                    for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                    for(;operando[pos] != ',' && pos < operando.size();++pos)
                                        s+=operando[pos];
                                    //falta validar los char
                                    s = ConvertirAHex(s);
                                    Hexpp(CL, 2);
                                    cadena_directiva +=s +' ';
                                }
                            }
                        }
                        else if(instruccion == "BSZ" || instruccion == "ZMB"){
                            cout << "BSZ " << instruccion << "-> " << operando << endl;
                            if(operando.size() == 0)
                                valido = false;
                            else{
                                string s = "";
                                int pos = 0, cantidad;
                                for(pos = 0; pos < operando.size();++pos){
                                    s = "";
                                    for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                    for(;operando[pos] != ' ' && pos < operando.size();++pos)
                                        s+=operando[pos];
                                    cantidad = sdtod(s);
                                    //falta validar los char
                                    Hexpp(CL, cantidad);
                                    for(int i = 0; i < cantidad;++i)
                                        cadena_directiva +="00 ";
                                }
                            }
                        }
                        else if(instruccion == "FILL"){
                            string dos = "00";
                            int cantidad = 0;
                            if(operando.size() == 0)
                                valido = false;
                            else{
                                string s = "";
                                int pos = 0, cantidad;
                                s = "";
                                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                for(;operando[pos] != ',' && pos < operando.size();++pos)
                                    s+=operando[pos];
                                cantidad = sdtod(s);
                                aumentarhexa8(dos, cantidad);
                                s = "";
                                //cout << "operador:" << dos << endl;
                                for(;operando[pos] != ' ' && pos < operando.size();++pos)
                                    s+=operando[pos];
                                cantidad = sdtod(s);
                                //cout << "cantidad:" << cantidad << endl;
                                //falta validar los char
                                Hexpp(CL, cantidad);
                                for(int i = 0; i < cantidad;++i)
                                    cadena_directiva +=dos+' ';
                                //cout << "cadena dir:" << cadena_directiva << endl;
                            }
                        }
                        else if(instruccion == "FCC"){
                            string dos = "00";
                            if(operando.size() == 0)
                                valido = false;
                            else{
                                string s = "";
                                int pos = 0, cantidad;
                                for(pos = 0; pos < operando.size();++pos){
                                    s = "";
                                    for(;operando[pos] == '/' && pos < operando.size();++pos);
                                    for(;operando[pos] != '/' && pos < operando.size();++pos){
                                        if(esLetra(operando[pos], 2))
                                           cantidad = operando[pos];
                                        else{
                                            valido = false;
                                            break;
                                        }
                                        aumentarhexa8(dos, cantidad);
                                        //falta validar los char
                                        Hexpp(CL, 1);
                                        cadena_directiva +=dos+' ';
                                        dos = "00";
                                    }
                                }
                            }
                        }
                        else if(instruccion == "FCB"){
                            string dos = "00";
                            if(operando.size() == 0)
                                valido = false;
                            else{
                                string s = "";
                                int pos = 0, cantidad;
                                for(pos = 0; pos < operando.size();++pos){
                                    s = "";
                                    for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                    for(;operando[pos] != ' ' && pos < operando.size();++pos)
                                        s+=operando[pos];
                                    cantidad = sdtod(s);
                                    aumentarhexa8(dos, cantidad);
                                    //falta validar los char
                                    Hexpp(CL, 1);
                                    cadena_directiva +=dos;
                                }
                            }
                        }
                        else{
                            valido = false;
                        }
                    }

                }
            }
            else{                                                //debe ser una instruccion
                for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //avanzo hasta encontrar el inicio de la instruccion
                tmp.erase(0, pos);                                        //borro esa parte inservible
                pos = tmp.find(' ');                                        //encuentro el fin de la instruccion
                instruccion = tmp.substr(0, pos);                           //guardo la instruccion
                tmp.erase(0, pos);                                        //elimino la instruccion de la cadena tmp
                for(pos = 0; pos < tmp.size() && tmp[pos] == ' ';++pos);    //busco el inicio del operando
                tmp.erase(0, pos);                                        //elimino la basura
                pos = tmp.find(' ');                                        //encuentro el final del operando
                operando = tmp.substr(0, pos-1);                            //guardo el operando
                //cout << "\nInstruccion o directiva|" << instruccion << "|operando|" << operando << endl;
                for(auto inst: instrucciones){                              //verifico si es instruccion o directiva
                    if(inst.getNombre() == instruccion){
                        //cout << inst.getNombre() << " = "<< instruccion << endl;
                        banderaI = true;
                    }
                }
                if(banderaI)
                {
                    aux = esInstruccion(instruccion, operando, instrucciones);
                    //cout << aux.nombre << ", " << aux.cop << ", " << aux.li << endl;
                    if(aux.nombre == "-")
                        valido = false;
                    else
                        Hexpp(CL, aux.li);
                }
                else{                                                            //es una directiva
                    if(instruccion == "ORG"){
                        CL = ConvertirAHex(operando);
                    }
                    else if(instruccion == "START"){
                        CL = "0000";
                    }
                    else if(instruccion == "END"){
                        fin_asm = true;
                    }
                    else if(instruccion == "EQU"){
                        valido = false;
                    }
                    else if(instruccion == "DC.B"){
                        if(operando.size() == 0){
                            Hexpp(CL, 1);
                            cadena_directiva +="00 ";
                        }
                        else{
                            string s = "";
                            int pos = 0;
                            for(pos = 0; pos < operando.size();++pos){
                                s = "";
                                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                for(;(operando[pos] != ',') && pos < operando.size();++pos)
                                    s+=operando[pos];
                                //falta validar los char
                                s = ConvertirAHex(s);
                                Hexpp(CL, 1);
                                cadena_directiva +=s +' ';
                            }
                        }
                    }
                    else if(instruccion == "DC.W"){
                        if(operando.size() == 0){
                            Hexpp(CL, 2);
                            cadena_directiva ="0000 ";
                        }
                        else{
                            string s = "";
                            int pos = 0;
                            for(pos = 0; pos < operando.size();++pos){
                                s = "";
                                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                for(;operando[pos] != ',' && pos < operando.size();++pos)
                                    s+=operando[pos];
                                s = ConvertirAHex(s);
                                Hexpp(CL, 2);
                                cadena_directiva +=s +' ';
                            }
                        }
                    }
                    else if(instruccion == "BSZ" || instruccion == "ZMB"){
                        if(operando.size() == 0)
                            valido = false;
                        else{
                            string s = "";
                            int pos = 0, cantidad;
                            for(pos = 0; pos < operando.size();++pos){
                                s = "";
                                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                for(;operando[pos] != ' ' && pos < operando.size();++pos)
                                    s+=operando[pos];
                                cantidad = sdtod(s);
                                //falta validar los char
                                Hexpp(CL, cantidad);
                                for(int i = 0; i < cantidad;++i)
                                    cadena_directiva +="00 ";
                            }
                        }
                    }
                    else if(instruccion == "FILL"){
                        string dos = "00";
                        int cantidad = 0;
                        if(operando.size() == 0)
                            valido = false;
                        else{
                            string s = "";
                            int pos = 0, cantidad;
                            s = "";
                            for(;operando[pos] == ' ' && pos < operando.size();++pos);
                            for(;operando[pos] != ',' && pos < operando.size();++pos)
                                s+=operando[pos];
                            cantidad = sdtod(s);
                            aumentarhexa8(dos, cantidad);
                            s = "";
                            for(pos+=1;operando[pos] != ' ' && pos < operando.size();++pos)
                                s+=operando[pos];
                            cantidad = sdtod(s);
                            //falta validar los char
                            Hexpp(CL, cantidad);
                            for(int i = 0; i < cantidad;++i)
                                cadena_directiva +=dos+' ';
                        }
                    }
                    else if(instruccion == "FCC"){
                        string dos = "00";
                        if(operando.size() == 0)
                            valido = false;
                        else{
                            string s = "";
                            int pos = 0, cantidad;
                            for(pos = 0; pos < operando.size();++pos){
                                s = "";
                                for(;operando[pos] == '/' && pos < operando.size();++pos);
                                for(;operando[pos] != '/' && pos < operando.size();++pos){
                                    if(esLetra(operando[pos], 2))
                                       cantidad = operando[pos];
                                    else{
                                        valido = false;
                                        break;
                                    }
                                    aumentarhexa8(dos, cantidad);
                                    //falta validar los char
                                    Hexpp(CL, 1);
                                    cadena_directiva +=dos+' ';
                                    dos = "00";
                                }
                            }
                        }
                    }
                    else if(instruccion == "FCB"){
                        string dos = "00";
                        if(operando.size() == 0)
                            valido = false;
                        else{
                            string s = "";
                            int pos = 0, cantidad;
                            for(pos = 0; pos < operando.size();++pos){
                                s = "";
                                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                                for(;operando[pos] != ' ' && pos < operando.size();++pos)
                                    s+=operando[pos];
                                cantidad = sdtod(s);
                                aumentarhexa8(dos, cantidad);
                                //falta validar los char
                                Hexpp(CL, 1);
                                cadena_directiva +=dos;
                            }
                        }
                    }
                    else{
                        valido = false;
                    }
                }
            }
            if(banderaI){                       //es una instruccion
                string cop = "0000", cop_real = aux.cop;
                //cout << operando << endl;
                int tam = 3;
                cop = ConvertirAHex(operando);
                //cout << cop << endl;
                for(int i = cop_real.size()-1;tam >= 0 && i >= 0;--i)
                {
                    if(cop_real[i] != ' ' && (cop_real[i] >= 'a' && cop_real[i] <='z'))
                        cop_real[i] = cop[tam--];
                }
                copia = copiacl+"\t"+cop_real+"\t"+copia;
                if(!valido)
                    copia+="\t\t; invalido\n";
                else copia+="\t\t; "+aux.nombre + "\n";
            }
            else{                               //es una directiva
                copia = copiacl+"\t"+cadena_directiva+"\t"+copia;
                if(!valido)
                    copia+="\t\t; invalido\n";
                else copia+="\t\t; directiva\n";
            }
            GuardarEt(etiquetas);
            GuardarLst(copia);
            cadena_directiva = "";
            if(fin_asm)
                break;
            //al final del ciclo debo guardar la desicion a la que llegue con el analisis en el archivo con nombre igual, pero terminacion .lst
        }
        archivo.close();
    }
    return 0;
}
