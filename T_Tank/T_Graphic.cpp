#include"stdafx.h"
#include"T_Graphic.h"


/**
 * T_Graph类默认构造函数
 **/
T_Graph::T_Graph(){
	hBmp = NULL;
}

/**
 * 使用指定文件创建T_Graph对象
 **/
T_Graph::T_Graph(LPCTSTR fileName){
	LoadImageFile(fileName);
}

T_Graph::~T_Graph(){
	Destroy();
}

/**
 * 释放资源
 **/
void T_Graph::Destroy(){
	if (hBmp){
		DeleteObject(hBmp);
		hBmp = NULL;
	}
}

/**
 * 加载图像（支持BMP,GIF,JPEG,PNG,TIFF格式）。
 * 该函数利用GDI+的Bitmap类实现位图文件加载，
 * 加载成功后将转换为位图句柄保存到hBmp类数据成员中，
 * 并将获取的位图宽、高值保存到ImageWidth、ImageHeight类数据成员中。
 **/
bool T_Graph::LoadImageFile(LPCTSTR path){
	Bitmap* pBmp = Bitmap::FromFile(path);
	if (!pBmp) return false;

	ImageWidth = pBmp->GetWidth();
	ImageHeight = pBmp->GetHeight();
	if (ImageWidth == 0 || ImageHeight == 0) return false;

	Status status = pBmp->GetHBITMAP(NULL,&hBmp);
	if (pBmp){
		delete pBmp;
		pBmp = NULL;
	}
	if (status == S_OK){
		return true;
	}
	else{
		return false;
	}
}

/**
 * 在指定位置绘制位图
 **/
void T_Graph::PaintImage(HDC hdc, int x, int y){
	//创建帧图的内存设备
	HDC memDC = CreateCompatibleDC(hdc);
	//将空白的帧图选进内存设备
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(memDC,hBmp);
	//以下使用GDI的BitBlt绘制
	BitBlt(hdc,x,y,ImageWidth,ImageHeight,memDC,0,0,SRCCOPY);
	//还原内存设备环境中的原来位图对象
	SelectObject(memDC,OldFrameBmp);
	DeleteDC(memDC);											//删除内存设备
	DeleteObject(OldFrameBmp);									//删除位图对象
}

/**
 * 在指定位置以指定宽、高绘制位图
 **/
void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height){
	//创建全图的内存设备
	HDC memDC = CreateCompatibleDC(hdc);
	//將源图选进内存设备中
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC,hBmp);
	SetStretchBltMode(hdc,COLORONCOLOR);
	StretchBlt(hdc,x,y,width,height,memDC,0,0,ImageWidth,ImageHeight,SRCCOPY);

	//还原内存设备环境中的原来位图对象
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);											//删除内存设备
	DeleteObject(hOldBitmap);									//删除位图对象
}

/**
 * 在指定位置以指定宽、高和透明度绘制位图
 **/
void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha){
	//创建全图的内存设备
	HDC memDC = CreateCompatibleDC(hdc);
	//將源图选进内存设备中
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBmp);

	BLENDFUNCTION dc_bf;
	dc_bf.BlendOp = AC_SRC_OVER;
	dc_bf.BlendFlags = 0;
	dc_bf.SourceConstantAlpha = alpha;
	dc_bf.AlphaFormat = AC_SRC_ALPHA;
	SetStretchBltMode(hdc, COLORONCOLOR);
	AlphaBlend(hdc,x,y,width,height,memDC,0,0,ImageWidth,ImageHeight,dc_bf);

	//还原内存设备环境中的原来位图对象
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);											//删除内存设备
	DeleteObject(hOldBitmap);									//删除位图对象
}

/**
 * 用指定颜色绘制空白位图
 **/
void T_Graph::PaintBlank(HBITMAP hbmp,int width,int height,COLORREF crColor){
	HDC memDC = ::CreateCompatibleDC(NULL);
	HBITMAP OldBmp = (HBITMAP)SelectObject(memDC, hbmp);

	HBRUSH hBrush = CreateSolidBrush(crColor);
	RECT rcBitmap = {0,0,width,height};
	FillRect(memDC,&rcBitmap,hBrush);

	//恢复内存设备中的位图对象
	SelectObject(memDC,OldBmp);
	//删除内存设备
	DeleteDC(memDC);
	//删除位图对象
	DeleteObject(OldBmp);
	//释放句柄
	OldBmp = NULL;
	//删除画笔
	DeleteObject(hBrush);
}

void T_Graph::PaintBlank(HDC hdc, int x, int y, int width, int height, Color crColor){
	Gdiplus::Graphics g(hdc);
	Rect rectangle(x,y,width,height);
	SolidBrush solidBrush(crColor);
	g.FillRectangle(&solidBrush,rectangle);
	g.ReleaseHDC(hdc);
}

void T_Graph::PaintBlank(HDC hdc, int x, int y, int width, int height, COLORREF crColor, int alphaLevel){
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc,width,height);
	HDC memdc = ::CreateCompatibleDC(NULL);
	HBITMAP OldBmp = (HBITMAP)SelectObject(memdc,hbitmap);

	HBRUSH hBrush = CreateSolidBrush(crColor);
	RECT rcBitmap = {x,y,width,height};
	FillRect(memdc,&rcBitmap,hBrush);

	BLENDFUNCTION frame_bf;
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.BlendFlags = 0;
	frame_bf.SourceConstantAlpha = alphaLevel;
	frame_bf.AlphaFormat = 0;
	AlphaBlend(hdc,x,y,width,height,memdc,0,0,width,height,frame_bf);
	
	//恢复内存设备中的位图对象
	SelectObject(memdc,OldBmp);
	//删除内存设备
	DeleteDC(memdc);
	//删除位图对象
	DeleteObject(OldBmp);
	//释放句柄
	OldBmp = NULL;
	//删除位图对象
	DeleteObject(hbitmap);
	//释放句柄
	hbitmap = NULL;
	//删除笔刷
	DeleteObject(hBrush);
}

HBITMAP T_Graph::CreateBlankBitmap(int width,int height,COLORREF color){
	BYTE* pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width*height * 4;
	HBITMAP blankBmp = CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,(void**)pBits,NULL,NULL);
	PaintBlank(blankBmp,width,height,color);
	return blankBmp;
}

void T_Graph::PaintFrame(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, int FrameCount, int RowFrames, int wFrame, int hFrame, float ratio, int rotType, BYTE alpha){
	//根据动画总帧数和每行帧数计算出每一张帧图在原图中的行列号
	int col = FrameCount%RowFrames;
	int row = (FrameCount - col) / RowFrames;
	PaintRegion(in_hbitmap, destDC, destX, destY, col*wFrame, row*hFrame, wFrame, hFrame, ratio, rotType, alpha);
}

Bitmap* T_Graph::HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc){
	BITMAP bmp;
	int width, height;
	//根据位图句柄获取 BITMAP 结构体对象的数据
	memset((void*)&bmp, 0, sizeof(BITMAP));
	GetObject(hbmp, sizeof(BITMAP), (void*)&bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
	byte* bmpBytes = (byte*)bmp.bmBits;
	if (width <= 0 && height <= 0) return NULL;

	//根据位图宽高新建一个 PixelFormat32bppPARGB 格式的 Bitmap 对象
	Bitmap* BmpCpy = new Bitmap(width, height, PixelFormat32bppPARGB);
	//采用 LockBits 函数为位图像素数据拷贝准备内存缓冲区
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	BmpCpy->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppPARGB, &bmpData);
	//计算出位图每行像素数，并将指针指向首行
	int lineSize = width * 4;
	byte* cpyBytes = (byte*)(bmpData.Scan0);
	//按照位图的高逐行拷贝像素数据至缓冲区
	for (int y = 0; y < height; y++){
		memcpy((y*lineSize) + cpyBytes, ((height - y - 1)*lineSize) + bmpBytes, lineSize);
	}
	//解锁内存缓冲区
	BmpCpy->UnlockBits(&bmpData);
	return BmpCpy;
}

void T_Graph::PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, int srcX, int srcY, int regionWidth, int regionHeight, float ratio, int rotType, BYTE alpha){
	//根据局部图像的宽高和缩放比重新计算局部图像的宽高
	int width = (int)(regionWidth*ratio);
	int height = (int)(regionHeight*ratio);
	// 设定要创建的临时 DIB 位图的参数
	BYTE* pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width*height * 4;
	//创建临时局部位图对象
	HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, NULL);
	HDC frameDC = ::CreateCompatibleDC(NULL);												//创建局部位图对应的设备
	//将局部位图选入设备
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(frameDC, hbitmap);
	HDC memDC = CreateCompatibleDC(destDC);													//创建原始位图对应的设备
	//将原始位图选入设备
	HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, in_hbitmap);
	BLENDFUNCTION frame_bf;//设定要 AlphaBlend 函数混合的参数
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.BlendFlags = 0;
	frame_bf.SourceConstantAlpha = 255;
	frame_bf.AlphaFormat = AC_SRC_ALPHA;
	//用AlphaBlend函数从原始位图设备中读取局部位图到局部位图设备
	AlphaBlend(frameDC, 0, 0, (int)(regionWidth*ratio), (int)(regionHeight*ratio), memDC, srcX, srcY, regionWidth, regionHeight, frame_bf);
	//通过自定义函数HBITMAP_TO_Bitmap获取对应的Bitmap对象
	Bitmap* nowFrameBmp = T_Graph::HBITMAP_To_Bitmap(hbitmap, destDC);
	SelectObject(frameDC, OldFrameBmp);														//使用原来对象还原内存设备中的位图对象
	DeleteDC(frameDC);																		//删除内存设备
	DeleteObject(OldFrameBmp);																//删除位图对象
	OldFrameBmp = NULL;
	DeleteObject(hbitmap);																	//删除位图对象
	hbitmap = NULL;
	SelectObject(memDC, OldMemBmp);															//使用原来对象还原内存设备中的位图对象
	DeleteDC(memDC);																		//删除内存设备
	DeleteObject(OldMemBmp);																//删除位图对象
	OldMemBmp = NULL;
	//通过GDI+的RotateFlip函数对局部位图进行旋转或翻转
	//注意设计90度和270度时必须将宽高值进行调换
	switch (rotType){
	case TRANS_NONE:
		break;
	case TRANS_NOFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipNone);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	case TRANS_NOFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipNone);
		width = (int)(regionWidth*ratio);
		height = (int)(regionHeight*ratio);
		break;
	case TRANS_NOFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipNone);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	case TRANS_VFLIP_NOROT:
		nowFrameBmp->RotateFlip(RotateNoneFlipY);
		width = (int)(regionWidth*ratio);
		height = (int)(regionHeight*ratio);
		break;
	case TRANS_VFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipY);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	case TRANS_VFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate90FlipY);
		width = (int)(regionWidth*ratio);
		height = (int)(regionHeight*ratio);
		break;
	case TRANS_VFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipY);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	case TRANS_HFLIP_NOROT:
		nowFrameBmp->RotateFlip(RotateNoneFlipX);
		width = (int)(regionWidth*ratio);
		height = (int)(regionHeight*ratio);
		break;
	case TRANS_HFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipX);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	case TRANS_HFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipX);
		width = (int)(regionWidth*ratio);
		height = (int)(regionHeight*ratio);
		break;
	case TRANS_HFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipX);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	}
	HBITMAP hbmp = NULL;												//重新定义一个新的位图句柄
	nowFrameBmp->GetHBITMAP(NULL, &hbmp);								//将处理后的Bitmap对象转换为位图句柄
	HDC bufDC = CreateCompatibleDC(destDC);								//创建要绘制的目的设备
	HBITMAP OldbufBmp = (HBITMAP)SelectObject(bufDC, hbmp);				//将最终的位图选入设备
	BLENDFUNCTION buf_bf;												//设定要AlphaBlend函数混合的参数
	buf_bf.BlendOp = AC_SRC_OVER;
	buf_bf.BlendFlags = 0;
	buf_bf.SourceConstantAlpha = alpha;
	buf_bf.AlphaFormat = AC_SRC_ALPHA;
	//用AlphaBlend函数把最终的局部位图从目的设备绘制到最终的目标设备
	AlphaBlend(destDC, destX, destY, width, height, bufDC, 0, 0, width, height, buf_bf);
	SelectObject(bufDC, OldbufBmp);										//使用原来对象还原内存设备中的位图对象
	DeleteDC(bufDC);													//删除内存设备
	DeleteObject(OldbufBmp);											//删除位图对象
	OldbufBmp = NULL;
	DeleteObject(hbmp);													//删除位图对象
	hbmp = NULL;
	DeleteObject(nowFrameBmp);											//删除位图对象
	delete nowFrameBmp;													//删除位图对象
	nowFrameBmp = NULL;
}

void T_Graph::PaintText(HDC hdc, RectF fontRect, LPCTSTR text, REAL fontSize, LPCTSTR fontName, Color fontColor, FontStyle style, StringAlignment align){
	Graphics graph(hdc);//使用GDI+方法进行绘制
	FontFamily fontFamily(fontName);
	Font font(&fontFamily, fontSize, style, UnitPoint);
	StringFormat format;
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetAlignment(align);
	SolidBrush solidBrush1(fontColor);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graph.DrawString(text, (INT)wcslen(text), &font, fontRect, &format, &solidBrush1);
	graph.ReleaseHDC(hdc);
}