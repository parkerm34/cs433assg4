// ch01.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////
//  triangles.cpp  from the OpenGL Red Book   Chapter 1
//////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#include "vgl.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/mat4x4.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "LoadShaders.h"

using namespace glm;

void init( void );

vec3 viewUp( 0.0f, 0.0f, 1.0f );
vec3 eye( 1.0f, 1.0f, 0.0f );
vec3 focal( 0.0f, 0.0f, 0.0f );

vec3 controlRotate( 0.0f, 0.0f, 0.0f );
vec3 controlTranslate( 0.0f, 0.0f, 0.0f );
vec3 controlScale( 1.0f, 1.0f, 1.0f );

vec3 gaze;

vec3 Ka(0.1f, 0.1f, 0.1f);
vec3 Kd(0.9f, 0.9f, 0.9f);
vec3 Ks(0.0f, 0.0f, 0.0f);

std::vector<float> fileVerts;
std::vector<float> fileNormals;
std::vector<float> fileColorsKa;
std::vector<float> fileColorsKd;
std::vector<float> fileColorsKs;
std::vector<int> fileFaces;
std::vector<float> fileTextures;
std::vector<int> fileFaceNormals;
std::vector<float> fileFaceTextures;
std::vector<float> fileTranslate;
std::vector<float> fileXRotation;
std::vector<float> fileYRotation;
std::vector<float> fileZRotation;
std::vector<float> fileScale;
std::vector<char> fileOrder;

float aspect = 1.0f;
float farDist = 200.0f;
float nearDist = 0.5f;

float angleV = 0.0f;
float DEG2RAD = 3.14159f / 180.f;

GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;
GLint normalLoc;

GLuint shaderProgram;

GLuint vaoTriangle[100];
GLuint vboTriangleVertices = 1;
GLuint vboTriangleNormals = 2;
GLuint vboKa = 3;
GLuint vboKd = 4;
GLuint vboKs = 5;
GLuint vboS = 6;

mat4 viewMatrix;
mat4 modelMatrix;
mat4 projectionMatrix;
mat3 normalMatrix;

mat4 transform;
mat4 translateMatrix;

const int debug = 0, debug_level2 = 0;
bool X = 1, Y = 1, Z = 0, W = 0, S = 0;

float xGreatest = 1.0f, xLeast = -1.0f;
float yGreatest = 1.0f, yLeast = -1.0f;
float zGreatest = 1.0f, zLeast = -1.0f;
float xRange, yRange, zRange;
float SCALE = 0.2f;
float sceneRadius = 0.2f;

int SIZEN = 3;
int CURR = 0;
GLuint endFaces[100];

ifstream objectFile;
ifstream materialFile;
ifstream controlFile;

GLuint NumVertices = 0;
GLuint NumColors = 0;
GLuint NumFaces = 0;
GLuint NumFaceNormals = 0;

int faceCounter = 0;
int faceTextures = 0;
int colorFlag = 0;
int normalFlag = 0;

struct LightProperties {
    bool isEnabled;
    bool isLocal;
    bool isSpot;
    vec3 ambient;
    vec3 color;
    vec3 position;
    vec3 halfVector;
    vec3 coneDirection;
    float spotCosCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

void upDownKey( float val )
{
	if(val != -1.0)
	{
		gaze[0] = focal[0] - eye[0];
		gaze[1] = focal[1] - eye[1];
		gaze[2] = focal[2] - eye[2];
		gaze = normalize(gaze);
		eye[0] += (SCALE*5 * sceneRadius) * gaze[0];
		eye[1] += (SCALE*5 * sceneRadius) * gaze[1];
		eye[2] += (SCALE*5 * sceneRadius) * gaze[2];
		focal[0] += (SCALE*5 * sceneRadius) * gaze[0];
		focal[1] += (SCALE*5 * sceneRadius) * gaze[1];
		focal[2] += (SCALE*5 * sceneRadius) * gaze[2];
	}
	else
	{
		gaze[0] = focal[0] - eye[0];
                gaze[1] = focal[1] - eye[1];
                gaze[2] = focal[2] - eye[2];
                gaze = normalize(gaze);
                eye[0] -= (SCALE*5 * sceneRadius) * gaze[0];
                eye[1] -= (SCALE*5 * sceneRadius) * gaze[1];
                eye[2] -= (SCALE*5 * sceneRadius) * gaze[2];
                focal[0] -= (SCALE*5 * sceneRadius) * gaze[0];
                focal[1] -= (SCALE*5 * sceneRadius) * gaze[1];
                focal[2] -= (SCALE*5 * sceneRadius) * gaze[2];
	}
}

void leftRightKey( float val )
{
	vec4 tempV4;
	vec3 axis;
	if(val == -1.0)
	{
		axis.x = 0.f;
		axis.y = 0.f;
		axis.z = 1.f;
		transform = rotate(mat4(1.0f), -1.0f * DEG2RAD, axis);
		tempV4.x = focal.x;
		tempV4.y = focal.y;
		tempV4.z = focal.z;
		tempV4.w = 1.0f;
		tempV4 = transform * tempV4;
		focal = (vec3)tempV4;
		tempV4.x = eye.x;
		tempV4.y = eye.y;
		tempV4.z = eye.z;
		tempV4.w = 1.0f;
		tempV4 = transform * tempV4;
		eye = vec3(tempV4);
	}
	else
	{
		axis.x = 0.f;
		axis.y = 0.f;
		axis.z = 1.f;
		transform = rotate(mat4(1.0f), 1.0f * DEG2RAD, axis);
		tempV4.x = focal.x;
		tempV4.y = focal.y;
		tempV4.z = focal.z;
		tempV4.w = 1.0f;
		tempV4 = transform * tempV4;
		focal = (vec3)tempV4;
		tempV4.x = eye.x;
		tempV4.y = eye.y;
		tempV4.z = eye.z;
		tempV4.w = 1.0f;
		tempV4 = transform * tempV4;
		eye = vec3(tempV4);
	}
}

void zxKey( float val )
{
	mat4 transform;
	gaze = focal - eye;
	gaze = normalize(gaze);
	if(val == -1.0)
		transform = rotate(mat4(1.f), -1.0f * DEG2RAD, gaze);
	else
		transform = rotate(mat4(1.f), 1.0f * DEG2RAD, gaze);
	vec4 tempV4;
	tempV4.x = viewUp.x;
	tempV4.y = viewUp.y;
	tempV4.z = viewUp.z;
	tempV4.w = 1.0f;
	tempV4 = transform * tempV4;
	viewUp = vec3(tempV4);
	viewUp = normalize(viewUp);
}

void dfKey( float val )
{
	if(val == -1.0)
	{
		focal[0] -= (SCALE * sceneRadius) * viewUp[0];
		focal[1] -= (SCALE * sceneRadius) * viewUp[1];
		focal[2] -= (SCALE * sceneRadius) * viewUp[2];
	}
	else
	{
		focal[0] += (SCALE * sceneRadius) * viewUp[0];
		focal[1] += (SCALE * sceneRadius) * viewUp[1];
		focal[2] += (SCALE * sceneRadius) * viewUp[2];
	}


}

void cvKey( float val )
{
	if(val == -1.0)
	{
		eye[0] -= (SCALE * sceneRadius) * viewUp[0];
		eye[1] -= (SCALE * sceneRadius) * viewUp[1];
		eye[2] -= (SCALE * sceneRadius) * viewUp[2];
	}
	else
	{
		eye[0] += (SCALE * sceneRadius) * viewUp[0];
                eye[1] += (SCALE * sceneRadius) * viewUp[1];
                eye[2] += (SCALE * sceneRadius) * viewUp[2];
	}

	if(debug)
        {
                cout << "new eye " <<  eye.x << " " << eye.y << " " << eye.z <<endl;
        }
}

void readVerts( void )
{
	stringstream ss;
	string line;
	string comp;
	float v1, v2, v3;
	if( objectFile.is_open() )
	{
		while( getline(objectFile, line) )
		{
			if(line[0] == 'm' && line[5] == 'b')
			{
				ss << line;
				ss >> comp;
				ss >> comp;
				materialFile.open(comp);
				if(!materialFile.is_open())
					cout << "Material file not found...." << endl;
			}
			else if( line[0] == 'v' && line[1] != 'n' && line[1] != 't')
			{
				ss << line;
				ss >> comp;
				if(debug)
					cout << line << endl;
				for(int i = 0; i < 3; i++)
				{
					ss >> v1;
					fileVerts.push_back(v1);
					//NumVertices++;
				}
				NumVertices++;
			}
			else if( line[0] == 'v' && line[1] == 't')
			{
				ss << line;
				ss >> comp;
				for(int i = 0; i < 2; i++)
				{
					ss >> v1;
					fileTextures.push_back(v1);
				}
			}
			else if( line[0] == 'v' && line[1] == 'n')
			{
				ss << line;
                                ss >> comp;
				if(debug)
                                	cout << line << endl;
                                for(int i = 0; i < 3; i++)
                                {
                                        ss >> v1;
                                        fileNormals.push_back(v1);
                                }

			}
			else if( line[0] == 'u' )
			{
				materialFile.seekg(ios::beg);
				faceCounter = 0;
				string helper, helper2;
				stringstream temp;
				ss.str("");
				ss << line;
				ss >> comp;
				ss >> comp;
				ss >> comp;

				if(materialFile.is_open())
				{
					colorFlag = 1;
					faceCounter = 0;
					while( getline(materialFile, helper) )
					{
						helper2 = "helper2";
						temp.str("");
						if( temp.fail())
						{
							temp.clear();
						}
						temp << helper;
						temp >> helper2;
						temp >> helper2;
						if(helper2 == comp)
						{
							temp.str("");
							helper = "";
							helper2 = "";
							getline(materialFile, helper);
							temp << helper;
							if( helper[3] == 'a' )
							{
								if(debug)
									cout << "helper: " << helper << endl;
								temp << helper;
								temp >> helper2;
								temp >> v1;
								temp >> v2;
								temp >> v3;
								Ka = vec3(v1, v2, v3);
								if(debug)
									cout << v1 << " " << v2 << " " << v3 << endl;
							}
							temp.str("");
							helper = "";
							helper2 = "";
							getline(materialFile, helper);
							if( helper[3] == 'd' )
                                                        {
                                                                temp << helper;
                                                                temp >> helper2;
                                                                temp >> v1;
                                                                temp >> v2;
                                                                temp >> v3;
                                                                Kd = vec3(v1, v2, v3);
								if(debug)
									cout << v1 << " " << v2 << " " << v3 << endl;
                                                        }
							if( helper[3] == 's' )
							{
								temp << helper;
								temp >> helper2;
								temp >> v1;
								temp >> v2;
								temp >> v3;
								Ks = vec3(v1, v2, v3);
								if(debug)
									cout << v1 << " " << v2 << " " << v3 << endl;
							}
							NumColors++;
							break;
						}
						helper = "";
					}
				}
			}
			else if(line[0] == 'f')
			{
				int f1, f2, f3, n1, n2, n3;
				normalFlag = 0;

				faceCounter ++;

				ss << line;
				ss >> comp;
				ss >> comp;
				string delim = "/";
				std::size_t pos;
				pos = comp.find(delim);
				if(pos != string::npos)
				{
					for(int i = 0; i < 3; i ++)
					{
						string temp = comp;
						comp = comp.substr(0, pos );
						if(i == 0)
							f1 = stoi(comp);
						else if( i == 1 )
							f2 = stoi(comp);
						else
							f3 = stoi(comp);
						comp = temp.substr(pos + 1, temp.length());
						int pos2 = comp.find(delim);
						//cout << "COMP1: " << comp << endl;
						if(pos2 != string::npos)
						{
							string tempS = comp.substr(pos-1, pos2);
							if(tempS.compare("/") != 0)
							{
							//	int tex = stoi(tempS);
								fileFaceTextures.push_back(stoi(tempS));
								faceTextures++;
							}
							normalFlag = 1;
							comp = comp.substr(pos2 + 1, comp.length());
							//cout << "COMP2: " << comp << endl;
							if(i == 0)
								n1 = stoi(comp);
							else if(i == 1)
								n2 = stoi(comp);
							else
								n3 = stoi(comp);
						}
						else
						{
							fileFaceTextures.push_back(stoi(comp));
							faceTextures++;
						}
						if(i != 2)
						{
							ss >> comp;
							pos = comp.find(delim);
						}
					}
				}
				else
				{
					ss.str("");
					ss << line;
					ss >> comp;
					ss >> f1;
					ss >> f2;
					ss >> f3;
					if(debug)
					{
						cout << "line: " << line << endl;
						cout << "comp: " << comp << endl;
						cout << f1 << " " << f2 << " " << f3 << endl;
					}
				}

				fileFaces.push_back(f1);
				fileFaces.push_back(f2);
				fileFaces.push_back(f3);

				if(normalFlag == 1)
				{
					fileFaceNormals.push_back(n1);
					fileFaceNormals.push_back(n2);
					fileFaceNormals.push_back(n3);
					NumFaceNormals++;
				}

				for(int p = 0; p < 3; p++)
				{
					fileColorsKa.push_back(Ka.x);
					fileColorsKa.push_back(Ka.y);
					fileColorsKa.push_back(Ka.z);
					fileColorsKd.push_back(Kd.x);
					fileColorsKd.push_back(Kd.y);
					fileColorsKd.push_back(Kd.z);
					fileColorsKs.push_back(Ks.x);
					fileColorsKs.push_back(Ks.y);
					fileColorsKs.push_back(Ks.z);
				}

				if(debug)
				{
                                        cout << line << endl;
					cout << f1 << " " << f2 << " " << f3 << endl;
					cout << "Ka " << Ka.x << " " << Ka.y << " " << Ka.z << endl;
					cout << "Kd " << Kd.x << " " << Kd.y << " " << Kd.z << endl;
					cout << "Ks " << Ks.x << " " << Ks.y << " " << Ks.z << endl;
				}
				NumFaces++;
				
			}
			ss.clear();
		}
	}
	else
	{
		cout << "Object File not open, quitting" << endl;
		exit(1);
	}

	if(debug)
	{
		for(int i = 0; i < NumVertices; i++)
			printf("%f\n", fileVerts[i] );
	}
	materialFile.close();
	objectFile.close();
}

int readControlFile( void )
{
        stringstream ss;
        string line;
        string comp;
        float v1, v2, v3;
	int inc = 0;

	for(int i = 0; i < fileNormals.size(); i++)
		fileNormals[i] = 0.0f;

        if( controlFile.is_open() )
        {
                while( getline(controlFile, line) )
                {
			ss << line;
			ss >> comp;
                        if(comp.compare("obj") == 0)
                        {
				inc = 0;
				cout << "obj" << endl;
				fileVerts.resize(0);
				fileNormals.resize(0);
				fileColorsKa.resize(0);
				fileColorsKd.resize(0);
				fileFaces.resize(0);
				//fileNormals.resize(0);
				NumFaces = 0;
				NumVertices = 0;
				faceTextures = 0;
                                ss >> comp;
                                objectFile.open(comp);
                                readVerts();
				for(int i = 0; i < 3; i++)
				{
					fileTranslate.push_back(0.0);
					fileScale.push_back(0.0);
					fileOrder.push_back('a');
				}
				fileXRotation.push_back(0.0);
				fileYRotation.push_back(0.0);
				fileZRotation.push_back(0.0);
				CURR++;
				return 1;
                        }
			else if(comp.compare("t") == 0)
			{
				ss >> v1;
				ss >> v2;
				ss >> v3;
				fileTranslate[(CURR-1)*3] = v1;
				fileTranslate[(CURR-1)*3 + 1] = v2;
				fileTranslate[(CURR-1)*3 + 2] = v3;
				fileOrder[(CURR-1)*3 + inc] = 't';
				inc++;
				if(debug)
					cout << "translate: " << v1 << " " << v2 << " " << v3  << endl;
			}
			else if(comp[0] == 'r')
			{
				v1 = 0.0;
				v2 = 0.0;
				v3 = 0.0;
				for(int j = 0; j < 3; j++)
				{
					if(comp[1] == 'x')
					{
						ss >> v1;
						fileXRotation[(CURR-1)] = v1;
					}
					else if(comp[1] == 'y')
					{
						ss >> v2;
						fileYRotation[(CURR-1)] = v2;
					}
					else if(comp[1] == 'z')
					{
						ss >> v3;
						fileZRotation[(CURR-1)] = v3;
					}
					ss >> comp;
				}

				fileOrder[(CURR-1)*3 + inc] = 'r';
				inc++;
				cout << "rotation: " << v1 << " " << v2 << " " << v3  << endl;
			}
			else if(comp.compare("s") == 0)
			{
				ss >> v1;
				ss >> v2;
				ss >> v3;
				fileScale[(CURR-1)*3] = v1;
				fileScale[(CURR-1)*3 + 1] = v2;
				fileScale[(CURR-1)*3 + 2] = v3;
				fileOrder[(CURR-1)*3 + inc] = 's';
				inc++;
				cout << "scale: " << v1 << " " << v2 << " " << v3  << endl;
			}
			comp = "";
			ss.str("");
			ss.clear();
                }
		controlFile.close();
		return -1;
        }
}

void minMaxFind(float x, float y, float z)
{
	if(xLeast > x)
		xLeast = x;
	if(yLeast > y)
		yLeast = y;
	if(zLeast > z)
		zLeast = z;
	if(xGreatest < x)
		xGreatest = x;
	if(yGreatest < y)
		yGreatest = y;
	if(zGreatest < z)
		zGreatest = z;
}

GLfloat calcColor(float ka, float kd)
{
	GLfloat temp;
	temp = ka+kd;
	if(temp > 1.0)
		temp = 1.0;
	if(temp < 0.0)
		temp = 0.0;
	return temp;
}

void initShape( int size )
{

        GLfloat singleTriangle [ size ][3];
        GLfloat singleTriangleColors [ size ][3];
	GLfloat singleTriangleNormals [ NumFaces ][3];
	GLfloat singleTriangleKa [ NumFaces * 3 ][3];
	GLfloat singleTriangleKd [ NumFaces * 3 ][3];
	GLfloat singleTriangleKs [ NumFaces * 3 ][3];
	GLfloat singleTriangleS [ NumFaces * 3 ][1];

	int count = 0;

	for(int i = 0; i < NumFaces ; i++)
	{
		if(debug || debug_level2)
			printf("face %d: %d %d %d\n", i, fileFaces[i*3], fileFaces[i*3 + 1], fileFaces[i*3 + 2]);

		for(int j = 0; j < 3; j++)
		{
			if(debug)
				printf("Vertice %d: ", j);
			for(int k = 0; k < 3; k++)
			{
				int face = fileFaces[i*3 + j] - 1;

				if(debug)
					printf(" %f ", fileVerts[face * 3 + k]);
				singleTriangle[i*3 + j][k] = fileVerts[face * 3 + k];
			}
			if(debug)
				printf("\n");
			if(normalFlag == 1)
				singleTriangleNormals[i][j] = fileNormals[i * 3 + j];
			if(colorFlag == 1)
			{
				GLfloat colorX, colorY, colorZ;

				singleTriangleKa[i*3 + j][0] = fileColorsKa[count];
				singleTriangleKd[i*3 + j][0] = fileColorsKd[count];
				singleTriangleKs[i*3 + j][0] = fileColorsKs[count];
				count++;
				singleTriangleKa[i*3 + j][1] = fileColorsKa[count];
				singleTriangleKd[i*3 + j][1] = fileColorsKd[count];
				singleTriangleKs[i*3 + j][1] = fileColorsKs[count];
				count++;
				singleTriangleKa[i*3 + j][2] = fileColorsKa[count];
				singleTriangleKd[i*3 + j][2] = fileColorsKd[count];
				singleTriangleKs[i*3 + j][2] = fileColorsKs[count];
				count++;
				
			}
			minMaxFind(singleTriangle[i*3 + j][0], singleTriangle[i*3 + j][1], singleTriangle[i*3 + j][2]);
		}
		if(normalFlag == 0)
		{
			vec3 normal, p1, p2, p3;
			for(int j = 0; j < 3; j++)
			{
				float x1, y1, z1;
				x1 = singleTriangle[i*3+j][0];
				y1 = singleTriangle[i*3+j][1];
				z1 = singleTriangle[i*3+j][2];

				if( j == 0 )
					p1 = vec3(x1, y1, z1);
				else if( j == 1 )
					p2 = vec3(x1, y1, z1);
				else
					p3 = vec3(x1, y1, z1);

			}
			vec3 dir = glm::cross(p2-p1, p3-p1);
			dir = normalize(dir);
			//printf("dir   %d: %f %f %f\n", i, dir.x, dir.y, dir.z);
			singleTriangleNormals[i][0] = dir.x;
			singleTriangleNormals[i][1] = dir.y;
			singleTriangleNormals[i][2] = dir.z;

		}		

		if(debug)
			printf("\n");
	}
	if(debug)
	{
		for(int i = 0; i < NumFaces * 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				 printf("Colors %d: ", j);
	                        for(int k = 0; k < 3; k++)
        	                {
	
        	                        if(debug)
                	                        printf(" %f ", singleTriangleColors[i*3 + j][k]);
                        	}
                        	printf("\n");
			}
		}
	}


        glBindBuffer( GL_ARRAY_BUFFER, vboTriangleVertices );
        glBufferData( GL_ARRAY_BUFFER, sizeof(singleTriangle) , singleTriangle, GL_DYNAMIC_DRAW );

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, vboTriangleNormals );
	glBufferData( GL_ARRAY_BUFFER, sizeof(singleTriangleNormals), singleTriangleNormals, GL_DYNAMIC_DRAW );

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( 1 );
	
	glBindBuffer( GL_ARRAY_BUFFER, vboKa );
	glBufferData( GL_ARRAY_BUFFER, sizeof(singleTriangleKa) , singleTriangleKa, GL_DYNAMIC_DRAW );

	if(debug_level2)
	{
		for(int j = 0; j < NumFaces; j++)
		{
			printf("Ka: ");
			for(int i = 0; i < 3; i++ )
			{
				printf("%f ", singleTriangleKa[j][i]);
			}
			printf("\n");
			printf("Kd: ");
			for(int i = 0; i < 3; i++)
			{
				printf("%f ", singleTriangleKd[j][i]);
			}
			printf("\n");
		}
	}

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( 2 );

	glBindBuffer( GL_ARRAY_BUFFER, vboKd );
	glBufferData( GL_ARRAY_BUFFER, sizeof(singleTriangleKd) , singleTriangleKd, GL_DYNAMIC_DRAW );

	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray( 3 );

	glBindBuffer( GL_ARRAY_BUFFER, vboKs );
        glBufferData( GL_ARRAY_BUFFER, sizeof(singleTriangleKs) , singleTriangleKs, GL_DYNAMIC_DRAW );

	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray( 4 );

//	glBindBuffer( GL_ARRAY_BUFFER, vboS );
  //      glBufferData( GL_ARRAY_BUFFER, sizeof(singleTriangleS) , singleTriangleS, GL_DYNAMIC_DRAW );

//	glVertexAttribPointer( 5, 1, GL_FLOAT, GL_FALSE, 0, 0);
  //      glEnableVertexAttribArray( 5 );

	if(debug_level2)
		printf("NumFaces: %d\tNumVerts: %d\n", NumFaces, NumVertices);

}

void getRange( void )
{
	xRange = xGreatest - xLeast;
	yRange = yGreatest - yLeast;
	zRange = zGreatest - zLeast;
}

void setView( void )
{
        nearDist = 1.5 * zLeast;
        farDist = 1.5 * zGreatest;
        eye = vec3(3*xRange, 3*yRange, zRange);
        focal = vec3((xGreatest - abs(xLeast))/2, (yGreatest - abs(yLeast))/2, (zGreatest - abs(zLeast))/2);
	if(debug)
	{
        	printf("neardist: %f fardist: %f\n", nearDist, farDist);
        	printf("EYE: %f %f %f\n", eye.x, eye.y, eye.z);
        	printf("FOCAL: %f %f %f\n", focal.x, focal.y, focal.z);
	}

}


/////////////////////////////////////////////////////
//  int
/////////////////////////////////////////////////////
void init (void )
{
	int help = readControlFile();
//	readVerts();
	if(help == -1)
		return;

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "viewer.vert" },
		{ GL_FRAGMENT_SHADER, "viewer.frag" },
		{ GL_NONE, NULL }
	};

        shaderProgram = LoadShaders( shaders );

	glUseProgram( shaderProgram );
	projectionLoc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
        modelLoc = glGetUniformLocation(shaderProgram, "ModelMatrix");
        viewLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
	normalLoc = glGetUniformLocation(shaderProgram, "NormalMatrix");

	glLineWidth( 2.0f );
	glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

	vaoTriangle[CURR-1] = CURR;

	glGenVertexArrays( 1, &vaoTriangle[CURR-1] );
	glBindVertexArray( vaoTriangle[CURR-1] );

	glGenBuffers( 1, &vboTriangleVertices );
	glBindBuffer( GL_ARRAY_BUFFER, vboTriangleVertices );

	glGenBuffers( 1, &vboTriangleNormals );
	glBindBuffer( GL_ARRAY_BUFFER, vboTriangleNormals );

	glGenBuffers( 1, &vboKa );
	glBindBuffer( GL_ARRAY_BUFFER, vboKa );

	glGenBuffers( 1, &vboKd );
	glBindBuffer( GL_ARRAY_BUFFER, vboKd );

	glGenBuffers( 1, &vboKs );
	glBindBuffer( GL_ARRAY_BUFFER, vboKs );

//	glGenBuffers( 1, &vboS );
//	glBindBuffer( GL_ARRAY_BUFFER, vboS );

	if(debug_level2)
	{
		for(int i = 0; i < NumFaces * 3; i=i+3)
		{
			cout << "Ka: " << fileColorsKa[i] << " " << fileColorsKa[i + 1] << " " << fileColorsKa[i + 2]  << endl;
			cout << "Kd: " << fileColorsKd[i] << " " << fileColorsKd[i + 1] << " " << fileColorsKd[i + 2]  << endl;
		}
	}

	initShape(NumFaces * 3);

	getRange();

	setView();

	glLineWidth( 1.0f );

	if(debug)
	{
		printf( "view matrix\n" );
		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 4; j++ )
				printf( "%6.2f ", viewMatrix[j][i] );
			printf( "\n" );
		}

		printf( "\nfrustum\n" );
		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 4; j++ )
				printf( "%6.2f ", projectionMatrix[j][i] );
			printf( "\n" );
		}
		printf( "\nmodel\n" );
                for ( int i = 0; i < 4; i++ )
                {
                        for ( int j = 0; j < 4; j++ )
                                printf( "%6.2f ", modelMatrix[j][i] );
                        printf( "\n" );
                }
	}
	if(controlFile.is_open())
	{
		endFaces[CURR-1] = NumFaces;
		init();
	}
}

////////////////////////////////////////////////////////////////////
//	display
////////////////////////////////////////////////////////////////////
void display (void )
{
	char order;
//	glClearColor( 0.0f, 0.25f, 0.3f, 1.0f );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	nearDist = 1.5 * zLeast;
	farDist = 1.5 * zGreatest;


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram( shaderProgram );

	modelMatrix = mat4(1.0f);

	viewMatrix = lookAt( eye, focal, viewUp );

	projectionMatrix = frustum(-1.0f, 1.0f, -aspect, aspect, nearDist, farDist);

	if(!debug)
        {
                printf( "view matrix\n" );
                for ( int i = 0; i < 4; i++ )
                {
                        for ( int j = 0; j < 4; j++ )
                                printf( "%6.2f ", viewMatrix[j][i] );
                        printf( "\n" );
                }

                printf( "\nfrustum\n" );
                for ( int i = 0; i < 4; i++ )
                {
                        for ( int j = 0; j < 4; j++ )
                                printf( "%6.2f ", projectionMatrix[j][i] );
                        printf( "\n" );
                }
                printf( "\nmodel\n" );
                for ( int i = 0; i < 4; i++ )
                {
                        for ( int j = 0; j < 4; j++ )
                                printf( "%6.2f ", modelMatrix[j][i] );
                        printf( "\n" );
                }
        }

	normalMatrix = transpose( inverse((mat3)viewMatrix * (mat3)modelMatrix) );

	glUniformMatrix4fv( viewLoc, 1, GL_FALSE, value_ptr( viewMatrix ) );
	glUniformMatrix4fv( projectionLoc, 1, GL_FALSE, value_ptr( projectionMatrix) );
	glUniformMatrix4fv( modelLoc, 1, GL_FALSE, value_ptr( modelMatrix ) );
	glUniformMatrix3fv( normalLoc, 1, GL_FALSE, value_ptr( normalMatrix ) );
	if(W)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		W = !W;
	}
	if(S)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		S= !S;
	}

	

	for(int i = 0; i < CURR; i++)
	{
		glBindVertexArray( vaoTriangle[i] );

		for(int j = 0; j < 3; j++)
		{
			vec3 trans = vec3(fileTranslate[i*3], fileTranslate[i*3+1], fileTranslate[i*3+2]);
			vec3 sca = vec3(fileScale[i*3], fileScale[i*3+1], fileScale[i*3+2]);
			vec3 axis = vec3(1.0, 0.0, 0.0);
			
			order = fileOrder[i*3+j];
			cout << order << " ORDER" << endl;

			switch(order) {
				case 's':
					modelMatrix = glm::scale(modelMatrix, sca);
					break;
				case 'r':
					modelMatrix = glm::rotate(modelMatrix, fileXRotation[i] * DEG2RAD, axis);
					axis.x = 0.0;
					axis.y = 1.0;
					modelMatrix = glm::rotate(modelMatrix, fileYRotation[i] * DEG2RAD, axis);
					axis.y = 0.0;
					axis.z = 1.0;
					modelMatrix = glm::rotate(modelMatrix, fileZRotation[i] * DEG2RAD, axis);
					break;
				case 't':
					modelMatrix = glm::translate(modelMatrix, trans);
					break;
				default:
					break;
			}
			
			//modelMatrix = glm::translate(modelMatrix, trans);
			//modelMatrix = glm::

			//modelMatrix = translateMatrix * modelMatrix;
		}
		normalMatrix = transpose( inverse((mat3)viewMatrix * (mat3)modelMatrix) );

        	glUniformMatrix4fv( viewLoc, 1, GL_FALSE, value_ptr( viewMatrix ) );
        	glUniformMatrix4fv( projectionLoc, 1, GL_FALSE, value_ptr( projectionMatrix) );
        	glUniformMatrix4fv( modelLoc, 1, GL_FALSE, value_ptr( modelMatrix ) );
        	glUniformMatrix3fv( normalLoc, 1, GL_FALSE, value_ptr( normalMatrix ) );


		if(debug)
			cout << "CURR: " << i << " VERTICES: " << endFaces[i]*3 << endl;

        	glDrawArrays( GL_TRIANGLES, 0, endFaces[i] * 3 );
		glFlush();
		modelMatrix = mat4(1.0);
	}
//	for(int i = 0; i < faceTextures; i++)
//	{
//		cout << "Face Texture " << i << ": " << fileFaceTextures[i] << endl;
//	}
}

void idle_CB()
{
//	glutPostRedisplay();
}

void SpecialInput(int key, int x, int y)
{
	switch(key)
	{
		/* Move camera forward along the gaze vector (focal_point - eye_point)
		   Move the eye and focal point by the same amounts.
		   (Use bounds of scene to scale the distance for each key press)
		*/
		case GLUT_KEY_UP:
			upDownKey( 1.0 );
			if(debug)
                                cout << "up received" << endl;
			break;

		/* Move camera backwards along the gaze vector (focal_point - eye_point)
		   Move the eye and the focal point by the same amounts.
		   (Use bounds of scene to scale the distance for each key press)
		*/
		case GLUT_KEY_DOWN:
		upDownKey( -1.0 );
			if(debug)
                                cout << "down received" << endl;
                        break;

		/* Rotate the eye position and focal point around the z axis
                   Left is 1 degree clockwise
		*/
		case GLUT_KEY_LEFT:
			leftRightKey( 1.0 );
			if(debug)
                                cout << "left received" << endl;
                        break;

		/* Rotate the eye position and focal point around the z axis
		   Right is 1 degree counter clockwise
		*/
		case GLUT_KEY_RIGHT:
			leftRightKey( -1.0 );
			if(debug)
                                cout << "right received" << endl;
                        break;

		default:
			if(debug)
                                cout << "[default] " << key << " received and not handled." << endl;
                        break;
	}
	glutPostRedisplay();
}

void key_CB(unsigned char key, int x_cord, int y_cord)
{
	switch (key)
	{
		/* Move the focal point along the view up vector.
		   D moves the focal point in the opposite direction of view up vector
		*/
		case 'd': case 'D':
			dfKey( -1.0 );
			if(debug)
                                cout << key << " received" << endl;
			break;

		/* Move the focal point along the view up vector.
		   F moves the focal point in the direction of view up.
		*/
                case 'f': case 'F':
			dfKey( 1.0 );
			if(debug)
                                cout << key << " received" << endl;
                        break;

		/* Move the camera along the view up vecotr.
		   C moves the camera down (opposite direction of view up)
		*/
                case 'c': case 'C':
			cvKey( -1.0 );
			if(debug)
                                cout << key << " received" << endl;
                        break;

		/* Move the camera along the view up vector.
		   V moves the camera up (along the direction of view up)
		*/
                case 'v': case 'V':
			cvKey( 1.0 );
			if(debug)
                                cout << key << " received" << endl;
                      	break;

		/* Reset View to default values
			eye: 	(2, 2, 0)
			focal: 	(0, 0, 0)
			viewUp: (0, 0, 1)
		*/
		case 'r': case 'R':
			setView();
			if(debug)
                                cout << key << " received" << endl;
			glutPostRedisplay();
                        break;

		/* Starts wirefram rendering */
		case 'w': case 'W':
			W = !W;
			if(debug)
                                cout << key << " received" << endl;
			display();
                        glutPostRedisplay();
                        break;

		/* Starts solid surface rendering */
		case 's': case 'S':
			S = !S;
			if(debug)
                                cout << key << " received" << endl;
			display();
                       	glutPostRedisplay();
                        break;

		/* Rotate view up vector by 1 degree clockwise around gaze vector */
                case 'x': case 'X':
			zxKey( -1.0 );
			if(debug)
                                cout << key << " received" << endl;
                        break;

		/* Rotate view up vector by 1 degree counter clockwise around gaze vector */
                case 'z': case 'Z':
			zxKey( 1.0 );
			if(debug)
                                cout << key << " received" << endl;
                        break;

                // Quits application
                case 'q': case 'Q': case 27:
			if(debug)
			{
				if(key != 'q' && key != 'Q')
					cout << "ESC received" << endl;
				cout << key << " received" << endl;
			}
                        exit(0); 
                        break;
                
                default:
			if(debug)
                        	cout << "[default] " << key << " received and not handled." << endl;
                        break;
	}
	glutPostRedisplay();
}                


void usage()
{
	printf("\n\nVIEWER USAGE\n");
	printf("\tviewer -c objectFile\n");
	printf("\n\tobjectFile is the exact file you want to display in the viewer.\n");
	printf("\n\tThis file is normally called an \"object file\" and ends in .obj\n");
	printf("\tThis file should contain the name of the object, the vertices, scale, and transformations\n\n");

	exit(0);
}

////////////////////////////////////////////////////////////////////////
//	main
////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	if(argv[1] != NULL)
	{
		if(strcmp("-h", argv[1]) == 0)
			usage();
		if(strcmp("-c", argv[1]) == 0)
			if(argv[2] != NULL)
				controlFile.open(argv[2]);
//				objectFile.open(argv[2]);
			else
			{
				printf("controlFile not found!");
				usage();
			}
		else
			usage();
	}
	else
	{
		printf("No arguments found, please read usage and put in the correct argument(s)");
		usage();
	}

	// initialize GLUT system
	glutInit( &argc, argv );

	// initialize display format
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH);
	/* UNCOMMENT THIS LINE FOR GS930, put in GLUT_DEPTH*/
//	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DEPTH );
//	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_RGBA );

	// initialize window size
	glutInitWindowSize( 512, 512 );

//	glEnable( GL_DEPTH_TEST );

	glutInitContextVersion( 4, 3 );
	glutInitContextProfile( GLUT_CORE_PROFILE );// GLUT_COMPATIBILITY_PROFILE );
	/*UNCOMMENT THESE 2 LINES FOR GS 930*/


	// create window with name from executable
	glutCreateWindow( argv[0] );

	glutIdleFunc(idle_CB);

	glutKeyboardFunc(key_CB);

	glutSpecialFunc(SpecialInput);

	glewExperimental = GL_TRUE;	// added for glew to work!
	if ( glewInit() )
	{
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit (EXIT_FAILURE );
	}

	// code from OpenGL 4 Shading Language cookbook, second edition
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString (GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv( GL_MAJOR_VERSION, &major );
	glGetIntegerv( GL_MINOR_VERSION, &minor );

	cout << "GL Vendor            :" << vendor << endl;
	cout << "GL Renderer          :" << renderer  << endl;
	cout << "GL Version (string)  :" << version << endl;
	cout << "GL Version (integer) :" << major << " " << minor << endl;
	cout << "GLSL Version         :" << glslVersion << endl;
	//
	//GLint nExtensions;
	//glGetIntegerv( GL_NUM_EXTENSIONS, &nExtensions );
	//for ( int i = 0; i < nExtensions; i++ )
	//	cout << glGetStringi( GL_EXTENSIONS, i )  << endl;

//	glutDisplayFunc( display );
	init();
	glutDisplayFunc( display );
	glutMainLoop();

//	glEnable(GL_DEPTH_TEST);

	return 0;
}

