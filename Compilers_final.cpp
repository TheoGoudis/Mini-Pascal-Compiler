/* Vlachogkountis Theofilos 1459
 * Euaggelopoulos Xrhstos 1595
 * 
 *Shmeiwseis 1
 * 1.Xrhsimopoihsame glwssa C++ alla kai kapoies sunarthseis ths C
 * 2.Prosthesame thn plhrh grammatikh pou efarmosame sto telos tou kwdika me th morfh sxoliou 
 * 3.Allaksame th dotheisa grammatikh se merika shmeia. O uposthrizomenos operator gia th sugkrish den einai o "==" (opws sth grammatikh pou mas dothike) alla o "=" (mias kai h anathesh timhs ginetai me to ":=" to sketo "=" den aksiopioutan pouthena)
 * 4.Prosthesame thn entolh select prosthetontas tous kanones <SELECT-STAT> kai <IS-EQUAL-PART>
 * 5.Merikous kanones de toys metatrepsame se sunasrthseis alla tous enswmatwsame se allous kanones.
 * oi kanones autoi einai: <RELATIONAL-OPER>, <OPTACTUALPARS>, <ADD-OPER>, <MUL-OPER>, <OPTIONAL-SIGN>
 * 
 * Shmeiwseis 2
 * 1.Xrhsimopoihsame Vectors kai oxi Lists opote kapoies sunarthseis de mas xreiasthkan opws oi makelist kai emptylist
 * 
 * Shmeiwseis 3
 * 1.Compile ginetai me thn entolh g++ -std=c++11 ./Compiler_3final_Vlachogkountis_Euaggelopoulos.cpp
 * 2.To programma ginetai compile alla otan to ekteloume me parametro ena test file mas petaei segmentation fault
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

//----TOKENID DEFINES
#define	idtk	100	//variable or function name (ID)
#define	numtk	101	//number (CONSTANT)
#define	addtk	200	// +  addition
#define	subtk	201	// -  subtraction
#define	multk	202	// *  multiplication
#define	divtk	203	// /  division
#define	smatk	204	// <  smaller than
#define	bigtk	205	// >  bigger than
#define	equtk	206	// =  equal to
#define	soetk	207	// <= smaller or equal
#define	boetk	208	// >= bigger or equal
#define	diftk	209	// <> different than
#define	asstk	210	// := assign value 
#define	semtk	300	// ;  semicolon
#define	comtk	301	// ,  comma
#define	paotk	302	// (  parentheses open
#define	pactk	303	// )  parentheses close
#define	sbotk	304	// [  square bracket open
#define	sbctk	305	// ]  square bracket close
#define coltk	306	// :  colon
#define	progtk	400	// program
#define	begitk	401	// begin
#define	endxtk	402	// end
#define	constk	403	// const
#define	varxtk	404	// var
#define	proctk	405	// procedure
#define	functk	406	// function
#define	ifxxtk	407	// if
#define	thentk	408	// then
#define	elsetk	409	// else
#define	whiltk	410	// while
#define	doxxtk	411	// do
#define	forxtk	412	// for
#define	toxxtk	413	// to
#define	steptk	414	// step
#define	calltk	415	// call
#define	printk	416	// print
#define	inputk	417	// input
#define	orxxtk	418	// or
#define	andxtk	419	// and
#define	notxtk	420	// not
#define retutk	421 // return
#define	seletk	422 // select
#define enseltk	423 // endselect
#define eoftk	001	//eof
#define errtk	000	//error

FILE *finp;
int lineCount;
const string reswords_array[] = {"program","begin","end","const","var","procedure","function","if","then","else","while","do","for","to","step","call","print","input","or","and","not","return","select", "endselect"};
const string symb = ";,()[]";
const string oper = "+-*/=<>:";
int tokenid;
string token;

//Data structures and variables for code generating.
typedef struct Quad Quad;
struct Quad{
	int Qpos;
	string V1;
	string V2;
	string V3;
	string V4;
	Quad(int pos, string i1, string i2, string i3, string i4){
		Qpos=pos;
		V1=i1;
		V2=i2;
		V3=i3;
		V4=i4;
	}
};

enum TEnt{
	FUNC,		//function
	PAR,		//parameter
	VAR,		//variable
	CVAR,		//constant
	TVAR		//temp variable
};

enum TPF{
	PROC,		//procedure
	FUNCC		//function
};

enum TPar{
	VAL,
	REF,
	DEF
};
enum TPar PDEF=DEF;

typedef struct Argument Argument;
struct Argument{
	string Name;
	TPar Ptype;
};

typedef struct Entity Entity;
struct Entity{
	string Name;
	TEnt Etype;
	//variable+constant
	int OffsetVar=0;
	//constant
	string Cvalue;
	//temporary variable
	int OffsetTemp=0;
	//parameter
	int OffsetPar=0;
	TPar Ptype;
	//function
	vector <Argument> MArgument;
	int Fquad;
	int FrLength=0;
};

typedef struct Scope Scope;
struct Scope{
	string Name;
	int NestingLevel;
	int CurrentOffset;
	vector <Entity> MEntity;
};

vector<Quad> MQuads;
Quad *CQuad=NULL;
int MQcounter=-1;
int MQnextquad;
string TempVar;
int TempVarCounter=-1;
vector <Scope> MScope;
Scope *CScope=NULL;
Entity *CEntity=NULL;
vector<string> PFnames;	//Program/Funtion names
int FFLength=0;		//function frame length

//----FUNCTIONS' DEFINITIONS
void spaceEater();
int lex();
void program();
void programblock();
void block();
void declarations();
void constdecl();
void assignlist();
void assignconst();
void vardecl();
void varlist();
void subprograms();
void procorfunc(enum TPF PF);
void procorfuncbody();
void formalpars();
void formalparlist();
void formalparitem();
void sequence();
void blockorstat();
void statement();
void assignmentstat();
void ifstat(vector<int>& StList);
void elsepart();
void whilestat(vector<int>& StList);
void forstat(vector<int>& StList);
void steppart(string& StList);
void callstat();
void printstat();
void inputstat();
void returnstat();
void selectstat(vector<int>& StList);
void isequalpart(string IId, vector<int> iStList, vector<int> IFalseL);
void actualpars(enum TPF PF, string name);
void actualparlist(enum TPF PF, string name);
void actualparitem();
void condition(vector<int>& TrueL, vector<int>& FalseL);
void boolterm(vector<int>& TrueL, vector<int>& FalseL);
void boolfactor(vector<int>& TrueL, vector<int>& FalseL);
void expression(string& IValueS);
void term(string &IValueT);
void factor(string &IValueF);

void nextQuad();
void genQuad(string I1, string I2, string I3, string I4);
void newTemp();
void merge(vector<int>& v1, vector<int>& v2);
void backpatch(vector<int> BList, int IQpos);

void addScope(string IScopeName);
void addEntity(string IEntityName, enum TEnt ITEnt, string ICValue, int FRLength);
void addArgument(string IArgumentName, enum TPar ITPar);
void deleteScope();
void searchVariableS(string name);
Entity* searchNameE(string name);
void startQuadF(string pname);


//--Variables and Funtions' declarations for Generating Final Code

fstream ffinal ("Final.obj");
int fLabelf=-1;
int returnf=-1;
int argcounter=0;
Quad *FCQuad=NULL;
int enestingLevel=0;

bool isNum(string& isstr);
void gnlvcode(string gtname);
void loadvr(string var,int reg);
void storerv(string var,int reg);
void generateFinalCode();
void createFFirstLabel();
void checkFunCall(string funcname);


//----FUNCTIONS
int main(int argc, char** argv){
	lineCount=0;
	if(argc>2){
		cout<<"\nError(main): Too many arguments. (argc>2. This programm supports only one argument, which is the path of the source code file for compilation) The right syntax is  ...$ 'COMPILER NAME' 'PATH OF SOURCECODE FILE'\n";
		return(0);
	}
	else if(argc<2){
		cout<<"\nError(main): You didnt give a source code file for compilation. The right syntax is  ...$ 'COMPILER NAME' 'PATH OF SOURCECODE FILE'\n"; 
		return(0);
	}
	finp=fopen(argv[1],"r");
	if(finp==NULL){
		cout<<"\nError(main): Can't open source code file. (fopen fail.)\n";
		return(0);
	}
	
	ffinal<<"L0:\t jmp\t L$\n";
	
	program();
	
	ofstream Qout ("Quads.txt");
	size_t qsi=MQuads.size();
	int qi=(int)qsi;
	for(int i=0;i<qi;i++)
		Qout<<MQuads.at(i).Qpos<<" "<<MQuads.at(i).V1<<" "<<MQuads.at(i).V2<<" "<<MQuads.at(i).V3<<" "<<MQuads.at(i).V4<<endl;
	Qout.close();
	
	if(tokenid==errtk)
		cout<<"\nThere has been an error. Please fix it and try to compile again.\n";
	else
		cout<<"\nCompilation was successful!\n";
	
	fclose(finp);
	return(0);
}

void spaceEater(){
	int gret=0;
	do{
		gret=getc(finp);
		if(gret==10)
					lineCount++;
	}while((gret==9)||(gret==32)||(gret==10));
	if(gret==47){
		int temp=gret;
		gret=getc(finp);
		if(gret==42){
			temp=-1;
			while(temp<0){
				gret=getc(finp);
				while(gret==42){
					gret=getc(finp);
					if(gret==47)
						temp=1;
				}
			}
		}
		else{
			ungetc(gret,finp);
			ungetc(temp,finp);
		}
	}
	else
		ungetc(gret,finp);
}

int checkNumValidity(){									//Check if number is a 32bit number.  number<=|2147483647|
	long int i=2147483647;
	long int j;
	char * pEnd;
	j=strtol(token.c_str(),&pEnd,10);
	if(j>i)
		return(1);
	return(0);
}

int lex(){
	unsigned int i;
	int gret=0;
	string temp="";
	token.clear();
	spaceEater();
	gret=getc(finp);
	if(gret==EOF)												//Found end of file 
		return(eoftk);
	else if((64<gret && gret<91) || (96<gret && gret<123)){		//If first element of token is a letter 
		do{
			token+=(char)gret;
			gret=getc(finp);
		}while((64<gret && gret<91) || (96<gret && gret<123) || (47<gret && gret<58));
		ungetc(gret,finp);
		if(token.length()>30)
			token.resize(30);
		i=0;
		while(i<24){
			if(token==reswords_array[i])						//Found reserved word. tokenid = token of found reserved word
				return(400+i);
			i++;
		}
		return(idtk);											//Found name. tokenid = idtk
	}
	else if(47<gret && gret<58){								//If first element of token is a digit
		do{
		token+=(char)gret;
		gret=getc(finp);
		}while(47<gret && gret<58);
		ungetc(gret,finp);
		if(checkNumValidity()==1){
			cout<<"\nError(lex): line "<<lineCount<<": Number of higher value than supported. MiniPascal supports only 32bit integer numbers.";
			return(errtk);
		}
		return(numtk);											//Found number. tokenid = numtk
	}
	else if((i=symb.find((char)gret))!=string::npos)	//Found one of ; , ( ) [ ]   tokenid = token of found symbol
		switch(i){
			case 0:
				token=(char)gret;
				return(semtk);
				break;
			case 1:
				token=(char)gret;
				return(comtk);
				break;
			case 2:
				token=(char)gret;
				return(paotk);
				break;
			case 3:
				token=(char)gret;
				return(pactk);
				break;
			case 4:
				token=(char)gret;
				return(sbotk);
				break;
			case 5:
				token=(char)gret;
				return(sbctk);
				break;
			default:
				cout << "\nError(lex): line "<<lineCount<<": found $gret in $sym string but didnt get caught in switch.";
				return(errtk);
				break;
		}
	else if((i=oper.find((char)gret))!=string::npos){
		token=(char)gret;
		switch(i){
			case 0 :
				return(addtk);									//Found + . tokenid = addtk 
				break;
			case 1 :
				return(subtk);									//Found - . tokenid = subtk
				break;
			case 2 :
				return(multk);									//Found * . tokenid = multk
				break;
			case 3 :
				return(divtk);									//Found / . tokenid = divtk
				break;				
			case 4 :
				return(equtk);									//Found = . tokenid = equtk
				break;
			case 5 :
				gret=getc(finp);
				if(gret==61){									//Found <= . tokenid = soetk
					token+=(char)gret;
					return(soetk);
				}
				else if(gret==62){
					token+=(char)gret;
					return(diftk);								//Found <> . tokenid = diftk
				}
				else{
					ungetc(gret,finp);
					return(smatk);								//Found < . tokenid = smatk
				}
				break;
			case 6 :
				gret=getc(finp);
				if(gret==61){
					token+=(char)gret;
					return(boetk);								// Found >= . tokenid = boetk
				}
				else{
					ungetc(gret,finp);
					return(bigtk);								//Found < . tokenid = bigtk
				}
				break;			
			case 7 :
				gret=getc(finp);
				if(gret==61){
					token+=(char)gret;
					return(asstk);								//Found := . tokenid = asstk
				}
				else{
					ungetc(gret,finp);
					return(coltk);
				}
				break;
			default:
				cout << "\nError(lex): line "<<lineCount<<": found $gret in $operm string but didnt get caught in switch.";
				return(errtk);
				break;
		}	
	}
	else{
		token=(char)gret;
		cout<<"\nError(lex): line "<<lineCount<<": Found character "<<token<<" which is not supported by MiniPascal.";
		return(errtk);
	}
	return(0);
}

void program(){
	
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	else if(tokenid==progtk){
		
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		if(tokenid!=idtk){
			cout<<"Error(syn:program): line "<<lineCount<<": Expected \"program name\", instead got "<<token<<". (MiniPascal requires a program name after the reserved word \"program\")";
			tokenid=errtk;
			exit(0);
		}
		else{
			
			PFnames.push_back(token);
			addScope(token);
			
			tokenid=lex();
			if(tokenid==errtk){
				cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
				exit(0);
			}
			programblock();
		}
	}
	else{
		cout<<"Error(syn:program): line "<<lineCount<<":  Expexted keywork \"program\", instead got "<<token<<". (MiniPascal requires the reserved word \"program\" in the beginning)";
		tokenid=errtk;
		exit(0);
	}
	
	
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid==eoftk)
		cout<<"Syntax check completed!";
	else{
		cout<<"Error(syn:program): line "<<lineCount<<":  Expected EOF, instead got "<<token<<". (Please read the MiniPascal syntax)";
		tokenid=errtk;
	}
}

void programblock(){
	declarations();
	subprograms();
	
	string pname=PFnames.back();
	genQuad("begin_block",pname,"_","_");
	FCQuad=CQuad;
	size_t i =1;
	if(i==PFnames.size())
		startQuadF(pname);
	block();
	if(i==PFnames.size())
		genQuad("halt","_","_","_");
	pname=PFnames.back();
	genQuad("end_block","pname","_","_");
	PFnames.pop_back()
	generateFinalCode();
	deleteScope();
	if(!MScope.empty())
		CEntity->FrLength=FFLength;
}

void block(){
	if(tokenid==begitk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		sequence();
		if(tokenid==endxtk){
			tokenid=lex();
			if(tokenid==errtk){
				cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
				exit(0);
			}
		}
		else{
			cout<<"Error(syn:block): line "<<lineCount<<":  Expected the keyword \"end\", instead got "<<token<<". (Please read the MiniPascal syntax)";
			tokenid=errtk;
			exit(0);
		}
	}
	else{
		cout<<"Error(syn:block): line "<<lineCount<<":  Expected the keyword \"begin\", instead got "<<token<<". (Please read the MiniPascal syntax)";
		tokenid=errtk;
		exit(0);
	}
}

void declarations(){
	while(tokenid==constk){
		constdecl();
	}
	while(tokenid==varxtk){
		vardecl();
	}
}

void constdecl(){
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	assignlist();
	if(tokenid!=semtk){
		cout<<"Error(syn:constdecl): line "<<lineCount<<":  Expected semicolon, instead got "<<token<<". (Please read the MiniPascal syntax on constant variable declarations)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
}

void assignlist(){
	assignconst();
	while(tokenid==comtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		assignconst();
	}
}

void assignconst(){
	if(tokenid!=idtk){
		cout<<"Error(syn:assigncinst): line "<<lineCount<<":  Expected constand variable ID, instead got "<<token<<". (Please read the MiniPascal syntax on constant variable declarations)";
		tokenid=errtk;
		exit(0);
	}
	else{
		string VarIDT=token;
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		if(tokenid!=asstk){
			cout<<"Error(syn:assignconst): line "<<lineCount<<":  Expected operator := , instead got "<<token<<". (Please read the MiniPascal syntax on constant variable declarations)";
			tokenid=errtk;
			exit(0);
		}
		else{
			tokenid=lex();
			if(tokenid==errtk){
				cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
				exit(0);
			}
			if(tokenid!=numtk){
				cout<<"Error(syn:assignconst): line "<<lineCount<<":  Expected number (to be assigned in constant), instead got "<<token<<". (Please read the MiniPascal syntax on constant variable declarations)";
				tokenid=errtk;
				exit(0);
			}
			searchVariableS(VarIDT);
			addEntity(VarIDT,CVAR,token,-1);
			tokenid=lex();
			if(tokenid==errtk){
				cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
				exit(0);
			}
		}
	}
}

void vardecl(){
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	varlist();
	if(tokenid!=semtk){
		cout<<"Error(syn:vardecl): line "<<lineCount<<":  Expected semicolon, instead got "<<token<<". (Please read the MiniPascal syntax on variable declarations)";
		tokenid=errtk;
		exit(0);
	}
	else{
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
}

void varlist(){
	if(tokenid!=idtk){
		cout<<"Error(syn:varlist): line "<<lineCount<<":  Expected variable ID, instead got "<<token<<". (Please read the MiniPascal syntax on variable declarations)";
		tokenid=errtk;
		exit(0);
	}
	else
	{
		searchVariableS(token);
		addEntity(token,VAR,"",-1);
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		while(tokenid==comtk){
			tokenid=lex();
			if(tokenid==errtk){
				cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
				exit(0);
			}
			if(tokenid!=idtk){
				cout<<"Error(syn:varlist): line "<<lineCount<<":  Expected variable ID, instead got "<<token<<". (Please read the MiniPascal syntax on variable declarations)";
				tokenid=errtk;
				exit(0);
			}
			searchVariableS(token);
			addEntity(token,VAR,"",-1);
		}
	}
	
}

void subprograms(){
	while((tokenid==proctk)||(tokenid==functk)){
		enum TPF PFtype;
		if(tokenid==proctk)
			PFtype=PROC;
		else
			PFtype=FUNCC;
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		procorfunc(PFtype);
	}
}

void procorfunc(enum TPF PF){
	if(tokenid!=idtk){
		cout<<"Error(syn:procorfunc): line "<<lineCount<<":  Expected procedure ID or function ID, instead got "<<token<<". (Please read the MiniPascal syntax on procedure and function declarations)";
		tokenid=errtk;
		exit(0);
	}
	else{
		PFnames.push_back(token);
		addEntity(token,FUNC,"",FFLength);
		addScope(token);
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		procorfuncbody();
	}
}

void procorfuncbody(){
	formalpars();
	programblock();
}

void formalpars(){
	if(tokenid!=paotk){
		cout<<"Error(syn:formalpars): line "<<lineCount<<":  Expected \"(\", instead got "<<token<<". (Please read the MiniPascal syntax on procedure and function declarations)";
		tokenid=errtk;
		exit(0);
	}
	else{
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	if((tokenid==idtk)||(tokenid==varxtk))
		formalparlist();
	if(tokenid!=pactk){
		cout<<"Error(syn:formalpars): line "<<lineCount<<":  Expected \")\", instead got "<<token<<". (Please read the MiniPascal syntax on procedure and function declarations)";
		tokenid=errtk;
		exit(0);
	}
	else{
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
}

void formalparlist(){
	formalparitem();
	while(tokenid==comtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		formalparitem();
	}
}

void formalparitem(){
	enum TEnt FTEnt=PAR;
	if(tokenid==idtk){		//BY VALUE
		enum TPar FTPar=VAL;
		addEntity(token,FTEnt,"",-1);
		CEntity->Ptype=FTPar;
		addArgument(token,FTPar);
		genQuad("par",token,"VAL","_");
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else if(tokenid==varxtk){		//BY REFERENCE
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		if(tokenid!=idtk){
			cout<<"Error(syn:formalparitem): line "<<lineCount<<":  Expected variable ID, instead got "<<token<<". (Please read the MiniPascal syntax on procedure and function declarations and variable declarations)";
			tokenid=errtk;
			exit(0);
		}
		enum TPar FTPar=REF;
		addEntity(token,FTEnt,"",-1);
		CEntity->Ptype=FTPar;
		addArgument(token,FTPar);
		genQuad("par",token,"REF","_");
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else{
		cout<<"Error(syn:formalparitem): line "<<lineCount<<":  Expected variable ID or keyword \"var\", instead got "<<token<<". (Please read the MiniPascal syntax on procedure and function declarations and variable declarations)";
		tokenid=errtk;
		exit(0);
	}
}

void sequence(){
	statement();
	while(tokenid==semtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		statement();
	}
}

void blockorstat(){
	if(tokenid==begitk)
		block();
	else
		statement();
}

void statement(){
	vector<int> StList;
	if(tokenid==idtk)
		assignmentstat();
	else if(tokenid==ifxxtk)
		ifstat(StList);
	else if(tokenid==whiltk)
		whilestat(StList);
	else if(tokenid==forxtk)
		forstat(StList);
	else if(tokenid==calltk)
		callstat();
	else if(tokenid==printk)
		printstat();
	else if(tokenid==inputk)
		inputstat();
	else if(tokenid==retutk)
		returnstat();
	else if(tokenid==seletk)
		selectstat(StList);
	else{
		cout<<"Error(syn:statement): line "<<lineCount<<":  Expected variable ID or one of the keywords  if, while, for, call, print, input, return, select , instead got "<<token<<". (Please read the MiniPascal syntax)";
		tokenid=errtk;
		exit(0);
	}
	if(!StList.empty()){
		nextQuad();
		backpatch(StList,MQnextquad);
	}
}

void assignmentstat(){
	string assignFrom;
	string assignTo=token;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=asstk){
		cout<<"Error(syn:assignmentstat): line "<<lineCount<<":  Expected operator \":=\", instead got "<<token<<". (Please read the MiniPascal syntax on value assignment in variables)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	expression(assignFrom);
	genQuad(":=",assignFrom,"_",assignTo);
}

void ifstat(vector<int>& StList){
	vector<int> ifTrueL;
	vector<int> ifFalseL;
	int FirstIfQuad;
	int FirstElseQuad;
	
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	condition(ifTrueL, ifFalseL);
	if(tokenid!=thentk){
		cout<<"Error(syn:ifstat): line "<<lineCount<<":  Expected keyword \"then\", instead got "<<token<<". (Please read the MiniPascal syntax on if statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	
	nextQuad();
	FirstIfQuad=MQnextquad;
	
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	blockorstat();
	nextQuad();
	StList.push_back(MQnextquad);
	genQuad("jump","_","_","_");
	
	nextQuad();
	FirstElseQuad=MQnextquad;
	
	if(tokenid==elsetk){
		elsepart();
	}
	
	backpatch(ifTrueL, FirstIfQuad);
	backpatch(ifFalseL, FirstElseQuad);
}

void elsepart(){
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	blockorstat();
}

void whilestat(vector<int>& StList){
	vector<int> whileTrueL;
	int FirstWhileQuad;
	int FirstCondQuad;
	
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	nextQuad();
	FirstCondQuad=MQnextquad;
	condition(whileTrueL, StList);
	if(tokenid!=doxxtk){
		cout<<"Error(syn:whilestat): line "<<lineCount<<":  Expected keyword \"do\", instead got "<<token<<". (Please read the MiniPascal syntax on while statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	nextQuad();
	FirstWhileQuad=MQnextquad;
	blockorstat();
	backpatch(whileTrueL,FirstWhileQuad);
	genQuad("jump","_","_",to_string(FirstCondQuad));
}

void forstat(vector<int>& StList){
	vector<int> for1L;
	int FFQuad;
	string VStep;
	string VId;
	string VEnd;
	
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=idtk){
		cout<<"Error(syn:forstat): line "<<lineCount<<":  Expected variable ID , instead got "<<token<<". (Please read the MiniPascal syntax on for statment)";
		tokenid=errtk;
		exit(0);
	}
	VId=token;
	assignmentstat();
	if(tokenid!=toxxtk){
		cout<<"Error(syn:forstat): line "<<lineCount<<":  Expected keyword \"to\" , instead got "<<token<<". (Please read the MiniPascal syntax on for statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	expression(VEnd);
	if(tokenid==steptk){
		string VStep;
		steppart(VStep);
	}
	nextQuad();
	FFQuad=MQnextquad;
	StList.push_back(MQnextquad);
	genQuad("=",VId,VEnd,"_");
	blockorstat();
	newTemp();
	genQuad("+",VId,VStep,TempVar);
	genQuad(":=",TempVar,"_",VId);
	genQuad("jump","_","_",to_string(FFQuad));
}

void steppart(string& StList){
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	expression(StList);
}

void callstat(){
	string PName;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=idtk){
		cout<<"Error(syn:callstat): line "<<lineCount<<":  Expected function ID , instead got "<<token<<". (Please read the MiniPascal syntax on call statment)";
		tokenid=errtk;
		exit(0);
	}
	PName=token;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	enum TPF TTPF=PROC;
	actualpars(TTPF,PName);
}

void printstat(){
	string PrintT;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=paotk){
		cout<<"Error(syn:printstat): line "<<lineCount<<":  Expected \"(\" , instead got "<<token<<". (Please read the MiniPascal syntax on print statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	expression(PrintT);
	genQuad("print",PrintT,"_","_");
	if(tokenid!=pactk){
		cout<<"Error(syn:printstat): line "<<lineCount<<":  Expected \")\" , instead got "<<token<<". (Please read the MiniPascal syntax on print statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
}

void inputstat(){
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=paotk){
		cout<<"Error(syn:inputstat): line "<<lineCount<<":  Expected \"(\" , instead got "<<token<<". (Please read the MiniPascal syntax on input statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=idtk){
		cout<<"Error(syn:inputstat): line "<<lineCount<<":  Expected variable ID , instead got "<<token<<". (Please read the MiniPascal syntax on input statment)";
		tokenid=errtk;
		exit(0);
	}
	genQuad("input",token,"_","_");
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=pactk){
		cout<<"Error(syn:inputstat): line "<<lineCount<<":  Expected \")\" , instead got "<<token<<". (Please read the MiniPascal syntax on input statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
}

void returnstat(){
	string RRet;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=paotk){
		cout<<"Error(syn:returnstat): line "<<lineCount<<":  Expected \"(\" , instead got "<<token<<". (Please read the MiniPascal syntax on return statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	expression(RRet);
	if(tokenid!=pactk){
		cout<<"Error(syn:returnstat): line "<<lineCount<<":  Expected \")\" , instead got "<<token<<". (Please read the MiniPascal syntax on return statment)";
		tokenid=errtk;
		exit(0);
	}
	genQuad("ret",RRet,"_","_");
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	
}

void selectstat(vector<int>& StList){
	vector<int> SFalseL;
	vector<int> TFalseL;
	string tId;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=idtk){
		cout<<"Error(syn:selectstat): line "<<lineCount<<":  Expected ID , instead got "<<token<<". (Please read the MiniPascal syntax on select statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(token!="is"){
		cout<<"Error(syn:selectstat): line "<<lineCount<<":  Expected \"is\" , instead got "<<token<<". (Please read the MiniPascal syntax on select statment)";
		tokenid=errtk;
		exit(0);
	}
	tId=token;
	isequalpart(tId,StList,TFalseL);
	while(token=="is"){
		nextQuad();
		backpatch(TFalseL,MQnextquad);
		isequalpart(tId,StList,TFalseL);
	}
	if(tokenid!=enseltk){
		cout<<"Error(syn:selectstat): line "<<lineCount<<":  Expected keyword \"endselect\" , instead got "<<token<<". (Please read the MiniPascal syntax on select statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
}

void isequalpart(string IId, vector<int> iStList, vector<int> IFalseL){
	string compVar;
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(token!="equal"){
		cout<<"Error(syn:isequalpart): line "<<lineCount<<":  Expected \"equal\" , instead got "<<token<<". (Please read the MiniPascal syntax on select statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if(tokenid!=toxxtk){
		cout<<"Error(syn:isequalpart): line "<<lineCount<<":  Expected \"to\" , instead got "<<token<<". (Please read the MiniPascal syntax on select statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	expression(compVar);
	nextQuad();
	IFalseL.push_back(MQnextquad);
	genQuad("<>",IId,compVar,"_");
	if(tokenid!=coltk){
		cout<<"Error(syn:isequalpart): line "<<lineCount<<":  Expected symbol \":\" , instead got "<<token<<". (Please read the MiniPascal syntax on select statment)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	block();
	nextQuad();
	iStList.push_back(MQnextquad);
	genQuad("jump","_","_","_");
}

void actualpars(enum TPF PF, string name){
	if(tokenid!=paotk){
		cout<<"Error(syn:actualpars): line "<<lineCount<<":  Expected \"(\", instead got "<<token<<". (Please read the MiniPascal syntax on call statement)";
		tokenid=errtk;
		exit(0);
	}
	tokenid=lex();
	if(tokenid==errtk){
		cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
		exit(0);
	}
	if((tokenid==varxtk)||(tokenid==addtk)||(tokenid==subtk)||(tokenid==numtk)||(tokenid==paotk)||(tokenid==idtk))
		actualparlist(PF,name);
	if(tokenid!=pactk){
		cout<<"Error(syn:actualpars): line "<<lineCount<<":  Expected \")\", instead got "<<token<<". (Please read the MiniPascal syntax on call statement )";
		tokenid=errtk;
		exit(0);
	}
	else{
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
}

void actualparlist(enum TPF PF, string name){
	actualparitem();
	while(tokenid==comtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		actualparitem();
	}
	if(PF==FUNCC){
		newTemp();
		string tmpR="RET";
		genQuad("par",TempVar,tmpR,"_");
	}
	genQuad("call","_","_",name);
	name=TempVar;
}

void actualparitem(){
	string tmp;
	if(tokenid==varxtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		if(tokenid!=idtk){
			cout<<"Error(syn:actualparitem): line "<<lineCount<<":  Expected variable ID, instead got "<<token<<". (Please read the MiniPascal syntax )";
			tokenid=errtk;
			exit(0);
		}
		genQuad("par",token,"REF","_");
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else{
		expression(tmp);
		genQuad("par",tmp,"CV","_");
	}
}

void condition(vector<int>& TrueL, vector<int>& FalseL){
	vector<int> TTrueL;
	vector<int> TFalse;
	boolterm(TTrueL, TFalse);
	TrueL=TTrueL;
	FalseL=TFalse;
	while(tokenid==orxxtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		nextQuad();
		backpatch(FalseL,MQnextquad);
		TTrueL.clear();
		boolterm(TTrueL,TFalse);
		merge(TrueL,TTrueL);
		FalseL=TFalse;
	}
}

void boolterm(vector<int>& TrueL, vector<int>& FalseL){
	vector<int> TTrueL;
	vector<int> TFalse;
	boolfactor(TTrueL,TFalse);
	TrueL=TTrueL;
	FalseL=TFalse;
	while(tokenid==andxtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		nextQuad();
		backpatch(TrueL,MQnextquad);
		boolfactor(TTrueL, TFalse);
		merge(FalseL,TFalse);
		TrueL=TTrueL;
	}
}

void boolfactor(vector<int>& TrueL, vector<int>& FalseL){
	if(tokenid==notxtk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		if(tokenid!=sbotk){
			cout<<"Error(syn:boolfactor): line "<<lineCount<<":  Expected \"[\" , instead got "<<token<<". (Please read the MiniPascal syntax on bool comparison)";
			tokenid=errtk;
			exit(0);
		}
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		condition(TrueL,FalseL);
		if(tokenid!=sbctk){
			cout<<"Error(syn:boolfactor): line "<<lineCount<<":  Expected \"]\" , instead got "<<token<<". (Please read the MiniPascal syntax on bool comparison)";
			tokenid=errtk;
			exit(0);
		}
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else if(tokenid==sbotk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		condition(TrueL,FalseL);
		if(tokenid!=sbctk){
			cout<<"Error(syn:boolfactor): line "<<lineCount<<":  Expected \"]\" , instead got "<<token<<". (Please read the MiniPascal syntax on bool comparison)";
			tokenid=errtk;
			exit(0);
		}
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else{
		string RelopL;
		string RelopR;
		string Relop;
		expression(RelopL);
		if((tokenid==equtk)||(tokenid==smatk)||(tokenid==bigtk)||(tokenid==boetk)||(tokenid==soetk)||(tokenid==diftk)){
			tokenid=lex();
			if(tokenid==errtk){
				cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
				exit(0);
			}
			Relop=token;
			expression(RelopR);
			nextQuad();
			TrueL.push_back(MQnextquad);
			genQuad(Relop,RelopL,RelopR,"_");
			nextQuad();
			FalseL.push_back(MQnextquad);
			genQuad("jump","_","_","_");
		}
		else{
			cout<<"Error(syn:boolfactor): line "<<lineCount<<":  Expected comparison operator , instead got "<<token<<". (Please read the MiniPascal syntax on bool comparison)";
			tokenid=errtk;
			exit(0);
		}
	}
}

void expression(string& IValueS){
	string PMOperL;
	string PMOperR;
	string PMOper="";
	if(tokenid==addtk){
		PMOper="+";
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else if(tokenid==subtk){
		PMOper="-";
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	term(PMOperL);
	if(PMOperL!=""){
		newTemp();
		genQuad(PMOper,PMOperL,"_",TempVar);
		PMOperL=TempVar;
	}
	while((tokenid==addtk)||(tokenid==subtk)){
		if(tokenid==addtk)
			PMOper="+";
		else
			PMOper="-";
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		term(PMOperR);
		newTemp();
		genQuad(PMOper,PMOperL,PMOperR,TempVar);
		PMOperL=TempVar;
	}
	IValueS=PMOperL;
}

void term(string &IValueT){
	string MDOperL;
	string MDOperR;
	string MDOper="";
	factor(MDOperL);
	while((tokenid==multk)||(tokenid==divtk)){
		if(tokenid==multk)
			MDOper="*";
		else
			MDOper="/";
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		factor(MDOperR);
		newTemp();
		genQuad(MDOper,MDOperL,MDOperR,TempVar);
		MDOperL=TempVar;
	}
	IValueT=MDOperL;
}

void factor(string &IValueF){
	string FVal;
	if(tokenid==numtk){
		IValueF=token;
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else if(tokenid==paotk){
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		expression(FVal);
		IValueF=FVal;
		if(tokenid!=pactk){
			cout<<"Error(syn:factor): line "<<lineCount<<":  Expected \")\", instead got "<<token<<". (Please read the MiniPascal syntax)";
			tokenid=errtk;
			exit(0);
		}
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
	}
	else if(tokenid==idtk){
		IValueF=token;
		tokenid=lex();
		if(tokenid==errtk){
			cout<<"Error(lex@syn): line "<<lineCount<<": Lex returned error in syn.";
			exit(0);
		}
		if(tokenid==paotk){
			enum TPF TTPF=FUNCC;
			actualpars(TTPF,IValueF);
		}
	}
	else{
		cout<<"Error(syn:factor): line "<<lineCount<<":  Expected \"(\" or ID or constant, instead got "<<token<<". (Please read the MiniPascal syntax)";
		tokenid=errtk;
		exit(0);
	}
	
}

void genQuad(string s1, string s2, string s3, string s4){
	MQcounter++;
	Quad tquad(MQcounter,s1,s2,s3,s4);
	MQuads.push_back(tquad);
	CQuad=&MQuads.back();
}

void nextQuad(){
	MQnextquad=MQcounter+1;
}

void merge(vector<int>& v1, vector<int>& v2){
	v1.insert(v1.end(), v2.begin(), v2.end());
}

void newTemp(){
	TempVarCounter++;
	string i=to_string(TempVarCounter);
	TempVar="T_"+i;
	enum TEnt MTVAR=TVAR;
	addEntity(TempVar,MTVAR,"",-1);
}

void backpatch(vector<int> BList, int IQpos){
	while(!BList.empty()){
		MQuads.at(BList.back()).V4=to_string(IQpos);
		BList.pop_back();
	}
}

void addScope(string IScopeName){
	Scope temp;
	temp.Name=IScopeName;
	if(MScope.empty()){
		temp.NestingLevel=1;
	}
	else{
		temp.NestingLevel=(MScope.back().NestingLevel)+1;;
	}
	MScope.push_back(temp);
	CScope=&(MScope.back());
	CEntity=NULL;
}

void addEntity(string IEntityName, enum TEnt ITEnt, string ICValue, int FRLength){
	Entity temp;
	temp.Name=IEntityName;
	temp.Etype=ITEnt;
	if(ITEnt==FUNC){
		temp.FrLength=FRLength;
		if(CScope->CurrentOffset==0)
			CScope->CurrentOffset=12;
		else
			CScope->CurrentOffset+=4;
	}
	else if(ITEnt==PAR){
		if(CScope->CurrentOffset==0){
			CScope->CurrentOffset=12;
			temp.OffsetPar=12;
		}
		else{
			CScope->CurrentOffset+=4;
			temp.OffsetPar=CScope->CurrentOffset;
		}
	}
	else if((ITEnt==VAR)||(ITEnt==CVAR)){
		if(CScope->CurrentOffset==0){
			CScope->CurrentOffset=12;
			temp.OffsetVar=12;
		}
		else{
			CScope->CurrentOffset+=4;
			temp.OffsetVar=CScope->CurrentOffset;
		}
		if(ITEnt==CVAR)
			temp.Cvalue=ICValue;
	}
	else if(ITEnt==TVAR){
		if(CScope->CurrentOffset==0){
			CScope->CurrentOffset=12;
			temp.OffsetTemp=12;
		}
		else{
			CScope->CurrentOffset+=4;
			temp.OffsetTemp=CScope->CurrentOffset;
		}
	}
	else
		cout<<"Error(intsymb@addEntity): Incorrect type of Entity";
	CScope->MEntity.push_back(temp);

	CEntity=&(CScope->MEntity.back());
}

void addArgument(string IArgumentName, enum TPar ITPar){
	Argument temp;
	temp.Name=IArgumentName;
	temp.Ptype=ITPar;
	CEntity->MArgument.push_back(temp);
}

void deleteScope(){
	if(MScope.empty()){
		cout<<"Error(intsymb@deleteScope): Scope Vector is Empty";
		exit(0);
	}
	else{
		int i=MScope.back().CurrentOffset;
		if(i==0)
			FFLength=12;
		else
			FFLength=i+4;
		MScope.back().MEntity.clear();
		MScope.pop_back();
		CScope=&MScope.back();
	}
}

Entity* searchNameE(string name){
	enestingLevel=0;
	size_t si=MScope.size();
	for(size_t i=0;i<si;i++){
		enestingLevel=MScope.at((int)i).NestingLevel;
		size_t ei=MScope.at((int)i).MEntity.size();
		for(size_t j=0;j<ei;j++){
			if(MScope.at((int)i).MEntity.at((int)j).Name==name)
				return(&(MScope.at((int)i).MEntity.at((int)j)));
		}
	}
	cout<<"Error(intsymb@searchNameE): Entity with name "<<name<<" wasn't found";
	exit(0);
}

void startQuadF(string pname){
	Entity* temp=searchNameE(pname);
	temp->Fquad=MQcounter+1;
}

void searchVariableS(string name){
	size_t si=CScope->MEntity.size();
	for(size_t i=0;i<si;i++)
		if(CScope->MEntity.at(i).Name==name){
			cout<<"Error(intsymb@searchVariableS): Variable with name "+name+" already declared";
			exit(0);
		}
}

bool isNum(string& isstr){
    string::const_iterator it = isstr.begin();
    while (it != isstr.end() && isdigit(*it)) ++it;
    return !isstr.empty() && it == isstr.end();
}

void gnlvcode(string gtname){
	int goffset;
	Entity *gEntity=NULL;
	gEntity=searchNameE(gtname);
	if(gEntity==NULL){
		cout<<"Error(final@gnlvcode): Entity with name "<<gtname<<" wasn't found";
		exit(0);
	}
	else{
		enum TEnt gTent=gEntity->Etype;
		if(gTent==VAR||gTent==CVAR)
			goffset=gEntity->OffsetVar;
		else if(goffset==PAR)
			goffset=gEntity->OffsetPar;
		else if(goffset==TVAR)
			goffset=gEntity->OffsetTemp;
		else{
			cout<<"Error(final@gnlvcode): Unable to get offset of variable in "<<gtname<<" entity";
			exit(0);
		}
	}
	ffinal<<"\t movi\t R[255],\t M[4+R[0]]\n";
	for(int i=enestingLevel;i<CScope->NestingLevel;i++)
		ffinal<<"\t movi\t R[255],\t M[R[255]+4]\n";
	ffinal<<"\t movi\t R[254],\t "<<goffset<<"\n";
	ffinal<<"\t addi\t R[255],\t R[255],\t R[254]\n";
}

void loadvr(string var,int reg){
	int loffset=0;
	Entity *lEntity=NULL;
	if(isNum(var))
		ffinal<<"\t movi\t R["<<reg<<"],\t\t"<<var<<"\n";
	else{
		lEntity=searchNameE(var);
		if(lEntity==NULL){
			cout<<"Error(final@loadvr): Variable "<<var<<" wasn't declared";
			exit(0);
		}
		enum TEnt lTent=lEntity->Etype;
		if(lTent==VAR||lTent==CVAR)
			loffset=lEntity->OffsetVar;
		else if(loffset==PAR)
			loffset=lEntity->OffsetPar;
		else if(loffset==TVAR)
			loffset=lEntity->OffsetTemp;
		else{
			cout<<"Error(final@loadvr): Unable to get offset of variable in "<<var<<" entity";
			exit(0);
		}
		if(enestingLevel==1)	//global var
			ffinal<<"\t movi\t R["<<reg<<"],\t\t M["<<(loffset+600)<<"]\n";
		else if(lTent==TVAR)	//temp var
			ffinal<<"\t movi\t R["<<reg<<"],\t\t M["<<loffset<<"+R[0]]\n";
		else if(enestingLevel==CScope->NestingLevel){
			if(lTent==VAR || lTent==CVAR)	//var or constant
				ffinal<<"\t movi\t R["<<reg<<"],\t\t M["<<loffset<<"+R[0]]\n";
			else if(lTent==PAR && lEntity->Ptype==VAL)	//parameter by value
				ffinal<<"\t movi\t R["<<reg<<"],\t\t M["<<loffset<<"+R[0]]\n";
			else if(lTent==PAR && lEntity->Ptype==REF)	//parameter by reference
				ffinal<<"\t movi\t R[255],\t M["<<loffset<<"+R[0]]\n\t movi\t R["<<reg<<"],\t\t M[R[255]]\n";
		}
		else if(enestingLevel<CScope->NestingLevel){
			gnlvcode(var);
			if(lTent==VAR || lTent==CVAR)	//local var
				ffinal<<"\t movi\t R["<<reg<<"],\t\t M[R[255]]\n";
			else if(lTent==PAR && lEntity->Ptype==VAL)	//parameter by value
				ffinal<<"\t movi\t R["<<reg<<"],\t\t M[R[255]]\n";
			else if(lTent==PAR && lEntity->Ptype==REF)	//parametetr by reference
				ffinal<<"\t movi\t R[255],\t M[R[255]]\n\t movi\t R["<<reg<<"],\t\t M[R[255]]\n";
		}
		else{
			cout<<"Error(final@loadvr): nestinglevel of scope "<<CScope->Name<<" didnt get caught in if checks";
			exit(0);
		}
	}
}

void storerv(string var,int reg){
	int soffset=0;
	Entity *sEntity=NULL;
	sEntity=searchNameE(var);
	if(sEntity==NULL){
			cout<<"Error(final@loadvr): Variable "<<var<<" wasn't declared";
			exit(0);
	}
	enum TEnt sTent=sEntity->Etype;
	if(sTent==VAR||sTent==CVAR)
		soffset=sEntity->OffsetVar;
	else if(soffset==PAR)
		soffset=sEntity->OffsetPar;
	else if(soffset==TVAR)
		soffset=sEntity->OffsetTemp;
	else{
		cout<<"Error(final@storerv): Unable to get offset of variable in "<<var<<" entity";
		exit(0);
	}
	if(enestingLevel==1)	//global var
		ffinal<<"\t movi\t M["<<(soffset+600)<<"],\t\t R["<<reg<<"]\n";
	else if(enestingLevel==CScope->NestingLevel){
		if(sTent==VAR || sTent==TVAR || sTent==CVAR)	//var or constant
			ffinal<<"\t movi\t M["<<soffset<<"+R[0]],\t R["<<reg<<"]\n";
		else if(sTent==PAR && sEntity->Ptype==VAL)	//parameter by value
			ffinal<<"\t movi\t M["<<soffset<<"+R[0]],\t R["<<reg<<"]\n";
		else if(sTent==PAR && sEntity->Ptype==REF)	//parameter by reference
			ffinal<<"\t movi\t R[255],\t M["<<soffset<<"+R[0]]\n\t movi\t M[R[255]],\t R["<<reg<<"]\n";
	}
	else if(enestingLevel<CScope->NestingLevel){
		gnlvcode(var);
		if(sTent==VAR || sTent==TVAR || sTent==CVAR)	//local var
			ffinal<<"\t movi\t M[R[255]],\t R["<<reg<<"]\n";
		else if(sTent==PAR && sEntity->Ptype==VAL)	//parameter by value
			ffinal<<"\t movi\t M[R[255]],\t R["<<reg<<"]\n";
		else if(sTent==PAR && sEntity->Ptype==REF)	//parameter by reference
			ffinal<<"\t movi\t R[255],\t M[R[255]]\n\t movi\t M[R[255]],\t R["<<reg<<"]\n";
	}
	else{
		cout<<"Error(final@storerv): nestinglevel of scope "<<CScope->Name<<" didnt get caught in if checks";
		exit(0);
	}
}

void generateFinalCode(){
	int goffset;
	int gframelength=-1;
	int igframelength=0;
	Entity *gEntity=NULL;
	if((FCQuad->V1=="par") && argcounter!=0)
		argcounter=0;
	if((CScope->Name==MScope.at(0).Name) && (fLabelf==-1)){	//first command for execution
		fLabelf=1;
		createFFirstLabel();
	}
	int i=FCQuad->Qpos;
	int j=(int)MQuads.size();
	while((j-i)>0){
		if(MQuads[i].V1=="jump")
			ffinal<<"L"<<MQuads[i].Qpos<<":\t jmp\t L"<<MQuads[i].V3<<"\n";
		else if(MQuads[i].V1=="+"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t addi\t R[3],\t\t R[1],\t R[2]\n";
			storerv(MQuads[i].V4,3);
		}
		else if(MQuads[i].V1=="-"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t subi\t R[3],\t\t R[1],\t R[2]\n";
			storerv(MQuads[i].V4,3);
			ffinal<<"L"<<MQuads[i].Qpos<<":";
		}
		else if(MQuads[i].V1=="*"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t muli\t R[3],\t\t R[1],\t R[2]\n";
			storerv(MQuads[i].V4,3);
		}
		else if(MQuads[i].V1=="/"){
			if(MQuads[i].V3=="0"){
				cout<<"Error(final@generateFinalCode): in Quad "<<MQuads[i].Qpos<<" division by zero";
				exit(0);
			}
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t divi\t R[3],\t\t R[1],\t R[2]\n";
			storerv(MQuads[i].V4,3);
		}
		else if(MQuads[i].V1=="<"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t cmpi\t R[1],\t\t R[2]\t\n\t ja\t L"<<MQuads[i].V4<<"\n";
		}
		else if(MQuads[i].V1==">"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t cmpi\t R[1],\t\t R[2]\t\n\t jb\t L"<<MQuads[i].V4<<"\n";
		}
		else if(MQuads[i].V1=="="){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t cmpi\t R[1],\t\t R[2]\t\n\t je\t L"<<MQuads[i].V4<<"\n";
		}
		else if(MQuads[i].V1=="<="){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t cmpi\t R[1],\t\t R[2]\t\n\t jae\t L"<<MQuads[i].V4<<"\n";
		}
		else if(MQuads[i].V1==">="){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t cmpi\t R[1],\t\t R[2]\t\n\t jbe\t L"<<MQuads[i].V4<<"\n";
		}
		else if(MQuads[i].V1=="<>"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			loadvr(MQuads[i].V3,2);
			ffinal<<"\t cmpi\t R[1],\t\t R[2]\t\n\t jne\t L"<<MQuads[i].V4<<"\n";
		}
		else if(MQuads[i].V1==">"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			storerv(MQuads[i].V4,1);
		}
		else if(MQuads[i].V1==":="){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			storerv(MQuads[i].V4,1);
		}
		else if(MQuads[i].V1=="par"){
			gframelength=CScope->CurrentOffset+4;
			igframelength=gframelength+12+(4*argcounter);
			argcounter++;
			if(MQuads[i].V3=="CV"){
				ffinal<<"L"<<MQuads[i].Qpos<<":";
				loadvr(MQuads[i].V2,255);
				ffinal<<"\t movi\t M["<<igframelength<<"+R[0]],\t R[255]\n";
			}
			else if(MQuads[i].V3=="REF"){
				gEntity=searchNameE(MQuads[i].V2);
				if(enestingLevel==CScope->NestingLevel){
					if((gEntity->Etype==PAR) && (gEntity->Ptype==REF)){
						goffset=gEntity->OffsetPar;
						ffinal<<"L"<<MQuads[i].Qpos<<":\t movi\t R[255],\t R[0]\n\t movi\t R[254],\t"<<goffset<<"\n\t addi\t R[255],\t R[254],\t R[255]\n\t movi\t R[1],\t M[R[255]]\n\t movi\t M["<<igframelength<<"+R[0]],\t R[1]\n";
					}
					else{
							goffset=gEntity->OffsetVar;
							ffinal<<"L"<<MQuads[i].Qpos<<":\t movi\t R[255],\t R[0]\n\t movi\t R[254],\t "<<goffset<<"\n\t addi\t R[255],\t R[254],\t R[255]\n\t movi\t M["<<igframelength<<"+R[0]],\t R[255]\n";
					}
				}
				else{
					if((gEntity->Etype==PAR) && (gEntity->Ptype==REF)){
						gnlvcode(MQuads[i].V2);
						ffinal<<"L"<<MQuads[i].Qpos<<":movi\t R[1],\t M[R[255]]\n\t movi\t M["<<igframelength<<"+R[0]],\t R[255]\n";
					}
					else{
						gnlvcode(MQuads[i].V2);
						ffinal<<"L"<<MQuads[i].Qpos<<":movi \t M["<<igframelength<<"+R[0]],\t R[255]\n";
					}
				}
			}
			else if(MQuads[i].V3=="RET"){
				gEntity=searchNameE(MQuads[i].V2);
				goffset=gEntity->OffsetPar;
				ffinal<<"L"<<MQuads[i].Qpos<<":\t movi\t R[255],\t R[0]\n\t movi\t R[254],\t"<<goffset<<"\n\t addi\t R[255],\t R[254],\t R[255]\n\t movi\t M["<<gframelength+8<<"+R[0]],\t R[255]\n";
			}
		}
		else if(MQuads[i].V1=="end_block")
			if(MQuads[i].V4==MScope.at(0).Name)
				ffinal<<"L"<<MQuads[i].Qpos<<":\t jmp\t M[R[0]]\n";
		else if(MQuads[i].V1=="call"){
			gframelength=CScope->CurrentOffset+4;
			igframelength=gframelength+12+(4*argcounter);
			checkFunCall(MQuads[i].V4);
			gEntity=searchNameE(MQuads[i].V4);
			if(enestingLevel==CScope->NestingLevel)
				ffinal<<"L"<<MQuads[i].Qpos<<": movi\t M["<<gframelength+4<<"+R[0]],\t R[0]\n";
			else if(enestingLevel-CScope->NestingLevel==-1)
				ffinal<<"L"<<MQuads[i].Qpos<<": movi\t R[255],\t M[4+R[0]]\n\t movi\t M["<<igframelength+4<<"+R[0]],\t R[255]\n";
			else{
				cout<<"Error(final@storerv): function "<<MQuads[i].V4<<" was not declared in this scope or in parent scope\n";
				exit(0);
			}
			ffinal<<"\t movi\t R[255],\t "<<gframelength<<"\n\t addi\t R[0],\t R[255],\t R[0]\n\t movi\t R[255],\t $\n\t movi\t R[254],\t 15\n\t addi\t R[255],\t R[254],\t R[255]\n\t movi\t M[R[0]],\t R[255]\n\t jmp\t L"<<gEntity->Fquad<<"\n\t movi\t R[255],\t "<<gframelength<<"\n\t subi\t R[0],\t R[0],\t R[255]\n";
		}
		else if(MQuads[i].V1=="input"){
			ffinal<<"L"<<MQuads[i].Qpos<<":\t ini\t R[1]\n";
			storerv(MQuads[i].V2,1);
		}
		else if(MQuads[i].V1=="print"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			ffinal<<"\t outi\t R[1]\n";
		}
		else if(MQuads[i].V1=="return"){
			ffinal<<"L"<<MQuads[i].Qpos<<":";
			loadvr(MQuads[i].V2,1);
			ffinal<<"\t movi\t R[255],\t M[8+R[0]]\n\t movi\t M[R[255]],\t R[1]\n";
		}
		else if(MQuads[i].V1=="halt")
			ffinal<<"L"<<MQuads[i].Qpos<<":\t halt\n";
		else
			ffinal<<"L"<<MQuads[i].Qpos<<":";
		i++;
		FCQuad=&(MQuads[i]);
	}
}

void checkFunCall(string funcname){
	Entity *cfEntity=NULL;
	int cfparameters=0;
	int cfarguments=0;
	int q;
	cfEntity=searchNameE(funcname);
	if(cfEntity==NULL){
		cout<<"Error(final@checkFunCall: func/proc "<<cfEntity->Name<<" wasn't declared";
		exit(0);
	}
	else if(cfEntity->Fquad==-1){
		cout<<"Error(final@checkFunCall: func/proc "<<cfEntity->Name<<" and variable "<<cfEntity->Name<<" have the same name";
		exit(0);
	}
	q=FCQuad->Qpos;
	while(MQuads[q-1].V1=="par"){
		q--;
		if(MQuads[q].V3!="RET")
			cfparameters++;
	}
	q++;
	cfarguments=(int)cfEntity->MArgument.size();
	if(cfarguments==cfparameters)
		for(int i=0;i<cfparameters;i++){
			if(((MQuads[q].V3=="CV") && (cfEntity->MArgument[i].Ptype==VAL)) || ((MQuads[q].V3=="REF") && (cfEntity->MArgument[i].Ptype==REF)))
				q++;
			else{
				cout<<"Error(final@checkFunCall: Func "<<CScope->Name<<" In func/proc "<<funcname<<"parameter"<<MQuads[q].V2<<"has wrong type"<<endl;
				exit(0);
			}
		}
	else if(cfarguments<cfparameters)
	{
		cout<<"Error(final@checkFunCall: Func "<<CScope->Name<<" Too many arguments in func/proc "<<funcname<<endl;
		exit(0);
	}
	else{
		cout<<"Error(final@checkFunCall: Func "<<CScope->Name<<" Too few arguments in func/proc "<<funcname<<endl;
		exit(0);
	}
}

void createFFirstLabel(){
	ofstream tempf;
	ffinal.close();
	tempf.open("Final.obj",ios::in | ios::out);
	tempf.seekp(11, ios::beg);
	string s=to_string((FCQuad->Qpos)+1);
	int i=(int)s.length();
	tempf.write(s.c_str(), i);
	tempf.close();
	ffinal.open("Final.obj",ios::in | ios::out | ios::app);
}


/*
MINI PASCAL GRAMMAR

<PROGRAM>			::= "program" ID  <PROGRAMBLOCK>

<PROGRAMBLOCK>		::= <DECLARATIONS>  <SUBPROGRAMS>  <BLOCK>

<BLOCK>				::= "begin"  <SEQUENCE>  "end"

<DECLARATIONS>		::= ( <CONSTDECL> )*  ( <VARDECL> )*

<CONSTDECL>			::= "const"  <ASSIGNLIST>  ";"  |
						ε
						
<ASSIGNLIST> 		::= ASSIGNCONST  ( , ASSIGNCONST )*

<ASSIGNCONST> 		::= ID  ":="  CONSTANT

<VARDECL> 			::= "var"  <VARLIST>  ";"  |
						ε
						
<VARLIST> 			::= ID  ( , ID )*

<SUBPROGRAMS> 		::= ( <PROCORFUNC> )*

<PROCORFUNC>		::= "procedure"  ID  <PROCORFUNCBODY>  |
						"function"  ID  <PROCORFUNCBODY>
						
<PROCORFUNCBODY>	::= <FORMALPARS>  <PROGRAMBLOCK>

<FORMALPARS> 		::= "(" <FORMALPARLIST> | ε ")"

<FORMALPARLIST> 	::= <FORMALPARITEM> ( , <FORMALPARITEM> )*

<FORMALPARITEM> 	::= ID |
						"var" ID
						
<SEQUENCE> 			::= <STATEMENT>  (  ";"  <STATEMENT> )*

<BLOCK-OR-STAT> 	::= <BLOCK> |
						<STATEMENT>
						
<STATEMENT>			::= <ASSIGNMENT-STAT>  |
						<IF-STAT> |
						<WHILE-STAT> |
						<FOR-STAT> |
						<CALL-STAT> |
						<PRINT-STAT> |
						<INPUT-STAT> |
						<RETURN-STAT>|
						<SELECT-STAT>
						
<ASSIGNMENT-STAT> 	::= ID  ":="  <EXPRESSION>

<IF-STAT> 			::= "if"  <CONDITION>  "then"  <BLOCK-OR-STAT>  <ELSEPART>

<ELSEPART>			::= ε  |
						"else" <BLOCK-OR-STAT>

<WHILE-STAT>		::= "while"  <CONDITION>  "do"  <BLOCK-OR-STAT>

<FOR-STAT>			::= "for"  <ASSIGNMENT-STAT>  "to"  <EXPRESSION>  <STEP-PART>  <BLOCK-OR-STAT>

<STEP-PART>			::= "step"  <EXPRESSION>  |
						ε

<CALL-STAT> 		::= "call"  ID  <ACTUALPARS>

<PRINT-STAT> 		::= "print" "(" <EXPRESSION> ")"

<INPUT-STAT> 		::= "input" "(" ID ")"

<RETURN-STAT>		::= "return" "(" <EXPRESSION> ")"

<SELECT-STAT>		::= "select" ID  <IS-EQUAL-PART> ( <IS-EQUAL-PART> )* "endselect"

<IS-EQUAL-PART>		::= "is equal to" <EXPRESSION> ":" <BLOCK>

<ACTUALPARS> 		::= "(" <ACTUALPARLIST> | ε  ")"

<ACTUALPARLIST> 	::= <ACTUALPARITEM> ( , <ACTUALPARITEM> )*

<ACTUALPARITEM> 	::= <EXPRESSION> |
						"var" ID

<CONDITION>			::= <BOOLTERM> ( "or" <BOOLTERM>)*

<BOOLTERM>			::= <BOOLFACTOR> ( "and"  <BOOLFACTOR> )*

<BOOLFACTOR>		::= "not" "[" <CONDITION> "]" |
						"[" <CONDITION> "]" |
						<EXPRESSION>  <RELATIONAL-OPER>  <EXPRESSION>

<RELATIONAL-OPER>	::= "=" |
						"<" |
						">" |
						">=" |
						"<=" |
						"<>"
						
<EXPRESSION>		:=  <OPTIONAL-SIGN>  <TERM>  ( <ADD-OPER> <TERM>)*

<TERM>				::= <FACTOR> (<MUL-OPER> <FACTOR>)*

<FACTOR>			::= CONSTANT |
						"(" <EXPRESSION> ")"  |
						ID  <OPTACTUALPARS>

<OPTACTUALPARS>		::= <ACTUALPARS> |
						ε
						
<ADD-OPER>			::= "+"  |  "-"

<MUL-OPER>			::= "*"  |  "/"

<OPTIONAL-SIGN>		::=  ε  |  <ADD-OPER>
*/
