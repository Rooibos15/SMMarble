//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE       0
#define SMMNODE_TYPE_RESTAURANT    1 
#define SMMNODE_TYPE_LABORATORY    2
#define SMMNODE_TYPE_HOME          3
#define SMMNODE_TYPE_GOTOLAB       4
#define SMMNODE_TYPE_FOODCHANCE    5 
#define SMMNODE_TYPE_FESTIVAL      6
#define SMMNODE_TYPE_MAX           7
#define SMM_LECTURE_REPLY          10

#define MAX_CHARNAME               200

#define MAX_GRADE                  9

//±¸Á¶Ã¼ 
typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0 = 1,
    smmObjGrade_Am = 2,
    smmObjGrade_Bp = 3,
    smmObjGrade_B0 = 4,
    smmObjGrade_Bm = 5,
    smmObjGrade_Cp = 6,
    smmObjGrade_C0 = 7,
    smmObjGrade_Cm = 8
} smmObjGrade_e;

typedef enum smmObjType{
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
} smmObjType_e;

typedef struct smmFood{
	char food_name[MAX_CHARNAME];
	int food_energy;
}smmFood_d;

typedef struct smmFest{
    char Fest_card[MAX_CHARNAME];
}smmFest_t;


/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/

//object generation
void smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade);

//member retrieving
char* smmObj_getNodeName(int node_nr);
int smmObj_getNodeType(int node_nr);
int smmObj_getNodeCredit(int node_nr);
int smmObj_getNodeEnergy(int node_nr);

//element to string
char* smmObj_getTypeName(int type);


#endif /* smm_object_h */
