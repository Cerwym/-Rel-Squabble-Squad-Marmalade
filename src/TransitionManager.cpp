#include "Iw2D.h"
#include "TransitionManager.h"
#include <iostream>

TransitionManager::TransitionManager()
{
	IW_CALLSTACK("TransitionManager::TransitionManager");
	Init();
}

TransitionManager::~TransitionManager()
{
	IW_CALLSTACK("TransitionManager::~TransitionManager");
	IwGxClear();
	IwGxFlush();
	Destroy();
}

void TransitionManager::Init()
{
	IW_CALLSTACK("TransitionManager::Init");
	m_StartTexture = NULL;
	m_EndTexture = NULL;

	IwGxSetColClear(0, 0, 0, 0);
	IwGxScreenOrient orient = IwGxGetScreenOrient();
	m_HasStarted = false;
	m_alphaValue = 0;
}

void TransitionManager::Destroy()
{
	IW_CALLSTACK("TransitionManager::Destroy");
	if (m_StartTexture != NULL)
	{
		delete m_StartTexture;
		m_StartTexture = NULL;
	}

	if (m_EndTexture != NULL)
	{
		delete m_EndTexture;
		m_EndTexture = NULL;
	}
	printf("Transition Manager cleaned up\n");
}

CIwTexture* TransitionManager::CaptureScreen()
{
	IW_CALLSTACK("TransitionManager::CaptureScreen")
	int w = IwGxGetDeviceWidth();
	int h = IwGxGetDeviceHeight();
	int length = w * h * 4;

	uint8* buffer = new uint8[length];
							//RGBA,	//Unsigned byte
	glReadPixels(0, 0, w, h, 0x1908, 0x1401, buffer);

	uint8* buffer2 = new uint8[length];

	int lineSize = w * 4;
	uint8* b1 = buffer;
	uint8* b2 = buffer2 + h * lineSize;;
	for(int y = h; y > 0; y--)
	{
		b2 -= lineSize;
		for(int x = w; x > 0; x--)
		{
			*b2++ = *b1++;
			*b2++ = *b1++;
			*b2++ = *b1++;
			*b2++ = *b1++;
		}
		b2 -= lineSize;
	}

	CIwTexture* texture = new CIwTexture;
	CIwImage& img = texture->GetImage();

	img.SetFormat(CIwImage::ABGR_8888);
	img.SetWidth(w);
	img.SetHeight(h);
	img.SetBuffers(buffer2, length);

	texture->SetMipMapping(false);
	texture->Upload();

	delete [] buffer;
	delete [] buffer2;

	return texture;
}

void TransitionManager::CaptureStartScreen()
{
	IwGxFlush();
	m_StartTexture = CaptureScreen();
}
void TransitionManager::CaptureEndScreen()
{
	IwGxFlush();
	m_EndTexture = CaptureScreen();
}

void TransitionManager::Fade(double transitionSpeed)
{
	CIwMaterial* startMat;
	CIwMaterial* endMat;
	if (m_alphaValue <= 255)
	{
		IwGxClear();

		startMat = IW_GX_ALLOC_MATERIAL();
		startMat->SetTexture(m_StartTexture);
		IwGxSetMaterial(startMat);
		IwGxDrawRectScreenSpace(&CIwSVec2(0, 0), &CIwSVec2(IwGxGetDeviceWidth(), IwGxGetDeviceHeight())); // Change this back to m_StartTextrue->GetWidth / GetHeight

		endMat = IW_GX_ALLOC_MATERIAL();
		endMat->SetTexture(m_EndTexture);
		endMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		endMat->SetColAmbient(255, 255, 255, m_alphaValue);

		IwGxSetMaterial(endMat);
		IwGxDrawRectScreenSpace(&CIwSVec2(0, 0), &CIwSVec2(m_EndTexture->GetWidth(), m_EndTexture->GetHeight()));

		IwGxFlush();
		IwGxSwapBuffers();

		m_alphaValue += transitionSpeed;
	}
}

void TransitionManager::Fade(CIwTexture* start, CIwTexture* end, double transitionSpeed)
{
	CIwTexture* tempStart = m_StartTexture;
	CIwTexture* tempEnd = m_EndTexture;
	m_StartTexture = start;
	m_EndTexture = end;

	Fade(transitionSpeed);

	m_StartTexture = tempStart;
	m_EndTexture = tempEnd;
}

bool TransitionManager::isFinished()
{
	if (m_alphaValue >= 255)
	{
		return true;
	}
	return false;
}

bool TransitionManager::TransitionIn(CIw2DImage* start, double transitionSpeed)
{
	bool f = isFinished();

	// Clear to black, capture the screen, draw the image to fade IN to and capture, then fade in to it
	if (m_HasStarted == false)
	{
		m_alphaValue = 0;
		Iw2DSurfaceClear(0xff000000);
		CaptureStartScreen();
		Iw2DDrawImage(start, CIwSVec2(0,0));
		CaptureEndScreen();
		m_HasStarted = true;
	}

	if (!f)
		Fade(transitionSpeed);
	else
	{
		//delete startMat;
		//delete endMat;
		Iw2DDrawImage(start, CIwSVec2(0,0)); // The image has fully faded in so draw it with its full alpha value
	}

	return f;
}

bool TransitionManager::TransitionBetween(CIw2DImage* start, CIw2DImage* end, double speed)
{
	bool f = isFinished();

	// Clear to black, capture the screen, draw the image to fade IN to and capture, then fade in to it
	if (m_HasStarted == false)
	{
		m_alphaValue = 0;
		Iw2DDrawImage(start, CIwSVec2(0,0));
		CaptureStartScreen();
		Iw2DDrawImage(end, CIwSVec2(0,0));
		CaptureEndScreen();
		m_HasStarted = true;
	}

	if (!f)
		Fade(speed);
	else
	{
		//delete startMat;
		//delete endMat;
		Iw2DDrawImage(end, CIwSVec2(0,0)); // The transition has fully occured so draw the target image with its full alpha value
	}

	return f;
}

bool TransitionManager::TransitionOut(CIw2DImage* end, double speed)
{
	bool f = isFinished();

	// Clear to black, capture the screen, draw the image to fade IN to and capture, then fade in to it
	if (m_HasStarted == false)
	{
		m_alphaValue = 0;
		Iw2DDrawImage(end, CIwSVec2(0,0));
		CaptureStartScreen();
		Iw2DSurfaceClear(0xff000000);
		CaptureEndScreen();
		std::cout << "Captured end screens" << std::endl;
		m_HasStarted = true;
	}

	if (!f)
		Fade(speed);
	else
	{
		//delete startMat;
		//delete endMat;
	}

	return f;
}

