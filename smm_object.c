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

char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}

typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;

//1.구조체 형식 정의 

typedef enum smmObjType{
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
} smmObjType_e;

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

//2.구조체 배열 변수 정의
//static smmobj_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;


//3.관련 함수 변경 
//object generation
void smmObj_genObject(char* name, smmObjType_e Objtype, int type, int credit, int energy,smmObjGrade_e grade)
{
    
	smmObj_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
	 
    strcpy(ptr->name, name);
    ptr->ObjType = ObjType
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
  
    }
    
//3. 관련 함수 변경
    
char* smmObj_genNodeNAme(void* obj)
   {
   	smmObject_t* ptr = (smmObject_t*)obj;
   	
   	return ptr->name;
   }



//member retrieving



//element to string
int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(int node_nr)
{
    return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(int node_nr)
{
    return smm_node[node_nr].energy;
}

