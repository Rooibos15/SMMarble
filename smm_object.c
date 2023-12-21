//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100
#define MAX_FESTCARD    100

//노드 이름 정의 
static char smmNODENAME[SMMNODE_TYPE_MAX][MAX_CHARNAME]=
{
"lectuer"
"restaurant"
"laboratory"
"home"
"gotolab"
"foodchance"
"festival"
};


//노드 이름 보내기 
char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}

//구조체 정의 
typedef enum smmObjGrade {
    smmObjGrade_Ap,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;


typedef enum smmObjType{
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
} smmObjType_e;


typedef struct smmObj{
	char name[MAX_CHARNAME];
	smmObjType_e Objtype;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

typedef struct smmFood{
	char food_name[MAX_CHARNAME];
	int food_energy;
}smmFood_d;

typedef struct smmFest{
    char Fest_card[MAX_CHARNAME];
}smmFest_t;


//object generation
void smmObj_genObject(char* name, smmObjType_e Objtype, int type, int credit, int energy, smmObjGrade_e grade)
{
    
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
	 
    strcpy(ptr->name, name);
    ptr->ObjType = ObjType
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
  
    }
//food generation    
void smmObj_foodCard(char* food_name, int food_energy)
{
	smmFood_d* Foodptr;
	
	foodptr = (smmFood_d*)malloc(sizeof(smmFood_d));
	
	strcpy(Foodptr->food_name, food_name);
	Foodptr->food_energy = food_energy;
	
	return Foodptr;
}
//festival generation
void smmObj_FestCard(char* Fest_card)
{
	smmFest_t* Festptr;
	
	Festptr = (smmFest_t*)malloc(sizeof(smmFest_t));
	
	strcpy(Festptr->Fest_card, Fest_card);
	
	return Festptr;
}

 

//get node element   
char* smmObj_getNodeName(void* obj)
   {
   	smmObject_t* ptr = (smmObject_t*)obj;
   	
   	return ptr->name;
   }
   
smmObjType_e smmObj_getNodeOBJType(void* obj)
   {
   	smmObject_t* ptr = (smmObject_t*)obj;
   	
   	return ptr->Objtype;
   }   

int smmObj_getNodeType(void* obj)
   {
   	smmObject_t* ptr = (smmObject_t*)obj;
   	
   	return ptr->type;
   }

int smmObj_getNodeCredit(void* obj)
   {
   	smmObject_t* ptr = (smmObject_t*)obj;
   	
   	return ptr->credit;
   }
  
int smmObj_getNodeEnergy(void* obj)
   {
   	smmObject_t* ptr = (smmObject_t*)obj;
   	
   	return ptr->energy;
   }

smmObjGrade_e smmObj_getNodeGrade(void* obj)
   {
	smmObject_t* ptr = (smmObject_t*)obj;
	return ptr->grade;  
   }
//get food card element   
char* smmObj_getFoodName(void* obj)
    {
   	smmFood_d* ptr = (smmFood_d*)obj;
	return ptr->food_name;
	} 
	
int smmObj_getFoodEnergy(void* obj)
   {
	smmFood_d* ptr = (smmFood_d*)obj;
	return ptr->food_energy;  
   }	  
//get festival card element   
char* smmObj_getFestCard(void* obj)
    {
   	smmFest_t* ptr = (smmFest_t*)obj;
	return ptr->Fest_card;
	}   
