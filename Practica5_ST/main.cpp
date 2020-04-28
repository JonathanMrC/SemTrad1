#include <bits/stdc++.h>
#include"Mnemonico.h"
using namespace std;

/*
Haber hay dos pasos
En el primero se deben leer las etiquetas y crear el contador de las localidades
En el segundo paso ya se calcula si es valido la instruccion
*/

void InsertarMn(vector<Mnemonico> &instrucciones){
    ifstream archivo("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\TABOP.txt", ios::in);
    vector<ModoDir> modos;
    string cadena, modo, cop, cli, instruccion;
    int li, tam;
    unsigned int pos;
    if(archivo.fail())
        cout << "No se encontro el archivo de los mnemonicos en la carpeta del proyecto\nDireccion:\nC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\TABOP.txt";
    else{
        while(getline(archivo, cadena)){
            //cout << endl << cadena << endl;
            modos.clear();
            li = pos = 0;
            instruccion = "";
            tam = cadena.size();
            for(; pos < tam && cadena[pos] != '|';++pos)
                instruccion += cadena[pos];
            ++pos;
            //cout << "INSTRUCCION NUEVA:\t" << instruccion << endl;
            for(; pos < tam;++pos){
                modo = cop = cli = "";
                //cout << "Modo:";
                for(; cadena[pos] != '%' && pos < tam; ++pos)
                    modo+=cadena[pos];//cout << cadena[pos];
                ++pos;
                for(; cadena[pos] != '$' && pos < tam; ++pos)
                    cop+=cadena[pos];//cout << cadena[pos];
                ++pos;
                for(;cadena[pos] != '|' && pos < tam;++pos)
                    cli+=cadena[pos];//cout << cadena[pos];
                li = StringDectoIntDec(cli);
                //cout << "\tResultado final:\nModo:"<< aux.first << "\tCOP:" << aux.second << endl;
                ModoDir md(modo, cop, li);
                modos.push_back(md);
            }

            Mnemonico nuevo(instruccion, modos);
            instrucciones.push_back(nuevo);
        }
    }
    return;
}

void GuardarEt(map<string, string> etiquetas){          //guarda las etiquetas en un archivo
    ofstream archivo;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\TABSIN.txt", ios::out);
    for(auto it : etiquetas)
        archivo << it.first << "\t$" << it.second << endl;
    archivo.close();
}

void GuardarLst(string s){                              //añade al archivo.lst una cadena
    ofstream archivo;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\P5.lst", ios::app);
    archivo<<s;
    archivo.close();
}

bool esEtiqueta(string s){
    bool b = false;         //bandera para saber si ya hubo un ':' en la cadena
    if(!esLetra(s[0], 2) && s[0] != '_')//si el primer caracter no es ni letra ni '_' no es etiqueta
        return false;
    s.erase(0, 1);
    for(auto candidato : s){
        if(!b){//si no ha aparecido el ':' entonces todavia es etiqueta
            if((!esLetra(candidato, 2)) && (!esNum(candidato)) && (candidato != '_')){//si no es letra, numero o guion bajo o es ':' o no es posible
                if(candidato == ':')
                    b = true;
                else return false;
            }
        }
        else return false;//si ya hubo un ':' entonces termino el label por lo que no es posible
    }
    return true;
}

ModoDir esOp(Mnemonico &instruccion, string operando){
    ModoDir aux;
    aux = instruccion.MnemoTieneMD("INH");
    if(aux.nombre != "noexiste"){
        if(instruccion.esINH(operando))
            return aux;
    }
    aux = instruccion.MnemoTieneMD("IMM");
    if(aux.nombre != "noexiste"){
        if(instruccion.esIMM(operando))
            return aux;
    }
    aux = instruccion.MnemoTieneMD("DIR");
    if(aux.nombre != "noexiste"){
        if(instruccion.esDIR(operando))
            return aux;
    }
    aux = instruccion.MnemoTieneMD("EXT");
    if(aux.nombre != "noexiste"){
        if(instruccion.esEXT(operando))
            return aux;
    }
    aux = instruccion.MnemoTieneMD("REL");
    if(aux.nombre != "noexiste"){
        if(instruccion.esREL(operando, instruccion.getNombre()))
            return aux;
    }
    return aux;
}

bool esDirectiva(string directiva, string operando, bool HayEti, string etiqueta, string &CL, string &cadena_dir, bool &ENDCOD, map<string, string> &etiquetas){
    if(directiva == "ORG"){
        CL = ConvertirAHex(operando);
        return true;
    }
    else if(directiva == "START"){
        CL = "0000";
        return true;
    }
    else if(directiva == "END"){
        ENDCOD = true;
        return true;
    }
    else if(directiva == "EQU"){
        if(HayEti){
            etiquetas[etiqueta] = ConvertirAHex(operando);
            return true;
        }
        return false;
    }
    else if(directiva == "DC.B"){
        if(operando == " "){
            Hexpp(CL, 1);
            cadena_dir +="00 ";
        }
        else{
            string s;
            int pos;
            for(pos = 0; pos < operando.size();++pos){
                s = "";
                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                for(;operando[pos] != ',' && pos < operando.size();++pos)
                    s+=operando[pos];
                s = ConvertirAHex(s);
                Hexpp(CL, 1);
                cadena_dir +=s +' ';
            }
        }
        return true;
    }
    else if(directiva == "DC.W"){
        if(operando.size() == 0){
            Hexpp(CL, 2);
            cadena_dir ="0000 ";
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
                cadena_dir +=s +' ';
            }
        }
        return true;
    }
    else if(directiva == "BSZ" || directiva == "ZMB"){  //cout << "BSZ " << directiva << "-> " << operando << endl;
        if(operando.size()){
            string s = "";
            int pos = 0, cantidad;
            for(pos = 0; pos < operando.size();++pos){
                s = "";
                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                for(;operando[pos] != ' ' && pos < operando.size();++pos)
                    s+=operando[pos];
                cantidad = StringDectoIntDec(s);
                //falta validar los char
                Hexpp(CL, cantidad);
                for(int i = 0; i < cantidad;++i)
                    cadena_dir +="00 ";
            }
            return true;
        }
        return false;
    }
    else if(directiva == "FILL"){
        string dos = "00";
        if(operando.size()){
            string s = "";
            int pos = 0, cantidad;
            s = "";
            for(;operando[pos] == ' ' && pos < operando.size();++pos);
            for(;operando[pos] != ',' && pos < operando.size();++pos)
                s+=operando[pos];
            cantidad = StringDectoIntDec(s);
            Hexpp2char(dos, cantidad);
            s = "";
            //cout << "operador:" << dos << endl;
            for(;operando[pos] != ' ' && pos < operando.size();++pos)
                s+=operando[pos];
            cantidad = StringDectoIntDec(s);
            //cout << "cantidad:" << cantidad << endl;
            //falta validar los char
            Hexpp(CL, cantidad);
            for(int i = 0; i < cantidad;++i)
                cadena_dir +=dos+' ';
            //cout << "cadena dir:" << cadena_directiva << endl;
            return true;
        }
        return false;
    }
    else if(directiva == "FCC"){
        string dos = "00";
        if(operando.size()){
            string s = "";
            int pos = 0, cantidad;
            for(pos = 0; pos < operando.size();++pos){
                s = "";
                for(;operando[pos] == '/' && pos < operando.size();++pos);
                for(;operando[pos] != '/' && pos < operando.size();++pos){
                    if(esLetra(operando[pos], 2))
                       cantidad = operando[pos];
                    else
                        return false;
                    Hexpp2char(dos, cantidad);
                    //falta validar los char
                    Hexpp(CL, 1);
                    cadena_dir +=dos+' ';
                    dos = "00";
                }
            }
            return true;
        }
        return false;
    }
    else if(directiva == "FCB"){
        string dos = "00";
        if(operando.size()){
            string s = "";
            int pos = 0, cantidad;
            for(pos = 0; pos < operando.size();++pos){
                s = "";
                for(;operando[pos] == ' ' && pos < operando.size();++pos);
                for(;operando[pos] != ' ' && pos < operando.size();++pos)
                    s+=operando[pos];
                cantidad = StringDectoIntDec(s);
                Hexpp2char(dos, cantidad);
                //falta validar los char
                Hexpp(CL, 1);
                cadena_dir +=dos;
            }
            return true;
        }
        return false;
    }
    return false;
}

Mnemonico ExisteInstruccion(string candidato, vector<Mnemonico> &instrucciones){
    StringtoUpper(candidato);
    for(auto inst: instrucciones){
        if(inst.getNombre() == candidato)
            return inst;
    }
    return Mnemonico("-");
}

void FormatoInstrOper(string &instruccion, string &operando, string &linea){
    int pos;
    for(pos = 0; pos < linea.size() && linea[pos] == ' ';++pos);    //avanzo hasta encontrar el inicio de la instruccion
    linea.erase(0, pos);                                            //borro esa parte inservible
    pos = linea.find(' ');                                          //encuentro el fin de la instruccion
    instruccion = linea.substr(0, pos);                             //guardo la instruccion
    linea.erase(0, pos);                                            //elimino la instruccion de la cadena linea
    for(pos = 0; pos < linea.size() && linea[pos] == ' ';++pos);    //busco el inicio del operando
    linea.erase(0, pos);                                            //elimino la basura
    pos = linea.find(' ');                                          //encuentro el final del operando
    operando = linea.substr(0, pos-1);                              //guardo el operando
    return;
}

int main()
{
    ifstream archivo;
    //reinicio el .lst y el tabsim
    ofstream lst, TABSIN;
    lst.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\P5.lst", ios::trunc);
    lst.close();
    TABSIN.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\TABSIN.txt", ios::trunc);
    TABSIN.close();
    //estructuras de datos
    vector<Mnemonico> instrucciones;
    Mnemonico mnemonico;
    map<string, string> etiquetas;
    //variables
    string linea, comentario, Clinea, instruccion, operando, CL = "0000", CCL, cadena_directiva = "", candidato, cop, copreal;
    int pos;
    bool valido = true, ENDCOD, BanderaesInstruc;
    //Inicializaciones
    InsertarMn(instrucciones);

    for(int i = 1; i <= 2;++i){  cout << "Paso " << i << endl;
    archivo.open("C:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\P5.asm", ios::in);
    if(!archivo.good()){                                         //si el archivo no esta bien
        cout << "No se encontro el archivo o esta dañado\nEn la ruta:\nC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\P5.asm";
        return 0;
    }
    while(getline(archivo, linea)){                           //leo una linea del archivo .asm
        ModoDir aux;
        CCL = CL;                                           //copia del contador de localidades
        Clinea = "\t"+linea;                                //copia de la linea leida
        valido = true;
        BanderaesInstruc = ENDCOD = false;
        cadena_directiva = "";
        pos = linea.find(';');
        if(pos != -1){                                      //si pos es != de -1 hay un comentario
            comentario = linea.substr(pos, linea.size()-pos);   //guardo el cometario por si acaso
            linea.erase(pos, linea.size()-pos);                 //elimino el cometario de la cadena
        }
        if(linea[0] != ' '){                                  //es etiqueta si no hay un espacio al principio de la cadena
            pos = linea.find(' ');                            //regresa la posicion donde aparece el primer espacio por lo tanto termina la etiqueta
            if(linea.substr(0, pos) == "")                    //si es una linea con un enter, saltamos a la siguiente linea
                continue;
            candidato = linea.substr(0, pos);
            linea.erase(0, pos);                              //borro la etiqeuta de la linea
            if(!esEtiqueta(candidato)){
                valido = false;
                //cout << "La etiqueta no es valida\t"<<candidato<< endl;
            }
            else{
                //elimino el ':' de la cadena candidato en caso de que lo tenga
                for(pos = 0; pos < candidato.size() && candidato[pos] != ':';++pos);
                candidato = candidato.substr(0, pos);
                etiquetas[candidato] = CL;
                //guarda en las cadenas instruccion y operando lo que puede o debe ser la instruccion y operando
                FormatoInstrOper(instruccion, operando, linea); //cout << "\nInstruccion o directiva|" << instruccion << "|operando|" << operando << endl;
                if(etiquetas.count(operando) != 0)
                    operando = etiquetas[operando];
                if(i == 2)cout << "\nInstruccion o directiva|" << instruccion << "|operando|" << operando << endl;
                mnemonico = ExisteInstruccion(instruccion, instrucciones);
                if(mnemonico.getNombre() != "-")
                {
                    BanderaesInstruc = true;
                    aux = esOp(mnemonico, operando);
                if(i == 2)cout << "CL" << CL << endl;
                    Hexpp(CL, aux.li);
                if(i == 2)cout << "CL" << CL << endl;                    if(aux.nombre == "-")
                        valido = false;
                }
                else            //es directiva
                    valido = esDirectiva(instruccion, operando, 1, candidato, CL, cadena_directiva, ENDCOD, etiquetas);
            }
        }
        else{                                                //debe ser una instruccion
            //guarda en las cadenas instruccion y operando lo que puede o debe ser la instruccion y operando
            FormatoInstrOper(instruccion, operando, linea);
            if(etiquetas.count(operando) != 0)
                operando = etiquetas[operando];
            if(i == 2)cout << "\nInstruccion o directiva|" << instruccion << "|operando|" << operando << endl;
            mnemonico = ExisteInstruccion(instruccion, instrucciones);
            if(mnemonico.getNombre() != "-")
            {
                BanderaesInstruc = true;
                aux = esOp(mnemonico, operando);
                if(i == 2)cout << "CL" << CL << endl;
                Hexpp(CL, aux.li);
                if(i == 2)cout << "CL" << CL << endl;
                if(aux.nombre == "-")
                    valido = false;
            }
            else                       //es directiva
                valido = esDirectiva(instruccion, operando, 0, candidato, CL, cadena_directiva, ENDCOD, etiquetas);                                                           //es una directiva
        }
        if(BanderaesInstruc){                       //es una instruccion
            cop = "0000", copreal = aux.cop;//cout << operando << endl;
            int indexcodreal = 3, ope;
            if(aux.nombre == "REL"){
                operando="$"+operando;
                ope = HextoDec(ConvertirAHex(operando), 0);
                if(i == 2){
                    cout << operando << "-" <<CL << endl;
                    cout << ope << "-" <<HextoDec(CL, 0) << endl;}
                ope = ope - HextoDec(CL, 0);
                if(i == 2)cout << ope << endl;
                Hexpp(cop, ope);
                if(ope > 0 && aux.li == 2 && cop[2] == 'f'){
                    valido = false;
                }
            }
            else
                cop = ConvertirAHex(operando);  //cout << cop << endl;
            for(int i = copreal.size()-1;indexcodreal >= 0 && i >= 0;--i)
            {
                if(esLetra(copreal[i], 1))
                    copreal[i] = cop[indexcodreal--];
            }
            Clinea = CCL+"\t"+copreal+"\t"+Clinea;
            if(valido)
                Clinea+="\t\t; "+aux.nombre + "\n";
            else Clinea+="\t\t; invalido\n";
        }
        else{                               //es una directiva
            Clinea = CCL+"\t"+cadena_directiva+"\t"+Clinea;
            if(valido)
                Clinea+="\t\t; directiva\n";
            else Clinea+="\t\t; invalido\n";
        }
        if(i  == 2)GuardarLst(Clinea);
        if(ENDCOD)
            break;
    }
    if(i & 1){
        for(auto e : etiquetas)
            cout << e.first << "\t" << e.second << endl;
    }
        cout << "\n\n\n\n\t\t\t\tTerminado\n\t\t\t\tGuardado en:\nC:\\Users\\jonat\\Desktop\\Seminario Traductores\\Practicas\\Practica5_ST\\P5.lst\n\n\n\n\n";
    GuardarEt(etiquetas);
    archivo.close();
    }
    return 0;
}
