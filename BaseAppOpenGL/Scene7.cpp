#include "Scene7.h"

CScene7::CScene7()
{
	int i = 0;
	int size = sizeof(vfCasaVerts) / sizeof(float);
	ofstream outFile;
	outFile.open("Normals.txt", ios::out | ios::app);
	while (i < size)
	{
		glm::vec3 p1, p2, p3, N;
		p1.x = vfCasaVerts[i++];
		p1.y = vfCasaVerts[i++];
		p1.z = vfCasaVerts[i++];

		p2.x = vfCasaVerts[i++];
		p2.y = vfCasaVerts[i++];
		p2.z = vfCasaVerts[i++];

		p3.x = vfCasaVerts[i++];
		p3.y = vfCasaVerts[i++];
		p3.z = vfCasaVerts[i++];

		N = CalculateTriangleNormalVector(p1, p2, p3);

		outFile << N.x << ", " << N.y << ", " << N.z << ", " << endl;
		outFile << N.x << ", " << N.y << ", " << N.z << ", " << endl;
		outFile << N.x << ", " << N.y << ", " << N.z << ", " << endl;
	}
	outFile.close();

	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f, 0.2f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureClamp(0, "../Scene7/rightSB.jpg");
	pTextures->CreateTextureClamp(1, "../Scene7/leftSB.jpg");
	pTextures->CreateTextureClamp(2, "../Scene7/downSB.jpg");
	pTextures->CreateTextureClamp(3, "../Scene7/upSB.jpg");
	pTextures->CreateTextureClamp(4, "../Scene7/frontSB.jpg");
	pTextures->CreateTextureClamp(5, "../Scene7/backSB.jpg");
	pTextures->CreateTextureAnisotropic(6, "../Scene7/TerTex.jpg");
	pTextures->CreateTextureAnisotropic(7, "../Scene7/water.jpg");
	pTextures->CreateTextureAnisotropic(8, "../Scene7/contTex.jpg");
	pTextures->CreateTextureAnisotropic(9, "../Scene7/ferroTex.jpg");


	// Carrega Terreno
	pTerreno = new CModel_3DS();
	pTerreno->Load("../Scene7/terreno.3DS");

	pCasa = new CModel_3DS();
	pCasa->Load("../Scene7/casa.3DS");

	fRotY = 0.0f;

	fPosX = 0.0f;
	fPosY = 20.0f;
	fPosZ = 30.0f;


	for (int i = 0; i < 38; i++)
		faceColor[i] = { (rand() % 255), (rand() % 255), (rand() % 255) };

	float lAmb = 0.4f;
	float matAmb = 0.2f;


	// Definição das configurações da fonte de luz (EMISSOR)
	LightAmbient[0] = lAmb; LightAmbient[1] = lAmb; LightAmbient[2] = lAmb; LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f; LightDiffuse[1] = 1.0f; LightDiffuse[2] = 1.0f; LightDiffuse[3] = 1.0f;
	LightSpecular[0] = 1.0f; LightSpecular[1] = 1.0f; LightSpecular[2] = 1.0f; LightSpecular[3] = 1.0f;
	LightPosition[0] = fPosX; LightPosition[1] = fPosY; LightPosition[2] = fPosZ; LightPosition[3] = 1.0f;

	// Definição das configurações do material do objeto (REFLEXÂO)
	MatAmbient[0] = matAmb; MatAmbient[1] = matAmb; MatAmbient[2] = matAmb; MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.8f; MatDiffuse[1] = 0.8f; MatDiffuse[2] = 0.8f; MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 1.0f; MatSpecular[1] = 1.0f; MatSpecular[2] = 1.0f; MatSpecular[3] = 1.0f;
	MatShininess[0] =  128.0f ;
	//MatShininess = 128.0f;

	fLowLight[0] = 50.0f; fLowLight[1] = 50.0f; fLowLight[2] = 50.0f; fLowLight[3] = 255.0f;


	Ruby = CMaterial(
		0.1745f, 0.01175f, 0.01175f, 0.55f,
		0.61424f, 0.04136f, 0.04136f, 0.55f,
		0.727811f, 0.626959f, 0.626959f, 0.55f,
		76.8f);

	Gold = CMaterial(
		0.19125f, 0.0735f, 0.0275f, 1.0f,
		0.75164f, 0.60648f, 0.22648f, 1.0f,
		0.628281f, 0.555802f, 0.366065f, 1.0f,
		51.2f);

}


CScene7::~CScene7(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pTerreno)
	{
		delete pTerreno;
		pTerreno = NULL;
	}

	if (pCasa)
	{
		delete pCasa;
		pCasa = NULL;
	}
}




int CScene7::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();



	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


	glEnable(GL_TEXTURE_2D);

	// Desenha o SkyBox
	CreateSkyBox(0.0f, 0.0f, 0.0f,
		2048.0f, 1536.0f, 2048.0f,
		pTextures);

	// Desenha lâmpada  ////
	LightPosition[0] = 50.0f;
	LightPosition[1] = 40.0f;
	LightPosition[2] = -70.0f;
	LightPosition[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess);

	glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

	// Habilita e configura o fog
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fLowLight); // 
	glFogf(GL_FOG_START, 2.0f); // distancia min
	glFogf(GL_FOG_END, 50.0f); // distancia max
	glFogi(GL_FOG_MODE, GL_LINEAR);



	glPushMatrix();
	pTextures->ApplyTexture(6);
	glRotatef(90,0,1,0);
	glTranslatef(0.0f, 0.0f, 0.0f);
	pTerreno->Draw();
	glPopMatrix();


	

	/*glPushMatrix();
	glTranslatef(50, 40, -70);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();*/
	/////////////////////////


	

	// Atribui os parâmetros da luz 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0); // Habilita a luz 0


	// Desenha a casa
	glPushMatrix();
	glTranslatef(-25.0f, -11.8f, 0.0f);
	glRotatef(120, 0, 1, 0);
	pCasa->Draw();
	glPopMatrix();
	glEnable(GL_LIGHTING); //  Habilita iluminação

	// Desenha Container

	glPushAttrib(GL_TEXTURE_BIT);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(8);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(9);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glTranslatef(-21.5f, 0.8f, 14.0f);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	// Atras
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0); glVertex3f(2.2f, 0.0f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0); glVertex3f(2.2f, 0.9f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0); glVertex3f(0.0f, 0.9f, 0.0f);
	// Cima 
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0); glVertex3f(0.0f, 0.9f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0); glVertex3f(0.0f, 0.9f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0); glVertex3f(2.2f, 0.9f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0); glVertex3f(2.2f, 0.9f, 0.0f);
	// Baixo
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0); glVertex3f(2.2f, 0.0f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0); glVertex3f(2.2f, 0.0f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0); glVertex3f(0.0f, 0.0f, 0.7f);
	// Frente
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 0.0);  glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0); glVertex3f(0.0f, 0.0f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 0.0);  glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0); glVertex3f(2.2f, 0.0f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 1.0);  glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0); glVertex3f(2.2f, 0.9f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 1.0);  glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0); glVertex3f(0.0f, 0.9f, 0.7f);
	// Direita
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0); glVertex3f(0.0f, 0.0f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0); glVertex3f(0.0f, 0.9f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0); glVertex3f(0.0f, 0.9f, 0.0f);
	// Esquerda
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0); glVertex3f(2.2f, 0.0f, 0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 0.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0); glVertex3f(2.2f, 0.0f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0); glVertex3f(2.2f, 0.9f, 0.7f);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 1.0); glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0); glVertex3f(2.2f, 0.9f, 0.0f);

	glEnd();
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	glDisable(GL_LIGHTING);




	// Habilita Blending
	glEnable(GL_BLEND);
	// Configura função de Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Desenha o rio
	glColor4f(1.0f, 1.0f, 1.0f, 0.6f);

	glPushAttrib(GL_TEXTURE_BIT);
	// Unidade de Textura 0
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(7);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Unidade de Textura 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(7);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

	glPushMatrix();
	glTranslatef(0.0f, -0.3f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 0.0f);
	glVertex3f(-28.0f, 0.0f, 28.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f + fRenderPosY, 0.0f);
	glVertex3f(28.0f, 0.0f, 28.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f + fRenderPosY, 1.0f);
	glVertex3f(28.0f, 0.0f, -28.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f + fRenderPosY);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 1.0f);
	glVertex3f(-28.0f, 0.0f, -28.0f);
	glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	// Desabilita Blending
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);


	// Desenha bandeira
	glColor3f(0.9f, 0.9f, 0.9f);
	glPushMatrix();
	glTranslatef(-23.5f, 2.8f, 20.0f);
	auxSolidCylinder(0.04f,3.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-23.5f, 0.8f, 20.0f);
	glRotatef(90, 1, 0, 0);
	auxSolidCone(0.1f, 0.0f);
	glPopMatrix();

	glColor3f(0.7f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(-23.5f, 3.2f, 20.0f);
	glBegin(GL_TRIANGLES);
	glm::vec3 normal = CalculateTriangleNormalVector(
		glm::vec3(-0.0f, -0.0f, 0.0f),
		glm::vec3(0.0f, -0.5f, 0.5f),
		glm::vec3(0.0f, 0.5f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(-0.0f, -0.0f, 0.0f);
	glVertex3f(0.5f, 0.3f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_FOG);


	fRotY += 1.0f;
	if (fRotY >= 360.0f)
		fRotY = 0.0f;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.0005f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 150, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("fRot: %.1f ", fRotY);




	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene7::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene7::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}


	if (GetKeyState(VK_LEFT) & 0x80)
		fPosX -= 0.1f;
	if (GetKeyState(VK_RIGHT) & 0x80)
		fPosX += 0.1f;
	if (GetKeyState(VK_UP) & 0x80)
		fPosZ -= 0.1f;
	if (GetKeyState(VK_DOWN) & 0x80)
		fPosZ += 0.1f;
	if (GetKeyState(VK_PRIOR) & 0x80)
		fPosY += 0.1f;
	if (GetKeyState(VK_NEXT) & 0x80)
		fPosY -= 0.1f;


}

void CScene7::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		break;

	case 'G':
	{
		MatAmbient[0] = Gold.rA;
		MatAmbient[1] = Gold.gA;
		MatAmbient[2] = Gold.bA;
		MatAmbient[3] = Gold.aA;

		MatDiffuse[0] = Gold.rD;
		MatDiffuse[1] = Gold.gD;
		MatDiffuse[2] = Gold.bD;
		MatDiffuse[3] = Gold.aD;

		MatSpecular[0] = Gold.rS;
		MatSpecular[1] = Gold.gS;
		MatSpecular[2] = Gold.bS;
		MatSpecular[3] = Gold.aS;

		MatShininess[0] = Gold.s;
	}
	break;

	case 'R':
	{
		MatAmbient[0] = Ruby.rA;
		MatAmbient[1] = Ruby.gA;
		MatAmbient[2] = Ruby.bA;
		MatAmbient[3] = Ruby.aA;

		MatDiffuse[0] = Ruby.rD;
		MatDiffuse[1] = Ruby.gD;
		MatDiffuse[2] = Ruby.bD;
		MatDiffuse[3] = Ruby.aD;

		MatSpecular[0] = Ruby.rS;
		MatSpecular[1] = Ruby.gS;
		MatSpecular[2] = Ruby.bS;
		MatSpecular[3] = Ruby.aS;

		MatShininess[0] = Ruby.s;
	}
	break;

	}

}


void CScene7::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posição especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}
//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene7::Draw3DSGrid(float width, float length)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene7::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


void CScene7::DrawCube()
{
	glBegin(GL_QUADS);
	// face frente
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	// face trás
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	// face direita
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);


	// face esquerda
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	// face baixo
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	// face cima
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();
}


void CScene7::DrawPyramid()
{
	glBegin(GL_TRIANGLES);

	// Face frente
	glColor4ub(222, 58, 219, 255);
	glm::vec3 normal = CalculateTriangleNormalVector(
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.0f, 0.5f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	// Face direita
	glColor4ub(181, 147, 54, 255);
	normal = CalculateTriangleNormalVector(
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.0f, 0.5f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	// Face esquerda
	glColor4ub(50, 160, 168, 255);
	normal = CalculateTriangleNormalVector(
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.0f, 0.5f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	// Face trás
	normal = CalculateTriangleNormalVector(
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.0f, 0.5f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z);
	glColor4ub(14, 201, 45, 255);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glEnd();
}


glm::vec3 CScene7::CalculateTriangleNormalVector(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	/*
			   p3
			   /\
			  /  \
			 /    \
			/      \
			+------+
		   p1      p2
	*/

	// Encontrar os vetores v1 e v2, resultado da subtração dos vetores (p2 - p1) e 
	// (p3 - p1), respectivamente
	glm::vec3 v1, v2;
	v1.x = p2.x - p1.x;
	v1.y = p2.y - p1.y;
	v1.z = p2.z - p1.z;

	v2.x = p3.x - p1.x;
	v2.y = p3.y - p1.y;
	v2.z = p3.z - p1.z;

	// Calcular o Cross Product para achar o vetor Normal (não normalizado)
	glm::vec3 normal;
	normal.x = v1.y * v2.z - v1.z * v2.y;
	normal.y = v1.z * v2.x - v1.x * v2.z;
	normal.z = v1.x * v2.y - v1.y * v2.x;

	// Calcula a magnitude do vetor normal
	double magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	// Normaliza o vetor normal
	glm::vec3 normalizedVector;
	normalizedVector.x = normal.x / magnitude;
	normalizedVector.y = normal.y / magnitude;
	normalizedVector.z = normal.z / magnitude;

	// Retorna o vetor Normal (normalizado)
	return normalizedVector;
}