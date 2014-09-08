// JpegFile.h: interface for the CJpegFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JPEGFILE_H__74FA23A2_3CF4_4BC4_8A37_74FAFA0FF2EE__INCLUDED_)
#define AFX_JPEGFILE_H__74FA23A2_3CF4_4BC4_8A37_74FAFA0FF2EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

#define JPEG_BUFFERSIZE 0x100000
class CJpegFile  
{
public:
	 LONG RGBToJpegBuffer( char ** filebuffer, BYTE *dataBuf, UINT widthPix,  UINT height,  BOOL color,   int quality);

	////////////////////////////////////////////////////////////////
	// read a JPEG file to an RGB buffer - 3 bytes per pixel
	// returns a ptr to a buffer .
	// caller is responsible for cleanup!!!
	// BYTE *buf = JpegFile::JpegFileToRGB(....);
	// delete [] buf;

	 BYTE * JpegFileToRGB(CString fileName,			// path to image
							   UINT *width,					// image width in pixels
							   UINT *height);				// image height


	 BYTE * JpegBufferToRGB(   BYTE*pDataBuffer,			// RGB buffer
		                       UINT nSize,
							   UINT *width,					// image width in pixels
							   UINT *height);				// image height
	////////////////////////////////////////////////////////////////
	// write a JPEG file from a 3-component, 1-byte per component buffer

	 BOOL RGBToJpegFile(CString fileName,				// path
							BYTE *dataBuf,					// RGB buffer
							UINT width,						// pixels
							UINT height,					// rows
							BOOL color,						// TRUE = RGB
															// FALSE = Grayscale
							int quality);					// 0 - 100


	////////////////////////////////////////////////////////////////
	// fetch width / height of an image
	
	 BOOL GetJPGDimensions(CString fileName,			// path
								UINT *width,				// pixels
								UINT *height);

	////////////////////////////////////////////////////////////////
	//	utility functions
	//	to do things like DWORD-align, flip, convert to grayscale, etc.
	//

	////////////////////////////////////////////////////////////////
	// allocates a DWORD-aligned buffer, copies data buffer
	// caller is responsible for delete []'ing the buffer

	 BYTE * MakeDwordAlignedBuf(BYTE *dataBuf,			// input buf
									 UINT widthPix,				// input pixels
									 UINT height,				// lines
									 UINT *uiOutWidthBytes);	// new width bytes


	////////////////////////////////////////////////////////////////
	// if you have a DWORD aligned buffer, this will copy the
	// RGBs out of it into a new buffer. new width is widthPix * 3 bytes
	// caller is responsible for delete []'ing the buffer
	
	 BYTE *RGBFromDWORDAligned(BYTE *inBuf,				// input buf
									UINT widthPix,				// input size
									UINT widthBytes,			// input size
									UINT height);

	////////////////////////////////////////////////////////////////
	// vertically flip a buffer - for BMPs
	// in-place
	
	// note, this routine works on a buffer of width widthBytes: not a 
	// buffer of widthPixels.
	 BOOL VertFlipBuf(BYTE * inbuf,						// input buf
					   UINT widthBytes,							// input width bytes
					   UINT height);							// height

	// NOTE :
	// the following routines do their magic on buffers with a whole number
	// of pixels per data row! these are assumed to be non DWORD-aligned buffers.

	////////////////////////////////////////////////////////////////
	// convert RGB to grayscale	using luminance calculation
	// in-place
	
	 BOOL MakeGrayScale(BYTE *buf,						// input buf 
						UINT widthPix,							// width in pixels
						UINT height);							// height

	////////////////////////////////////////////////////////////////
	// swap Red and Blue bytes
	// in-place
	
	 BOOL BGRFromRGB(BYTE *buf,							// input buf
					UINT widthPix,								// width in pixels
					UINT height);								// lines

	////////////////////////////////////////////////////////////////
	// these do nothing
	CJpegFile();		// creates an empty object
	~CJpegFile();	// destroys nothing

};

#endif // !defined(AFX_JPEGFILE_H__74FA23A2_3CF4_4BC4_8A37_74FAFA0FF2EE__INCLUDED_)
