#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <jpeglib.h>

#define IMAGE_HEIGHT 1
#define IMAGE_WIDTH  2
#define IMAGE_BPP    3
#define IMAGE_SIZE   4

typedef unsigned char BYTE;

/* -----------------------------------------------
 * JPEG���顼������¤��
 */
struct JpegErrorManager
{
  struct jpeg_error_mgr fieldPublic;
  jmp_buf setJmpBuffer;
};
typedef struct JpegErrorManager* JpegErrorManagerPtr;


/* -----------------------------------------------
 * JPEG���顼�����״ؿ�
 * ���顼��ȯ�������Ȥ���setjmp�ؿ������
 */
void exitByLongJump(j_common_ptr pInfo)
{
  JpegErrorManagerPtr pError = (JpegErrorManagerPtr) pInfo->err;
  (*pInfo->err->output_message) (pInfo);
  longjmp(pError->setJmpBuffer, 1);
}


/* -----------------------------------------------
 * JPEG�����ɤ߹��ߴؿ�
 */
int readJpeg(char *filename,       /* �����ե�����̾ */
	     BYTE **imageBuffer)   /* �����ǡ�����Ƭ�ݥ��󥿤ؤΥݥ��� */
{
  /* ��¤�Ρ��ѿ������ */
  struct jpeg_decompress_struct cInfo;
  struct JpegErrorManager jpegError;
  JSAMPARRAY rowBuffer;
  FILE *infile = NULL;
  int imageSize = 0;
  int rowStride = 0;
  int pt = 0;

  /* �ե�����򳫤� */
  if ((infile = fopen(filename, "rb")) == NULL)
    {
      fprintf(stderr, "can't open %s\n", filename);
      return 0;
    }
	
  /* JPEG ���顼���������� */
  cInfo.err = jpeg_std_error(&jpegError.fieldPublic);
  jpegError.fieldPublic.error_exit = exitByLongJump;
  if (setjmp(jpegError.setJmpBuffer))
    {
      jpeg_destroy_decompress(&cInfo);
      fclose(infile);
      return 0;
    }

  /* JPEG�ǡ�����Ÿ�����֥����������� */
  jpeg_create_decompress(&cInfo);
	
  /* �ǡ������Ϥ�����ʥե����뤫��� */
  jpeg_stdio_src(&cInfo, infile);

  /* �إå���������ɤ߹��� */
  (void) jpeg_read_header(&cInfo, TRUE);

  /* �����ǡ���Ÿ���γ��� */
  (void) jpeg_start_decompress(&cInfo);

  /* ������¸�ΰ�γ��� */
  rowStride   = cInfo.output_width * cInfo.output_components;
  imageSize   = rowStride * cInfo.output_height;
  *imageBuffer = (JSAMPLE *)malloc(imageSize);
  rowBuffer = (*cInfo.mem->alloc_sarray)
    ((j_common_ptr) &cInfo, JPOOL_IMAGE, rowStride, 1);

  /* Ÿ�����줿�����ǡ������Ԥ����ɤ߽Ф� */
  pt = 0;
  while (cInfo.output_scanline < cInfo.output_height)
    {
      (void) jpeg_read_scanlines(&cInfo, rowBuffer, 1);
      memcpy(*imageBuffer+pt, *rowBuffer, rowStride);
      pt += rowStride;
    }
	
  /* �����ǡ���Ÿ�������ν�λ */
  (void) jpeg_finish_decompress(&cInfo);
    
  /* ��λ */
  jpeg_destroy_decompress(&cInfo);
  fclose(infile);
  return imageSize;
}


/* -----------------------------------------------
 * JPEG�إå���������ؿ�
 */
int readHeader(char *filename,   /* �����ե�����̾ */
	       int infoType)     /* �����������μ��� */
{
  /* ��¤�Ρ��ѿ������ */
  struct jpeg_decompress_struct cInfo;
  struct JpegErrorManager jpegError;
  JSAMPARRAY rowBuffer;
  FILE *infile = NULL;
  int returnValue = 0;
  int rowStride = 0;

  /* �ե�����򳫤� */
  if ((infile = fopen(filename, "rb")) == NULL)
    {
      fprintf(stderr, "can't open %s\n", filename);
      return 0;
    }
	
  /* JPEG ���顼���������� */
  cInfo.err = jpeg_std_error(&jpegError.fieldPublic);
  jpegError.fieldPublic.error_exit = exitByLongJump;
  if (setjmp(jpegError.setJmpBuffer))
    {
      jpeg_destroy_decompress(&cInfo);
      fclose(infile);
      return 0;
    }

  /* JPEG�ǡ�����Ÿ�����֥����������� */
  jpeg_create_decompress(&cInfo);
	
  /* �ǡ������Ϥ�����ʥե����뤫��� */
  jpeg_stdio_src(&cInfo, infile);

  /* �إå���������ɤ߹��� */
  (void) jpeg_read_header(&cInfo, TRUE);

  /* �����ǡ���Ÿ���γ��� */
  (void) jpeg_start_decompress(&cInfo);

  /* ������¸�ΰ�γ��� */
  switch (infoType)
    {
    case IMAGE_HEIGHT:
      returnValue = cInfo.output_height;
      break;
    case IMAGE_WIDTH:
      returnValue = cInfo.output_width;
      break;
    case IMAGE_BPP:
      returnValue = cInfo.output_components;
      break;
    case IMAGE_SIZE:
      returnValue = cInfo.output_height
	* cInfo.output_width * cInfo.output_components;
      break;
    }
  rowStride   = cInfo.output_width * cInfo.output_components;
  rowBuffer = (*cInfo.mem->alloc_sarray)
    ((j_common_ptr) &cInfo, JPOOL_IMAGE, rowStride, 1);

  while (cInfo.output_scanline < cInfo.output_height)
    {
      (void) jpeg_read_scanlines(&cInfo, rowBuffer, 1);
    }
	
  /* �����ǡ���Ÿ�������ν�λ */
  (void) jpeg_finish_decompress(&cInfo);
    
  /* ��λ */
  jpeg_destroy_decompress(&cInfo);
  fclose(infile);
  return returnValue;
}


/* -----------------------------------------------
 * JPEG�������ϴؿ�
 */
int writeJpeg(char *filename,      /* �ե�����̾ */
	      BYTE *imageBuffer,   /* �����ǡ����������Ƭ�ݥ��� */
	      int width,           /* �Ԥβ��ǿ� */
	      int height,          /* ���β��ǿ� */
	      int bpp,             /* �����ǤΥХ��ȿ� */
	      int quality)         /* JPEG���ϲ��(1-100) */
{
  /* ��¤�Ρ��ѿ������ */
  struct jpeg_compress_struct cInfo;
  struct jpeg_error_mgr jpegError;
  JSAMPROW rowPointer[1];
  FILE *outfile = NULL;
  int rowStride = 0;
  
  /* JPEG ���顼���������� */
  cInfo.err = jpeg_std_error(&jpegError);
  
  /* JPEG�ǡ����ΰ��̥��֥����������� */
  jpeg_create_compress(&cInfo);
  
  /* �ե�����򳫤� */
  if ((outfile = fopen(filename, "wb")) == NULL)
    {
      fprintf(stderr, "can't open %s\n", filename);
      return 0;
    }
  
  /* �ǡ������Ϥ�����ʥե�����ؽ񤭽Ф��� */
  jpeg_stdio_dest(&cInfo, outfile);
  
  /* �إå������������ */
  cInfo.image_width = width;
  cInfo.image_height = height;
  cInfo.input_components = bpp;
  if(bpp == 3) cInfo.in_color_space = JCS_RGB;
  else if(bpp == 1) cInfo.in_color_space = JCS_GRAYSCALE;
  jpeg_set_defaults(&cInfo);
  jpeg_set_quality(&cInfo, quality, TRUE);

  /* �����ǡ������̽����γ��� */
  (void) jpeg_start_compress(&cInfo, TRUE);

  /* �����ǡ������Ԥ��Ľ��Ϥ��� */
  rowStride   = width * bpp;
  while (cInfo.next_scanline < cInfo.image_height) {
    rowPointer[0] = & imageBuffer[cInfo.next_scanline * rowStride];
    (void) jpeg_write_scanlines(&cInfo, rowPointer, 1);
  }
	
  /* �����ǡ������̽����ν�λ */
  (void) jpeg_finish_compress(&cInfo);
    
  /* ��λ */
  fclose(outfile);
  jpeg_destroy_compress(&cInfo);
  return 1;
}
