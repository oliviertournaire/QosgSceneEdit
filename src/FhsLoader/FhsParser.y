/******************************************************************************/
/* Project: SceneEdit                                                         */
/* Name   : FhsParser.y                                                       */
/* Author : Andre Normann                                                     */
/* Date   : 23.08.2007                                                        */
/* License: GPL                                                               */
/*                                                                            */
/* Description:                                                               */
/* This is a grammar file for the FHS file format.                            */
/******************************************************************************/

%{

/* System */        
#include <stdio.h>
#include <stdlib.h>
#define YYINCLUDED_STDLIB_H
#include <fstream>
#include <string>

/* OpenSceneGraph */
#include <osg/Texture>
    
/* Project */    
#include "FlexLexer.h"
#include "FhsLoader.h"

/* Forward declaration */    
static void FhsParser_error(const char *);
static int  FhsParser_lex();

/* Global variables */
static std::string fileName = "(stdin)";
static FhsLoader *fhs = 0L;    
static yyFlexLexer *flexer = 0L;
    
%}

/* Set entry point */
%start model_rule

/* FHS tokens */
%token FT_UNKNOWN
%token FT_MODEL
%token FT_GEOMETRY 
%token FT_LODGEOMETRY 
%token FT_ASSEMBLY 
%token FT_LODASSEMBLY 
%token FT_LOD 
%token FT_ACOUSTIC
%token FT_TUBEGEOMETRY 
%token FT_EXTFILE 
%token FT_NAME
%token FT_SPHERE 
%token FT_CONE 
%token FT_TORUS 
%token FT_TEXT 
%token FT_BOX
%token FT_POLYGON 
%token FT_TRISTRIP 
%token FT_QUADSTRIP 
%token FT_LINE 
%token FT_CLOSEDLINE 
%token FT_TRIFAN 
%token FT_TRIANGLES 
%token FT_LINES 
%token FT_QUADS
%token FT_POINTS 
%token FT_PPOOL 
%token FT_POLYPOOL 
%token FT_TUBE
%token FT_PATCHES 
%token FT_UNIFORM 
%token FT_ADAPTIVE 
%token FT_POOL
%token FT_SCRIPT 
%token FT_TYPE 
%token FT_FROM
%token FT_IDEAL 
%token FT_BBOX
%token FT_DCS 
%token FT_SCS 
%token FT_TRANSLATE 
%token FT_ROTATE 
%token FT_SCALE 
%token FT_STATIC 
%token FT_DYNAMIC 
%token FT_MATRIX 
%token FT_ORIENTATION
%token FT_TEXTURE 
%token FT_TEXTRANSCOLOR 
%token FT_RAD 
%token FT_BRDF 
%token FT_UNLIT 
%token FT_LIT 
%token FT_ID
%token FT_TEXFILTER 
%token FT_POINT
%token FT_LINEAR 
%token FT_MIPMAP 
%token FT_SHARPEN
%token FT_TEXWRAP 
%token FT_REPEAT 
%token FT_CLAMP 
%token FT_SELECT
%token FT_TEXQUALITY 
%token FT_HIGH 
%token FT_LOW
%token FT_TEXBLEND 
%token FT_MODULATE 
%token FT_DECAL 
%token FT_BLEND 
%token FT_REPLACE 
%token FT_ADD
%token FT_DIFFUSE 
%token FT_SPECULAR 
%token FT_EMISSION 
%token FT_ALPHA 
%token FT_TRANSPARENCY 
%token FT_SHININESS 
%token FT_IOR 
%token FT_POSITION 
%token FT_COLOR 
%token FT_DIRECTION
%token FT_UP 
%token FT_LUMEN 
%token FT_LVK 
%token FT_KD 
%token FT_KS 
%token FT_KT 
%token FT_DEFMAT 
%token FT_USEMAT
%token FT_LEFT_BRACE 
%token FT_LEFT_PARENTHESIS 
%token FT_COMMA
%token FT_RIGHT_BRACE 
%token FT_RIGHT_PARENTHESIS 
%token FT_NUMBER 
%token FT_STRING
%token FT_RESOLUTION
%token FT_LIGHT 
%token FT_ENVIRONMENT 
%token FT_AMBIENT 
%token FT_POINTLIGHT 
%token FT_DIRECTLIGHT 
%token FT_SPOTLIGHT 
%token FT_BACKGROUND
%token FT_COL 
%token FT_TEX 
%token FT_POS 
%token FT_NORM 
%token FT_IND 
%token FT_CIND 
%token FT_NIND 
%token FT_TIND 
%token FT_T3IND 
%token FT_CNIND 
%token FT_CTIND 
%token FT_CT3IND 
%token FT_TNIND 
%token FT_T3NIND 
%token FT_CTNIND 
%token FT_CT3NIND
%token FT_CTRIND 
%token FT_CTRNIND 
%token FT_CTRTIND 
%token FT_CTRT3IND 
%token FT_CTRTNIND 
%token FT_CTRT3NIND 
%token FT_TRANSP
%token FT_BILLBOARD 
%token FT_XAXIS 
%token FT_YAXIS 
%token FT_ZAXIS 
%token FT_AXIS 
%token FT_CENTER
%token FT_POLYELEMX 
%token FT_OBJECTX 
%token FT_DESCX 
%token FT_PRIMX 
%token FT_POLYX 
%token FT_MATX 
%token FT_ENVX 
%token FT_LIGHTX
%token FT_POINTORDER 
%token FT_CW
%token FT_CCW
%token FT_EXTRUDE 
%token FT_REVOLUTE
%token FT_LSDIREKT 
%token FT_LSFLANKEN 
%token FT_TSDIREKT 
%token FT_TSFLANKEN
%token FT_KSNACHHALL 
%token FT_FLAECHENMASSE 
%token FT_ABSTRAHL 
%token FT_INVERLUST 
%token FT_KOINZIDENZ
%token FT_OKTAV 
%token FT_TERZ 
%token FT_REAL 
%token FT_KOMPLEX 
%token FT_TENSOR 
%token FT_FLAG 
%token FT_LUFT 
%token FT_TEMP
%token FT_ABSEXP 
%token FT_ABSIMP 
%token FT_STREU 
%token FT_TRANSMISSION
%token FT_FONT
%token FT_SWITCH 
%token FT_WHICHCHILD
%token FT_SOUNDSOURCE 
%token FT_FILENAME 
%token FT_LIVE 
%token FT_GAIN 
%token FT_ACTIVE 
%token FT_STARTTIME 
%token FT_STOPTIME 
%token FT_LOOP 
%token FT_PITCH
%token FT_VERSION

%union {
    char    *string;
    int      integer;
    float    real;
    bool     boolean;
};

%type <string>  string
%type <real>    real
%type <integer> integer pointorder_rule texwrap_rule texfilter_rule

%destructor { free($$); } string

%%

model_rule      : model_head model_body             	{ fhs->endModel(); }
                | model_head version_rule model_body	{ fhs->endModel(); }
                ;

model_head      : FT_MODEL string  { fhs->beginModel( $2 ); }
                | FT_MODEL         { fhs->beginModel( "<unnamed>" ); }
                ;

model_body      : /* empty */
                | model_body fhs_element
                ;

version_rule  	: FT_VERSION real { fhs->setVersion( $2 ); }
                ;

fhs_elements    : /* empty */
                | fhs_elements fhs_element
                ;

fhs_element     : assembly_rule
                | geometry_rule
                | light_rule
                | environment_rule
                | file_rule
                | mat_rule
                | text_rule
                | FT_POINTORDER pointorder_rule 
                    { 
                        FhsLoader::POINT_ORDER po = (FhsLoader::POINT_ORDER)$2; 
                        fhs->setPointOrder(po); 
                    }
                | FT_OBJECTX FT_STRING
                ;

pointorder_rule : FT_CW     { $$ = FhsLoader::CW; }
                | FT_CCW    { $$ = FhsLoader::CCW; }
                ;

file_rule       : FT_EXTFILE FT_STRING descriptions FT_LEFT_BRACE FT_NAME FT_STRING FT_RIGHT_BRACE
                ;

assembly_rule   : FT_LODASSEMBLY string { fhs->beginLOD( $2 ); } descriptions FT_LEFT_BRACE lod_rule FT_RIGHT_BRACE { fhs->endLOD(); }
                | FT_LODASSEMBLY descriptions FT_LEFT_BRACE lod_rule FT_RIGHT_BRACE
                | FT_SWITCH string	{ fhs->beginSwitch( $2 ); } descriptions FT_LEFT_BRACE fhs_elements FT_RIGHT_BRACE { fhs->endSwitch(); }
                | FT_SWITCH { fhs->beginSwitch( "<unnamed>" ); } descriptions FT_LEFT_BRACE fhs_elements FT_RIGHT_BRACE { fhs->endSwitch(); }
                | assembly_head descriptions FT_LEFT_BRACE fhs_elements FT_RIGHT_BRACE { fhs->endAssembly(); }
                | assembly_head FT_FROM string descriptions
                ;

assembly_head   : FT_ASSEMBLY string   { fhs->beginAssembly( $2 ); }
                | FT_ASSEMBLY          { fhs->beginAssembly( "<unnamed>" ); }
                | FT_BILLBOARD string  { fhs->beginBillboard( $2 ); }
                | FT_BILLBOARD         { fhs->beginBillboard( "<unnamed>" ); }
                ;

bboard_rule     : FT_XAXIS
		        | FT_YAXIS
		        | FT_ZAXIS
		        | FT_AXIS vector
		        | FT_CENTER vector
		        ;
		
descriptions    : description_rule
		        | descriptions description_rule
		        ;

description_rule: FT_SCRIPT
		        | FT_ID FT_NUMBER	
		        | FT_TYPE FT_NUMBER	
                | FT_FLAG FT_NUMBER	
                | FT_LUFT FT_NUMBER	
                | FT_TEMP FT_NUMBER	
		        | FT_RESOLUTION FT_NUMBER	
		        | FT_RESOLUTION FT_NUMBER FT_NUMBER	
		        | FT_DESCX FT_STRING	
		        | transformation 
                | ppool_rule
                | polypool_rule
		        | usemat_rule
                | FT_WHICHCHILD integer { fhs->setWhichChild( $2 ); } 
 		        | bboard_rule
                ;

lod_rule        : /* empty */
		        | lod_rule lod_element
		        ;

lod_element     : FT_LOD real { fhs->setLODDistance( $2 ); } FT_LEFT_BRACE fhs_elements FT_RIGHT_BRACE
                | FT_LOD real real FT_LEFT_BRACE fhs_elements FT_RIGHT_BRACE
                | FT_BBOX FT_NUMBER FT_LEFT_BRACE vector vector FT_RIGHT_BRACE 
                | FT_IDEAL FT_LEFT_BRACE fhs_elements FT_RIGHT_BRACE 
                ;

geometry_rule   : geometry_head descriptions geometry_body { fhs->endGeometry(); }
		        | geometry_head FT_FROM FT_STRING descriptions
		        ;

geometry_head   : FT_GEOMETRY string       { fhs->beginGeometry( $2 ); }
		        | FT_GEOMETRY              { fhs->beginGeometry( "<unnamed>" ); }
		        | FT_LODGEOMETRY string
		        | FT_LODGEOMETRY
		        | FT_TUBEGEOMETRY string
		        | FT_TUBEGEOMETRY
		        ;


geometry_body   : FT_LEFT_BRACE lodgeometry_rule FT_RIGHT_BRACE
		        | FT_LEFT_BRACE primitive_rule FT_RIGHT_BRACE
		        | FT_LEFT_BRACE FT_RIGHT_BRACE
		        | primitive_rule
		        ;

lodgeometry_rule: /* empty */
		        | lodgeometry_rule lodgeometry_element
		        ;

lodgeometry_element	: FT_LOD FT_NUMBER descriptions FT_LEFT_BRACE primitive_rule FT_RIGHT_BRACE
		        	| FT_LOD FT_NUMBER FT_NUMBER descriptions FT_LEFT_BRACE primitive_rule FT_RIGHT_BRACE
		        	| FT_BBOX FT_NUMBER FT_LEFT_BRACE vector vector FT_RIGHT_BRACE 
                	| FT_IDEAL descriptions FT_LEFT_BRACE primitive_rule FT_RIGHT_BRACE 
                	;

primitive_rule  : FT_EXTRUDE FT_NUMBER FT_LEFT_BRACE vectors2 FT_RIGHT_BRACE 
                | FT_REVOLUTE FT_NUMBER FT_LEFT_BRACE vectors2 FT_RIGHT_BRACE 
                | FT_SPHERE FT_LEFT_PARENTHESIS
                          FT_NUMBER opt_comma
                          FT_NUMBER opt_comma
                          FT_NUMBER opt_comma
                          FT_NUMBER FT_RIGHT_PARENTHESIS 
                | FT_CONE FT_LEFT_PARENTHESIS 
                         FT_NUMBER opt_comma 
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_TORUS FT_LEFT_PARENTHESIS
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_BOX FT_LEFT_PARENTHESIS
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER opt_comma
                         FT_NUMBER FT_RIGHT_PARENTHESIS
                | polyeder
                | FT_PRIMX FT_STRING
                ;

polyeder        	: polyhedra_elements
                	;

polyhedra_elements	: polyhedra_element
                	| polyhedra_elements polyhedra_element
                	;

polyhedra_element	: polygon_rule
                	| usemat_rule
                	;

polygon_rule       	: FT_POLYGON     { fhs->beginPolygon(); }     FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_TRISTRIP    { fhs->beginTriStrip(); }    FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_TRIFAN      { fhs->beginTriFan(); }      FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_QUADSTRIP   { fhs->beginQuadStrip(); }   FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_LINE        { fhs->beginLine(); }        FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_LINES       { fhs->beginLines(); }       FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_CLOSEDLINE  { fhs->beginClosedLine(); }  FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_POINTS      { fhs->beginPoints(); }      FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_TRIANGLES   { fhs->beginTriangles(); }   FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
                	| FT_QUADS       { fhs->beginQuads(); }       FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE { fhs->endPrimitive(); }
		        	| FT_TUBE FT_LEFT_BRACE polygon_elements FT_RIGHT_BRACE
		        	| FT_POLYX FT_STRING
		        	| mat_rule
		        	;

polygon_elements  	: /* empty */
		        	| polygon_elements polygon_element
		        	;

polygon_element : FT_POS vector               { fhs->addPos(); }
		        | FT_TEX FT_NUMBER	
                | FT_TEX vect2                { fhs->addTexture2(); }
                | FT_TEX vector               { fhs->addTexture3(); }
		        | FT_NORM FT_NUMBER	
                | FT_NORM vector              { fhs->addNormal(); }	
		        | FT_COL FT_NUMBER	
                | FT_COL vector               { fhs->addColor(); }
		        | FT_TRANSP FT_NUMBER	
                | FT_COL vect4                { fhs->addColor4(); }
                | FT_IND integer              { fhs->addIndex( $2 ); }
		        | FT_RAD FT_NUMBER	
                | FT_CIND integer             { fhs->addColorIndex( $2 ); }
                | FT_NIND integer             { fhs->addNormalIndex( $2 ); }
                | FT_TIND integer             { fhs->addTextureIndex( $2 ); }
                | FT_T3IND integer            { fhs->addTexture3Index( $2 ); }
		        | FT_CNIND integer            { fhs->addColorNormalIndex( $2 ); }
		        | FT_CTIND integer            { fhs->addColorTextureIndex( $2 ); }
		        | FT_CT3IND integer           { fhs->addColorTexture3Index( $2 ); }
                | FT_TNIND integer            { fhs->addTextureNormalIndex( $2 ); }
		        | FT_T3NIND integer           { fhs->addTexture3NormalIndex( $2 ); }
		        | FT_CTNIND integer           { fhs->addColorTextureNormalIndex( $2 ); }
		        | FT_CT3NIND integer          { fhs->addColorTexture3NormalIndex( $2 ); }
		        | FT_CTRIND FT_NUMBER
		        | FT_CTRNIND FT_NUMBER
                | FT_CTRTIND FT_NUMBER
                | FT_CTRT3IND FT_NUMBER
                | FT_CTRTNIND FT_NUMBER
		        | FT_CTRT3NIND FT_NUMBER
		        | lvect
		        | FT_POLYELEMX FT_STRING
		        ;

text_rule       	: text_head descriptions FT_LEFT_BRACE text_body FT_RIGHT_BRACE
                	;

text_head	    	: FT_TEXT FT_STRING
                	;

text_body	    	: /* empty */
			    	| text_body text_body_elements
			    	;
			
text_body_elements	: FT_COL vector
			    	| FT_POS vector
			    	| FT_FONT FT_STRING
			    	| FT_TEX FT_STRING
			    	| FT_RAD FT_NUMBER
			    	| FT_IND FT_NUMBER
			    	| FT_NORM FT_NUMBER
			    	;

usemat_rule     : FT_USEMAT string              { fhs->setMaterial( $2 ); } 
                | FT_USEMAT FT_FROM string      { fhs->setFromMaterial( $3 ); }
                ;
    
mat_rule        :  FT_DEFMAT string { fhs->beginMaterial( $2 ); }          FT_LEFT_BRACE mat_elements FT_RIGHT_BRACE  { fhs->endMaterial(); }
                |  FT_DEFMAT        { fhs->beginMaterial( "<unnamed>" ); } FT_LEFT_BRACE mat_elements FT_RIGHT_BRACE  { fhs->endMaterial(); }
                ;

mat_elements    : mat_element
		        | mat_elements mat_element
		        ;

mat_element     : FT_LEFT_PARENTHESIS real opt_comma /* R   */
                                      real opt_comma /* G   */
                                      real opt_comma /* B   */
                                      real opt_comma /* kd  */
                                      real opt_comma /* ks  */
                                      real opt_comma /* n   */
                                      real opt_comma /* kt  */
                                      real FT_RIGHT_PARENTHESIS  /* ior */
                        {
                            fhs->setColor( $2, $4, $6 );
                            fhs->setDiffuse( $8 );
                            fhs->setSpecular( $10 );
                            fhs->setShininess( $12 );
                            fhs->setTransparency( $14 );
                        }
                | FT_COLOR FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS          { fhs->setColor( $3, $5, $7 ); }		
                | FT_COLOR FT_STRING		
                | FT_DIFFUSE real                                                                               { fhs->setDiffuse( $2 ); }
                | FT_KD real                                                                                    { fhs->setDiffuse( $2 ); }
                | FT_DIFFUSE FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS        { fhs->setDiffuse( $3, $5, $7 ); }
                | FT_SPECULAR real                                                                              { fhs->setSpecular( $2 ); }
                | FT_KS real                                                                                    { fhs->setSpecular( $2 ); }
                | FT_SPECULAR FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS       { fhs->setSpecular( $3, $5, $7 ); }
                | FT_AMBIENT real                                                                               { fhs->setAmbient( $2 ); }
                | FT_AMBIENT FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS        { fhs->setAmbient( $3, $5, $7 ); }
                | FT_EMISSION real                                                                              { fhs->setEmission( $2 ); }
                | FT_EMISSION FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS       { fhs->setEmission( $3, $5, $7 ); }
                | FT_ALPHA real                                                                                 { fhs->setAlpha( $2 ); }
                | FT_TRANSPARENCY real                                                                          { fhs->setTransparency( $2 ); }
                | FT_KT real                                                                                    { fhs->setTransparency( $2 ); }	
                | FT_SHININESS real                                                                             { fhs->setShininess( $2 ); }	
                | FT_TEXTURE string                                                                             { fhs->setTexture( $2 ); }
                | FT_TEXFILTER texfilter_rule texfilter_rule
                | FT_TEXWRAP texwrap_rule texwrap_rule                                                          { fhs->setTexWrap( $2, $3 ); }  
                | FT_TEXQUALITY texquality_rule
                | FT_TEXBLEND texblend_rule
                | FT_TEXTRANSCOLOR vector
                | FT_UNLIT                                                                                      { fhs->setUnlit(true); }		
                | FT_LIT                                                                                        { fhs->setUnlit(false); }
                | FT_MATX string                                                                                { fhs->callMaterialExtension( $2 ); }
                | FT_IOR FT_NUMBER		
                | FT_RAD FT_NUMBER		
                | FT_BRDF FT_STRING		
                | FT_TYPE FT_NUMBER		
                | FT_FLAECHENMASSE FT_NUMBER	
                | FT_KOINZIDENZ FT_NUMBER	
                ;

texfilter_rule  : FT_POINT      { $$ = osg::Texture::NEAREST;              } 
                | FT_LINEAR     { $$ = osg::Texture::LINEAR;               } 
                | FT_MIPMAP     { $$ = osg::Texture::LINEAR_MIPMAP_LINEAR; }
                | FT_SHARPEN    { $$ = osg::Texture::LINEAR;               }
                ;

texwrap_rule    : FT_REPEAT     { $$ = osg::Texture::REPEAT; } 
		        | FT_CLAMP      { $$ = osg::Texture::CLAMP;  }
                | FT_SELECT     { $$ = osg::Texture::REPEAT; }
                ;

texquality_rule : FT_HIGH
                | FT_LOW
                ;

texblend_rule   : FT_MODULATE
                | FT_DECAL
                | FT_REPLACE
                | FT_BLEND
                | FT_ADD
                ;

ppool_rule      : ppool_header FT_LEFT_BRACE ppool_body FT_RIGHT_BRACE
                ;

ppool_header    : FT_PPOOL
                ;

ppool_body      : vectors  
                ;
		
polypool_rule   : polypool_header FT_LEFT_BRACE polypool_body FT_RIGHT_BRACE { fhs->endPolypool(); }
                ;

polypool_header : FT_POLYPOOL { fhs->beginPolypool(); }
                ;

polypool_body   : polygon_elements  
                ;
		                    
light_rule      : FT_LIGHT FT_STRING descriptions FT_LEFT_BRACE light_elements FT_RIGHT_BRACE
                | FT_LIGHT descriptions FT_LEFT_BRACE light_elements FT_RIGHT_BRACE
                ;

light_elements  : /* empty */
                | light_elements light_element
                ;

light_element   : FT_POINTLIGHT FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_POINTLIGHT 
                | FT_DIRECTLIGHT FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_DIRECTLIGHT
                | FT_SPOTLIGHT FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_SPOTLIGHT FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_AMBIENT FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_POSITION FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_COLOR FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_COLOR FT_STRING
                | FT_DIRECTION FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_LVK FT_LEFT_PARENTHESIS FT_STRING opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_BACKGROUND FT_LEFT_PARENTHESIS FT_NUMBER opt_comma FT_NUMBER opt_comma FT_NUMBER FT_RIGHT_PARENTHESIS
                | FT_LIGHTX FT_STRING
                ;

environment_rule: FT_ENVIRONMENT string { fhs->beginEnvironment( $2 ); }          FT_LEFT_BRACE env_elements FT_RIGHT_BRACE { fhs->endEnvironment(); }
                | FT_ENVIRONMENT        { fhs->beginEnvironment( "<unnamed>" ); } FT_LEFT_BRACE env_elements FT_RIGHT_BRACE { fhs->endEnvironment(); }
                ;

env_elements    : env_element
                | env_elements env_element
                ;

env_element     : FT_BACKGROUND FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS { fhs->setEnvBackground( $3, $5, $7 ); }
                | FT_BACKGROUND string                                                                      { fhs->setEnvBackground( $2 ); }
                | FT_AMBIENT FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS    { fhs->setEnvAmbient( $3, $5, $7 ); }
                | FT_AMBIENT string                                                                         { fhs->setEnvAmbient( $2 ); }
                | FT_ENVX FT_STRING
                ;

opt_comma       : /* empty */
                | FT_COMMA
                ;
		
vectors         : /* empty */
                |  vectors vector
                ;
                
vectors2        : /* empty */
                | vectors2 vect2
                ;

vector          : FT_LEFT_PARENTHESIS real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS { fhs->setVector( $2, $4, $6 ); }
                ;

vect4           : FT_LEFT_PARENTHESIS real opt_comma real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS { fhs->setVector4( $2, $4, $6, $8 ); }
                ;

vect2           : FT_LEFT_PARENTHESIS real opt_comma real FT_RIGHT_PARENTHESIS { fhs->setVector2( $2, $4 ); }
                ;

lvect			: FT_LEFT_PARENTHESIS lvect_body FT_RIGHT_PARENTHESIS
                ;
		
lvect_body      : /* empty */
                | lvect_body FT_NUMBER opt_comma
                ;

matrix          : FT_LEFT_PARENTHESIS real opt_comma real opt_comma real opt_comma real opt_comma
                        real opt_comma real opt_comma real opt_comma real opt_comma
                        real opt_comma real opt_comma real opt_comma real opt_comma
		                real opt_comma real opt_comma real opt_comma real FT_RIGHT_PARENTHESIS
                    { 
                        fhs->setMatrix( $2,  $4,  $6,  $8,
                                        $10, $12, $14, $16,
                                        $18, $20, $22, $24,
                                        $26, $28, $30, $32 );
                    }
                ;

transformation  : FT_DCS matrix                            { fhs->setTransformationByMatrix(); }
                | FT_SCS matrix                            { fhs->setTransformationByMatrix(); }
                | FT_MATRIX matrix                         { fhs->setTransformationByMatrix(); }
                | FT_TRANSLATE vector                      { fhs->setTransformationByTranslate(); }
                | FT_ORIENTATION vector vector vector  
                | FT_ROTATE vector FT_NUMBER              
                | FT_ROTATE axis FT_NUMBER                
                | FT_SCALE vector                          { fhs->setTransformationByScale(); }
                | FT_SCALE real                            { fhs->setVector( $2, $2, $2 ); fhs->setTransformationByScale(); }
                | FT_STATIC                            
                | FT_DYNAMIC                           
                ;

axis            : FT_XAXIS
                | FT_YAXIS
                | FT_ZAXIS
                ;

real            : FT_NUMBER { $$ = atof(flexer->YYText()); }
                ;

integer         : FT_NUMBER { $$ = atoi(flexer->YYText()); }
                ;

string          : FT_STRING { $$ = strdup( flexer->YYText() ); }
                ;
				
%%

static int FhsParser_lex()
{
    if (flexer == 0L)
        fprintf(stderr, "Internal error!\n");
    return flexer->yylex();
}

static void FhsParser_error(const char *errmsg)
{
    fprintf(stderr, 
            "\nFhsLoader::parseFile(\"%s\") : %s - Line %d at or before \"%s\"\n",
            fileName.c_str(),
            errmsg, 
            flexer->lineno(),
            flexer->YYText());
}

bool FhsLoader::readFile(const std::string& file)
{
    fileName.clear();    
    fileName = file;
    
    if (fileName.empty())
    {
        fprintf(stderr, "FhsLoader::readFile() - Can't find file \"%s\".\n", file.c_str());
        return false;
    }
    
    bool retval = true;    
    {
        std::ifstream ifs(fileName.c_str());
        flexer = new yyFlexLexer(&ifs);
        fhs = this;
        retval = FhsParser_parse() == 0 ? true : false;
        ifs.close();
        delete flexer;
    }
    
    return retval;
}
