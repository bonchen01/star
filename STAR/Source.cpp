#include "stdafx.h"

#include <windows.h>
#include <windowsx.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dwmapi.h>
#include <tchar.h>	


//define program and shader fun name
PFNGLATTACHSHADERPROC glAttachShader;	
PFNGLSHADERSOURCEPROC glShaderSource;	
PFNGLCOMPILESHADERPROC glCompileShader; 
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

static GLuint vertShader;		//vert Shader
static GLuint fragShader;		//fragShader
static GLuint glslProgram;		//glsl Program

#pragma comment(lib,"glew32.lib")			//add glew32.lib
#pragma comment (lib, "opengl32.lib")		//add opengl32.lib
#pragma comment (lib, "glu32.lib")			//add glu32.lib
#pragma comment (lib, "dwmapi.lib")			//add dwmapi.lib


const TCHAR szAppName[] = _T("TransparentGL");		//lpClassName
const TCHAR wcWndName[] = _T("TransparentGL");		//lpWindowsName

#define SLEEP_TIME	30				// between two render time
#define FRAG_FILE	"range.frag"	// frag shader file name	
#define Aphanous	 true			// need background 

HDC hDC;							//
HGLRC m_hrc;						//
#define w	1920					//windows width
#define h	1024					//windows height


int readShaderSource(GLuint shader, char *file)			//read Shader file
{
	FILE *fp;
	const GLchar *source;
	GLsizei length;
	int ret;
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");	//get shader source function name
	ret = fopen_s(&fp, file, "rb");													// open shader file
	if (ret) {
		return -1;
	}
	fseek(fp, 0L, SEEK_END);														//point file end
	length = ftell(fp);																//get file size
	source = (GLchar *)malloc(length);												//malloc file size memory
	if (source == NULL) {															//malloc false
		fprintf(stderr, "Could not allocate read buffer./n");						//out put false message
		return -1;																	//return -1
	}
	fseek(fp, 0L, SEEK_SET);														//point file star
	ret = fread((void *)source, 1, length, fp) != (size_t)length;					//read shader file all size
	fclose(fp);																		//close shader file
	if (ret)																		//out put false read file
		fprintf(stderr, "Could not read file: %s./n", file);
	else
		glShaderSource(shader, 1, &source, &length);								//put shader source into shader
	free((void *)source);															//free file size memory
	return ret;																		//return 
}



//out put Shader LOG

void printShaderInfoLog(GLuint shader)
{
	GLsizei bufSize;
	GLchar *infoLog;
	GLsizei length;
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");	//get function name of Shader info Log
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);		//get info size
	if (bufSize > 0) {											//have info need output	
		infoLog = (GLchar *)malloc(bufSize);					//malloc info size
		if (infoLog != NULL) {
			glGetShaderInfoLog(shader, bufSize, &length, infoLog);	//put shader info to infolog
			printf("%s/n", infoLog);								//output shader info
			free(infoLog);											//free info memory
		}
		else
			printf("Could not allocate InfoLog buffer./n");			//not enough
	}
}



//out put program info LOG

void printProgramInfoLog(GLuint program)
{
	GLsizei bufSize;
	GLchar *infoLog;
	GLsizei length;

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"); //get the function's name of the Programe info log
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);			//get the size of program info log
	if (bufSize > 0) {												//have info log need to out put
		infoLog = (GLchar *)malloc(bufSize);						//malloc info log size
		if (infoLog != NULL) {										
			glGetProgramInfoLog(program, bufSize, &length, infoLog);//get info log input infolog
			printf("%s/n", infoLog);								//output info log
			free(infoLog);											//free info log's memory
		}
		else
			printf("Could not allocate InfoLog buffer./n");			//output memory is not enough
	}
}

//初始化
void init(void)
{
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");		//get the name of Attach Shader function
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");		//get the name of Compile Shader function
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");		//get the name of Create Program function
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");		//get the name of Create Shader function
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");			//get the name of Link Program function
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");				//get the name of Use Program function
	glGetShaderiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetShaderiv");			//get the name of Get Shaderiv function
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");		//get the name of Get Programiv function 
	GLint compiled, linked;

	vertShader = glCreateShader(GL_VERTEX_SHADER);			//get vert Shader id
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);		//get frag Shader id

	readShaderSource(vertShader, "TestGLBlit.vert");		//read vert shader from file 
	readShaderSource(fragShader, FRAG_FILE);				//read frag shader from file

	glCompileShader(vertShader);							//compile vert shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);//get vert shader compile message
	if (compiled == GL_FALSE) {								//compile is false
		printShaderInfoLog(vertShader);						//out put the compile info log
		printf("Compile error in vertex shader./n");		//out put vert shander error 
		exit(1);											//exit program
	}

	glCompileShader(fragShader);							//compile frag shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);//get frag shader compile message
	if (compiled == GL_FALSE) {								//compile is false
		printShaderInfoLog(fragShader);						//out put the compile info log
		printf("Compile error in vertex shader./n");		//out put frag shander error 
		exit(1);											//exit program
	}
	glslProgram = glCreateProgram();						//Create Program
	glAttachShader(glslProgram, vertShader);				//band Program with vert Shader
	glAttachShader(glslProgram, fragShader);				//band Program with frag Shader

	glLinkProgram(glslProgram);								//Link Program
	glGetProgramiv(glslProgram, GL_LINK_STATUS, &linked);	//Get Link message
	if (linked == GL_FALSE) {								//Link false
		printProgramInfoLog(glslProgram);					//output link false message
		printf("Link error./n");							//out put link error
		exit(1);											//exit program
	}
	glUseProgram(glslProgram);								//choise use program

}



LRESULT CALLBACK WindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {		//Windows Function
	PAINTSTRUCT ps;

	switch (msg) {									
	case WM_CREATE:								//creat
		break;

	case WM_DESTROY:							//destroy
		if (m_hrc) {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hrc);
		}
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);	//default
	}

	return 0;
}




GLfloat iTime = 0.1;
int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode) {
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEX size
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);	//icon set 	
	wc.style = CS_HREDRAW | CS_VREDRAW;				//move resize will redraw
	wc.lpfnWndProc = (WNDPROC)WindowFunc;			//windows funciton 
	wc.hInstance = hThisInst;						//this  handle
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		//icon set
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		//cursor handle
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(0x00000000);	//set background
	wc.lpszClassName = szAppName;								//Class Name

	if (!RegisterClassEx(&wc)) {					//register Class Ex false
		return FALSE;								// return false
	}

	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, wcWndName,
		WS_VISIBLE | WS_POPUP, 0, 0, w, h,
		NULL, NULL, hThisInst, NULL);				//creat a window 

	if (!hWnd) {									//false
		return FALSE;								//return false
	}
	if(Aphanous == true)							//need aphanous
	{
		DWM_BLURBEHIND bb = { 0 };					
		HRGN hRgn = CreateRectRgn(0, 0, -1, -1);		//creat rect			need gdi32.lib
		bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;//set class style
		bb.hRgnBlur = hRgn;								//set rect
		bb.fEnable = TRUE;								//enalbe
		DwmEnableBlurBehindWindow(hWnd, &bb);			//enable windows aphanous
	}
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                                // Version Number
		PFD_DRAW_TO_WINDOW |			  // Format Must Support Window
		PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
		PFD_SUPPORT_COMPOSITION |         // Format Must Support Composition
		PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
		PFD_TYPE_RGBA,                    // Request An RGBA Format
		32,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
		8,                                // An Alpha Buffer
		0,                                // Shift Bit Ignored
		0,                                // No Accumulation Buffer
		0, 0, 0, 0,                       // Accumulation Bits Ignored
		24,                               // 16Bit Z-Buffer (Depth Buffer)
		8,                                // Some Stencil Buffer
		0,                                // No Auxiliary Buffer
		PFD_MAIN_PLANE,                   // Main Drawing Layer
		0,                                // Reserved
		0, 0, 0                           // Layer Masks Ignored
	};

	HDC hdc = GetDC(hWnd);								//evice context of device that the rendering context			
	int PixelFormat = ChoosePixelFormat(hdc, &pfd);		//get pix format
	if (PixelFormat == 0) {								//get false
		return FALSE;									//return false
	}

	BOOL bResult = SetPixelFormat(hdc, PixelFormat, &pfd);	//set pix format
	if (bResult == FALSE) {									//set false
		return FALSE;										//return false
	}

	m_hrc = wglCreateContext(hdc);							//create a rendering context
	if (!m_hrc) {											//creat false
		return FALSE;										//return false
	}

	wglMakeCurrent(hdc, m_hrc);								//choise current render
	glEnable(GL_ALPHA_TEST);								//enable alpha
	glEnable(GL_DEPTH_TEST);								//enable depth
	glEnable(GL_BLEND);										//enable blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//set alpha is MINUS mode
	glewInit();												//init glew
	init();													//init shader and program

	int mvploc = glGetUniformLocation(glslProgram, "resolution");	//get shader param "resolution" ID
	if (mvploc == -1)												//not existence
		printf("error2 \n");										//out put error
	float resolution[2] = { w,h };									//set resolution

	glUniform2f(mvploc, resolution[0], resolution[1]);				//put resolution into shader "resolution"
	int loop = 0;								
	for(loop = 0;loop<200;loop++)									//loop 200
	{
		int mvploc = glGetUniformLocation(glslProgram, "time");		//get shader param "time"
		if (mvploc == -1)											//not existence
			printf("error \n");										//out put error
		glUniform1f(mvploc, iTime);									//put iTime into shader "time"
		iTime += 0.1;												//
		if (iTime >= 180.0)											//turn back.											
			iTime = 0.1;

		glClearColor(0, 0, 0, 0);									//clear render 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//do clear
		glRectf(-1, -1, 1.0, 1.0);									//draw rectf
		glFlush();													//wait finish draw
		SwapBuffers(hdc);											//swap DC buffer	
		Sleep(SLEEP_TIME);											//sleep

	}

	ReleaseDC(hWnd, hdc);											//release DC
	return (TRUE);													//End return True
}
