#include "Scene4.h"

CScene4::CScene4()
{
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
	pCamera = new CCamera(0.0f, 1.0f, 1.0f, 0.5f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	constColorAlphaFactor = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();	
	pTextures->CreateTextureAnisotropic(0, "../Scene1/B1.jpg");
	pTextures->CreateTextureAnisotropic(1, "../Scene1/Blood_Spatter.bmp");
	pTextures->CreateTextureAnisotropic(2, "../Scene1/LightMapB.jpg");
	pTextures->CreateTextureAnisotropic(3, "../Scene1/LightMapA.jpg");
	pTextures->CreateTextureAnisotropic(4, "../Scene1/G1.bmp");
	pTextures->CreateTextureAnisotropic(5, "../Scene1/grass.png");
	pTextures->CreateTextureAnisotropic(6, "../Scene1/grass.bmp");
	pTextures->CreateTextureAnisotropic(7, "../Scene1/neve.bmp");
	pTextures->CreateTextureAnisotropic(8, "../Scene4/TerTex.jpg");


	// Carrega Terreno HeightMap
	pTerreno = NULL;
	pTerreno = new CTerreno();
	pTerreno->LoadRawFile("../Scene4/Terreno.raw");


	
}


CScene4::~CScene4(void)
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

	if (pTerreno)
	{
		delete pTerreno;
		pTerreno = NULL;
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
}




int CScene4::DrawGLScene(void)	// Função que desenha a cena
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

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

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
	
	glColor4ub(255, 255, 255, 255);


	////
	//// Desenha Terreno
	////
	//glPushMatrix();
	//glTranslatef(-500.0f, -200.0f, -500.0f);
	//pTextures->ApplyTexture(8);
	//pTerreno->RenderHeightMap(false);
	//glPopMatrix();



	//
	// Desenha Terreno (Layers Mask)
	//
	///*glPushAttrib(GL_TEXTURE_BIT);
	// Unidade de Textura 0
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(7);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Unidade de Textura 1
	//glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(8);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Unidade de Textura 2
	//glActiveTexture(GL_TEXTURE2);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(6);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	//glPushMatrix();
	//glTranslatef(-500.0f, -200.0f, -500.0f);
	//pTerreno->RenderHeightMapMultiTex(false);
	//glPopMatrix();

	//glActiveTexture(GL_TEXTURE2);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE1);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_TEXTURE_2D);
	//glPopAttrib();*/



	
	// Desenha Polígono com Multi-Texturing
	
	glPushAttrib(GL_TEXTURE_BIT);
	// Unidade de Textura 0
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Unidade de Textura 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	
	glPushMatrix();		
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glEnd();
	glPopMatrix();


	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	





	//
	// Desenha Polígono com Multi-Texturing (LightMaping)
	//
	//glPushAttrib(GL_TEXTURE_BIT);
	//// Unidade de Textura 0
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(0);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	//// Unidade de Textura 1
	//glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(3);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	//
	//glPushMatrix();		
	//glBegin(GL_QUADS);

	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 0.0f);
	//glVertex3f(-5.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 5.0f + fRenderPosY, 0.0f);
	//glVertex3f(5.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 5.0f + fRenderPosY, 1.0f);
	//glVertex3f(5.0f, 1.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 1.0f);
	//glVertex3f(-5.0f, 1.0f, 0.0f);

	//glEnd();
	//glPopMatrix();

	//glActiveTexture(GL_TEXTURE1);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_TEXTURE_2D);



	//// Unidade de Textura 0
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(4);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	//// Unidade de Textura 1
	//glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(2);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

	//glPushMatrix();
	//glBegin(GL_QUADS);

	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 0.0f);
	//glVertex3f(-5.0f, 0.0f, 1.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 5.0f + fRenderPosY, 0.0f);
	//glVertex3f(5.0f, 0.0f, 1.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 5.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 5.0f + fRenderPosY, 1.0f);
	//glVertex3f(5.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f + fRenderPosY, 1.0f);
	//glVertex3f(-5.0f, 0.0f, 0.0f);

	//glEnd();
	//glPopMatrix();

	//glActiveTexture(GL_TEXTURE1);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_TEXTURE_2D);
	//glPopAttrib();





	//
	// Desenha Polígono com Multi-Texturing (MASK)
	//

	//glPushAttrib(GL_TEXTURE_BIT);
	//// Unidade de Textura 0
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(7);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//// Unidade de Textura 1
	//glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(5);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//// Unidade de Textura 2
	//glActiveTexture(GL_TEXTURE2);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(6);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	//glPushMatrix();		
	//glBegin(GL_QUADS);

	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	//glVertex3f(-1.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	//glVertex3f(1.0f, 1.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	//glVertex3f(-1.0f, 1.0f, 0.0f);

	//glEnd();
	//glPopMatrix();

	//glActiveTexture(GL_TEXTURE2);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE1);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_TEXTURE_2D);
	//glPopAttrib();





	//
	// Desenha Polígono com Multi-Texturing (Fading)
	//
	//glPushAttrib(GL_TEXTURE_BIT);
	//// Unidade de Textura 0
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(0);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//// Unidade de Textura 1
	//glActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//pTextures->ApplyTexture(1);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//// Unidade de Textura 2
	//glActiveTexture(GL_TEXTURE2);
	//glEnable(GL_TEXTURE_2D);
	//static GLfloat constColor[4] = { 0.0, 0.0, 0.0, 0.0 };
	//constColor[3] = constColorAlphaFactor;
	//glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);

	//pTextures->ApplyTexture(0);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);

	//glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
	//glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_SRC2_RGB, GL_CONSTANT);
	//glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);


	//glPushMatrix();
	//glBegin(GL_QUADS);

	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
	//glVertex3f(-1.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
	//glVertex3f(1.0f, 1.0f, 0.0f);

	//glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	//glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
	//glVertex3f(-1.0f, 1.0f, 0.0f);

	//glEnd();
	//glPopMatrix();

	//glActiveTexture(GL_TEXTURE2);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE1);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_TEXTURE_2D);
	//glPopAttrib();









	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.005f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

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
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));


	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene4::MouseMove(void) // Tratamento de movimento do mouse
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

void CScene4::KeyPressed(void) // Tratamento de teclas pressionadas
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

	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		if (constColorAlphaFactor <= 1.0f)
			constColorAlphaFactor += 0.01f;
	}

	if (GetKeyState(VK_NEXT) & 0x80)
	{
		if (constColorAlphaFactor >= 0.0f)
			constColorAlphaFactor -= 0.01f;
	}

}

void CScene4::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
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

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene4::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3f(i, 0.0f, j + 1);
			glVertex3f(i + 1, 0.0f, j + 1);
			glVertex3f(i + 1, 0.0f, j);
			glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene4::DrawAxis()
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


