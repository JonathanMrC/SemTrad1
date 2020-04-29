#ifndef MNEMONICO_H_INCLUDED
#define MNEMONICO_H_INCLUDED
#include<bits/stdc++.h>

using namespace std;

/*utilidades************************************************************************/

int ABS(int valor){
    if(valor < 0)
        valor*=-1;
    return valor;
}



void Complementoa1(string &s){
    for(auto &c : s){
        if(c == '1')
            c = '0';
        else
            c = '1';
    }
}

void Complementoa2(string &s){
    Complementoa1(s);
    bool unomas = false;
    for(int i = s.size()-1; i >= 0;--i){
        if(s[i] == '0'){
            unomas = true;
            s[i] = '1';
            break;
        }
        else
            s[i] = '0';
    }
    if(!unomas)
        s="1"+s;
}

string DectoBin(int valor){
    string s = "";
    bool neg = false;
    if(valor < 0)
        neg = true;
    valor = ABS(valor);
    while(valor){
        if(valor & 1)
            s+='1';
        else s+='0';
        valor>>=1;
    }
    reverse(s.begin(), s.end());
    if(neg)Complementoa2(s);
    return s;
}

bool esNum(char &c){                //es para saber si un char es un numero
    if(c >= '0' && c <= '9')
        return true;
    return false;
}

bool esABDXYS(string c){
    return c == "A" || c == "B" || c == "D" || c == "X" || c == "Y" || c == "SP";
}

bool esXYSPPC(string c){
    return c == "X" || c == "Y" || c == "SP" || c == "PC";
}

bool esDecValido(string cadena){
    if(cadena[0] == '-')
        cadena.erase(0, 1);
    if(esNum(cadena[0])){
        for(char car : cadena){
            if(!esNum(car))
                return false;
        }
        return true;
    }
    return false;
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
    if((modo == 2) && (minusc || mayus))
        return true;
    return false;
}

long long int potencia(int base,int pot){
    long long int res = 1;
    for(int i = 0; i < pot;++i)
        res*=base;
    return res;
}

bool es9bits(int valor){
    return valor <= 255 && valor >= -255;
}

int is8or16(int valor){ //si es un entero de 8 bits regresa 1, si es de 16 bit regresa 2, sino -1
    if((valor <= 255 && valor >= 0) || (valor <= 127 && valor >= -127))
        return 1;
    else if((valor <= 65535 && valor >= 0) || (valor <= 32767 && valor >= -32768))
        return 2;
    return -1;
}

bool MSBH(string s){
    char a = tolower(s[0]);
    return (a >= '8' || (a >= 'a' && a <= 'f'));
}

void CharppHex(char &a){             //aumenta el char como si fuera un hexadecimal
    if(a <= '8' && a >= '0'){
        a++;
        return;
    }
    if(a == '9'){
        a = 'a';
        return;    }
    if(a >= 'a' && a <= 'e'){
        a++;
        return;
    }    if(a == 'f'){
        a = '0';
        return;
    }
    cout << "El valor hexadecimal no esta en el formato correcto:" << a << " no es hexadecimal\n" ;
    a = '-';
    return;
}

int HextoDec(string valor, bool cmp2){
    char c;
    int val = 0;
    if(cmp2){       //si es requerido el complemento a 2
        if(MSBH(valor)){     //valido si el bit mas significativo esta encendido
            if(valor.size() == 2)  //veo si es un valor de 2 Bytes
                val-=256;
            else                    //en caso contrario es un valor de 4 Bytes
                val-=65536;
        }
    }
    reverse(valor.begin(), valor.end());//Hago reverse al valor
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '9')
            val+=(c-'0')*(potencia(16, i));
        else if(c >= 'a' && c <= 'f')
            val+=(10+ c-'a')*(potencia(16, i));
        else return 1e7;
    }
    return val;
}

int StringDectoIntDec(string valor){
    bool neg = false;
    if(valor[0] == '-'){
        neg = true;
        valor.erase(0, 1);
    }
    reverse(valor.begin(), valor.end());
    int val = 0, pot;
    for(unsigned int i = 0; i < valor.size();++i){
        if(valor[i] >= '0' && valor[i] <= '9'){
            pot = potencia(10, i);
            val+=(valor[i]-'0')*pot;
        }
        else return 1e7;
    }
    if(neg)
        val*=-1;
    return val;
}

int OcttoDec(string valor, bool cmp2){
    char c = tolower(valor[0]);
    int val = 0;
    bool neg = false;
    if(cmp2){
        if(c <= '7' && c >= '4'){
            neg = true;
            valor[0] = '0';
        }
    }
    reverse(valor.begin(), valor.end());
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '8')
            val+=(c-'0')*(potencia(8, i));
        else return 1e7;
    }
    if(neg && cmp2)
        val*=-1;
    return val;
}

int BintoDec(string valor, bool cmp2){
    bool neg = false;
    if(cmp2 && valor[0] == '1'){
        neg = true;
        valor[0] = '0';
    }
    reverse(valor.begin(), valor.end());
    int val = 0;
    char c;
    for(unsigned int i = 0; i < valor.size();++i){
        c = tolower(valor[i]);
        if(c >= '0' && c <= '1')
            val+=(c-'0')*(potencia(2, i));
        else return 1e7;
    }
    if(neg && cmp2)
        val*=-1;
    return val;
}

void Hexpp(string &CL, int a){      //formato de cadena++ en hexadecimal
//    cout << "Contenido de CL"<<CL <<"\n";
    if(a < 0)
        a = 65536 + a;
    char uno = CL[3], dos = CL[2], tres = CL[1], cuatro = CL[0];
    while(a--){
//        cout << "uno ->" << uno << endl;
        CharppHex(uno);
//        cout << "uno ->" <<uno << endl;
        if(uno == '0'){
//            cout << "dos ->" <<dos << endl;
            CharppHex(dos);
//            cout << "dos ->" <<dos << endl;
            if(dos == '0'){
//                cout << "tres ->" <<tres << endl;
                CharppHex(tres);
//                cout << "tres ->" <<tres << endl;
                if(tres == '0'){
//                    cout << "cuatro ->" <<cuatro<< endl;
                    CharppHex(cuatro);
//                    cout << "cuatro->" <<dos << endl;
                }
            }
        }
        CL[0] = cuatro;
        CL[1] = tres;
        CL[2] = dos;
        CL[3] = uno;
//        cout << "CL++ = " << CL << endl;
    }
}

string ConvertirAHex(string valor){         //convierte de cualquier base a hexadecimal
    if(!valor.size()) return "";
    char c = valor[0];
    if(esDecValido(valor)){           //es decimal
        string formato = "0000";
        Hexpp(formato, StringDectoIntDec(valor));
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
        Hexpp(formato, StringDectoIntDec(valor));
        return formato;
    }
    if(c == '$'){               //es hexa
        for(int i = 0; valor.size() < 4;++i)
            valor = "0"+valor;
        return valor;
    }
    if(c == '@'){                //es oct
        Hexpp(formato, OcttoDec(valor, 0));
        return formato;
    }
    if(c == '%'){                //es binario
        Hexpp(formato, BintoDec(valor, 0));
        return formato;
    }
    return "";
}

void Hexpp2char(string &cadena, int a){      //mismo que Hexpp pero dos caracteres solamente
    char uno = cadena[1], dos = cadena[0];
    while(a--){
        CharppHex(uno);
        if(uno == '0'){
            CharppHex(dos);
        }
        cadena[0] = dos;
        cadena[1] = uno;
        //cout << "CL++ = " << CL << endl;
    }
}

void StringtoUpper(string &s){//cambia una string de minusculas a mayusculas
    for(char &letra : s)
        letra = toupper(letra);
}

/*struct para los modos de direccion**************************************************/
struct ModoDir{
    string nombre, cop;
    int li;
    ModoDir(){};
    ModoDir(string s){nombre = s;};
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

/*clase Mnemonico*********************************************************************/
class Mnemonico{
    string instruccion;
    vector<ModoDir> ModosDir;

public:
    Mnemonico();
    Mnemonico(string nombre);
    Mnemonico(string instruccion, vector<ModoDir> modos);
    string getNombre();
    vector<ModoDir> getModos();
    void Add_MD(vector<ModoDir> ModosDir);
    void Add_VMD(ModoDir modo);
    //utilidades para los modos de direccionamiento
    ModoDir MnemoTieneMD(string MD);       //la instruccion puede usar este modo de direccionamiento
    //validar los modos de dir
    bool esINH(string operando);
    bool esIMM(string operando);
    bool esDIR(string operando);
    bool esEXT(string operando);
    bool esREL(string operando, string branch);
    bool esREL9(string operando);
    bool esIDX(string operando);
    bool esIDX1(string operando);
    bool es_IDX2_(string operando);
    bool esDIDX(string operando);

};

/*Implementacion de las funciones de la clase Mnemonico***********************************/

Mnemonico::Mnemonico(){};
Mnemonico::Mnemonico(string nombre){instruccion = nombre;};
Mnemonico::Mnemonico(string instruccion, vector<ModoDir> modos){this->instruccion = instruccion;this->ModosDir = modos;}

string Mnemonico:: getNombre(){
    return this->instruccion;
}

vector<ModoDir> Mnemonico:: getModos(){
    return ModosDir;
}

void Mnemonico:: Add_MD(vector<ModoDir> modos){
    this->ModosDir = modos;
}

void Mnemonico:: Add_VMD(ModoDir modo){
    this->ModosDir.push_back(modo);
}
//*************************************************************

ModoDir Mnemonico:: MnemoTieneMD(string MD){  //devuelve el modo de direccionamiento si el mnemonico usa ese modo de direccion
    for(auto modo : ModosDir){
        if(modo.nombre == MD)
            return modo;
    }
    ModoDir aux("noexiste");
    return aux;
}

//******************************************************************
bool Mnemonico:: esINH(string operando){
    /*en inherente no es necesario ningun operando
      por lo que el operando debe estar vacio*/
    for(auto caracter : operando){          //
        if( caracter != ' ')        //si hay algo que no sea un espacio, no puede ser inh
            return false;
    }                     //se devuelve el MD si es correcto
    return true;
}

bool Mnemonico:: esIMM(string operando){
    int valor = 1e5;
    if(operando[0] == '#'){            //si no lo tiene entonces no es valido
        operando.erase(0, 1);
        char c = operando[0];
        if(esDecValido(operando))               //es decimal
            valor = StringDectoIntDec(operando);
        operando.erase(0, 1);         //borro el indicador de la base, pero ya lo guarde en c
        if(c == '$')                            //es hexa
            valor = HextoDec(operando, 0);      //si es de 8 bits retorna 1
        if(c == '@')                       //es oct
            valor = OcttoDec(operando, 0);         //si es de 8 bits retorna 1
        if(c == '%')                       //es binario
            valor = BintoDec(operando, 0);        //si es de 8 bits retorna 1
    }
    return MnemoTieneMD("IMM").li-1 == is8or16(valor);
}
bool Mnemonico:: esDIR(string operando){
    char c = operando[0];
    if(esDecValido(operando))                    //es decimal
        return is8or16(StringDectoIntDec(operando)) == 1;   //si es de 8 bits return true
    operando.erase(0, 1);
    if(c == '$')                                 //es hexa
        return is8or16(HextoDec(operando, 0)) == 1;
    if(c == '@')                                 //es oct
        return is8or16(OcttoDec(operando, 0)) == 1;
    if(c == '%')                                 //es binario
        return is8or16(BintoDec(operando, 0)) == 1;
    return false;
}
bool Mnemonico:: esEXT(string operando){
    char c = operando[0];
    if(esDecValido(operando))                                   //es decimal
        return is8or16(StringDectoIntDec(operando)) != -1;
    operando.erase(0, 1);
    if(c == '$')                                                //es hexa
        return is8or16(HextoDec(operando, 0)) != -1;
    if(c == '@')                                                //es oct
        return is8or16(OcttoDec(operando, 0)) != -1;
    if(c == '%')                                                //es binario
        return is8or16(BintoDec(operando, 0)) != -1;
    return false;
}

bool Mnemonico:: esREL(string operando, string branch){
    char c = operando[0];
    int longinstr = MnemoTieneMD("REL").li, valor;
    if(tolower(branch[0]) == 'l')       // si es una instruccion de salto largo
        longinstr -= 2;
    if(esDecValido(operando))           //es decimal
        valor = StringDectoIntDec(operando);
    else{
        operando.erase(0, 1);
        if(c == '$')
            valor = HextoDec(operando, 0);
        else if(c == '@')
            valor = OcttoDec(operando, 0);
        else if(c == '%')
            valor = BintoDec(operando, 0);
        else valor = 1e5;
    }
    if(longinstr >= is8or16(valor) && (is8or16(valor) != -1))
        return true;
    return false;
}

bool Mnemonico:: esREL9(string operando){
    //este modo tiene dos parametros, el primero valida que sea un registro a, b, d, x, y o sp
    //**************************validando el primer parametro*********************************
    int pos = operando.find(',');
    if(pos == -1) return false;
    if(!esABDXYS(operando.substr(0, pos))) return false;
    operando.erase(0, pos+1);
    //**************************validando el segundo parametro********************************
    int longinstr = MnemoTieneMD("REL9").li, valor;
    char c = operando[0];
    if(esDecValido(operando))           //es decimal
        valor = StringDectoIntDec(operando);
    else{
        operando.erase(0, 1);
        if(c == '$')
            valor = HextoDec(operando, 0);
        else if(c == '@')
            valor = OcttoDec(operando, 0);
        else if(c == '%')
            valor = BintoDec(operando, 0);
        else valor = 1e5;
    }
    if(longinstr >= is8or16(valor) && (is8or16(valor) != -1))
        return true;
    return false;
}

bool Mnemonico:: esIDX(string operando){//Es la primera formula
    string parte1 = "", parte2;
    int pos = operando.find(','), valor;   //busco la coma del operando
    if(pos == -1) return false;     //si no hay coma entonces el operando no es correcto para idx
    parte1 = operando.substr(0, pos);
    parte2 = operando.substr(pos+1, operando.size()-pos+1);
    if(!esDecValido(parte1) && (parte1 != "")) return false;//si el primer valor no es un numero
    valor = StringDectoIntDec(parte1);     //obtengo el valor del numero en decimal
    if(valor < -16 || valor > 15) return false;             //veo si es valido para idx
    if(!esXYSPPC(parte2))   return false;   //si el segundo valor no es un registro valido
    return true;                            //en caso de no entrar a ninguno de los if anteriores es valido
}
bool Mnemonico:: esIDX1(string operando){//Es la segunda formula
    string parte1, parte2;
    int pos = operando.find(','), valor;    //busco la coma del operando
    if(pos == -1) return false;             //si no hay coma entonces el operando no es correcto para idx
    parte1 = operando.substr(0, pos);
    parte2 = operando.substr(pos+1, operando.size()-pos+1);
    if(!esDecValido(parte1)) return false;        //si el primer valor no es un numero
    valor = StringDectoIntDec(parte1);      //obtengo el valor del numero en decimal
    if(valor < -65536 || valor > 65535) return false;             //veo si es valido para idx1
    if(!esXYSPPC(parte2)) return false;     //si el segundo valor no es un registro valido
    return true;                            //en caso de no entrar a ninguno de los if anteriores es valido
}

bool Mnemonico:: es_IDX2_(string operando){//Es la tercera formula
    string parte1, parte2;
    if(operando[0] != '[' || operando[operando.size()-1] != ']') return false;
    operando.erase(0, 1);                   //Elimino  los corchetes
    operando.erase(operando.size()-1, 1);   //Eliminio el corchete final
    int pos = operando.find(','), valor;    //busco la coma del operando
    if(pos == -1) return false;             //si no hay coma entonces el operando no es correcto para idx
    parte1 = operando.substr(0, pos);
    parte2 = operando.substr(pos+1, operando.size()-pos+1);
    if(!esDecValido(parte1)) return false;        //si el primer valor no es un numero
    valor = StringDectoIntDec(parte1);      //obtengo el valor del numero en decimal
    if(valor < 0 || valor > 65535) return false;             //veo si es valido para idx1
    if(!esXYSPPC(parte2)) return false;     //si el segundo valor no es un registro valido
    return true;                            //en caso de no entrar a ninguno de los if anteriores es valido
}

bool Mnemonico:: esDIDX(string operando){//Es la sexta formula
    string parte1, parte2;
    if(operando[0] != '[' || operando[operando.size()-1] != ']') return false;
    operando.erase(0, 1);                   //Elimino  los corchetes
    operando.erase(operando.size()-1, 1);   //Eliminio el corchete final
    int pos = operando.find(','), valor;    //busco la coma del operando
    if(pos == -1) return false;             //si no hay coma entonces el operando no es correcto para idx
    parte1 = operando.substr(0, pos);
    if(parte1 != "D") return false;     //si el segundo valor no es un registro valido
    parte2 = operando.substr(pos+1, operando.size()-pos+1);
    if(!esXYSPPC(parte2)) return false;     //si el segundo valor no es un registro valido
    return true;                            //en caso de no entrar a ninguno de los if anteriores es valido
}


#endif // MNEMONICO_H_INCLUDED
