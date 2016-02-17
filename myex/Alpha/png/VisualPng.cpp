/*------------------------------------
*  VisualPng.C -- Shows a PNG image
*------------------------------------
*
* Copyright 2000, Willem van Schaik.
*
* This code is released under the libpng license.
* For conditions of distribution and use, see the disclaimer
* and license in png.h
*/

/* switches */

/* defines */

#define PROGNAME  "VisualPng"
#define LONGNAME  "Win32 Viewer for PNG-files"
#define VERSION   "1.0 of 2000 June 07"

/* constants */

#define MARGIN 8

/* standard includes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "zlib.h"

/* application includes */

#include "png.h"
#include "pngfile.h"
#include "resource.h"


BOOL LoadImageFile(HWND hwnd, PTSTR pstrPathName,
				   png_byte **ppbImage, int *pxImgSize, int *pyImgSize, int *piChannels,
				   png_color *pBkgColor);

BOOL DisplayImage (HWND hwnd, BYTE **ppDib,
				   BYTE **ppDiData, int cxWinSize, int cyWinSize,
				   BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
				   BOOL bStretched);

BOOL InitBitmap (
	BYTE *pDiData, int cxWinSize, int cyWinSize);

BOOL FillBitmap (
	BYTE *pDiData, int cxWinSize, int cyWinSize,
	BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
	BOOL bStretched);

/* a few global variables */

static char *szProgName = PROGNAME;
static char *szAppName = LONGNAME;
static char *szIconName = PROGNAME;
static char szCmdFileName [MAX_PATH];

/* MAIN routine */



void LoadPng()
{
	PTSTR pstrPathName=L"";
	png_byte **ppbImage;
	int *pxImgSize; 
	int *pyImgSize; 
	int *piChannels; 
	png_color pBkgColor={127, 127, 127};
	PngLoadImage (pstrPathName, ppbImage, pxImgSize, pyImgSize, piChannels,
		&pBkgColor);

	/* display the PNG into the DIBitmap */
	/*DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
		pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);*/
}

/*---------------
*  CenterAbout
*---------------
*/
BOOL CenterAbout (HWND hwndChild, HWND hwndParent)
{
	RECT    rChild, rParent, rWorkArea;
	int     wChild, hChild, wParent, hParent;
	int     xNew, yNew;
	BOOL  bResult;

	/* Get the Height and Width of the child window */
	GetWindowRect (hwndChild, &rChild);
	wChild = rChild.right - rChild.left;
	hChild = rChild.bottom - rChild.top;

	/* Get the Height and Width of the parent window */
	GetWindowRect (hwndParent, &rParent);
	wParent = rParent.right - rParent.left;
	hParent = rParent.bottom - rParent.top;

	/* Get the limits of the 'workarea' */
	bResult = SystemParametersInfo(
		SPI_GETWORKAREA,  /* system parameter to query or set */
		sizeof(RECT),
		&rWorkArea,
		0);
	if (!bResult) {
		rWorkArea.left = rWorkArea.top = 0;
		rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
		rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	/* Calculate new X position, then adjust for workarea */
	xNew = rParent.left + ((wParent - wChild) /2);
	if (xNew < rWorkArea.left) {
		xNew = rWorkArea.left;
	} else if ((xNew+wChild) > rWorkArea.right) {
		xNew = rWorkArea.right - wChild;
	}

	/* Calculate new Y position, then adjust for workarea */
	yNew = rParent.top  + ((hParent - hChild) /2);
	if (yNew < rWorkArea.top) {
		yNew = rWorkArea.top;
	} else if ((yNew+hChild) > rWorkArea.bottom) {
		yNew = rWorkArea.bottom - hChild;
	}

	/* Set it, and return */
	return SetWindowPos (hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE |
		SWP_NOZORDER);
}


/*-----------------
*  LoadImageFile
*-----------------
*/

BOOL LoadImageFile (HWND hwnd, PTSTR pstrPathName,
					png_byte **ppbImage, int *pxImgSize, int *pyImgSize,
					int *piChannels, png_color *pBkgColor)
{
	static TCHAR szTmp [MAX_PATH];

	/* if there's an existing PNG, free the memory */

	if (*ppbImage)
	{
		free (*ppbImage);
		*ppbImage = NULL;
	}

	/* Load the entire PNG into memory */

	SetCursor (LoadCursor (NULL, IDC_WAIT));
	ShowCursor (TRUE);

	PngLoadImage (pstrPathName, ppbImage, pxImgSize, pyImgSize, piChannels,
		pBkgColor);

	ShowCursor (FALSE);
	SetCursor (LoadCursor (NULL, IDC_ARROW));

	if (*ppbImage != NULL)
	{
		//success
	}
	else
	{
		//error
	}

	return TRUE;
}

/*----------------
*  DisplayImage
*----------------
*/
BOOL DisplayImage (HWND hwnd, BYTE **ppDib,
				   BYTE **ppDiData, int cxWinSize, int cyWinSize,
				   BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
				   BOOL bStretched)
{
	BYTE                       *pDib = *ppDib;
	BYTE                       *pDiData = *ppDiData;
	/* BITMAPFILEHEADER        *pbmfh; */
	BITMAPINFOHEADER           *pbmih;
	WORD                        wDIRowBytes;
	png_color                   bkgBlack = {0, 0, 0};
	png_color                   bkgGray  = {127, 127, 127};
	png_color                   bkgWhite = {255, 255, 255};

	/* allocate memory for the Device Independant bitmap */

	wDIRowBytes = (WORD) ((3 * cxWinSize + 3L) >> 2) << 2;

	if (pDib)
	{
		free (pDib);
		pDib = NULL;
	}

	if (!(pDib = (BYTE *) malloc (sizeof(BITMAPINFOHEADER) +
		wDIRowBytes * cyWinSize)))
	{
		//error
	}
	*ppDib = pDib;
	memset (pDib, 0, sizeof(BITMAPINFOHEADER));

	/* initialize the dib-structure */

	pbmih = (BITMAPINFOHEADER *) pDib;
	pbmih->biSize = sizeof(BITMAPINFOHEADER);
	pbmih->biWidth = cxWinSize;
	pbmih->biHeight = -((long) cyWinSize);
	pbmih->biPlanes = 1;
	pbmih->biBitCount = 24;
	pbmih->biCompression = 0;
	pDiData = pDib + sizeof(BITMAPINFOHEADER);
	*ppDiData = pDiData;

	/* first fill bitmap with gray and image border */

	InitBitmap (pDiData, cxWinSize, cyWinSize);

	/* then fill bitmap with image */

	if (pbImage)
	{
		FillBitmap (
			pDiData, cxWinSize, cyWinSize,
			pbImage, cxImgSize, cyImgSize, cImgChannels,
			bStretched);
	}

	return TRUE;
}

/*--------------
*  InitBitmap
*--------------
*/
BOOL InitBitmap (BYTE *pDiData, int cxWinSize, int cyWinSize)
{
	BYTE *dst;
	int x, y, col;

	/* initialize the background with gray */

	dst = pDiData;
	for (y = 0; y < cyWinSize; y++)
	{
		col = 0;
		for (x = 0; x < cxWinSize; x++)
		{
			/* fill with GRAY */
			*dst++ = 127;
			*dst++ = 127;
			*dst++ = 127;
			col += 3;
		}
		/* rows start on 4 byte boundaries */
		while ((col % 4) != 0)
		{
			dst++;
			col++;
		}
	}

	return TRUE;
}

/*--------------
*  FillBitmap
*--------------
*/
BOOL FillBitmap (
	BYTE *pDiData, int cxWinSize, int cyWinSize,
	BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
	BOOL bStretched)
{
	BYTE *pStretchedImage;
	BYTE *pImg;
	BYTE *src, *dst;
	BYTE r, g, b, a;
	const int cDIChannels = 3;
	WORD wImgRowBytes;
	WORD wDIRowBytes;
	int cxNewSize, cyNewSize;
	int cxImgPos, cyImgPos;
	int xImg, yImg;
	int xWin, yWin;
	int xOld, yOld;
	int xNew, yNew;

	if (bStretched)
	{
		cxNewSize = cxWinSize - 2 * MARGIN;
		cyNewSize = cyWinSize - 2 * MARGIN;

		/* stretch the image to it's window determined size */

		/* the following two are mathematically the same, but the first
		* has side-effects because of rounding
		*/
		/*      if ((cyNewSize / cxNewSize) > (cyImgSize / cxImgSize)) */
		if ((cyNewSize * cxImgSize) > (cyImgSize * cxNewSize))
		{
			cyNewSize = cxNewSize * cyImgSize / cxImgSize;
			cxImgPos = MARGIN;
			cyImgPos = (cyWinSize - cyNewSize) / 2;
		}
		else
		{
			cxNewSize = cyNewSize * cxImgSize / cyImgSize;
			cyImgPos = MARGIN;
			cxImgPos = (cxWinSize - cxNewSize) / 2;
		}

		pStretchedImage = (BYTE*)malloc (cImgChannels * cxNewSize * cyNewSize);
		pImg = pStretchedImage;

		for (yNew = 0; yNew < cyNewSize; yNew++)
		{
			yOld = yNew * cyImgSize / cyNewSize;
			for (xNew = 0; xNew < cxNewSize; xNew++)
			{
				xOld = xNew * cxImgSize / cxNewSize;

				r = *(pbImage + cImgChannels * ((yOld * cxImgSize) + xOld) + 0);
				g = *(pbImage + cImgChannels * ((yOld * cxImgSize) + xOld) + 1);
				b = *(pbImage + cImgChannels * ((yOld * cxImgSize) + xOld) + 2);
				*pImg++ = r;
				*pImg++ = g;
				*pImg++ = b;
				if (cImgChannels == 4)
				{
					a = *(pbImage + cImgChannels * ((yOld * cxImgSize) + xOld)
						+ 3);
					*pImg++ = a;
				}
			}
		}

		/* calculate row-bytes */

		wImgRowBytes = cImgChannels * cxNewSize;
		wDIRowBytes = (WORD) ((cDIChannels * cxWinSize + 3L) >> 2) << 2;

		/* copy image to screen */

		for (yImg = 0, yWin = cyImgPos; yImg < cyNewSize; yImg++, yWin++)
		{
			if (yWin >= cyWinSize - cyImgPos)
				break;
			src = pStretchedImage + yImg * wImgRowBytes;
			dst = pDiData + yWin * wDIRowBytes + cxImgPos * cDIChannels;

			for (xImg = 0, xWin = cxImgPos; xImg < cxNewSize; xImg++, xWin++)
			{
				if (xWin >= cxWinSize - cxImgPos)
					break;
				r = *src++;
				g = *src++;
				b = *src++;
				*dst++ = b; /* note the reverse order  */
				*dst++ = g;
				*dst++ = r;
				if (cImgChannels == 4)
				{
					a = *src++;
				}
			}
		}

		/* free memory */

		if (pStretchedImage != NULL)
		{
			free (pStretchedImage);
			pStretchedImage = NULL;
		}

	}

	/* process the image not-stretched */

	else
	{
		/* calculate the central position */

		cxImgPos = (cxWinSize - cxImgSize) / 2;
		cyImgPos = (cyWinSize - cyImgSize) / 2;

		/* check for image larger than window */

		if (cxImgPos < MARGIN)
			cxImgPos = MARGIN;
		if (cyImgPos < MARGIN)
			cyImgPos = MARGIN;

		/* calculate both row-bytes */

		wImgRowBytes = cImgChannels * cxImgSize;
		wDIRowBytes = (WORD) ((cDIChannels * cxWinSize + 3L) >> 2) << 2;

		/* copy image to screen */

		for (yImg = 0, yWin = cyImgPos; yImg < cyImgSize; yImg++, yWin++)
		{
			if (yWin >= cyWinSize - MARGIN)
				break;
			src = pbImage + yImg * wImgRowBytes;
			dst = pDiData + yWin * wDIRowBytes + cxImgPos * cDIChannels;

			for (xImg = 0, xWin = cxImgPos; xImg < cxImgSize; xImg++, xWin++)
			{
				if (xWin >= cxWinSize - MARGIN)
					break;
				r = *src++;
				g = *src++;
				b = *src++;
				*dst++ = b; /* note the reverse order  */
				*dst++ = g;
				*dst++ = r;
				if (cImgChannels == 4)
				{
					a = *src++;
				}
			}
		}
	}

	return TRUE;
}

/*-----------------
*  end of source
*-----------------
*/
