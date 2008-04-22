/******************************************************************************/
/* Project: SceneEdit                                                         */
/* Name   : FhsLexer.l                                                        */
/* Author : Andre Normann                                                     */
/* Date   : 23.08.2007                                                        */
/* License: GPLv2                                                             */
/*                                                                            */
/* Description:                                                               */
/* This is a lexer for the FHS file format.                                   */
/******************************************************************************/

%{
/* System */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Project */
#include "FhsParser.h"    
%}

%option c++
%option noyywrap

%x comment

%%

[+-]+                   yyterminate();
[ \t]+                  /* eat up whitespace */
\r	                    /* eat up */
\n              	    ++yylineno;

([/][/]|[#]).*          /* single line comment */

"/*"				    {BEGIN(comment);}
<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++yylineno;
<comment>"*"+"/"        BEGIN(INITIAL);
 
","				    	{return (FT_COMMA);}
"("				    	{return (FT_LEFT_PARENTHESIS);}
")"				    	{return (FT_RIGHT_PARENTHESIS);}
"{"	                    {return (FT_LEFT_BRACE);}
"}"	                    {return (FT_RIGHT_BRACE);}

"MODEL"       		    {return (FT_MODEL);}
"FHSVERSION"		    {return (FT_VERSION);}

"FILE"       		    {return (FT_EXTFILE);}
"NAME"       		    {return (FT_NAME);}

"ASSEMBLY"     	    	{return (FT_ASSEMBLY);}

"LODASSEMBLY"         	{return (FT_LODASSEMBLY);}
"LOD"     		    	{return (FT_LOD);}

"SWITCH"     	    	{return (FT_SWITCH);}
"WHICHCHILD"	    	{return (FT_WHICHCHILD);}

"LIGHT"     	    	{return (FT_LIGHT);}
"POINTLIGHT"         	{return (FT_POINTLIGHT);}
"DIRECTLIGHT"         	{return (FT_DIRECTLIGHT);}
"SPOTLIGHT"         	{return (FT_SPOTLIGHT);}

"ENVIRONMENT"	    	{return (FT_ENVIRONMENT);}
"BACKGROUND"         	{return (FT_BACKGROUND);}

"LSDIREKT"	         	{return (FT_LSDIREKT);}
"LSFLANKEN"         	{return (FT_LSFLANKEN);}
"TSDIREKT"          	{return (FT_TSDIREKT);}
"TSFLANKEN"         	{return (FT_TSFLANKEN);}
"KSNACHHALL"        	{return (FT_KSNACHHALL);}
"FLAECHENMASSE"	    	{return (FT_FLAECHENMASSE);}
"TRANSMISSION"         	{return (FT_TRANSMISSION);}
"ABSTRAHL"          	{return (FT_ABSTRAHL);}
"INVERLUST"         	{return (FT_INVERLUST);}
"KOINZIDENZ"        	{return (FT_KOINZIDENZ);}
"TERZ"      	    	{return (FT_TERZ);}
"REAL"		          	{return (FT_REAL);}
"KOMPLEX" 	         	{return (FT_KOMPLEX);}
"TENSOR" 	         	{return (FT_TENSOR);}
"OKTAV"              	{return (FT_OKTAV);}
\$.*\n		            {++yylineno; return (FT_SCRIPT);}
"TYPE"			    	{return (FT_TYPE);}
"FLAG"			    	{return (FT_FLAG);}
"LUFT"	                {return (FT_LUFT);}
"TEMP"		            {return (FT_TEMP);}
"ABSEXP"	            {return (FT_ABSEXP);}
"ABSIMP"	            {return (FT_ABSIMP);}
"STREU"			    	{return (FT_STREU);}
"IDEAL"     	    	{return (FT_IDEAL);}

"SCS"			    	{return (FT_SCS);}
"MATRIX"		    	{return (FT_MATRIX);}
"DCS"			    	{return (FT_DCS);}
"STATIC"		    	{return (FT_STATIC);}
"DYNAMIC"		    	{return (FT_DYNAMIC);}
"TRANSLATE"		    	{return (FT_TRANSLATE);}
"ROTATE"		    	{return (FT_ROTATE);}
"SCALE"			    	{return (FT_SCALE);}
"ORIENTATION"	    	{return (FT_ORIENTATION);}

"POLYELEMX"		    	{return (FT_POLYELEMX);}
"OBJECTX"		    	{return (FT_OBJECTX);}
"DESCX"			    	{return (FT_DESCX);}
"PRIMX"			    	{return (FT_PRIMX);}
"POLYX"			    	{return (FT_POLYX);}
"MATX"			    	{return (FT_MATX);}
"ENVX"			    	{return (FT_ENVX);}
"LIGHTX"		    	{return (FT_LIGHTX);}

"BBOX"			    	{return (FT_BBOX);}
"RESOLUTION"	    	{return (FT_RESOLUTION);}
"ID"			    	{return (FT_ID);}

"BILLBOARD"		    	{return (FT_BILLBOARD);}
"XAXIS"			    	{return (FT_XAXIS);}
"YAXIS"			    	{return (FT_YAXIS);}
"ZAXIS"			    	{return (FT_ZAXIS);}
"CENTER"		    	{return (FT_CENTER);}
"AXIS"			    	{return (FT_AXIS);}

"GEOMETRY"       	    {return (FT_GEOMETRY);}
"LODGEOMETRY"           {return (FT_LODGEOMETRY);}
"TUBEGEOMETRY"          {return (FT_TUBEGEOMETRY);}
"DEFMAT"   		    	{return (FT_DEFMAT);}
"USEMAT"   		    	{return (FT_USEMAT);}
"POLYGON"     	    	{return (FT_POLYGON);}
"TRISTRIP"     	    	{return (FT_TRISTRIP);}
"TRIANGLE_STRIP"        {return (FT_TRISTRIP);}
"TRIFAN"     	    	{return (FT_TRIFAN);}
"TRIANGLE_FAN"	    	{return (FT_TRIFAN);}
"TRIANGLES"         	{return (FT_TRIANGLES);}
"QUADSTRIP"         	{return (FT_QUADSTRIP);}
"QUAD_STRIP"         	{return (FT_QUADSTRIP);}
"QUADS"     	    	{return (FT_QUADS);}
"TUBE"     		    	{return (FT_TUBE);}
"CLOSEDLINE"         	{return (FT_CLOSEDLINE);}
"LINE_LOOP"         	{return (FT_CLOSEDLINE);}
"LINE"     		    	{return (FT_LINE);}
"LINE_STRIP"         	{return (FT_LINE);}
"LINES"     	    	{return (FT_LINES);}
"POINTS"     	    	{return (FT_POINTS);}
"EXTRUDE"	            {return (FT_EXTRUDE);}
"REVOLUTE"		    	{return (FT_REVOLUTE);}
"PPOOL"    		    	{return (FT_PPOOL);}
"POLYPOOL"    	    	{return (FT_POLYPOOL);}
"SPHERE"     	    	{return (FT_SPHERE);}
"CONE"     		    	{return (FT_CONE);}
"TORUS"     	    	{return (FT_TORUS);}
"TEXT"     		    	{return (FT_TEXT);}
"BOX"			    	{return (FT_BOX);}
"F"[ \t]*"="	    	{return (FT_FONT);}
"C"[ \t]*"="	    	{return (FT_COL);}
"N"[ \t]*"="	    	{return (FT_NORM);}
"T"[ \t]*"="	    	{return (FT_TEX);}
"T3"[ \t]*"="	    	{return (FT_TEX);}
"P"[ \t]*"="	    	{return (FT_POS);}
"I"[ \t]*"="	    	{return (FT_IND);}
"R"[ \t]*"="	    	{return (FT_RAD);}
"TR"[ \t]*"="	    	{return (FT_TRANSP);}
"CI"[ \t]*"="	    	{return (FT_CIND);}
"CTRI"[ \t]*"="	    	{return (FT_CTRIND);}
"NI"[ \t]*"="	    	{return (FT_NIND);}
"TI"[ \t]*"="	    	{return (FT_TIND);}
"T3I"[ \t]*"="	    	{return (FT_T3IND);}
"IC"[ \t]*"="	    	{return (FT_CIND);}
"ICTR"[ \t]*"="	    	{return (FT_CTRIND);}
"IN"[ \t]*"="	    	{return (FT_NIND);}
"IT"[ \t]*"="	    	{return (FT_TIND);}
"IT3"[ \t]*"="	    	{return (FT_T3IND);}
"CNI"[ \t]*"="	    	{return (FT_CNIND);}
"CTI"[ \t]*"="	    	{return (FT_CTIND);}
"CT3I"[ \t]*"="	    	{return (FT_CT3IND);}
"CTRNI"[ \t]*"="    	{return (FT_CTRNIND);}
"CTRTI"[ \t]*"="    	{return (FT_CTRTIND);}
"CTRT3I"[ \t]*"="    	{return (FT_CTRT3IND);}
"TNI"[ \t]*"="	    	{return (FT_TNIND);}
"T3NI"[ \t]*"="	    	{return (FT_T3NIND);}
"NCI"[ \t]*"="	    	{return (FT_CNIND);}
"TCI"[ \t]*"="	    	{return (FT_CTIND);}
"T3CI"[ \t]*"="	    	{return (FT_CT3IND);}
"NCTRI"[ \t]*"="    	{return (FT_CTRNIND);}
"TCTRI"[ \t]*"="    	{return (FT_CTRTIND);}
"T3CTRI"[ \t]*"="    	{return (FT_CTRT3IND);}
"NTI"[ \t]*"="	    	{return (FT_TNIND);}
"NT3I"[ \t]*"="	    	{return (FT_T3NIND);}
"CIN"[ \t]*"="	    	{return (FT_CNIND);}
"CIT"[ \t]*"="	    	{return (FT_CTIND);}
"CIT3"[ \t]*"="	    	{return (FT_CT3IND);}
"CTRIN"[ \t]*"="    	{return (FT_CTRNIND);}
"CTRIT"[ \t]*"="    	{return (FT_CTRTIND);}
"CTRIT3"[ \t]*"="    	{return (FT_CTRT3IND);}
"TIN"[ \t]*"="	    	{return (FT_TNIND);}
"T3IN"[ \t]*"="	    	{return (FT_T3NIND);}
"NIC"[ \t]*"="	    	{return (FT_CNIND);}
"TIC"[ \t]*"="	    	{return (FT_CTIND);}
"T3IC"[ \t]*"="	    	{return (FT_CT3IND);}
"NICTR"[ \t]*"="    	{return (FT_CTRNIND);}
"TICTR"[ \t]*"="    	{return (FT_CTRTIND);}
"T3ICTR"[ \t]*"="    	{return (FT_CTRT3IND);}
"NIT"[ \t]*"="	    	{return (FT_TNIND);}
"NIT3"[ \t]*"="	    	{return (FT_T3NIND);}
"ICN"[ \t]*"="	    	{return (FT_CNIND);}
"ICT"[ \t]*"="	    	{return (FT_CTIND);}
"ICT3"[ \t]*"="	    	{return (FT_CT3IND);}
"ICTRN"[ \t]*"="    	{return (FT_CTRNIND);}
"ICTRT"[ \t]*"="    	{return (FT_CTRTIND);}
"ICTRT3"[ \t]*"="    	{return (FT_CTRT3IND);}
"ITN"[ \t]*"="	    	{return (FT_TNIND);}
"IT3N"[ \t]*"="	    	{return (FT_T3NIND);}
"INC"[ \t]*"="	    	{return (FT_CNIND);}
"ITC"[ \t]*"="	    	{return (FT_CTIND);}
"IT3C"[ \t]*"="		    {return (FT_CT3IND);}
"INCTR"[ \t]*"="	    {return (FT_CTRNIND);}
"ITCTR"[ \t]*"="	    {return (FT_CTRTIND);}
"IT3CTR"[ \t]*"="	    {return (FT_CTRT3IND);}
"INT"[ \t]*"="	    	{return (FT_TNIND);}
"INT3"[ \t]*"="	    	{return (FT_T3NIND);}
"CTNI"[ \t]*"="	    	{return (FT_CTNIND);}
"CT3NI"[ \t]*"="    	{return (FT_CT3NIND);}
"TCNI"[ \t]*"="	    	{return (FT_CTNIND);}
"T3CNI"[ \t]*"="    	{return (FT_CT3NIND);}
"CNTI"[ \t]*"="	    	{return (FT_CTNIND);}
"CNT3I"[ \t]*"="    	{return (FT_CT3NIND);}
"TNCI"[ \t]*"="	    	{return (FT_CTNIND);}
"T3NCI"[ \t]*"="    	{return (FT_CT3NIND);}
"NCTI"[ \t]*"="	    	{return (FT_CTNIND);}
"NCT3I"[ \t]*"="    	{return (FT_CT3NIND);}
"NTCI"[ \t]*"="	    	{return (FT_CTNIND);}
"NT3CI"[ \t]*"="    	{return (FT_CT3NIND);}
"CTIN"[ \t]*"="	    	{return (FT_CTNIND);}
"CT3IN"[ \t]*"="    	{return (FT_CT3NIND);}
"TCIN"[ \t]*"="	    	{return (FT_CTNIND);}
"T3CIN"[ \t]*"="    	{return (FT_CT3NIND);}
"CNIT"[ \t]*"="	    	{return (FT_CTNIND);}
"CNIT3"[ \t]*"="    	{return (FT_CT3NIND);}
"TNIC"[ \t]*"="	    	{return (FT_CTNIND);}
"T3NIC"[ \t]*"="    	{return (FT_CT3NIND);}
"NCIT"[ \t]*"="	    	{return (FT_CTNIND);}
"NCIT3"[ \t]*"="    	{return (FT_CT3NIND);}
"NTIC"[ \t]*"="	    	{return (FT_CTNIND);}
"NT3IC"[ \t]*"="    	{return (FT_CT3NIND);}
"CITN"[ \t]*"="	    	{return (FT_CTNIND);}
"CIT3N"[ \t]*"="    	{return (FT_CT3NIND);}
"TICN"[ \t]*"="	    	{return (FT_CTNIND);}
"T3ICN"[ \t]*"="    	{return (FT_CT3NIND);}
"CINT"[ \t]*"="	    	{return (FT_CTNIND);}
"CINT3"[ \t]*"="	    {return (FT_CT3NIND);}
"TINC"[ \t]*"="	    	{return (FT_CTNIND);}
"T3INC"[ \t]*"="    	{return (FT_CT3NIND);}
"NICT"[ \t]*"="	    	{return (FT_CTNIND);}
"NICT3"[ \t]*"="    	{return (FT_CT3NIND);}
"NITC"[ \t]*"="	    	{return (FT_CTNIND);}
"NIT3C"[ \t]*"="    	{return (FT_CT3NIND);}
"ICTN"[ \t]*"="	    	{return (FT_CTNIND);}
"ICT3N"[ \t]*"="    	{return (FT_CT3NIND);}
"ITCN"[ \t]*"="	    	{return (FT_CTNIND);}
"IT3CN"[ \t]*"="    	{return (FT_CT3NIND);}
"ICNT"[ \t]*"="	    	{return (FT_CTNIND);}
"ICNT3"[ \t]*"="    	{return (FT_CT3NIND);}
"ITNC"[ \t]*"="	    	{return (FT_CTNIND);}
"IT3NC"[ \t]*"="    	{return (FT_CT3NIND);}
"INCT"[ \t]*"="	    	{return (FT_CTNIND);}
"INCT3"[ \t]*"="    	{return (FT_CT3NIND);}
"INTC"[ \t]*"="	    	{return (FT_CTNIND);}
"INT3C"[ \t]*"="    	{return (FT_CT3NIND);}
"CTRTNI"[ \t]*"="    	{return (FT_CTRTNIND);}
"CTRT3NI"[ \t]*"="    	{return (FT_CTRT3NIND);}
"TCTRNI"[ \t]*"="    	{return (FT_CTRTNIND);}
"T3CTRNI"[ \t]*"="    	{return (FT_CTRT3NIND);}
"CTRNTI"[ \t]*"="    	{return (FT_CTRTNIND);}
"CTRNT3I"[ \t]*"="    	{return (FT_CTRT3NIND);}
"TNCTRI"[ \t]*"="    	{return (FT_CTRTNIND);}
"T3NCTRI"[ \t]*"="	    {return (FT_CTRT3NIND);}
"NCTRTI"[ \t]*"="	    {return (FT_CTRTNIND);}
"NCTRT3I"[ \t]*"="    	{return (FT_CTRT3NIND);}
"NTCTRI"[ \t]*"="    	{return (FT_CTRTNIND);}
"NT3CTRI"[ \t]*"="    	{return (FT_CTRT3NIND);}
"CTRTIN"[ \t]*"="    	{return (FT_CTRTNIND);}
"CTRT3IN"[ \t]*"="	    {return (FT_CTRT3NIND);}
"TCTRIN"[ \t]*"="    	{return (FT_CTRTNIND);}
"T3CTRIN"[ \t]*"="	    {return (FT_CTRT3NIND);}
"CTRNIT"[ \t]*"="    	{return (FT_CTRTNIND);}
"CTRNIT3"[ \t]*"="    	{return (FT_CTRT3NIND);}
"TNICTR"[ \t]*"="    	{return (FT_CTRTNIND);}
"T3NICTR"[ \t]*"="	    {return (FT_CTRT3NIND);}
"NCTRIT"[ \t]*"="	    {return (FT_CTRTNIND);}
"NCTRIT3"[ \t]*"="	    {return (FT_CTRT3NIND);}
"NTICTR"[ \t]*"="	    {return (FT_CTRTNIND);}
"NT3ICTR"[ \t]*"="	    {return (FT_CTRT3NIND);}
"CTRITN"[ \t]*"="	    {return (FT_CTRTNIND);}
"CTRIT3N"[ \t]*"="      {return (FT_CTRT3NIND);}
"TICTRN"[ \t]*"="    	{return (FT_CTRTNIND);}
"T3ICTRN"[ \t]*"="    	{return (FT_CTRT3NIND);}
"CTRINT"[ \t]*"="    	{return (FT_CTRTNIND);}
"CTRINT3"[ \t]*"="    	{return (FT_CTRT3NIND);}
"TINCTR"[ \t]*"="    	{return (FT_CTRTNIND);}
"T3INCTR"[ \t]*"="    	{return (FT_CTRT3NIND);}
"NICTRT"[ \t]*"="    	{return (FT_CTRTNIND);}
"NICTRT3"[ \t]*"="    	{return (FT_CTRT3NIND);}
"NITCTR"[ \t]*"="    	{return (FT_CTRTNIND);}
"NIT3CTR"[ \t]*"="    	{return (FT_CTRT3NIND);}
"ICTRTN"[ \t]*"="    	{return (FT_CTRTNIND);}
"ICTRT3N"[ \t]*"="    	{return (FT_CTRT3NIND);}
"ITCTRN"[ \t]*"="    	{return (FT_CTRTNIND);}
"IT3CTRN"[ \t]*"="    	{return (FT_CTRT3NIND);}
"ICTRNT"[ \t]*"="    	{return (FT_CTRTNIND);}
"ICTRNT3"[ \t]*"="    	{return (FT_CTRT3NIND);}
"ITNCTR"[ \t]*"="    	{return (FT_CTRTNIND);}
"IT3NCTR"[ \t]*"="    	{return (FT_CTRT3NIND);}
"INCTRT"[ \t]*"="    	{return (FT_CTRTNIND);}
"INCTRT3"[ \t]*"="    	{return (FT_CTRT3NIND);}
"INTCTR"[ \t]*"="    	{return (FT_CTRTNIND);}
"INT3CTR"[ \t]*"="    	{return (FT_CTRT3NIND);}

"COLOR"			    	{return (FT_COLOR);}
"DIFFUSE"		    	{return (FT_DIFFUSE);}
"KD"			    	{return (FT_KD);}
"SPECULAR"		    	{return (FT_SPECULAR);}
"AMBIENT"		    	{return (FT_AMBIENT);}
"EMISSION"		    	{return (FT_EMISSION);}
"KS"			    	{return (FT_KS);}
"KT"			    	{return (FT_KT);}
"ALPHA"			    	{return (FT_ALPHA);}
"TRANSPARENCY"	    	{return (FT_TRANSPARENCY);}
"SHININESS"		    	{return (FT_SHININESS);}
"IOR"			    	{return (FT_IOR);}
"BRDF"			    	{return (FT_BRDF);}
"RAD"			    	{return (FT_RAD);}
"PATCHES"		    	{return (FT_PATCHES);}
"UNIFORM"		    	{return (FT_UNIFORM);}
"ADAPTIVE"		    	{return (FT_ADAPTIVE);}
"POOL"			    	{return (FT_POOL);}
"POSITION"		    	{return (FT_POSITION);}
"DIRECTION"		    	{return (FT_DIRECTION);}
"LVK"			    	{return (FT_LVK);}
"TEXFILTER"		    	{return (FT_TEXFILTER);}
"TEXWRAP"		    	{return (FT_TEXWRAP);}
"TEXQUALITY"	    	{return (FT_TEXQUALITY);}
"TEXBLEND"		    	{return (FT_TEXBLEND);}
"TEXTRANSCOLOR"	    	{return (FT_TEXTRANSCOLOR);}
"MODULATE"		    	{return (FT_MODULATE);}
"DECAL"			    	{return (FT_DECAL);}
"BLEND"		        	{return (FT_BLEND);}
"REPLACE"		    	{return (FT_REPLACE);}
"ADD"			    	{return (FT_ADD);}
"POINT"			    	{return (FT_POINT);}
"LINEAR"		    	{return (FT_LINEAR);}
"MIPMAP"		    	{return (FT_MIPMAP);}
"SHARPEN"		    	{return (FT_SHARPEN);}
"REPEAT"		    	{return (FT_REPEAT);}
"CLAMP"			    	{return (FT_CLAMP);}
"SELECT"		    	{return (FT_SELECT);}
"HIGH"			    	{return (FT_HIGH);}
"LOW"			    	{return (FT_LOW);}
"FROM"     		    	{return (FT_FROM);}
"TEXTURE"     	    	{return (FT_TEXTURE);}
"UNLIT"			    	{return (FT_UNLIT);}
"LIT"			    	{return (FT_LIT);}

"SOUNDSOURCE"           {return (FT_SOUNDSOURCE);}
"LIVE"                  {return (FT_LIVE);}
"LOOP"                  {return (FT_LOOP);}
"ACTIVE"                {return (FT_ACTIVE);}
"GAIN"                  {return (FT_GAIN);}
"PITCH"                 {return (FT_PITCH);}
"STARTTIME"  	    	{return (FT_STARTTIME);}
"STOPTIME"  	    	{return (FT_STOPTIME);}
"FILENAME"     	    	{return (FT_FILENAME);}

"POINTORDER"		    {return (FT_POINTORDER);}
"CW"				    {return (FT_CW);}
"CCW"				    {return (FT_CCW);}
"CLOCKWISE"		    	{return (FT_CW);}
"COUNTERCLOCKWISE"    	{return (FT_CCW);}

["]                     {
                            /* read until next '"' */
                            char c;
                            int i = 0;
                            while ((c = yyinput()) != '"')
                                yytext[i++] = c;
                            yytext[i] = '\0';
                            return (FT_STRING);
                        }

[~/a-zA-Z!@#$%^&*<>._\[\]=|':;?][~/a-zA-Z0-9+!@#$%^&<>._\[\]|':;?-]*	{return (FT_STRING);}


[+-]?([0-9]+"."?[0-9]*|[0-9]*"."?[0-9]+)([eE][+-]?[0-9]+)?     {return (FT_NUMBER);}
[+-]?([0-9]+"."?[0-9]*|[0-9]*"."?[0-9]+)([eE][+-]?[0-9]+)?[dD] {return (FT_NUMBER);}
 
%%
