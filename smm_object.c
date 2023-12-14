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
static smmobj_t smm_node[MAX_NODE];
static int smmObj_noNode = 0;

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
	return (char*)
}
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
    
    
char* smmObj_genNodeNAme(void* Obj)
   {
   	smmObject_t* ptr = (smmObject_t*)Obj;
   	
   	return ptr->name;
   }



//member retrieving



//element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}

