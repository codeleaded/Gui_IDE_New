/*
#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/ObjectNotation.h"

#define SYNTAX_PATH     "/home/codeleaded/System/SyntaxFiles/Path_Syntax.alxon"
#define SYNTAX_C        "/home/codeleaded/System/SyntaxFiles/C_Syntax.alxon"
#define SYNTAX_OMML     "/home/codeleaded/System/SyntaxFiles/OMML_Syntax.alxon"

#define EDISPLAY_SIDE_NONE          0
#define EDISPLAY_SIDE_NORTH         1
#define EDISPLAY_SIDE_SOUTH         2
#define EDISPLAY_SIDE_WEST          3
#define EDISPLAY_SIDE_EAST          4
#define EDISPLAY_SIDE_NORTHWEST     5
#define EDISPLAY_SIDE_NORTHEAST     6
#define EDISPLAY_SIDE_SOUTHWEST     7
#define EDISPLAY_SIDE_SOUTHEAST     8

typedef struct EDisplay {
    Editor Field;
    String File;
    float DistMaxX;
    float DistMaxY;
    char Side;
    char ShowLines;
    char IntellisenseOpen;
} EDisplay;

EDisplay EDisplay_New(float x,float y,float w,float h,int Lines,char *Path, int Columns, int Rows, int CharSizeX, int CharSizeY, int AlxFontSizeX, int AlxFontSizeY,char* Syntax,char* File){
    EDisplay d;
    d.Field = Editor_New(Input_New(INPUT_MAXLENGTH,Lines),(Rect){ { x,y },{ w,h } },Path,Columns,Rows,CharSizeX,CharSizeY,AlxFontSizeX,AlxFontSizeY,BLACK);
    //Input_SetText(&d.Field.In,"");
    Editor_SetSyntax(&d.Field,Syntax);
    d.File = String_Make(File);

    d.DistMaxX = 5.0f;
    d.DistMaxY = 5.0f;

    d.Side = EDISPLAY_SIDE_NONE;
    d.ShowLines = 0;
    d.IntellisenseOpen = 0; 
    return d;
}
void EDisplay_Update(EDisplay* e,States* strokes,Vec2 Mouse){
    Editor_Update(&e->Field,strokes,GetMouse());
    
    if(e->Field.In.Enabled){
        if(Input_Stroke(&e->Field.In,ALX_MOUSE_L).PRESSED){
            float we = F32_Abs(Mouse.x - e->Field.r.p.x);
            float ea = F32_Abs(Mouse.x - (e->Field.r.p.x + e->Field.r.d.x));
            float no = F32_Abs(Mouse.y - e->Field.r.p.y);
            float so = F32_Abs(Mouse.y - (e->Field.r.p.y + e->Field.r.d.y));

            if(     no < e->DistMaxY && we < e->DistMaxX)   e->Side = EDISPLAY_SIDE_NORTHWEST;
            else if(no < e->DistMaxY && ea < e->DistMaxX)   e->Side = EDISPLAY_SIDE_NORTHEAST;
            else if(so < e->DistMaxY && we < e->DistMaxX)   e->Side = EDISPLAY_SIDE_SOUTHWEST;
            else if(so < e->DistMaxY && ea < e->DistMaxX)   e->Side = EDISPLAY_SIDE_SOUTHEAST;
            else if(no < e->DistMaxY)                       e->Side = EDISPLAY_SIDE_NORTH;
            else if(so < e->DistMaxY)                       e->Side = EDISPLAY_SIDE_SOUTH;
            else if(we < e->DistMaxX)                       e->Side = EDISPLAY_SIDE_WEST;
            else if(ea < e->DistMaxX)                       e->Side = EDISPLAY_SIDE_EAST;
        }
        
        if(Input_Stroke(&e->Field.In,ALX_KEY_CTRL).DOWN && Input_Stroke(&e->Field.In,ALX_KEY_SHIFT).PRESSED){
            e->IntellisenseOpen = !e->IntellisenseOpen;
        }

        if(e->Side!=EDISPLAY_SIDE_NONE){
            if(e->Side == EDISPLAY_SIDE_NORTH){
                e->Field.r.d.y += e->Field.r.p.y - Mouse.y;
                e->Field.r.p.y = Mouse.y;
            }else if(e->Side == EDISPLAY_SIDE_SOUTH){
                e->Field.r.d.y += Mouse.y - (e->Field.r.p.y + e->Field.r.d.y);
            }else if(e->Side == EDISPLAY_SIDE_WEST){
                e->Field.r.d.x += e->Field.r.p.x - Mouse.x;
                e->Field.r.p.x = Mouse.x;
            }else if(e->Side == EDISPLAY_SIDE_EAST){
                e->Field.r.d.x += Mouse.x - (e->Field.r.p.x + e->Field.r.d.x);
            }else if(e->Side == EDISPLAY_SIDE_NORTHWEST){
                e->Field.r.d.y += e->Field.r.p.y - Mouse.y;
                e->Field.r.p.y = Mouse.y;
                e->Field.r.d.x += e->Field.r.p.x - Mouse.x;
                e->Field.r.p.x = Mouse.x;
            }else if(e->Side == EDISPLAY_SIDE_NORTHEAST){
                e->Field.r.d.y += e->Field.r.p.y - Mouse.y;
                e->Field.r.p.y = Mouse.y;
                e->Field.r.d.x += Mouse.x - (e->Field.r.p.x + e->Field.r.d.x);
            }else if(e->Side == EDISPLAY_SIDE_SOUTHWEST){
                e->Field.r.d.y += Mouse.y - (e->Field.r.p.y + e->Field.r.d.y);
                e->Field.r.d.x += e->Field.r.p.x - Mouse.x;
                e->Field.r.p.x = Mouse.x;
            }else if(e->Side == EDISPLAY_SIDE_SOUTHEAST){
                e->Field.r.d.y += Mouse.y - (e->Field.r.p.y + e->Field.r.d.y);
                e->Field.r.d.x += Mouse.x - (e->Field.r.p.x + e->Field.r.d.x);
            }
        }
    }

    if(Input_Stroke(&e->Field.In,ALX_MOUSE_L).RELEASED){
        e->Side = EDISPLAY_SIDE_NONE;
    }

    e->Field.r.d.x = F32_Clamp(e->Field.r.d.x,10.0f,GetWidth());
    e->Field.r.d.y = F32_Clamp(e->Field.r.d.y,10.0f,GetHeight());
    e->Field.r.p.x = F32_Clamp(e->Field.r.p.x,0.0f,GetWidth() - e->Field.r.d.x);
    e->Field.r.p.y = F32_Clamp(e->Field.r.p.y,0.0f,GetHeight() - e->Field.r.d.y);
}
void EDisplay_Render(unsigned int* Target,int Target_Width,int Target_Height,EDisplay* e){
    Editor_Render(Target,Target_Width,Target_Height,&e->Field);

    if(e->IntellisenseOpen){
        int CurserX = Input_CurserX(&e->Field.In,e->Field.In.Curser);
        int CurserY = Input_CurserY(&e->Field.In,e->Field.In.Curser);
        int posx = e->Field.r.p.x+(CurserX + e->Field.ScrollX) *  e->Field.font.CharSizeX - e->Field.font.CharSizeX;
        int posy = e->Field.r.p.y+(CurserY + e->Field.ScrollY) * (e->Field.font.CharSizeY * INPUT_GAP_FAKTOR) + e->Field.font.CharSizeY;
        int boxw = 100;
        int boxh = 100;
        
        posx = F32_Clamp(posx,e->Field.r.p.x,e->Field.r.p.x + e->Field.r.d.x - boxw);
        posx = F32_Clamp(posx,e->Field.r.p.x,e->Field.r.p.y + e->Field.r.d.y - boxh);
        Rect_RenderXX(Target,Target_Width,Target_Height,posx,posy,boxw,boxh,BLACK);
        Rect_RenderXXWire(Target,Target_Width,Target_Height,posx,posy,boxw,boxh,WHITE,1.0f);
    }
}
void EDisplay_Free(EDisplay* d){
    Editor_Free(&d->Field);
}

*/
