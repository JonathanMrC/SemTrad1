#include <bits/stdc++.h>
#include"Mnemonico.h"
using namespace std;

/*
En el primero se deben leer las etiquetas y crear el contador de las localidades
En el segundo paso ya se calcula si es valido la instruccion
*/

void InsertarMn(vector<Mnemonico> &instrucciones){
    ifstream archivo("TABOP.txt", ios::in);
    vector<ModoDir> modos;
    string cadena, modo, cop, cli, instruccion;
    int li, tam;
    unsigned int pos;
    if(archivo.fail())
        cout << "No se encontro el archivo de los mnemonicos en la carpeta del proyecto\nDireccion:\nTABOP.txt";
    else{
        while(getline(archivo, cadena)){
            modos.clear();
            li = pos = 0;
            instruccion = "";
            tam = cadena.size();
            for(; pos < tam && cadena[pos] != '|';++pos)
                instruccion += cadena[pos];
            ++pos;
            for(; pos < tam;++pos){
                modo = cop = cli = "";
                for(; cadena[pos] != '%' && pos < tam; ++pos)
                    modo+=cadena[pos];
                ++pos;
                for(; cadena[pos] != '$' && pos < tam; ++pos)
                    cop+=cadena[pos];
                ++pos;
                for(;cadena[pos] != '|' && pos < tam;++pos)
                    cli+=cadena[pos];
                li = StringDectoIntDec(cli);
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
    archivo.open("TABSIN.txt", ios::out);
    for(auto it : etiquetas)
        archivo << it.first << "\t" << it.second << endl;
    archivo.close();
}

void GuardarLst(string s){                              //añade al archivo.lst una cadena
    ofstream archivo;
    archivo.open("P7.lst", ios::app);
    archivo<<s;
    archivo.close();
}

bool esEtiqueta(string s){
    bool b = false;             //bandera para saber si ya hubo un ':' en la cadena
    if(!esLetra(s[0], 2) && s[0] != '_')//si el primer caracter no es ni letra ni '_' no es etiqueta
        return false;
    s.erase(0, 1);
    for(auto candidato : s){
        if(!b){                 //si no ha aparecido el ':' entonces todavia es etiqueta
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
    ModoDir aux, tmp("noexiste");
    aux = instruccion.MnemoTieneMD("INH");
    if(aux.nombre != "noexiste"){
        if(instruccion.esINH(operando))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("IMM");
    if(aux.nombre != "noexiste"){
        if(instruccion.esIMM(operando))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("DIR");
    if(aux.nombre != "noexiste"){
        if(instruccion.esDIR(operando))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("EXT");
    if(aux.nombre != "noexiste"){
        if(instruccion.esEXT(operando))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("REL");
    if(aux.nombre != "noexiste"){
        if(instruccion.esREL(operando, instruccion.getNombre()))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("REL9");
    if(aux.nombre != "noexiste"){
        if(instruccion.esREL9(operando))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("IDX");
    if(aux.nombre != "noexiste"){
        if(instruccion.esIDX(operando))
            return aux;
        else tmp = aux;
    }
    aux = instruccion.MnemoTieneMD("IDX1");
    if(aux.nombre != "noexiste"){
        if(instruccion.esIDX1(operando))
            return aux;
        else tmp = aux;
    }
    return tmp;
}

bool esDirectiva(string directiva, string operando, bool HayEti, string etiqueta, string &CL, string &cadena_dir, bool &ENDCOD, map<string, string> &etiquetas){
    StringtoUpper(directiva);
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
            etiquetas[etiqueta] = "$"+ConvertirAHex(operando);
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
            for(;operando[pos] != ' ' && pos < operando.size();++pos)
                s+=operando[pos];
            cantidad = StringDectoIntDec(s);
            Hexpp(CL, cantidad);
            for(int i = 0; i < cantidad;++i)
                cadena_dir +=dos+' ';
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
    //*********************************************************************
    for(pos = 0; pos < linea.size() && linea[pos] == ' ';++pos);    //avanzo hasta encontrar el inicio de la instruccion
    linea.erase(0, pos);                                            //borro esa parte inservible
    //*********************************************************************
    pos = linea.find(' ');                                          //encuentro el fin de la instruccion
    instruccion = linea.substr(0, pos);                             //guardo la instruccion
    linea.erase(0, pos);                                            //elimino la instruccion de la cadena linea
    //*********************************************************************
    for(pos = 0; pos < linea.size() && linea[pos] == ' ';++pos);    //busco el inicio del operando
    linea.erase(0, pos);                                            //elimino la basura
    //*********************************************************************;
    int sig;

    for(pos = 0; pos < linea.size();++pos){                         //recorro lo restante de la cadena, ya que en teoria solo queda el operando
        if(linea[pos] == ' '){                                      //si encuentro un espacio, lo voy a borrar ya que no me es necesario
            for(sig = pos; linea[sig] == ' ' && sig < linea.size();++sig);//recorro la linea mientras encuentre espacios
            linea.erase(pos, sig-pos);                              //borro los n espacios que encontre
        }
    }
    operando = linea;
    return;
}

int main()
{
    ifstream archivo;
    //reinicio el .lst y el tabsim
    ofstream lst, TABSIN;
    lst.open("P7.lst", ios::trunc);
    lst.close();
    TABSIN.open("TABSIN.txt", ios::trunc);
    TABSIN.close();
    //estructuras de datos
    vector<Mnemonico> instrucciones;
    Mnemonico mnemonico;
    map<string, string> etiquetas;
    map<string, string> rr = {{"X", "00"},{"Y", "01"},{"SP", "10"},{"PC", "11"}};
    //variables
    string linea, comentario, Clinea, instruccion, operando, CL = "0000", CCL, cadena_directiva = "", candidato, cop, copreal;
    int pos;
    bool valido = true, ENDCOD, BanderaesInstruc;
    //Inicializaciones
    InsertarMn(instrucciones);

    for(int i = 1; i <= 2;++i){  cout << "Paso " << i << endl;
    CL = "0000";
    archivo.open("P7.asm", ios::in);
    if(!archivo.good()){                                         //si el archivo no esta bien
        cout << "No se encontro el archivo o esta dañado\nEn la ruta:\n\t\t\tP7.asm";
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
        if(pos != -1){                                          //si pos es != de -1 hay un comentario
            comentario = linea.substr(pos, linea.size()-pos);   //guardo el cometario por si acaso
            linea.erase(pos, linea.size()-pos);                 //elimino el cometario de la cadena
        }
        if(linea[0] != ' '){                                  //es etiqueta si no hay un espacio al principio de la cadena
            pos = linea.find(' ');                            //regresa la posicion donde aparece el primer espacio por lo tanto termina la etiqueta
            if(linea.substr(0, pos) == "")                    //si es una linea con un enter, saltamos a la siguiente linea
                continue;
            candidato = linea.substr(0, pos);
            linea.erase(0, pos);                              //borro la etiqeuta de la linea
            if(!esEtiqueta(candidato))
                valido = false;
            else{
                //elimino el ':' de la cadena candidato en caso de que lo tenga
                for(pos = 0; pos < candidato.size() && candidato[pos] != ':';++pos);
                candidato = candidato.substr(0, pos);
                etiquetas[candidato] = "$"+CL;
                //guarda en las cadenas instruccion y operando lo que puede o debe ser la instruccion y operando
                FormatoInstrOper(instruccion, operando, linea);
                if(etiquetas.count(operando) != 0)
                    operando = etiquetas[operando];
                mnemonico = ExisteInstruccion(instruccion, instrucciones);
                if(mnemonico.getNombre() != "-")
                {
                    BanderaesInstruc = true;
                    aux = esOp(mnemonico, operando);
                    if(aux.nombre != "noexiste")
                        Hexpp(CL, aux.li);                    else
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
            mnemonico = ExisteInstruccion(instruccion, instrucciones);
            if(mnemonico.getNombre() != "-")
            {
                BanderaesInstruc = true;
                aux = esOp(mnemonico, operando);
                if(aux.nombre != "noexiste"){
                    Hexpp(CL, aux.li);
                }
                else
                    valido = false;
            }
            else                       //es directiva
                valido = esDirectiva(instruccion, operando, 0, candidato, CL, cadena_directiva, ENDCOD, etiquetas);                                                           //es una directiva
        }
        if(BanderaesInstruc){                       //es una instruccion
            cop = "0000", copreal = aux.cop;
            int indexcodreal = 3, ope, iterador;
            string registro;
            if(aux.nombre == "REL"){
                ope = HextoDec(ConvertirAHex(operando), 0); //convierte el operando a decimal
                ope = ope - HextoDec(CL, 0);                //realiza la resta entre el operando y el contador de localidades
                Hexpp(cop, ope);                            //aumenta el cop, ope veces
                //si la longitud de instruccion es 2
                //Y el resultado de la resta es positivo pero representado en hex es negativo
                //O ope es menor a -128 entonces es invalido por que es de 8 bits
                if(aux.li == 2 && ((ope >= 0 && MSBH(cop)) || (ope < -128)))
                    valido = false;
            }
            else if(aux.nombre == "REL9"){
                iterador = operando.find(',');//obtengo la ultima posicion antes de la coma
                registro = operando.substr(0, iterador);//hago una subcadena donde obtengo el que en teoria es el registro
                StringtoUpper(registro);                //lo convierto a mayus
                if(iterador != -1){                     //si hay una coma entonces puede ser valido
                    operando.erase(0, iterador+1);      //el operando es lo que sigue de la coma
                    if(etiquetas.count(operando) != 0) //si mi mapa de etiquetas contiene el operando
                        operando = etiquetas[operando]; //cambio el nombre de la etiqueta por el valor
                    operando = ConvertirAHex(operando); //Lo combierto a a hexadecimal y le doy formato
                    if(esABDXYS(registro)){                 //Compruebo si es un registro
                        ope = HextoDec(operando, 1);        //Lo combierto a decimal para realizar la resta
                        ope = ope - HextoDec(CL, 0);        //Realizo la resta del operando con el contador de localidades
                        Hexpp(cop, ope);                    //aumento el cop, ope veces
                                        //Si llegue hasta aca, entonces es valido todo, solo hago algunas validaciones extra del rel9
                        if(registro == "A")
                            copreal[4] = '0';
                        else if(registro == "B")
                            copreal[4] = '1';
                        else if(registro == "D")
                            copreal[4] = '4';
                        else if(registro == "X")
                            copreal[4] = '5';
                        else if(registro == "Y")
                            copreal[4] = '6';
                        else if(registro == "SP")
                            copreal[4] = '7';
                        if(ope < 0)
                            copreal[3] = '9';
                        else copreal[3] = '8';
                    }
                    else valido = false;                //Si el primer operando no es un registro entonces es invalido
                }
                else valido = false;    //es invalido ya que solo hay un operador -> el campo del registro
            }
            else if(aux.nombre == "IDX"){//si el modo es IDX de 8 bits
                iterador = operando.find(',');
                string n = operando.substr(0, iterador), r, binario = "rr0nnnnn";
                r = operando.substr(iterador+1, operando.size()-iterador+1);
                binario = DectoBin(StringDectoIntDec(n));
                for(int i = binario.size(); i < 5;++i)
                    binario = '0'+binario;
                binario = rr[r]+"0"+binario;
                Hexpp(cop, BintoDec(binario, 0));

            }
            else if(aux.nombre == "IDX1"){//si el modo es IDX1 de 9 bits y 16 bits
                cop = "00";
                iterador = operando.find(',');
                string n = operando.substr(0, iterador), r, binario = "", z = "0", s = "0";
                r = operando.substr(iterador+1, operando.size()-iterador+1);
                int valor = StringDectoIntDec(n);
                if(valor < -256 || valor > 255)
                    z = "1";
                if(valor < 0) s = "1";
                binario = "111"+rr[r]+"0"+z+s;
                Hexpp2char(cop, BintoDec(binario, 0));
                if(z == "1")
                    cop += ""+ConvertirAHex(n);
                else
                    cop += ""+ConvertirAHex(n).substr(2, 2);
            }
            else                        //si no es ningun relativo entonces el operando no realiza ningun cambio mas que el de pasarlo a hexadecimal
                cop = ConvertirAHex(operando);
            //Un for donde cambio los valores del copreal por los obtenidos del calculo
            for(int i = copreal.size()-1;indexcodreal >= 0 && i >= 0;--i)
            {
                if(esLetra(copreal[i], 1))//solo cambio los valores del cop real si estos son minusculas
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
        if(i == 2)GuardarLst(Clinea);   //de esta manera solo guardo el .lst una vez
        if(ENDCOD)                      //si se encontro la directiva END, termina el analisis
            break;
    }
    if(i & 1)
        for(auto e : etiquetas)
            cout << e.first << "\t" << e.second << endl;
    else cout << "\n\n\n\n\t\t\t\tTerminado\n\t\t\t\tGuardado en:\n\t\t\t\tP7.lst\n\n\n\n\n";

    GuardarEt(etiquetas);//guardo las etiquetas
    archivo.close();//cierro el archivo .asm

    }
    return 0;
}
